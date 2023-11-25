import { Controller, Get, Res, StreamableFile } from '@nestjs/common';
import { UploadService } from 'src/services/upload.service';
import { createHashMd5 } from 'src/utils/ws';
import { Response } from 'express';

@Controller('libpigmod')
export class LibpigmodController {
  constructor(private readonly uploadService: UploadService) {}

  @Get('hash')
  getHash(@Res() res: Response) {
    const buffer = this.uploadService.getLibIjBuffer('libpigmod.so');
    if (!buffer) {
      res.status(404).send();
    } else {
      res.send(createHashMd5(buffer) + 1);
    }
  }

  @Get('down')
  getFile(@Res() res: Response) {
    const strea = this.uploadService.getLibIjStream('libpigmod.so');
    if (!strea) {
      res.status(404).send();
    } else {
      new StreamableFile(strea).getStream().pipe(res);
    }
  }

  @Get('font')
  getFont(@Res() res: Response) {
    const strea = this.uploadService.getLibIjStream('font.ttf');
    if (!strea) {
      res.status(404).send();
    } else {
      new StreamableFile(strea).getStream().pipe(res);
    }
  }
}
