import { Module } from '@nestjs/common';
import { X67ServiceService } from './x67-service.service';
import { AsmService } from './asm.service';

const services = [X67ServiceService];

@Module({
  providers: [...services, X67ServiceService, AsmService],
  exports: [...services],
})
export class ServiceModule {}
