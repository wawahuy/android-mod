import {
  MongooseModuleOptions,
  MongooseOptionsFactory,
} from '@nestjs/mongoose/dist/interfaces/mongoose-options.interface';
import { Injectable } from '@nestjs/common';
import { ConfigService } from '@nestjs/config';

@Injectable()
export class MongoConfig implements MongooseOptionsFactory {
  constructor(private configService: ConfigService) {}

  createMongooseOptions():
    | Promise<MongooseModuleOptions>
    | MongooseModuleOptions {
    const uriEnv = this.configService.get('MONGO_URI');
    if (uriEnv) {
      return {
        uri: uriEnv,
      };
    }

    const host = this.configService.get('MONGO_HOST');
    const port = this.configService.get('MONGO_PORT');
    const db = this.configService.get('MONGO_DB');
    const uri = `mongodb://${host}:${port}/${db}`;
    return {
      uri,
    };
  }
}
