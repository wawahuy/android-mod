import { InjectQueue, Process, Processor } from '@nestjs/bull';
import { Job, Queue } from 'bull';
import { JobAdsData } from 'src/dtos/pkg-hdr.dto';
import {
  JOB_HDR_ADS_REWARD_PREFIX_ADS1,
  JOB_HDR_ADS_REWARD_PREFIX_ADS2,
  QUEUE_HDR_ADS_REWARD,
} from 'src/utils/constants';
import { PkgHdrAccountService } from '../pkg-hdr-account.service';
import { PkgHdrRpcGameService } from '../pkg-hdr-rpc-game';
import { Logger } from '@nestjs/common';
import * as moment from 'moment';
import { TelegramService } from '../telegram.service';
import { AxiosError } from 'axios';
import { AnyKeysHdrAccount } from 'src/schema/pkg-hdr-account.schema';

@Processor(QUEUE_HDR_ADS_REWARD)
export class PkgHdrAdsRewardProcessor {
  private readonly _logger = new Logger('PkgHdrAdsRewardProcessor');
  readonly jobAdsPrefix2JobType = {
    [JOB_HDR_ADS_REWARD_PREFIX_ADS1]: 0,
    [JOB_HDR_ADS_REWARD_PREFIX_ADS2]: 1,
  };

  constructor(
    private readonly _telegramService: TelegramService,
    private readonly _pkgHdrAccountService: PkgHdrAccountService,
    private readonly _pkgHdrRpcGameService: PkgHdrRpcGameService,
    @InjectQueue(QUEUE_HDR_ADS_REWARD)
    private readonly _hdrAdsRewardQueue: Queue<JobAdsData>,
  ) {}

  @Process({ name: '*', concurrency: 1 })
  async transcode(job: Job<JobAdsData>) {
    const jobNameSplit = job.name.split('_');
    const jobAdsPrefix = jobNameSplit[0] + '_';

    // valid account
    const data = job.data;
    const account = await this._pkgHdrAccountService.getUnexpiredAccount(
      data.id,
    );
    if (!account) {
      this._telegramService.sendMessage(
        `Remove job - uid: ${data.id} - account expired`,
      );
      return false;
    }

    // reward
    const reqRewardData =
      this._pkgHdrRpcGameService.getAdDataRequestFromAccount(
        data.type,
        account,
      );
    const resRewardData = await this._pkgHdrRpcGameService
      .rewardAdGiftBox(reqRewardData)
      .then((rs) => rs.data)
      .catch((err: AxiosError) => err);

    if (resRewardData instanceof AxiosError) {
      const status = resRewardData.response.status;
      if ((status >= 500 && status <= 599) || status === 403) {
        this._telegramService.sendMessage(
          `failed reward ${account.uid}, status: ${status}, attempt: ${
            account.httpFailed || 0
          }`,
        );
        await this._pkgHdrAccountService.increaseHttpFailed(account._id, 1);
      }
      return Promise.reject(resRewardData);
    }

    const $inc: AnyKeysHdrAccount = {};
    const $set: AnyKeysHdrAccount = {
      httpFailed: 0,
    };

    if (!resRewardData._d) {
      this._telegramService.sendMessage(
        `Remove job - uid: ${data.id} - rpc error`,
      );
      return false;
    }

    let rewardNextTime: number;
    const _d = resRewardData._d;
    const giftIndex = this.jobAdsPrefix2JobType[jobAdsPrefix];
    if (_d.ret == -1) {
      const resPopData = await this._pkgHdrRpcGameService
        .popAdGiftBox(reqRewardData)
        .then((rs) => rs.data);
      const d = resPopData._d.data.extra;
      rewardNextTime = d.giftBoxInfo[giftIndex].giftBoxCDTime;
    } else {
      const d = _d.data;
      rewardNextTime = d.giftBoxInfo[giftIndex].giftBoxCDTime;

      // statistic
      let isNewDay = false;
      if (
        !account.stsDayTime ||
        !moment(account.stsDayTime).isSame(moment().startOf('day'))
      ) {
        isNewDay = true;
        $set.stsDayTime = moment().startOf('day').toDate();
      }
      d.reward.forEach((r) => {
        switch (r.type) {
          case 'tili':
            $inc.stsAllTili = r.num;
            if (isNewDay) {
              $set.stsDayTili = r.num;
            } else {
              $inc.stsDayTili = r.num;
            }
            break;
          case 'snowBall':
            $inc.stsAllSnowball = r.num;
            if (isNewDay) {
              $set.stsDaySnowball = r.num;
            } else {
              $inc.stsDaySnowball = r.num;
            }
            break;
        }
      });
    }

    if (data.type == 'adGiftBox1') {
      account.nextTimeAd1 = moment().add(rewardNextTime, 'second').toDate();
    }
    if (data.type == 'adGiftBox2') {
      account.nextTimeAd2 = moment().add(rewardNextTime, 'second').toDate();
    }
    await this._pkgHdrAccountService.updateSetInc(account._id, $set, $inc);

    this._logger.warn(
      `add job accId: ${data.id} - ${data.type} - CD: ${rewardNextTime}`,
    );
    this._pkgHdrAccountService.addJobAdsReward(
      jobAdsPrefix,
      data.id,
      data.type,
      (rewardNextTime + 1) * 1000,
    );
  }
}
