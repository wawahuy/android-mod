import { Controller, Get, Res, StreamableFile } from '@nestjs/common';
import { UploadService } from 'src/services/upload.service';
import { createHashMd5 } from 'src/utils/ws';
import { Response } from 'express';
import { GameConfigService } from 'src/services/game-config.service';

@Controller('libpigmod')
export class LibpigmodController {
  constructor(
    private readonly uploadService: UploadService,
    private readonly gameConfigService: GameConfigService,
  ) {}

  @Get('hash')
  async getHash(@Res() res: Response) {
    const hash = await this.gameConfigService.getLibijHash('libpigmod.so');
    if (!hash) {
      res.status(404).send();
    } else {
      res.send(hash);
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
