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
