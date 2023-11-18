import { Prop, Schema, SchemaFactory } from '@nestjs/mongoose';
import { HydratedDocument } from 'mongoose';

export type TelegramBotDocument = HydratedDocument<TelegramBot>;

@Schema({
  timestamps: true,
})
export class TelegramBot {
  @Prop()
  chatId: string;

  @Prop()
  uid: string;

  @Prop()
  username: string;
}

export const TelegramBotSchema = SchemaFactory.createForClass(TelegramBot);
