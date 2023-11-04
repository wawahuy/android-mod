import { Injectable } from '@nestjs/common';
import { X67Server } from 'src/x67-server/x67-server';
import X67Socket from 'src/x67-server/x67-socket';

@Injectable()
export class X67ServiceService {
    _server: X67Server;

    constructor() {
        this.init();
    }

    init() {
        const port = 1235;
        this._server = new X67Server;
        this._server.listen(port);
        this._server.eventClients.on('test', (data: any, socket: X67Socket) => {
        })
    }
}
