import { WebSocket } from 'ws';

const host = "192.168.128.70";
const port = 1235;

const w = new WebSocket(`ws://${host}:${port}`);
w.on('open', () => {
    console.log('open');
    setTimeout(() => {
        w.send(JSON.stringify({ 
            cmd: 1,
            offset: "1FF656C",
            size: 5
         }));
    }, 1000)
})
w.on('error', (err) => {
    console.log(err);
})