import { Logger } from '@nestjs/common';
import * as events from 'events';
import * as net from 'net';
import X67Socket from './x67-socket';
import { X67Command } from './x67-command';

export class X67Server {
  private _logger = new Logger('X67Socket');
  private _port: number;
  private _server: net.Server;
  private _clients: { [key: string]: X67Socket } = {};

  readonly eventClients = new events.EventEmitter();

  constructor() {
    this._server = net.createServer();
    this._server.on('close', this.onClose.bind(this));
    this._server.on('error', this.onError.bind(this));
    this._server.on('listening', this.onListening.bind(this));
    this._server.on('connection', this.onConnection.bind(this));
  }

  listen(port: number, host?: string) {
    this._port = port;
    this._server.listen(this._port, host);
  }

  onError(err: Error) {
    this._logger.error(err);
  }

  onClose() {
    this._logger.error('closed!');
  }

  onListening() {
    this._logger.debug('listening...');
  }

  onConnection(socket: net.Socket) {
    const client = new X67Socket(socket);
    client.eventSystem.on('command', this.onClientCommand.bind(this));
    client.eventSystem.once('close', () => {
      delete this._clients[client.id];
    });
    this._clients[client.id] = client;
  }

  onClientCommand(command: X67Command<any>, socket: X67Socket) {
    this.eventClients.emit(command.command, command.data, socket);
  }

  broadcastCommand<T>(command: string, data: T) {
    for (const clientId in this._clients) {
      this._clients[clientId].command(command, data);
    }
  }
}
