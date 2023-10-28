import { WebSocket } from 'ws';

const host = "192.168.128.70";
const port = 1234;

const w = new WebSocket(`ws://${host}:${port}`);
w.on('open', () => {
    console.log('open');
    w.send(JSON.stringify({ test: 123 }));
})
w.on('error', (err) => {
    console.log(err);
})