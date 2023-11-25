import { Prop, Schema, SchemaFactory } from '@nestjs/mongoose';
import { HydratedDocument, ObjectId, SchemaTypes, Types } from 'mongoose';

export type PkgHdrAccountDocument = HydratedDocument<PkgHdrAccount>;

@Schema({
  timestamps: true,
})
export class PkgHdrAccount {
  @Prop({ type: Types.ObjectId })
  gameKeyId: ObjectId;

  @Prop()
  mtkey: string;

  @Prop()
  skey: string;

  @Prop({ unique: true })
  uid: string;

  @Prop()
  trialExpired: Date;

  @Prop()
  stsAllTili: number;

  @Prop()
  stsAllSnowball: number;

  @Prop()
  stsDayTili: number;

  @Prop()
  stsDaySnowball: number;
}

export const PkgHdrAccountSchema = SchemaFactory.createForClass(PkgHdrAccount);
