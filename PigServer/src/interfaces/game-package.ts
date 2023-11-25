import { CommandLoginRequest } from 'src/dtos/x67-server.dto';
import X67Socket from 'src/x67-server/x67-socket';

export interface IGamePackage {
  getMenuDescription(): any;
  getPackageName(): string;
  getClassName(): string;
}
