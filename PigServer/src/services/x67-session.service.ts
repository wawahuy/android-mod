import { Injectable } from '@nestjs/common';
import { GameKeyDocument } from 'src/schema/game-key.schema';
import { PkgHdrAccountDocument } from 'src/schema/pkg-hdr-account.schema';
import X67Socket from 'src/x67-server/x67-socket';

export interface X67SessionData {
  package: string;
  trial: boolean;
  timeoutTrial: NodeJS.Timeout;
  key: GameKeyDocument;
  account: PkgHdrAccountDocument;
}

@Injectable()
export class X67SessionService {
  data: {
    [id: string]: X67SessionData;
  } = {};

  session(socket: X67Socket) {
    if (!this.data[socket.id]) {
      const data = (this.data[socket.id] = <X67SessionData>(<unknown>{}));
      socket.eventSystem.once('close', () => {
        this.delete(socket);
        if (data.timeoutTrial) {
          clearTimeout(data.timeoutTrial);
        }
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

  get<T extends keyof X67SessionData>(
    socket: X67Socket,
    key: T,
  ): X67SessionData[T] {
    return this.data[socket.id][key];
  }

  remove<T extends keyof X67SessionData>(socket: X67Socket, key: T) {
    delete this.data[socket.id][key];
  }

  delete(socket: X67Socket) {
    delete this.data[socket.id];
  }
}
