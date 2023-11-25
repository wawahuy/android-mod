import { Module } from '@nestjs/common';
import { X67GatewayService } from './x67-gateway.service';
import { AsmService } from './asm.service';
import { TelegramService } from './telegram.service';
import { TelegramConfig } from 'src/configs/telegram.config';
import { SchemaModule } from 'src/schema/schema.module';
import { UploadConfig } from 'src/configs/upload.config';
import { UploadService } from './upload.service';
import { PackageHdrService } from './package-hdr.service';
import { X67SenderService } from './x67-sender.service';
import { X67SessionService } from './x67-session.service';
import { PackageDtdService } from './package-dtd.service';

const services = [
  X67GatewayService,
  X67SenderService,
  X67SessionService,
  TelegramService,
  AsmService,
  UploadService,
  PackageHdrService,
  PackageDtdService,
];

const serviceImport = [TelegramConfig, UploadConfig];

@Module({
  imports: [SchemaModule],
  providers: [...services, ...serviceImport],
  exports: [...services],
})
export class ServiceModule {}
