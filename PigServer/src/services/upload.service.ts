import { Injectable, Logger } from '@nestjs/common';
import { ConfigService } from '@nestjs/config';
import * as stream from 'stream';
import * as fs from 'fs';
import * as path from 'path';
import { UploadConfig } from 'src/configs/upload.config';
import { createHashMd5 } from 'src/utils/ws';
import * as _ from 'lodash';
import { GameConfigService } from './game-config.service';

@Injectable()
export class UploadService {
  private _logger = new Logger('UploadService');
  constructor(
    private readonly _uploadConfig: UploadConfig,
    private readonly _configService: ConfigService,
    private readonly _gameConfigService: GameConfigService,
  ) {
    this.watchFolderLibso();
  }

  watchFolderLibso() {
    const changeDebouncedFunc: { [key: string]: _.DebouncedFunc<any> } = {};
    fs.watch(this._uploadConfig.getFolderLibso(), (event, filename) => {
      console.log(filename);
      if (filename) {
        let cb = changeDebouncedFunc[filename];
        if (!cb) {
          cb = _.debounce(
            this.onChangeLibsoHash.bind(this),
            this._uploadConfig.getWatchDebounce(),
          );
          changeDebouncedFunc[filename] = cb;
          this._logger.debug(`add debounce watch to ${filename}`);
        }
        cb(filename);
      }
    });
  }

  private async onChangeLibsoHash(filename: string) {
    const hash = this.getRealLibsoHash(filename);
    if (hash) {
      await this._gameConfigService.setLibijHash(filename, hash);
      this._logger.debug(`New hash ${filename}: ${hash}`);
    } else {
      this._logger.debug(`Hash null`);
    }
  }

  getRealLibsoHash(filename: string) {
    const buffer = this.getLibIjBuffer(filename);
    if (!buffer) {
      return null;
    }
    const libsoHash = createHashMd5(buffer);
    return libsoHash;
  }

  uploadLibIj(strPackage: string, fileStream: stream.Readable) {
    const newFile = fs.createWriteStream(this.getLibIjPath(strPackage));
    return fileStream.pipe(newFile);
  }

  getLibIjPath(strPackage: string) {
    return path.join(this._uploadConfig.getFolderLibso(), strPackage);
  }

  getLibIjStream(strPackage: string) {
    const p = this.getLibIjPath(strPackage);
    if (!fs.existsSync(p)) {
      return null;
    }
    return fs.createReadStream(p);
  }

  getLibIjBuffer(strPackage: string) {
    const p = this.getLibIjPath(strPackage);
    if (!fs.existsSync(p)) {
      return null;
    }
    return fs.readFileSync(p);
  }
}
