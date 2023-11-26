import { Injectable } from '@nestjs/common';
import { InjectModel } from '@nestjs/mongoose';
import { Model } from 'mongoose';
import {
  PkgHdrAccountActive,
  PkgHdrAccountActiveDocument,
} from 'src/schema/pkg-hdr-account-active.schema';

@Injectable()
export class PkgHdrAccountActiveService {
  constructor(
    @InjectModel(PkgHdrAccountActive.name)
    private readonly _pkgHdrAccountActiveModel: Model<PkgHdrAccountActiveDocument>,
  ) {}
}
