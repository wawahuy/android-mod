import { IsAlphanumeric, MaxLength } from 'class-validator';

export class HdrUserDataRequest {
  @MaxLength(255)
  @IsAlphanumeric()
  mtkey: string;

  @MaxLength(255)
  @IsAlphanumeric()
  skey: string;

  @MaxLength(255)
  @IsAlphanumeric()
  uid: string;
}

export enum JobAdsType {
  Ads1 = 1,
  Ads2 = 2,
}

export class JobAdsData {
  id: string;
  type: JobAdsType;
}
