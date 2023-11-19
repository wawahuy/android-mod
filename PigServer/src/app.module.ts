import { Module } from '@nestjs/common';
import { AppController } from './app.controller';
import { AppService } from './app.service';
import { ServiceModule } from './services/service.module';
import { ConfigModule } from '@nestjs/config';
import { SchemaModule } from './schema/schema.module';

@Module({
  imports: [
    ServiceModule,
    ConfigModule.forRoot({ isGlobal: true }),
    SchemaModule,
  ],
  controllers: [AppController],
  providers: [AppService],
})
export class AppModule {}
