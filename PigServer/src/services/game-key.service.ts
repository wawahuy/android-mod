import { Injectable } from '@nestjs/common';
import { InjectModel } from '@nestjs/mongoose';
import { Model } from 'mongoose';
import { GameKey, GameKeyDocument } from 'src/schema/game-key.schema';
import * as moment from 'moment';
import { TelegramService } from './telegram.service';

@Injectable()
export class GameKeyService {
  constructor(
    private _telegramService: TelegramService,
    @InjectModel(GameKey.name)
    private _gameKeyModel: Model<GameKeyDocument>,
  ) {}

  async validateOrActive(pkg: string, key: string, mac: string) {
    const telegramMsg = [`Game: ${pkg}`, `Key: ${key}`];
    const gameKey = await this._gameKeyModel.findOne({
      key: key,
      package: pkg,
    });

    const result = {
      model: gameKey,
      error: null as null | string,
    };

    if (!gameKey) {
      result.error = 'Key khong ton tai';
      return result;
    }

    if (gameKey.expiredAt && moment().isAfter(gameKey.expiredAt)) {
      result.error = 'Key het han';
      return result;
    }

    let needUpdate = false;
    let newDevice = false;
    gameKey.devices = gameKey.devices || [];
    if (!gameKey.devices.includes(mac)) {
      newDevice = true;
    }
    if (newDevice) {
      if (gameKey.devices.length < gameKey.maximumDevice) {
        needUpdate = true;
        gameKey.devices.push(mac);
        telegramMsg.push(`Push devices: ${mac}`);
      } else {
        result.error = 'So luong thiet bi toi da';
        return result;
      }
    }

    if (!gameKey.expiredAt) {
      needUpdate = true;
      gameKey.expiredAt = moment().add(gameKey.amountSec, 'second').toDate();
      telegramMsg.push(`Expired at: ${moment(gameKey.expiredAt).format()}`);
    }

    if (needUpdate) {
      await gameKey.save();
      this._telegramService.sendMessage(telegramMsg.join('\r\n'));
    }

    return result;
  }
}
