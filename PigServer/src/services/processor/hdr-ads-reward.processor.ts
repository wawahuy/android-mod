import { Processor } from '@nestjs/bull';
import { QUEUE_HDR_ADS_REWARD } from 'src/utils/constants';

@Processor(QUEUE_HDR_ADS_REWARD)
export class PkgHdrAdsRewardProcessor {}
