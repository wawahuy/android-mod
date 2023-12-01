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
      .then((rs) => rs.data);

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
      console.log(d);
      rewardNextTime = d.giftBoxInfo[giftIndex].giftBoxCDTime;

      // statistic
      let isNewDay = false;
      if (
        !account.stsDayTime ||
        !moment(account.stsDayTime).isSame(moment().startOf('day'))
      ) {
        isNewDay = true;
        account.stsDayTime = moment().startOf('day').toDate();
      }
      d.reward.forEach((r) => {
        switch (r.type) {
          case 'tili':
            account.stsAllTili = (account.stsAllTili || 0) + r.num;
            if (isNewDay) {
              account.stsDayTili = r.num;
            } else {
              account.stsDayTili = (account.stsDayTili || 0) + r.num;
            }
            break;
          case 'snowBall':
            account.stsAllSnowball = (account.stsAllSnowball || 0) + r.num;
            if (isNewDay) {
              account.stsDaySnowball = r.num;
            } else {
              account.stsDaySnowball = (account.stsDaySnowball || 0) + r.num;
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
    await this._pkgHdrAccountService.updateNonDocument(account);

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
