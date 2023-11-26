import { Injectable, Logger } from '@nestjs/common';
import X67Server from 'src/x67-server/x67-server';
import { validate } from 'class-validator';
import { ConfigService } from '@nestjs/config';
import { TelegramService } from './telegram.service';
import X67Socket from 'src/x67-server/x67-socket';
import { CommandLoginRequest } from 'src/dtos/x67-server.dto';
import { IGamePackage } from 'src/interfaces/game-package';
import { PkgHdrService } from './pkg-hdr.service';
import { plainToClass } from 'class-transformer';
import { X67SenderService } from './x67-sender.service';
import { X67SessionService } from './x67-session.service';
import { UploadService } from './upload.service';
import { GameConfigService } from './game-config.service';
import { GameKeyService } from './game-key.service';

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
    private readonly _pkgHdrService: PkgHdrService,
    private readonly _gameConfigService: GameConfigService,
    private readonly _gameKeyService: GameKeyService,
  ) {
    this.init();
  }

  init() {
    const port = 1235;
    this._server = new X67Server();
    this._server.listen(port);

    // package mapping
    this._packageMapping = {
      [PkgHdrService.packageName]: this._pkgHdrService,
    };

    // route
    const onAuthorizatedBinded = this.onAuthorizated.bind(this);
    const listener = this._server.eventClients;
    listener.on('login', this.onLogin.bind(this));
    listener.on('get-menu', onAuthorizatedBinded(this.onGetMenu.bind(this)));
    listener.on('get-lib-ij', onAuthorizatedBinded(this.onGetLibIj.bind(this)));
    listener.on(
      'menu-action',
      onAuthorizatedBinded(this.onMenuAction.bind(this)),
    );

    // add route pkg
    for (const packageName in this._packageMapping) {
      const service = this._packageMapping[packageName];
      const routes = service.getRoutes();
      for (const route in routes) {
        listener.on(route, onAuthorizatedBinded(routes[route]));
      }
    }
  }

  private async onLogin(data: CommandLoginRequest, socket: X67Socket) {
    data = plainToClass(CommandLoginRequest, data);
    const errors = await validate(data);
    if (errors && errors.length) {
      this._sender.sendMessageValidationError(socket, errors[0]);
      this._logger.error(errors);
      return;
    }

    let msgError: string | null;
    const pkg = data.package;
    const version = data.version;
    const telegramMsg = [
      `${data.package} (LOGIN)`,
      `--------------------------`,
      `|  Key: ${data.key}`,
      `--------------------------`,
      `IsTrial: ${data.trial}`,
      `Version: ${data.version}`,
      `MAC: ${data.mac}`,
    ];

    const supportVersion = await this._gameConfigService.getVersion(pkg);
    const service = this._packageMapping[pkg];
    if (!service) {
      msgError = 'Game khong ho tro';
    } else if (supportVersion !== version) {
      msgError = 'Chung toi dang cap nhat phien ban moi';
    } else if (data.trial) {
      if (service.canTrial()) {
        service.onTrial(socket);
      } else {
        msgError = 'Khong ho tro dung thu';
      }
    } else {
      const active = await this._gameKeyService.validateOrActive(
        pkg,
        data.key,
        data.mac,
      );
      msgError = active.error;
    }

    if (!msgError) {
      const result = await this.onLoginSuccess(data, socket);
      if (!result) {
        msgError = 'Loi xu ly';
      }
    }

    if (msgError) {
      telegramMsg.push(msgError);
      this._sender.sendLoginFailed(socket, msgError);
    }

    if (!data.re) {
      this._telegramService.sendMessage(telegramMsg.join('\r\n'));
    }
  }

  private async onLoginSuccess(data: CommandLoginRequest, socket: X67Socket) {
    const pkg = data.package;
    const service = this._packageMapping[pkg];
    const libIjHash = await this._gameConfigService.getLibijHash(pkg);
    if (!libIjHash) {
      return false;
    }

    this._session.set(socket, 'package', pkg);
    this._session.set(socket, 'trial', data.trial);
    this._sender.sendLoginSuccess(socket, {
      isLogin: true,
      libIjHash,
      packageName: service.getPackageName(),
      className: service.getClassName(),
    });
    return true;
  }

  private onAuthorizated(
    next: (data: CommandLoginRequest, socket: X67Socket) => void,
  ) {
    return (data: CommandLoginRequest, socket: X67Socket) => {
      if (this._session.has(socket)) {
        next(data, socket);
      }
    };
  }

  private async onGetMenu(data: any, socket: X67Socket) {
    const pkg = this._session.get(socket, 'package');
    const service = this._packageMapping[pkg];
    if (service) {
      this._sender.sendMenu(socket, service.getMenuDescription(socket));
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
