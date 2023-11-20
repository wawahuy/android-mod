import { IsBoolean, IsString } from 'class-validator';

export class CommandLoginRequest {
  @IsString()
  key: string;

  @IsString()
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
