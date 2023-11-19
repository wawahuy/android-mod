import { Injectable } from '@nestjs/common';
import { ConfigService } from '@nestjs/config';

@Injectable()
export class TelegramConfig {
  constructor(private readonly configService: ConfigService) {}

  getToken() {
    return this.configService.get<string>('TELEGRAM_BOT_TOKEN');
  }
}
