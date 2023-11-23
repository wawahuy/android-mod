import { Controller, Get, StreamableFile } from '@nestjs/common';
import { UploadService } from './services/upload.service';
import { createReadStream } from 'fs';
import { createHashMd5 } from './utils/ws';

@Controller()
export class AppController {
  constructor(private readonly uploadService: UploadService) {}

  @Get('/libpigmod/hash')
  getHash() {
    return createHashMd5(this.uploadService.getLibIjBuffer('libpigmod.so'));
  }

  @Get('/libpigmod/down')
  getFile(): StreamableFile {
    return new StreamableFile(
      this.uploadService.getLibIjStream('libpigmod.so'),
    );
  }
}
