import { Injectable, Logger } from '@nestjs/common';
import X67Server from 'src/x67-server/x67-server';
import * as fs from 'fs';
import * as path from 'path';
import * as moment from 'moment';
import { validate } from 'class-validator';
import { ConfigService } from '@nestjs/config';
import { TelegramService } from './telegram.service';
import X67Socket from 'src/x67-server/x67-socket';
import { CommandLoginRequest } from 'src/dtos/x67-server.dto';
import { IGamePackage } from 'src/interfaces/game-package';
import { PackageHdrService } from './package-hdr.service';
import { GameKey, GameKeyDocument } from 'src/schema/game-key.schema';
import { InjectModel } from '@nestjs/mongoose';
import { Model } from 'mongoose';
import { plainToClass } from 'class-transformer';

@Injectable()
export class X67ServiceService {
  _logger = new Logger('X67ServiceService');
  _server: X67Server;
  _packageMapping: {
    [key: string]: IGamePackage;
  } = {};

  constructor(
    private readonly _configService: ConfigService,
    private readonly _telegramService: TelegramService,
    private readonly _packageHdrService: PackageHdrService,
    @InjectModel(GameKey.name)
    private _gameKeyModel: Model<GameKeyDocument>,
  ) {
    this.init();
    fs.readFileSync(
      path.join(this._configService.get('FOLDER_LIBSO'), 'libpigmodij.so'),
    );
  }

  init() {
    const port = 1235;
    this._server = new X67Server();
    this._server.listen(port);

    // package mapping
    this._packageMapping = {
      [PackageHdrService.packageName]: this._packageHdrService,
    };

    // route
    this._server.eventClients.on('login', this.onLogin.bind(this));
  }

  private async onLogin(data: CommandLoginRequest, socket: X67Socket) {
    data = plainToClass(CommandLoginRequest, data);
    const errors = await validate(data);
    if (errors && errors.length) {
      const msg = Object.values(errors[0]?.constraints)?.[0];
      this.sendLoginFailed(socket, msg || 'Internal');
      this._logger.error(errors);
      return;
    }

    const telegramMsg = [
      `${data.package} (LOGIN)`,
      `Key: ${data.key}`,
      `IsTrial: ${data.trial}`,
      `Version: ${data.version}`,
      `MAC: ${data.mac}`,
    ];

    const pkg = data.package;
    if (!pkg) {
      this._logger.error('package not found!');
      return;
    }

    let msgError: string | null;
    let needUpdate = false;
    let gameKey: GameKeyDocument;
    if (!data.trial) {
      gameKey = await this._gameKeyModel.findOne({
        key: data.key,
        package: data.package,
      });

      if (!gameKey) {
        msgError = 'Key khong ton tai';
      } else if (gameKey.expiredAt && moment().isAfter(gameKey.expiredAt)) {
        msgError = 'Key het han';
      } else {
        let newDevice = false;
        const devices = (gameKey.devices = gameKey.devices || []);
        if (!devices.includes(data.mac)) {
          newDevice = true;
        }
        if (newDevice) {
          if (devices.length < gameKey.maximumDevice) {
            devices.push(data.mac);
            needUpdate = true;
            telegramMsg.push(`Push devices: ${data.mac}`);
          } else {
            msgError = 'So luong thiet bi toi da';
          }
        }
      }
    }

    if (!msgError) {
      if (gameKey && !gameKey.expiredAt) {
        needUpdate = true;
        gameKey.expiredAt = moment().add(gameKey.amountSec, 'second').toDate();
        telegramMsg.push(`Expired at: ${moment(gameKey.expiredAt).format()}`);
      }
    } else {
      telegramMsg.push(msgError);
    }

    if (gameKey && needUpdate) {
      await gameKey.save();
    }

    if (!data.re) {
      this._telegramService.sendMessage(telegramMsg.join('\r\n'));
    }

    if (!msgError) {
      const service = this._packageMapping[pkg];
      service.onLogin(data, socket);
    } else {
      this.sendLoginFailed(socket, msgError);
    }
  }

  sendLoginFailed(socket: X67Socket, msg: string) {
    socket.command('is-login', {
      isLogin: false,
    });
    socket.command('sys-message', {
      msg,
      color: [255, 0, 0, 255],
    });
  }
}
