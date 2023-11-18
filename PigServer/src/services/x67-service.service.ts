import { Injectable, Logger } from '@nestjs/common';
import X67Server from 'src/x67-server/x67-server';
import * as fs from 'fs';
import * as path from 'path';
import { ConfigService } from '@nestjs/config';
import { TelegramService } from './telegram.service';
import X67Socket from 'src/x67-server/x67-socket';
import { CommandLoginRequest } from 'src/dtos/x67-server.dto';
import { IGamePackage } from 'src/interfaces/game-package';
import { PackageHdrService } from './package-hdr.service';
import { GameKey, GameKeyDocument } from 'src/schema/game-key.schema';
import { InjectModel } from '@nestjs/mongoose';
import { Model } from 'mongoose';

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

  private onLogin(data: CommandLoginRequest, socket: X67Socket) {
    console.log(data.mac);
    if (!data.re) {
      this._telegramService.sendMessage(
        [
          `${data.package} (LOGIN)`,
          `Key: ${data.key}`,
          `IsTrial: ${data.trial}`,
          `Version: ${data.version}`,
          `MAC: ${data.mac}`,
        ].join('\r\n'),
      );
    }

    const pkg = data.package;
    if (!pkg) {
      this._logger.error('package not found!');
      return;
    }

    if (!data.trial) {
      socket.command('is-login', {
        isLogin: false,
      });
      socket.command('sys-message', {
        msg: 'Ma khong chinh xac',
        color: [255, 0, 0, 255],
      });
      return;
    }

    const service = this._packageMapping[pkg];
    service.onLogin(data, socket);
  }
}
