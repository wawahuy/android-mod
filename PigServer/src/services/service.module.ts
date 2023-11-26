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
];

const serviceImport = [TelegramConfig, UploadConfig];

@Module({
  imports: [SchemaModule],
  providers: [...services, ...serviceImport],
  exports: [...services],
})
export class ServiceModule {}
