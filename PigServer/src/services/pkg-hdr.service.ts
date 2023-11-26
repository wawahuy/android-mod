import { Injectable, Logger } from '@nestjs/common';
import * as md5 from 'md5';
import { IGamePackage } from 'src/interfaces/game-package';
import { WidgetMenuItem, ArgDataPushType } from 'src/x67-menu/config';
import { UploadService } from './upload.service';
import { TelegramService } from './telegram.service';
import X67Socket from 'src/x67-server/x67-socket';
import { X67SessionService } from './x67-session.service';
import { X67SenderService } from './x67-sender.service';
import { HdrUserDataRequest } from 'src/dtos/pkg-hdr.dto';
import {
  PkgHdrAccount,
  PkgHdrAccountDocument,
} from 'src/schema/pkg-hdr-account.schema';
import { Model } from 'mongoose';
import { InjectModel } from '@nestjs/mongoose';
import * as moment from 'moment';
import { plainToClass } from 'class-transformer';
import { validate } from 'class-validator';

const globalMenuTrial = [
  {
    label: 'System',
    action: 'system',
    items: [
      {
        label: 'Dang chut nhe...',
        type: WidgetMenuItem.Text,
      },
    ],
  },
];

const globalMenu = [
  {
    label: 'Nang Luong',
    action: 'nangluong',
    items: [
      {
        label: 'Trang thai',
        valueDefault: true,
        type: WidgetMenuItem.ServerSwitch,
        action: 'autoNangLuong',
        items: [
          {
            label: 'Test',
            type: WidgetMenuItem.Text,
          },
        ],
      },
      {
        type: WidgetMenuItem.Call,
        action: 'userData',
        delay: 10000,
      },
    ],
  },
  {
    label: 'Cay Thong',
    action: 'cayThong',
    items: [
      {
        label: 'Auto trung',
        valueDefault: true,
        type: WidgetMenuItem.Switch,
        action: 'autoTrung',
      },
      {
        label: 'Ban nhanh',
        valueDefault: true,
        type: WidgetMenuItem.Switch,
        action: 'banNhanh',
      },
      {
        label: 'Ban 1 cham',
        valueDefault: true,
        type: WidgetMenuItem.Switch,
        action: 'ban1Cham',
      },
      {
        label: 'Nap dan nhanh',
        valueDefault: true,
        type: WidgetMenuItem.Switch,
        action: 'napDanNhanh',
        items: [
          {
            arg: 'count',
            label: 'So luong (vien/1 lan)',
            type: WidgetMenuItem.InputInt,
            valueDefault: 1,
            valueMin: 1,
            valueMax: 8,
            valueStep: 1,
            valueStepFast: 2,
            valueWidth: 250,
          },
          {
            arg: 'speed',
            label: 'Toc do (giay)',
            type: WidgetMenuItem.SliderFloat,
            valueDefault: 0.25,
            valueMin: 0,
            valueMax: 1,
            valueStep: 0.1,
            valueStepFast: 0.2,
            valueWidth: 250,
          },
        ],
      },
    ],
  },
  {
    label: 'Vong xoay',
    action: 'bet',
    items: [
      {
        label: 'Vong xoay',
        valueDefault: false,
        type: WidgetMenuItem.Switch,
        action: 'vongXoay',
        items: [
          {
            type: WidgetMenuItem.SliderInt,
            arg: 'count',
            valueDefault: 10,
            valueMin: 1,
            valueMax: 2000,
            valueStep: 10,
            valueStepFast: 50,
            valueWidth: 400,
            hashId: 'vongXoay.count.slider',
          },
          {
            label: 'x100',
            valueDefault: false,
            type: WidgetMenuItem.Button,
            pushArgs: [['count', 100]],
            pushArgsType: ArgDataPushType.Focus,
            callReMapArgsHashId: ['vongXoay.count.slider'],
            sameLine: true,
          },
          {
            label: 'x500',
            valueDefault: false,
            type: WidgetMenuItem.Button,
            pushArgs: [['count', 500]],
            pushArgsType: ArgDataPushType.Focus,
            callReMapArgsHashId: ['vongXoay.count.slider'],
            sameLine: true,
          },
          {
            label: 'x1000',
            valueDefault: false,
            type: WidgetMenuItem.Button,
            pushArgs: [['count', 1000]],
            pushArgsType: ArgDataPushType.Focus,
            callReMapArgsHashId: ['vongXoay.count.slider'],
          },
        ],
      },
    ],
  },
];

@Injectable()
export class PkgHdrService implements IGamePackage {
  static readonly packageName = 'com.aladinfun.clashofsky_th_pig';
  static readonly className = 'com.aladinfun.piggyboom.MainAppActivity';
  readonly trialSecond = 3 * 24 * 60 * 60;

  _logger = new Logger('PkgHdrService');

  constructor(
    private readonly _uploadService: UploadService,
    private readonly _telegramService: TelegramService,
    private readonly _session: X67SessionService,
    private readonly _sender: X67SenderService,
    @InjectModel(PkgHdrAccount.name)
    private _pkgHdrAccountModel: Model<PkgHdrAccountDocument>,
  ) {}

  canTrial() {
    return true;
  }

  onTrial(socket: X67Socket) {
    const timeout = setTimeout(
      () => {
        this._sender.sendDestroy(socket);
        this._session.remove(socket, 'timeoutTrial');
        this._session.delete(socket);
      },
      30 * 60 * 1000,
    );
    this._session.set(socket, 'timeoutTrial', timeout);
  }

  getPackageName(): string {
    return PkgHdrService.packageName;
  }

  getClassName(): string {
    return PkgHdrService.className;
  }

  getMenuDescription(socket: X67Socket) {
    const isTrial = this._session.get(socket, 'trial');
    const timeoutTrial = this._session.get(socket, 'timeoutTrial');
    if (isTrial && timeoutTrial) {
      return this.buildMenu(globalMenuTrial);
    }
    return this.buildMenu(globalMenu);
  }

  getRoutes(): { [key: string]: () => void } {
    return {
      'hdr:user-data': this.actionUserData.bind(this),
    };
  }

  async actionUserData(data: HdrUserDataRequest, socket: X67Socket) {
    data = plainToClass(HdrUserDataRequest, data);
    const errors = await validate(data);
    if (errors && errors.length) {
      this._logger.error(errors);
      return;
    }

    const telegramMsg: string[] = [];
    let allowTrial = false;
    let hdrAccount = await this._pkgHdrAccountModel.findOne({
      uid: data.uid,
    });
    if (!hdrAccount) {
      allowTrial = true;
      hdrAccount = await this._pkgHdrAccountModel.create({});
      hdrAccount.uid = data.uid;
      telegramMsg.push(`New HDR account: ${data.uid}`);
    } else {
      if (
        !hdrAccount.trialExpired ||
        moment(hdrAccount.trialExpired).isAfter(moment())
      ) {
        allowTrial = true;
      }
    }
    hdrAccount.mtkey = data.mtkey;
    hdrAccount.skey = data.skey;

    const isTrial = this._session.get(socket, 'trial');
    if (isTrial) {
      const timeoutTrial = this._session.get(socket, 'timeoutTrial');
      if (timeoutTrial) {
        clearTimeout(timeoutTrial);
        this._session.remove(socket, 'timeoutTrial');

        if (!hdrAccount.trialExpired) {
          telegramMsg.push(`Start trial HDR account: ${data.uid}`);
          hdrAccount.trialExpired = moment()
            .add(this.trialSecond, 'second')
            .toDate();
        }

        if (allowTrial) {
          this._sender.sendMenu(socket, this.buildMenu(globalMenu));
        } else {
          telegramMsg.push(`Denide trial HDR account: ${data.uid}`);
          this._sender.sendMessageError(socket, 'Ban da het han dung thu');
          this._sender.sendDestroy(socket);
          this._session.delete(socket);
        }
      }
    }

    hdrAccount.save();
    if (telegramMsg.length) {
      this._telegramService.sendMessage(telegramMsg.join('\r\n'));
    }
  }

  private buildMenu(menu: any) {
    const description = {
      versionHash: md5(JSON.stringify(menu)),
      menu,
    };
    return description;
  }
}
