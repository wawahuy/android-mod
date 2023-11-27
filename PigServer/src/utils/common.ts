import * as util from 'util';

export const randomUint8 = () => {
  return Math.floor(Math.random() * 255);
};

export const logObject = (obj: any) => {
  return util.inspect(obj, { showHidden: false, depth: null, colors: true });
};
