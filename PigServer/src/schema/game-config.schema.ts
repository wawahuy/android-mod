import { Prop, Schema, SchemaFactory } from '@nestjs/mongoose';
import { HydratedDocument, SchemaTypes } from 'mongoose';

export type GameConfigDocument = HydratedDocument<GameConfig>;

@Schema({
  timestamps: true,
})
export class GameConfig {
  @Prop()
  version: string;

  @Prop()
  package: string;

  @Prop()
  libijHash: string;
}

export const GameConfigSchema = SchemaFactory.createForClass(GameConfig);
