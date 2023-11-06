import { Injectable } from '@nestjs/common';
import * as moment from 'moment';
import * as md5 from 'md5';
import { X67Server } from 'src/x67-server/x67-server';
import X67Socket from 'src/x67-server/x67-socket';

const menu = [
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
                activeDefault: true,
                il2cppPatch: [
                    {
                        offset: '1D47d70',
                        patch: '61000014'
                    },
                ],
            },
            {
                name: 'Ban 1 cham',
                activeDefault: true,
                il2cppPatch: [
                    {
                        offset: '1D4766C',
                        patch: '1F2003D5' // NOP
                    },
                ],
            },
            {
                name: 'Ban nhanh (8v/1cay)',
                activeDefault: false,
                active: false,
                il2cppPatch: [
                    {
                        offset: '01D47620',
                        patch: '1F2003D5' // NOP
                    },
                ],
            },
            {
                name: 'Ban sieu toc (x100v/1cay)',
                activeDefault: false,
                active: false,
                il2cppPatch: [
                    {
                        offset: '1D47618',
                        patch: '1F2003D5' // NOP
                    },
                ],
            },
        ]
    }
]

const description = {
    versionHash: md5(JSON.stringify(menu)),
    menu,
}

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
            // socket.command("is-login", false);
            // socket.command("sys-message", {
            //     msg: "Ma khong chinh xac",
            //     color: [255, 0, 0, 255]
            // });
            socket.command("is-login", true);
        });
        this._server.eventClients.on('get-menu', (data: any, socket: X67Socket) => {
            socket.command("menu", description);
        })
    }
}
