import { Module } from '@nestjs/common';
import { ModelDefinition, MongooseModule } from '@nestjs/mongoose';
import { MongoConfig } from '../configs/mongo.config';
import { TelegramBot, TelegramBotSchema } from './telegram.schema';
import { GameKey, GameKeySchema } from './game-key.schema';
import { PkgHdrAccount, PkgHdrAccountSchema } from './pkg-hdr-account.schema';
import { GameConfig, GameConfigSchema } from './game-config.schema';

export const features: ModelDefinition[] = [
  {
    name: TelegramBot.name,
    schema: TelegramBotSchema,
  },
  {
    name: GameKey.name,
    schema: GameKeySchema,
  },
  {
    name: GameConfig.name,
    schema: GameConfigSchema,
  },
  {
    name: PkgHdrAccount.name,
    schema: PkgHdrAccountSchema,
  },
];

@Module({
  imports: [
    MongooseModule.forRootAsync({
      useClass: MongoConfig,
    }),
    MongooseModule.forFeature(features),
  ],
  exports: [MongooseModule.forFeature(features)],
})
export class SchemaModule {}
