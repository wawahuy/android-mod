import { Injectable, Logger } from '@nestjs/common';
import { InjectModel } from '@nestjs/mongoose';
import * as moment from 'moment';
import { Model, PipelineStage, Types } from 'mongoose';
import {
  PkgHdrAccount,
  PkgHdrAccountDocument,
} from 'src/schema/pkg-hdr-account.schema';
import { TelegramService } from './telegram.service';
import { GameKey, GameKeyDocument } from 'src/schema/game-key.schema';
import { logObject } from 'src/utils/common';
import { InjectQueue } from '@nestjs/bull';
import { Job, Queue } from 'bull';
import {
  JOB_HDR_ADS_REWARD_PREFIX_ADS1,
  JOB_HDR_ADS_REWARD_PREFIX_ADS2,
  QUEUE_HDR_ADS_REWARD,
} from 'src/utils/constants';
import { HdrGameRpcAdsType, JobAdsData } from 'src/dtos/pkg-hdr.dto';

@Injectable()
export class PkgHdrAccountService {
  private readonly _logger = new Logger('PkgHdrAccountService');
  constructor(
    private readonly _telegramService: TelegramService,
    @InjectModel(PkgHdrAccount.name)
    private readonly _pkgHdrAccountModel: Model<PkgHdrAccountDocument>,
    @InjectModel(GameKey.name)
    private readonly _gameKeyModel: Model<GameKeyDocument>,
    @InjectQueue(QUEUE_HDR_ADS_REWARD)
    private readonly _hdrAdsRewardQueue: Queue<JobAdsData>,
  ) {
    this.updateAccountActive({ progress: (val) => {} } as any);
  }

  async newOrUpdateAccount(
    gameKeyId: Types.ObjectId,
    uid: string,
    mtkey: string,
    skey: string,
  ) {
    let hdrAccount = await this._pkgHdrAccountModel.findOne({
      uid,
    });
    const result = {
      hdrAccount,
      allowTrial: false,
    };
    if (!hdrAccount) {
      result.allowTrial = true;
      hdrAccount = await this._pkgHdrAccountModel.create({});
      hdrAccount.uid = uid;
      this._telegramService.sendMessage(`New HDR account: ${uid}`);
    } else {
      if (
        !hdrAccount.trialExpired ||
        moment(hdrAccount.trialExpired).isAfter(moment())
      ) {
        result.allowTrial = true;
      }
    }

    if (gameKeyId) {
      hdrAccount.gameKeyId = new Types.ObjectId(gameKeyId.toString());
    } else {
      hdrAccount.gameKeyId = null;
    }
    hdrAccount.mtkey = mtkey;
    hdrAccount.skey = skey;
    await hdrAccount.save();
    return result;
  }

  async findAccountActive(
    excludeIds?: Types.ObjectId[],
  ): Promise<PkgHdrAccountDocument[]> {
    const [paid, trial] = await Promise.all([
      this.findPaidAccountActive(excludeIds),
      this.findTrialAccountActive(excludeIds),
    ]);
    return paid.concat(trial);
  }

  async findPaidAccountActive(
    excludeIds?: Types.ObjectId[],
  ): Promise<PkgHdrAccountDocument[]> {
    const aggFilter: PipelineStage[] = [];
    if (excludeIds) {
      aggFilter.push({
        $match: {
          _id: {
            $nin: excludeIds,
          },
        },
      });
    }

    const aggs: PipelineStage[] = [
      ...aggFilter,
      {
        $lookup: {
          from: this._gameKeyModel.collection.name,
          localField: 'gameKeyId',
          foreignField: '_id',
          as: 'gameKey',
          pipeline: [
            {
              $match: {
                expiredAt: {
                  $gt: new Date(),
                },
              },
            },
          ],
        },
      },
      {
        $match: {
          'gameKey.0': {
            $exists: true,
          },
        },
      },
    ];
    // await this._pkgHdrAccountModel
    //   .aggregate(aggs)
    //   .explain('executionStats')
    //   .then((rs) => {
    //     console.log(logObject(rs));
    //   });
    const account = await this._pkgHdrAccountModel.aggregate(aggs);
    return account;
  }

  async findTrialAccountActive(
    excludeIds?: Types.ObjectId[],
  ): Promise<PkgHdrAccountDocument[]> {
    const aggFilter: PipelineStage[] = [];
    if (excludeIds) {
      aggFilter.push({
        $match: {
          _id: {
            $nin: excludeIds,
          },
        },
      });
    }

    const aggs: PipelineStage[] = [
      ...aggFilter,
      {
        $match: {
          trialExpired: {
            $gt: new Date(),
          },
          gameKeyId: {
            $eq: null,
          },
        },
      },
    ];
    // await this._pkgHdrAccountModel
    //   .aggregate(aggs)
    //   .explain('executionStats')
    //   .then((rs) => {
    //     console.log(logObject(rs));
    //   });
    const account = await this._pkgHdrAccountModel.aggregate(aggs);
    return account;
  }

  async getUnexpiredAccount(accountId: string): Promise<PkgHdrAccountDocument> {
    const aggs: PipelineStage[] = [
      {
        $match: {
          _id: new Types.ObjectId(accountId),
        },
      },
      {
        $lookup: {
          from: this._gameKeyModel.collection.name,
          localField: 'gameKeyId',
          foreignField: '_id',
          as: 'gameKey',
        },
      },
    ];
    const accounts = await this._pkgHdrAccountModel.aggregate(aggs);
    const account = accounts?.[0] as PkgHdrAccountDocument;
    if (!account) {
      return null;
    }

    const gameKeys = (account as any).gameKey as GameKeyDocument[];
    const gameKey = gameKeys?.[0];
    if (gameKey) {
      if (moment(gameKey.expiredAt).isBefore(new Date())) {
        return null;
      }
    } else {
      if (moment(account.trialExpired).isBefore(new Date())) {
        return null;
      }
    }

    delete (account as any).gameKey;
    return account;
  }

  updateNonDocument(obj: any) {
    return this._pkgHdrAccountModel.updateOne(
      { _id: obj._id },
      {
        $set: obj,
      },
    );
  }

  async getHashAdsJobName() {
    const jobs = await this._hdrAdsRewardQueue.getJobs([
      'delayed',
      'active',
      'waiting',
    ]);
    const result = jobs.reduce<{ [key: string]: boolean }>((val, job) => {
      const name = job.name;
      if (!val[name]) {
        val[name] = true;
      }
      return val;
    }, {});
    return result;
  }

  async getListAccountActiveID(): Promise<Types.ObjectId[]> {
    const jobs = await this._hdrAdsRewardQueue.getJobs([
      'delayed',
      'active',
      'waiting',
    ]);
    const bk: { [key: string]: number } = {};
    const result = jobs.reduce<{ [key: string]: Types.ObjectId }>(
      (val, job) => {
        const name = job.name;
        [
          JOB_HDR_ADS_REWARD_PREFIX_ADS1,
          JOB_HDR_ADS_REWARD_PREFIX_ADS2,
        ].forEach((pr) => {
          if (name.startsWith(pr)) {
            const id = job.name.replace(pr, '');
            bk[id] = (bk[id] || 0) + 1;
            if (!val[id] && bk[id] >= 2) {
              val[id] = new Types.ObjectId(id);
            }
          }
        });
        return val;
      },
      {},
    );
    return Object.values(result);
  }

  async findNewAccountActive() {
    const accountActiveIds = await this.getListAccountActiveID();
    const newAccountActive = this.findAccountActive(accountActiveIds);
    return newAccountActive;
  }

  async updateAccountActive(job: Job<any>) {
    job.progress(10);
    const hashAdsJobName = await this.getHashAdsJobName();
    const newAccountActive = await this.findNewAccountActive();
    if (newAccountActive && newAccountActive.length) {
      newAccountActive.forEach((acc) => {
        this._logger.warn('add job: ' + acc._id.toString());
        this._telegramService.sendMessage('add job: ' + acc._id.toString());
        this.addJobAdsRewardIfNotExists(
          hashAdsJobName,
          JOB_HDR_ADS_REWARD_PREFIX_ADS1,
          acc._id,
          'adGiftBox1',
        );
        this.addJobAdsRewardIfNotExists(
          hashAdsJobName,
          JOB_HDR_ADS_REWARD_PREFIX_ADS2,
          acc._id,
          'adGiftBox2',
        );
      });
    }
    job.progress(100);
    return newAccountActive.length;
  }

  addJobAdsRewardIfNotExists(
    hash: { [key: string]: boolean },
    jobNamePrefix: string,
    id: Types.ObjectId | string,
    type: HdrGameRpcAdsType,
    delay = 0,
  ) {
    const jobName = jobNamePrefix + id.toString();
    if (!hash[jobName]) {
      return this.addJobAdsReward(jobNamePrefix, id, type, delay);
    }
  }

  addJobAdsReward(
    jobNamePrefix: string,
    id: Types.ObjectId | string,
    type: HdrGameRpcAdsType,
    delay = 0,
  ) {
    const jobName = jobNamePrefix + id.toString();
    return this._hdrAdsRewardQueue.add(
      jobName,
      {
        id: id.toString(),
        type,
      },
      { removeOnComplete: true, delay },
    );
  }
}
