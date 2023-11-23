import { Controller, Get, Res, StreamableFile } from '@nestjs/common';
import { UploadService } from './services/upload.service';
import { createReadStream } from 'fs';
import { createHashMd5 } from './utils/ws';

@Controller()
export class AppController {
  constructor(private readonly uploadService: UploadService) {}

  @Get('/libpigmod/hash')
  getHash(@Res() res) {
    const buffer = this.uploadService.getLibIjBuffer('libpigmod.so');
    if (!buffer) {
      res.status(404).send();
    } else {
      res.send(createHashMd5(buffer));
    }
  }

  @Get('/libpigmod/down')
  getFile(@Res() res) {
    const strea = this.uploadService.getLibIjStream('libpigmod.so');
    if (!strea) {
      res.status(404).send();
    } else {
      new StreamableFile(strea).getStream().pipe(res);
    }
  }
}
