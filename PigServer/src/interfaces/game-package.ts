import X67Socket from 'src/x67-server/x67-socket';

type MappingActionCallback = (data: any, socket: X67Socket) => void;

export interface IGamePackage {
  getMenuDescription(socket: X67Socket): Promise<any>;
  getPackageName(): string;
  getClassName(): string;
  getRoutes(): { [key: string]: MappingActionCallback };

  canTrial(): boolean;
  onTrial(socket: X67Socket): void;
}
