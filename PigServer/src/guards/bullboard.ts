import { ConfigService } from '@nestjs/config';
import * as expressBasicAuth from 'express-basic-auth';
import * as bcrypt from 'bcrypt';
import * as express from 'express';

export function factorySuperAuthorizer(configService: ConfigService) {
  let timeSuperLoginError = new Date().getTime();

  const middleware = (
    req: express.Request,
    res: express.Response,
    next: express.NextFunction,
  ) => {
    if (new Date().getTime() - timeSuperLoginError < 5000) {
      res.status(403).send();
    } else {
      next();
    }
  };

  const checker = (username: string, password: string) => {
    const userMatches = expressBasicAuth.safeCompare(
      username,
      configService.get('XH_SUPER_USER'),
    );
    const passwordMatches = bcrypt.compareSync(
      password,
      configService.get('XH_SUPER_PWD'),
    );
    const isMatches = userMatches && passwordMatches;
    if (!isMatches) {
      timeSuperLoginError = new Date().getTime();
    } else {
      timeSuperLoginError = 0;
    }
    return isMatches;
  };
  return {
    checker,
    middleware,
  };
}
