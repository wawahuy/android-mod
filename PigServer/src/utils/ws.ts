import * as crypto from 'crypto';

export const wsKeyPattern = /sec\-websocket\-key:\s/gim;

export function generateWebSocketAcceptKey(clientWebSocketKey: string) {
  const magicString = '258EAFA5-E914-47DA-95CA-C5AB0DC85B11';
  const combinedKey = clientWebSocketKey + magicString;
  const sha1 = crypto.createHash('sha1');
  sha1.update(combinedKey);
  return sha1.digest('base64');
}

export function createHashMd5(buffer: Buffer) {
  return crypto.createHash('md5').update(buffer).digest('hex');
}
