import { Socket } from 'net';
import { createHash } from 'crypto';

function generateWebSocketAcceptKey(clientWebSocketKey) {
    const magicString = '258EAFA5-E914-47DA-95CA-C5AB0DC85B11';
    const combinedKey = clientWebSocketKey + magicString;
    const sha1 = createHash('sha1');
    sha1.update(combinedKey);
    return sha1.digest('base64');
  }
  
const clientKey = 'the_client_websocket_key';

const sk = new Socket();

sk.connect(80, 'x67huy-dev.j2run.com').on('connect', () => {
    const k = generateWebSocketAcceptKey(clientKey);
    console.log('gen key', k);
    
    const data = 'GET / HTTP/1.1\r\n' +
    'Host: x67huy-dev.j2run.com\r\n' +
    'Connection: Upgrade\r\n' +
    'Sec-Websocket-Extensions: permessage-deflate; client_max_window_bits\r\n' +
    'Sec-Websocket-Key: ' +  k + '\r\n' +
    'Sec-Websocket-Version: 13\r\n' +
    'Upgrade: websocket\r\n\r\n';
    
    sk.on('data', function (d) {
        console.log(d.toString('utf-8'));
        setTimeout(function () {
            sk.write(Math.random().toString());
        }, 100)
    })
    
    sk.write(data);
    sk.write('wtf wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww')
}).on('error', function (err) {
    console.log(err);
})