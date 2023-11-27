import { Prop, Schema, SchemaFactory } from '@nestjs/mongoose';
import { HydratedDocument, Types } from 'mongoose';

export type PkgHdrAccountActiveDocument = HydratedDocument<PkgHdrAccountActive>;

@Schema({
  timestamps: true,
})
export class PkgHdrAccountActive {
  @Prop({ type: Types.ObjectId, unique: true })
  gameHdrAccountId: Types.ObjectId;

  @Prop()
  nextTimeAd1: Date;

  @Prop()
  nextTimeAd2: Date;
}

export const PkgHdrAccountActiveSchema =
  SchemaFactory.createForClass(PkgHdrAccountActive);
