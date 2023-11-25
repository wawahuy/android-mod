import { NestFactory } from '@nestjs/core';
import { ConfigService } from '@nestjs/config';
import * as expressBasicAuth from 'express-basic-auth';
import { createBullBoard } from '@bull-board/api';
import { ExpressAdapter } from '@bull-board/express';
import { Queue } from 'bull';
import { BullAdapter } from '@bull-board/api/bullAdapter';

import { AppModule } from './app.module';
import { factorySuperAuthorizer } from './guards/bullboard';

async function bootstrap() {
  const app = await NestFactory.create(AppModule);
  const configService = app.get(ConfigService);
  const superAuthorizer = factorySuperAuthorizer(configService);

  app.use(
    '/mana',
    superAuthorizer.middleware,
    expressBasicAuth({
      authorizer: superAuthorizer.checker,
      challenge: true,
    }),
  );

  const bullAdapter = new ExpressAdapter();
  bullAdapter.setBasePath('/mana/bull');
  createBullBoard({
    queues: [
      // new BullAdapter(app.get<Queue>('BullQueue_' + JOB_NAME_HDR_ACTIVE)),
    ],
    serverAdapter: bullAdapter,
  });
  app.use('/mana/bull', bullAdapter.getRouter());

  await app.listen(3000);
}
bootstrap();
