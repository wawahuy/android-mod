import { IsBoolean, IsString, Matches } from 'class-validator';

export class CommandLoginRequest {
  @IsString()
  @Matches(/^[\._\-a-zA-Z0-9]+$/gm)
  key: string;

  @IsString()
  @Matches(/^[\._\-a-zA-Z0-9]+$/gm)
  package: string;

  @IsBoolean()
  trial: boolean;

  @IsString()
  version: string;

  @IsString()
  mac: string;

  @IsBoolean()
  re: boolean;
}

export class CommandLoginSuccessResponse {
  isLogin: boolean;
  libIjHash: string;
}
