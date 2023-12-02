import { Prop, Schema, SchemaFactory } from '@nestjs/mongoose';
import {
  AnyKeys,
  AnyObject,
  HydratedDocument,
  Types,
  Document,
} from 'mongoose';

export type PkgHdrAccountDocument = HydratedDocument<PkgHdrAccount>;

@Schema({
  timestamps: true,
})
export class PkgHdrAccount {
  @Prop({ type: Types.ObjectId })
  gameKeyId: Types.ObjectId;

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

  @Prop()
  stsDayTime: Date;

  @Prop()
  nextTimeAd1: Date;

  @Prop()
  nextTimeAd2: Date;

  @Prop()
  httpFailed: number;
}

export const PkgHdrAccountSchema = SchemaFactory.createForClass(PkgHdrAccount);

PkgHdrAccountSchema.index({ trialExpired: 1 });
PkgHdrAccountSchema.index({ gameKeyId: 1 });
PkgHdrAccountSchema.index({ trialExpired: 1, gameKeyId: 1 });
PkgHdrAccountSchema.index({ trialExpired: 1, gameKeyId: 1, _id: 1 });
PkgHdrAccountSchema.index({
  trialExpired: 1,
  gameKeyId: 1,
  _id: 1,
  httpFailed: 1,
});
PkgHdrAccountSchema.index({ httpFailed: 1 });
PkgHdrAccountSchema.index({ httpFailed: 1, _id: 1 });

export type AnyKeysHdrAccount = AnyKeys<
  Document<unknown, unknown, PkgHdrAccount> &
    PkgHdrAccount & {
      _id: Types.ObjectId;
    }
> &
  AnyObject;
