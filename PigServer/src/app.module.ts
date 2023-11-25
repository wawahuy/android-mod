import { Module } from '@nestjs/common';
import { AppController } from './app.controller';
import { ServiceModule } from './services/service.module';
import { ConfigModule } from '@nestjs/config';
import { SchemaModule } from './schema/schema.module';
import { ControllersModule } from './controllers/controllers.module';

@Module({
  imports: [
    ServiceModule,
    ConfigModule.forRoot({ isGlobal: true }),
    SchemaModule,
    ControllersModule,
  ],
  controllers: [AppController],
  providers: [],
})
export class AppModule {}
