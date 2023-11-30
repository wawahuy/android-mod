const { createServer } = require('http');
const { parse } = require('url');
const { WebSocketServer, WebSocket } = require('ws');

const server = createServer();
const wss = new WebSocketServer({ noServer: true });
let ns;

wss.on('connection', function connection(ws) {

  for (let i =0 ; i<1; i++) {
    const c = new WebSocket(ns);
    c.addEventListener('open', function () {
      console.log('connect', i);
      ws.addEventListener('message', function (data) {
        const d2 = data.data.readUint16BE(0);
        console.log(d2);
        console.log('-------------------------------')
        console.log(data.data.toString())
        console.log(data.data);
        console.log(data.data.subarray(d2, data.data.length).toString());

        // console.log('send', data.data.toString());
        c.send(data.data);
      });
    });
    c.addEventListener('close', function () {
      console.log('close', i);
    });
    c.addEventListener('message', function (data) {

      // console.log('recv', data.data.toString());
      // if (i == 49) {
        ws.send(data.data);
      // } 
    });
  }




  ws.on('error', console.error);
});

server.on('upgrade', function upgrade(request, socket, head) {
  const { pathname, query } = parse(request.url);
  ns = atob(query.split('=')[1]);

  wss.handleUpgrade(request, socket, head, function done(ws) {
    wss.emit('connection', ws, request);
  });
});

server.listen(1236);