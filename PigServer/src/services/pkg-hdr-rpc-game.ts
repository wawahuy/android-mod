import { Injectable } from '@nestjs/common';
import axios, {
  Axios,
  AxiosInstance,
  AxiosRequestConfig,
  AxiosResponse,
} from 'axios';
import {
  HdrGameRpcAds1RewardType,
  HdrGameRpcAds2RewardType,
  HdrGameRpcAdsPopRequest,
  HdrGameRpcAdsPopResponse,
  HdrGameRpcAdsRewardRequest,
  HdrGameRpcAdsRewardResponse,
  HdrGameRpcAdsType,
  HdrGameRpcBaseRequest,
} from 'src/dtos/pkg-hdr.dto';
import { GameConfigService } from './game-config.service';
import { PkgHdrService } from './pkg-hdr.service';
import { PkgHdrAccountDocument } from 'src/schema/pkg-hdr-account.schema';

@Injectable()
export class PkgHdrRpcGameService {
  private readonly client: AxiosInstance;
  private readonly baseURL =
    'https://d2fd20abim5npz.cloudfront.net/planetpigth/m';

  private packageVersion: string;
  private packageVersionGetTime: number;

  constructor(private readonly gameConfigService: GameConfigService) {
    this.client = axios.create({
      baseURL: this.baseURL,
    });
    this.client.interceptors.request.use(this.interceptRequest.bind(this));
  }

  getAdDataRequestFromAccount(
    name: HdrGameRpcAdsType,
    account: PkgHdrAccountDocument,
  ): HdrGameRpcAdsRewardRequest {
    return {
      name,
      _mtkey: account.mtkey,
      _skey: account.skey,
      _uid: account.uid,
      deviceToken: account.uid,
    };
  }

  rewardAdGiftBox(data: HdrGameRpcAdsRewardRequest) {
    const uri = '/videoAD/reward/';
    return this.client.post<
      HdrGameRpcAdsRewardResponse<
        HdrGameRpcAds1RewardType | HdrGameRpcAds2RewardType
      >
    >(uri, data);
  }

  popAdGiftBox(data: HdrGameRpcAdsPopRequest) {
    const uri = '/videoAD/pop/';
    return this.client.post<HdrGameRpcAdsPopResponse>(uri, data);
  }

  private async getBaseRequest(): Promise<HdrGameRpcBaseRequest> {
    const version = await this.getPackageVersion();
    return {
      _version: version,
      _device: 'android',
      _channel: 'android',
      _lan: 'vi_vn',
      _reversion: version,
    };
  }

  private async getPackageVersion() {
    if (
      !this.packageVersion ||
      new Date().getTime() - this.packageVersionGetTime > 60 * 60 * 1000
    ) {
      this.packageVersion = await this.gameConfigService.getVersion(
        PkgHdrService.packageName,
      );
    }
    return this.packageVersion;
  }

  private async interceptRequest(
    value: AxiosRequestConfig<any>,
  ): Promise<AxiosRequestConfig<any>> {
    const headers = value.headers;
    const method = value.method;
    headers[
      'User-Agent'
    ] = `Dalvik/2.1.0 (Linux; U; Android 10; CPH1819 Build/QP1A.190711.020)`;
    if (method && ['post', 'put'].includes(method)) {
      headers['Content-Type'] = 'application/x-www-form-urlencoded';
      value.data = {
        ...(await this.getBaseRequest()),
        ...value.data,
      };
    }
    return value;
  }
}
