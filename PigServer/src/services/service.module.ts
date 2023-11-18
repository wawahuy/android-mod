import { Module } from '@nestjs/common';
import { X67ServiceService } from './x67-service.service';
import { AsmService } from './asm.service';
import { TelegramService } from './telegram.service';
import { TelegramConfig } from 'src/configs/telegram.config';
import { SchemaModule } from 'src/schema/schema.module';
import { UploadConfig } from 'src/configs/upload.config';
import { UploadService } from './upload.service';

const services = [
  X67ServiceService,
  TelegramService,
  AsmService,
  UploadService,
];

const serviceImport = [TelegramConfig, UploadConfig];

@Module({
  imports: [SchemaModule],
  providers: [...services, ...serviceImport],
  exports: [...services],
})
export class ServiceModule {}
