import { Injectable } from '@nestjs/common';
import { ConfigService } from '@nestjs/config';

@Injectable()
export class UploadConfig {
  constructor(private readonly configService: ConfigService) {}

  getFolderLibso() {
    return this.configService.get<string>('FOLDER_LIBSO');
  }

  getWatchDebounce() {
    return Number(this.configService.get<number>('LIBSO_WATCH_DEBOUNCE'));
  }
}
