import { Processor, Process, InjectQueue } from '@nestjs/bull';
import Bull, { Job, Queue } from 'bull';
import {
  JOB_ACCOUNT_ACTIVE_HDR,
  QUEUE_ACCOUNT_ACTIVE,
} from 'src/utils/constants';
import { PkgHdrAccountService } from '../pkg-hdr-account.service';

@Processor(QUEUE_ACCOUNT_ACTIVE)
export class AccountActiveProcessor {
  constructor(
    private readonly _pkgHdrAccountService: PkgHdrAccountService,
    @InjectQueue(QUEUE_ACCOUNT_ACTIVE)
    private readonly accountActiveQueue: Queue<any>,
  ) {
    this.addRepeatHdrAccountActive();
  }

  async addRepeatHdrAccountActive() {
    const cron = '*/30 * * * * *';
    await this.accountActiveQueue.removeRepeatable(JOB_ACCOUNT_ACTIVE_HDR, {
      cron,
    });

    return await this.accountActiveQueue.add(JOB_ACCOUNT_ACTIVE_HDR, null, {
      repeat: { cron },
      removeOnComplete: true,
    });
  }

  @Process({ name: JOB_ACCOUNT_ACTIVE_HDR, concurrency: 1 })
  transcode(job: Job<any>) {
    return this._pkgHdrAccountService.updateAccountActive(job);
  }
}
