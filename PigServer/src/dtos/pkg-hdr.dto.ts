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

export type HdrGameRpcAdsType = 'adGiftBox1' | 'adGiftBox2';

export class JobAdsData {
  id: string;
  type: HdrGameRpcAdsType;
}

export type HdrGameRpcAds1RewardType =
  | 'money'
  | 'tili'
  | 'speaker'
  | 'snowBall'
  | 'petFeedEnergy';
export type HdrGameRpcAds2RewardType =
  | 'money'
  | 'tili'
  | 'miniShield'
  | 'tongjiling';

export class HdrGameRpcAdsRewardResponse<
  T = HdrGameRpcAds1RewardType | HdrGameRpcAds2RewardType,
> {
  _t: number;
  _s: any[];
  _d: {
    ret: 0 | -1;
    data: {
      popCount: number;
      rewardCount: number;
      ad: number;
      share: number;
      id: number;
      name: HdrGameRpcAdsType;
      ret: 0 | -1;
      reward: [{ type: T; num: number }];
      amount: [{ type: T; num: number }];
      giftBoxInfo: [
        {
          giftBoxID: 'adGiftBox1';
          giftBoxCDTime: number;
          giftBoxRewardNum: number;
        },
        {
          giftBoxID: 'adGiftBox2';
          giftBoxCDTime: number;
          giftBoxRewardNum: number;
        },
      ];
    };
  };
}

export class HdrGameRpcBaseRequest {
  _version: string;
  _device: string;
  _channel: string;
  _lan: string;
  _reversion: string;
}

export class HdrGameRpcAdsRewardRequest {
  name: HdrGameRpcAdsType;
  _mtkey: string;
  _skey: string;
  _uid: string;
  deviceToken: string;
}

export class HdrGameRpcAdsPopRequest {
  name: HdrGameRpcAdsType;
  _mtkey: string;
  _skey: string;
  _uid: string;
  deviceToken: string;
}

export class HdrGameRpcAdsPopResponse {
  _t: number;
  _s: any[];
  _d: {
    ret: 0 | -1;
    data: {
      popCount: number;
      rewardCount: number;
      extra: {
        giftBoxInfo: [
          {
            giftBoxID: 'adGiftBox1';
            giftBoxCDTime: number;
            giftBoxRewardNum: number;
          },
          {
            giftBoxID: 'adGiftBox2';
            giftBoxCDTime: number;
            giftBoxRewardNum: number;
          },
        ];
      };
    };
  };
}
