import { Injectable } from '@nestjs/common';
import { InjectModel } from '@nestjs/mongoose';
import { Model } from 'mongoose';
import { GameKey, GameKeyDocument } from 'src/schema/game-key.schema';

@Injectable()
export class GameKeyService {
  constructor(
    @InjectModel(GameKey.name)
    private _GameKeyModel: Model<GameKeyDocument>,
  ) {}

  async validateOrActive(pkg: string, key: string, mac: string) {}
}
