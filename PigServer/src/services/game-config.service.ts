import { Injectable } from '@nestjs/common';
import { InjectModel } from '@nestjs/mongoose';
import { Model } from 'mongoose';
import { GameConfig, GameConfigDocument } from 'src/schema/game-config.schema';

@Injectable()
export class GameConfigService {
  constructor(
    @InjectModel(GameConfig.name)
    private readonly _gameConfigModel: Model<GameConfigDocument>,
  ) {}

  async getVersion(pkg: string) {
    const doc = await this._gameConfigModel.findOne({ package: pkg });
    if (doc) {
      return doc.version;
    }
    return null;
  }

  async setVersion(pkg: string, version: string) {
    let doc = await this._gameConfigModel.findOne({ package: pkg });
    if (!doc) {
      doc = await this._gameConfigModel.create({});
    }
    doc.package = pkg;
    doc.version = version;
    await doc.save();
    return doc;
  }

  async setLibijHash(pkg: string, hash: string) {
    let doc = await this._gameConfigModel.findOne({ package: pkg });
    if (!doc) {
      doc = await this._gameConfigModel.create({});
      doc.package = pkg;
    }
    doc.libijHash = hash;
    await doc.save();
    return doc;
  }

  async getLibijHash(pkg: string) {
    const doc = await this._gameConfigModel.findOne({ package: pkg });
    if (doc) {
      return doc.libijHash;
    }
    return null;
  }
}
