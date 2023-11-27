import { Injectable } from '@nestjs/common';
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

@Injectable()
export class PkgHdrAccountService {
  constructor(
    private readonly _telegramService: TelegramService,
    @InjectModel(PkgHdrAccount.name)
    private readonly _pkgHdrAccountModel: Model<PkgHdrAccountDocument>,
    @InjectModel(GameKey.name)
    private readonly _gameKeyModel: Model<GameKeyDocument>,
  ) {}

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
}
