import { Injectable } from '@nestjs/common';
import { InjectModel } from '@nestjs/mongoose';
import * as moment from 'moment';
import { Model } from 'mongoose';
import {
  PkgHdrAccount,
  PkgHdrAccountDocument,
} from 'src/schema/pkg-hdr-account.schema';
import { TelegramService } from './telegram.service';

@Injectable()
export class PkgHdrAccountService {
  constructor(
    private readonly _telegramService: TelegramService,
    @InjectModel(PkgHdrAccount.name)
    private readonly _pkgHdrAccountModel: Model<PkgHdrAccountDocument>,
  ) {}

  async newOrUpdateAccount(uid: string, mtkey: string, skey: string) {
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
    hdrAccount.mtkey = mtkey;
    hdrAccount.skey = skey;
    await hdrAccount.save();
    return result;
  }
}
