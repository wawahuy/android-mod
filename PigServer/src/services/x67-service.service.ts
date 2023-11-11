import { Injectable, Logger } from '@nestjs/common';
import * as md5 from 'md5';
import { X67Server } from 'src/x67-server/x67-server';
import X67Socket from 'src/x67-server/x67-socket';
import { AsmService } from './asm.service';
import * as fs from 'fs';
import * as path from 'path';
import { createHashMd5 } from 'src/utils/ws';

// const menu = [
//   {
//     name: 'Cay Thong',
//     items: [
//       {
//         name: 'Auto trung',
//         activeDefault: true,
//         il2cppPatch: [
//           {
//             offset: '223C604',
//             patch: '05000014',
//           },
//           {
//             offset: '223C64c',
//             patch: '31000014',
//           },
//         ],
//       },
//       {
//         name: 'Ban nhanh',
//         activeDefault: true,
//         il2cppPatch: [
//           {
//             offset: '1D47d70',
//             patch: '61000014',
//           },
//         ],
//       },
//       {
//         name: 'Ban 1 cham',
//         activeDefault: true,
//         il2cppPatch: [
//           {
//             offset: '1D4766C',
//             patch: '1F2003D5', // NOP
//           },
//         ],
//       },
//       // {
//       //   name: 'Ban nhanh (8v/1cay)',
//       //   activeDefault: false,
//       //   active: false,
//       //   il2cppPatch: [
//       //     {
//       //       offset: '01D47620',
//       //       patch: '1F2003D5', // NOP
//       //     },
//       //   ],
//       // },
//       {
//         name: 'Nap dan nhanh',
//         activeDefault: false,
//         active: false,
//         il2cppPatch: [
//           {
//             offset: '1D47618',
//             patch: '1F2003D5', // NOP
//           },
//         ],
//       },
//     ],
//   },
// ];

enum WidgetMenuItem  {
  Switch = 1,
  InputInt = 2,
  SliderFloat = 3,
}

const menu = [
  {
    label: 'Cay Thong',
    action: 'caythong',
    items: [
      {
        label: 'Auto trung',
        valueDefault: true,
        type: WidgetMenuItem.Switch,
        action: 'autoTrung',
      },
      {
        label: 'Ban nhanh',
        valueDefault: true,
        type: WidgetMenuItem.Switch,
        action: 'banNhanh',
      },
      {
        label: 'Ban 1 cham',
        valueDefault: true,
        type: WidgetMenuItem.Switch,
        action: 'ban1Cham',
      },
      {
        label: 'Nap dan nhanh',
        valueDefault: false,
        type: WidgetMenuItem.Switch,
        action: 'napDanNhanh',
        items: [
          {
            arg: 'count',
            label: 'So luong (vien/1 lan)',
            type: WidgetMenuItem.InputInt,
            valueDefault: 1,
            valueMin: 1,
            valueMax: 8,
            valueStep: 1,
            valueStepFast: 2,
            valueWidth: 250,
          },
          {
            arg: 'speed',
            label: 'Toc do',
            type: WidgetMenuItem.SliderFloat,
            valueDefault: 0.1,
            valueMin: 0,
            valueMax: 1,
            valueStep: 0.1,
            valueStepFast: 0.2,
            valueWidth: 250,
          }
        ]
      }
    ]
  }
]

const description = {
  versionHash: md5(JSON.stringify(menu)),
  menu,
};

@Injectable()
export class X67ServiceService {
  _logger = new Logger('X67ServiceService');
  _server: X67Server;

  constructor(
    private readonly _asmService: AsmService,
  ) {
    this.init();
  }

  init() {
    const port = 1235;
    this._server = new X67Server();
    this._server.listen(port);
    this._server.eventClients.on('login', (data: any, socket: X67Socket) => {
      // socket.command("is-login", false);
      // socket.command("sys-message", {
      //     msg: "Ma khong chinh xac",
      //     color: [255, 0, 0, 255]
      // });
      const ijBuffer = fs.readFileSync(path.join(__dirname, '../../../GameHeoDenRoi/libpigmodij/.build/libpigmodij.so'));
      socket.command('is-login', {
        isLogin: true,
        libIjHash: createHashMd5(ijBuffer)
      });
    });
    this._server.eventClients.on('get-menu', (data: any, socket: X67Socket) => {
      socket.command('menu', description);
    });
    this._server.eventClients.on('get-lib-ij', (data: any, socket: X67Socket) => {
      socket.sendLibIj(fs.readFileSync(path.join(__dirname, '../../../GameHeoDenRoi/libpigmodij/.build/libpigmodij.so')));
    });
  }
}
