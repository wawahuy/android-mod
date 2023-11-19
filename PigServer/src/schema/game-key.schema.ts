import { Prop, Schema, SchemaFactory } from '@nestjs/mongoose';
import { HydratedDocument, SchemaTypes } from 'mongoose';

export type GameKeyDocument = HydratedDocument<GameKey>;

@Schema({
  timestamps: true,
})
export class GameKey {
  @Prop()
  key: string;

  @Prop()
  package: string;

  @Prop()
  expiredAt: Date;

  @Prop({ default: 30 * 24 * 60 * 60 })
  amountSec: number;

  @Prop({ default: 1 })
  maximumDevice: number;

  @Prop({ type: [SchemaTypes.String] })
  devices: string[];
}

export const GameKeySchema = SchemaFactory.createForClass(GameKey);
