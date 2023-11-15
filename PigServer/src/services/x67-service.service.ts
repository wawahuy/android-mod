import { Injectable, Logger } from '@nestjs/common';
import * as md5 from 'md5';
import { X67Server } from 'src/x67-server/x67-server';
import X67Socket from 'src/x67-server/x67-socket';
import { AsmService } from './asm.service';
import * as fs from 'fs';
import * as path from 'path';
import * as ws from  'ws';
import { createHashMd5 } from 'src/utils/ws';
import { ConfigService } from '@nestjs/config';

enum WidgetMenuItem {
  Switch = 1,
  InputInt = 2,
  SliderFloat = 3,
  SliderInt = 4,
  Button = 5,

  ServerSwitch = 101,
  Call = 102,
}

enum ArgDataPushType {
  No = 0,
  First = 1,
  Focus = 2,
  Always = 3,
};

const menu = [
  // {
  //   label: 'Nang Luong',
  //   action: 'nangluong',
  //   items: [
  //     {
  //       label: 'Auto nhan nang luong',
  //       valueDefault: true,
  //       type: WidgetMenuItem.ServerSwitch,
  //       action: 'autoNanhNangLuong',
  //     },
  //     {
  //       type: WidgetMenuItem.Call,
  //       action: 'autoGetData',
  //       interval: 10000,
  //     },
  //   ]
  // },
  {
    label: 'Cay Thong',
    action: 'cayThong',
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
        valueDefault: true,
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
            label: 'Toc do (giay)',
            type: WidgetMenuItem.SliderFloat,
            valueDefault: 0.25,
            valueMin: 0,
            valueMax: 1,
            valueStep: 0.1,
            valueStepFast: 0.2,
            valueWidth: 250,
          },
        ],
      },
    ],
  },
  {
    label: 'Vong xoay',
    action: 'bet',
    items: [
      {
        label: 'Vong xoay',
        valueDefault: true,
        type: WidgetMenuItem.Switch,
        action: 'vongXoay',
        items: [
          {
            type: WidgetMenuItem.SliderInt,
            arg: 'count',
            valueDefault: 1,
            valueMin: 1,
            valueMax: 2000,
            valueStep: 10,
            valueStepFast: 50,
            valueWidth: 400,
            hashId: "vongXoay.count.slider",
          },
          {
            label: 'x100',
            valueDefault: false,
            type: WidgetMenuItem.Button,
            pushArgs: [
              ['count', 100]
            ],
            pushArgsType: ArgDataPushType.Always,
            callReMapArgsHashId: [
              "vongXoay.count.slider"
            ],
            sameLine: true,
          },
          {
            label: 'x500',
            valueDefault: false,
            type: WidgetMenuItem.Button,
            pushArgs: [
              ['count', 500]
            ],
            pushArgsType: ArgDataPushType.Always,
            callReMapArgsHashId: [
              "vongXoay.count.slider"
            ],
            sameLine: true,
          },
          {
            label: 'x1000',
            valueDefault: false,
            type: WidgetMenuItem.Button,
            pushArgs: [
              ['count', 1000]
            ],
            pushArgsType: ArgDataPushType.Always,
            callReMapArgsHashId: [
              "vongXoay.count.slider"
            ],
          },
        ]
      }
    ]
  }
];

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
    private readonly _configService: ConfigService,
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
    this._server.eventClients.on('login', (data: any, socket: X67Socket) => {
      // socket.command("is-login", false);
      // socket.command("sys-message", {
      //     msg: "Ma khong chinh xac",
      //     color: [255, 0, 0, 255]
      // });
      const ijBuffer = fs.readFileSync(
        path.join(this._configService.get('FOLDER_LIBSO'), 'libpigmodij.so'),
      );
      socket.command('is-login', {
        isLogin: true,
        libIjHash: createHashMd5(ijBuffer),
      });
    });
    this._server.eventClients.on('get-menu', (data: any, socket: X67Socket) => {
      socket.command('menu', description);
    });
    this._server.eventClients.on(
      'get-lib-ij',
      (data: any, socket: X67Socket) => {
        socket.sendLibIj(
          fs.readFileSync(
            path.join(
              this._configService.get('FOLDER_LIBSO'),
              'libpigmodij.so',
            ),
          ),
        );
      },
    );
    this._server.eventClients.on('menu-action', (data: any) => {
      console.log(data);
    });
  }
}
