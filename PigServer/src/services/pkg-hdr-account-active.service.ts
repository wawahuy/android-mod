import { Injectable, Logger } from '@nestjs/common';
import { InjectModel } from '@nestjs/mongoose';
import { Model, Types } from 'mongoose';
import {
  PkgHdrAccountActive,
  PkgHdrAccountActiveDocument,
} from 'src/schema/pkg-hdr-account-active.schema';
import { logObject } from 'src/utils/common';
import { PkgHdrAccountService } from './pkg-hdr-account.service';
import { Job } from 'bull';

@Injectable()
export class PkgHdrAccountActiveService {
  private readonly _logger = new Logger('PkgHdrAccountActiveService');
  constructor(
    private readonly _pkgHdrAccountService: PkgHdrAccountService,
    @InjectModel(PkgHdrAccountActive.name)
    private readonly _pkgHdrAccountActiveModel: Model<PkgHdrAccountActiveDocument>,
  ) {}

  getListID(): Promise<Types.ObjectId[]> {
    return this._pkgHdrAccountActiveModel
      .find()
      .select('gameHdrAccountId')
      .exec()
      .then((rs) => {
        return rs.map((row) => row.gameHdrAccountId);
      });
  }

  async findNewAccountActive() {
    const accountActiveIds = await this.getListID();
    const newAccountActive =
      await this._pkgHdrAccountService.findAccountActive(accountActiveIds);
    return newAccountActive;
  }

  async updateAccountActive(job: Job<any>) {
    job.progress(10);
    const newAccountActive = await this.findNewAccountActive();
    if (newAccountActive && newAccountActive.length) {
      const rows = newAccountActive.map((item) => {
        return { gameHdrAccountId: item._id } as PkgHdrAccountActiveDocument;
      });
      await this._pkgHdrAccountActiveModel.insertMany(rows);
      this._logger.log('insert account active: ' + rows.length);
    }
    job.progress(100);
    return newAccountActive.length;
  }
}
