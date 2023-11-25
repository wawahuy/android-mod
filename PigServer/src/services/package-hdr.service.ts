import { Injectable } from '@nestjs/common';
import * as md5 from 'md5';
import { IGamePackage } from 'src/interfaces/game-package';
import { WidgetMenuItem, ArgDataPushType } from 'src/x67-menu/config';
import { UploadService } from './upload.service';
import { TelegramService } from './telegram.service';

const menu = [
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

const description = {
  versionHash: md5(JSON.stringify(menu)),
  menu,
};

@Injectable()
export class PackageHdrService implements IGamePackage {
  static readonly packageName = 'com.aladinfun.clashofsky_th_pig';
  static readonly className = 'com.aladinfun.piggyboom.MainAppActivity';

  constructor(
    private readonly _uploadService: UploadService,
    private readonly _telegramService: TelegramService,
  ) {}

  getPackageName(): string {
    return PackageHdrService.packageName;
  }

  getClassName(): string {
    return PackageHdrService.className;
  }

  getMenuDescription() {
    return description;
  }
}
