import { Controller, Get, Res, StreamableFile } from '@nestjs/common';
import { Response } from 'express';
import { UploadService } from './services/upload.service';
import { createHashMd5 } from './utils/ws';

@Controller()
export class AppController {
  constructor(private readonly uploadService: UploadService) {}

  @Get('/libpigmod/hash')
  getHash(@Res() res: Response) {
    const buffer = this.uploadService.getLibIjBuffer('libpigmod.so');
    if (!buffer) {
      res.status(404).send();
    } else {
      res.send(createHashMd5(buffer) + 1);
    }
  }

  @Get('/libpigmod/down')
  getFile(@Res() res: Response) {
    const strea = this.uploadService.getLibIjStream('libpigmod.so');
    if (!strea) {
      res.status(404).send();
    } else {
      new StreamableFile(strea).getStream().pipe(res);
    }
  }

  @Get('/libpigmod/font')
  getFont(@Res() res: Response) {
    const strea = this.uploadService.getLibIjStream('font.ttf');
    if (!strea) {
      res.status(404).send();
    } else {
      new StreamableFile(strea).getStream().pipe(res);
    }
  }
}
