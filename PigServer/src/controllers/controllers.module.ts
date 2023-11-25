import { Module } from '@nestjs/common';
import { LibpigmodController } from './libpigmod.controller';
import { ServiceModule } from 'src/services/service.module';

@Module({
  imports: [ServiceModule],
  controllers: [LibpigmodController],
})
export class ControllersModule {}
