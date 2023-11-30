import {
  IsBoolean,
  IsString,
  Matches,
  MaxLength,
  ValidateIf,
} from 'class-validator';

export class CommandLoginRequest {
  @ValidateIf((value) => !value)
  @IsString()
  @Matches(/^[\._\-a-zA-Z0-9]+$/gm)
  @MaxLength(255)
  key: string;

  @IsString()
  @Matches(/^[\._\-a-zA-Z0-9]+$/gm)
  @MaxLength(255)
  package: string;

  @IsBoolean()
  trial: boolean;

  @IsString()
  @MaxLength(255)
  version: string;

  @IsString()
  @MaxLength(255)
  mac: string;

  @IsBoolean()
  re: boolean;
}

export class CommandLoginSuccessResponse {
  isLogin: boolean;
  libIjHash: string;
  packageName: string;
  className: string;
}
