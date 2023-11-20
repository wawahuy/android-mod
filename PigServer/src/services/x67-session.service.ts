import { Injectable } from '@nestjs/common';
import X67Socket from 'src/x67-server/x67-socket';

export interface X67SessionData {
  package: string;
}

@Injectable()
export class X67SessionService {
  data: {
    [id: string]: X67SessionData;
  } = {};

  session(socket: X67Socket) {
    if (!this.data[socket.id]) {
      this.data[socket.id] = <any>{};
      socket.eventSystem.once('close', () => {
        this.delete(socket);
      });
    }
    return this.data[socket.id];
  }

  set<T extends keyof X67SessionData>(
    socket: X67Socket,
    key: T,
    value: X67SessionData[T],
  ) {
    const data = this.session(socket);
    data[key] = value;
  }

  has(socket: X67Socket) {
    return !!this.data[socket.id];
  }

  get(socket: X67Socket, key: keyof X67SessionData) {
    return this.data[socket.id][key];
  }

  delete(socket: X67Socket) {
    delete this.data[socket.id];
  }
}
