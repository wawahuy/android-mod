import { Injectable, Logger } from '@nestjs/common';
import { TelegramConfig } from 'src/configs/telegram.config';
import * as TelegramBot from 'node-telegram-bot-api';
import { InjectModel } from '@nestjs/mongoose';
import { TelegramBotDocument } from 'src/schema/telegram.schema';
import { Model } from 'mongoose';
import { UploadService } from './upload.service';

@Injectable()
export class TelegramService {
  private _logger = new Logger('TelegramService');
  private _bot: TelegramBot;

  private readonly _msgLogin = /^\/login(\s+)(?<password>(.{6,32}))$/;
  private readonly _msgUploadLibIj = /^\/libij(\s+)(?<package>(.*))$/;

  private _msgNoLoggedMapping = [];
  private _msgLoggedMapping = [];

  constructor(
    private readonly _uploadService: UploadService,
    private readonly _telegramConfig: TelegramConfig,
    @InjectModel(TelegramBot.name)
    private _telegramBotModel: Model<TelegramBotDocument>,
  ) {
    this._logger.warn('Connect telegram!');

    this._msgNoLoggedMapping = [
      { match: this._msgLogin, callback: this.onLogin.bind(this) },
    ];
    this._msgLoggedMapping = [
      { match: this._msgUploadLibIj, callback: this.onUploadLibIj.bind(this) },
    ];

    this._bot = new TelegramBot(this._telegramConfig.getToken(), {
      polling: true,
    });
    this._bot.on('message', this.onMessage.bind(this));
  }

  async onMessage(msg: TelegramBot.Message) {
    this._msgNoLoggedMapping.forEach((mapping) => {
      if (
        (msg.text && msg.text.match(mapping.match)) ||
        (msg.caption && msg.caption.match(mapping.match))
      ) {
        mapping.callback(msg);
      }
    });

    if (await this.validate(msg)) {
      this._msgLoggedMapping.forEach((mapping) => {
        if (
          (msg.text && msg.text.match(mapping.match)) ||
          (msg.caption && msg.caption.match(mapping.match))
        ) {
          mapping.callback(msg);
        }
      });
    }
  }

  async validate(msg: TelegramBot.Message) {
    const chat = msg.chat;
    const chatDocument = await this._telegramBotModel.findOne({
      chatId: chat.id.toString(),
    });
    return !!chatDocument;
  }

  async onLogin(msg: TelegramBot.Message) {
    const m = this._msgLogin.exec(msg.text);
    if (!m) {
      return;
    }

    const password = m.groups['password'];
    if (password != 'adadad') {
      this.sendMessage('failed!');
      return;
    }

    this._logger.log(`Login ${msg.chat.username}: ${password}`);
    const chat = msg.chat;
    const chatDocument = await this._telegramBotModel.findOne({
      chatId: chat.id.toString(),
      username: chat.username,
    });
    if (!chatDocument) {
      this._logger.log(`Insert chat ${chat.username}, chatId: ${chat.id}`);
      await this._telegramBotModel.insertMany([
        {
          chatId: chat.id.toString(),
          username: chat.username,
        },
      ]);
      this.sendMessage(`${chat.username} Logged!`);
    } else {
      this.sendMessage(`${chat.username} re-logged!`);
    }
  }

  async sendMessage(msg: string) {
    const chatDocuments = await this._telegramBotModel.find({});
    chatDocuments.forEach((chatDocument) => {
      this._bot.sendMessage(chatDocument.chatId, msg);
    });
  }

  async onUploadLibIj(msg: TelegramBot.Message) {
    const m = this._msgUploadLibIj.exec(msg.caption);
    if (!m) {
      return;
    }

    const strPackage = m.groups['package'];
    if (!strPackage) {
      return;
    }

    const document = msg.document;
    if (!document || !document.file_id) {
      return;
    }

    const s = this._bot.getFileStream(document.file_id);
    this._uploadService
      .uploadLibIj(strPackage, s)
      .on('finish', () => {
        this.sendMessage(`New libij ${strPackage}`);
      })
      .on('error', (err) => {
        this.sendMessage(`New libij error ${err.message}`);
      });
  }
}
