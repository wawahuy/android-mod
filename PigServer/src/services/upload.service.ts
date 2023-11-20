import { Injectable, Logger } from '@nestjs/common';
import { ConfigService } from '@nestjs/config';
import * as stream from 'stream';
import * as fs from 'fs';
import * as path from 'path';

@Injectable()
export class UploadService {
  private _logger = new Logger('UploadService');
  constructor(private readonly _configService: ConfigService) {}

  getFolderLibso() {
    return this._configService.get<string>('FOLDER_LIBSO');
  }

  uploadLibIj(strPackage: string, fileStream: stream.Readable) {
    const newFile = fs.createWriteStream(this.getLibIjPath(strPackage));
    return fileStream.pipe(newFile);
  }

  getLibIjPath(strPackage: string) {
    return path.join(this.getFolderLibso(), strPackage);
  }

  getLibIjStream(strPackage: string) {
    return fs.createReadStream(this.getLibIjPath(strPackage));
  }

  getLibIjBuffer(strPackage: string) {
    return fs.readFileSync(this.getLibIjPath(strPackage));
  }
}
