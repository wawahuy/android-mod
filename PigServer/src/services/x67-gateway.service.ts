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
import { X67SenderService } from './x67-sender.service';
import { X67SessionService } from './x67-session.service';
import { UploadService } from './upload.service';
import { createHashMd5 } from 'src/utils/ws';

@Injectable()
export class X67GatewayService {
  _logger = new Logger('X67GatewayService');
  _server: X67Server;
  _packageMapping: {
    [key: string]: IGamePackage;
  } = {};

  constructor(
    private readonly _configService: ConfigService,
    private readonly _telegramService: TelegramService,
    private readonly _sender: X67SenderService,
    private readonly _session: X67SessionService,
    private readonly _uploadService: UploadService,
    private readonly _packageHdrService: PackageHdrService,
    @InjectModel(GameKey.name)
    private _gameKeyModel: Model<GameKeyDocument>,
  ) {
    this.init();
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
    const onAuthorizatedBinded = this.onAuthorizated.bind(this);
    const listener = this._server.eventClients;
    listener.on('login', this.onLogin.bind(this));
    listener.on('get-menu', onAuthorizatedBinded(this.onGetMenu));
    listener.on('get-lib-ij', onAuthorizatedBinded(this.onGetLibIj));
    listener.on('menu-action', onAuthorizatedBinded(this.onMenuAction));
  }

  private async onLogin(data: CommandLoginRequest, socket: X67Socket) {
    data = plainToClass(CommandLoginRequest, data);
    const errors = await validate(data);
    if (errors && errors.length) {
      this._sender.sendMessageValidationError(socket, errors[0]);
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
      const buffer = this._uploadService.getLibIjBuffer(data.package);
      if (buffer) {
        const libIjHash = createHashMd5(buffer);
        this._session.set(socket, 'package', data.package);
        this._sender.sendLoginSuccess(socket, {
          isLogin: true,
          libIjHash,
        });
        return;
      }

      msgError = 'Loi file!';
    }

    this._sender.sendLoginFailed(socket, msgError);
  }

  private onAuthorizated(
    next: (data: CommandLoginRequest, socket: X67Socket) => void,
  ) {
    const _next = next.bind(this);
    return (data: CommandLoginRequest, socket: X67Socket) => {
      if (this._session.has(socket)) {
        _next(data, socket);
      }
    };
  }

  private async onGetMenu(data: any, socket: X67Socket) {
    const pkg = this._session.get(socket, 'package');
    const service = this._packageMapping[pkg];
    if (service) {
      this._sender.sendMenu(socket, service.getMenuDescription());
    }
  }

  private async onGetLibIj(data: any, socket: X67Socket) {
    const pkg = this._session.get(socket, 'package');
    const buffer = this._uploadService.getLibIjBuffer(pkg);
    socket.sendLibIj(buffer);
    this._telegramService.sendMessage(`Send libij ${pkg}`);
  }

  private async onMenuAction(data: any, socket: X67Socket) {}
}
