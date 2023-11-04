import { Injectable } from '@nestjs/common';
import { X67Server } from 'src/x67-server/x67-server';
import X67Socket from 'src/x67-server/x67-socket';

const test = [
    {
        name: "Cay Thong",
        items: [
            {
                name: 'Auto trung',
                activeDefault: true,
                il2cppPatch: [
                    {
                        offset: '223C604',
                        patch: '05000014'
                    },
                    {
                        offset: '223C64c',
                        patch: '31000014'
                    }
                ],
            },
            {
                name: 'Ban nhanh',
                activeDefault: false,
                il2cppPatch: [
                    {
                        offset: '1D47d70',
                        patch: '61000014'
                    },
                ],
            }
        ]
    }
]

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
        this._server.eventClients.on('login', (data: any, socket: X67Socket) => {
            // if (Math.random() > 0.8) {
            //     socket.command("is-login", true);
            //     socket.command("menu", test);
            //     return;
            // }
            // socket.command("is-login", false);
            // socket.command("sys-message", {
            //     msg: "Ma khong chinh xac",
            //     color: [255, 0, 0, 255]
            // });
            socket.command("is-login", true);
            socket.command("menu", test);
        })
    }
}
