import net from 'node:net';


const host = "192.168.128.70";
// const host = "192.168.1.42";
const port = 1234;

const client = net.connect({ host, port }).on('connect', () => {
    console.log('oke');
    client.write("123");
    client.write("456");
}).on('data', (data) => {
    console.log(data.toString('utf-8'));
})
