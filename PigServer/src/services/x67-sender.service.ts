import { Injectable } from '@nestjs/common';
import { ValidationError } from 'class-validator';
import { CommandLoginSuccessResponse } from 'src/dtos/x67-server.dto';
import X67Socket from 'src/x67-server/x67-socket';

@Injectable()
export class X67SenderService {
  sendMenuTextServer(socket: X67Socket, name: string, data: any) {
    socket.command('menu:text-server', {
      name,
      data,
    });
  }

  sendDestroy(socket: X67Socket) {
    socket.command('destroy', {});
  }

  sendMenu(socket: X67Socket, menu: any) {
    socket.command('menu', menu);
  }

  sendLoginSuccess(socket: X67Socket, dto: CommandLoginSuccessResponse) {
    socket.command('is-login', dto);
  }

  sendLoginFailed(socket: X67Socket, msg: string) {
    socket.command('is-login', {
      isLogin: false,
    });
    this.sendMessageError(socket, msg);
  }

  sendMessageValidationError(socket: X67Socket, err: ValidationError) {
    const msg = Object.values(err?.constraints)?.[0];
    this.sendMessageError(socket, msg || 'Internal');
  }

  sendMessageError(socket: X67Socket, msg: string) {
    this.sendMessage(socket, msg, [255, 0, 0, 255]);
  }

  sendMessage(
    socket: X67Socket,
    msg: string,
    color: [number, number, number, number],
  ) {
    socket.command('sys-message', {
      msg,
      color,
    });
  }
}
