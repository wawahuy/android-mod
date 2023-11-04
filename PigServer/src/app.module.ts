import { Module } from '@nestjs/common';
import { AppController } from './app.controller';
import { AppService } from './app.service';
import { ServiceModule } from './services/service.module';

@Module({
  imports: [ServiceModule, ServiceModule],
  controllers: [AppController],
  providers: [AppService],
})
export class AppModule {}
