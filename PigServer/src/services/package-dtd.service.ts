import { Injectable } from '@nestjs/common';
import * as md5 from 'md5';
import { IGamePackage } from 'src/interfaces/game-package';
import { WidgetMenuItem, ArgDataPushType } from 'src/x67-menu/config';
import { UploadService } from './upload.service';
import { TelegramService } from './telegram.service';

const menu = [];

const description = {
  versionHash: md5(JSON.stringify(menu)),
  menu,
};

@Injectable()
export class PackageDtdService implements IGamePackage {
  static readonly packageName = 'com.g4m.daitaydu';
  static readonly className = 'com.unity3d.player.UnityPlayerActivity';

  constructor(
    private readonly _uploadService: UploadService,
    private readonly _telegramService: TelegramService,
  ) {}

  getPackageName(): string {
    return PackageDtdService.packageName;
  }

  getClassName(): string {
    return PackageDtdService.className;
  }

  getMenuDescription() {
    return description;
  }
}
