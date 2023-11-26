import { Module } from '@nestjs/common';
import { X67GatewayService } from './x67-gateway.service';
import { AsmService } from './asm.service';
import { TelegramService } from './telegram.service';
import { TelegramConfig } from 'src/configs/telegram.config';
import { SchemaModule } from 'src/schema/schema.module';
import { UploadConfig } from 'src/configs/upload.config';
import { UploadService } from './upload.service';
import { PkgHdrService } from './pkg-hdr.service';
import { X67SenderService } from './x67-sender.service';
import { X67SessionService } from './x67-session.service';
import { GameConfigService } from './game-config.service';
import { GameKeyService } from './game-key.service';
import { PkgHdrAccountService } from './pkg-hdr-account.service';
import { PkgHdrAccountActiveService } from './pkg-hdr-account-active.service';
import { BullModule } from '@nestjs/bull';
import {
  QUEUE_ACCOUNT_ACTIVE,
  QUEUE_HDR_ADS_REWARD,
} from 'src/utils/constants';
import { BullConfig } from 'src/configs/bull.config';
import { PkgHdrAdsRewardProcessor } from './processor/hdr-ads-reward.processor';
import { AccountActiveProcessor } from './processor/account-active.processor';

const services = [
  X67GatewayService,
  X67SenderService,
  X67SessionService,
  TelegramService,
  AsmService,
  UploadService,
  PkgHdrService,
  PkgHdrAccountService,
  PkgHdrAccountActiveService,
  GameConfigService,
  GameKeyService,

  AccountActiveProcessor,
  PkgHdrAdsRewardProcessor,
];

const serviceImport = [TelegramConfig, UploadConfig];

@Module({
  imports: [
    SchemaModule,
    BullModule.forRootAsync({ useClass: BullConfig }),
    BullModule.registerQueue(
      { name: QUEUE_HDR_ADS_REWARD },
      { name: QUEUE_ACCOUNT_ACTIVE },
    ),
  ],
  providers: [...services, ...serviceImport],
  exports: [...services],
})
export class ServiceModule {}
