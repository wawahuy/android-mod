const net = require('net');



const sr = net.createServer();
sr.on('connection', (socket) => {

    const sk = net.createConnection({
        host: 'tdkk1.g4m.dev',
        port: 5800
    })

    sk.on('connect', () => {
        console.log('connected');
        let cancelNext = false;
        socket.on('data', (data) => {
            console.log('real send', data);

            // if (data.equals(Buffer.from([0x00, 0x03, 0x00, 0x01, 0x04]))) {
            //     sk.write(Buffer.from([0x01]));
            //     sk.write(Buffer.from([0x00, 0x05, 0x01, 0x92, 0x02, 0x2c, 0x05]));

            //     // sk.write(Buffer.from([0x29]));
            //     // sk.write(Buffer.from([0x00, 0x03, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x03, 0x00, 0x01, 0x00]));
            //     cancelNext = true;
            // } else {
            //     if (!cancelNext) {
            //         sk.write(data);
            //     }
            //     cancelNext = false;
            // }

            // if (data.equals(Buffer.from([0xf2]))) {
            //     bf = [];
            //     bf.push(data);
            //     cancelNext = true;
            // } else {
            //     if (cancelNext) {
            //         bf.push(data);
            //         for(let i = 0; i< 1000; i++) {
            //             bf.forEach((b) => {
            //                 sk.write(b);
            //                 console.log('send', b);
            //             })
            //         }
            //         cancelNext = false;
            //     } else {
            //         sk.write(data);
            //     }
            // }

            // if (data.equals(Buffer.from([0x3c]))) {
            //     cancelNext = true;
            // } else {
            //     if (cancelNext) {
            //         for(let i = 0; i <= 8; i += 1) {
            //             sk.write(Buffer.from([0x3c]));
            //             sk.write(Buffer.from([0x00, 0x03, 0x00, 0x01, i]));
            //             console.log('send', Buffer.from([0x00, 0x03, 0x00, 0x01, i]))
            //         }
            //         cancelNext = false;
            //     } else {
            //         console.log('send', data);
            //         sk.write(data);
            //     }
            // }

            if (data.equals(Buffer.from([0xe2]))) {
                sk.write(Buffer.from([0xe2]));
                sk.write(Buffer.from([0x00, 0x05, 0x93, 0x00, 0x00, 0xff, 0xff]));
                cancelNext = true;
            } else if (!cancelNext) {
                console.log('wtf', data);
                sk.write(data);
            } else {
                cancelNext = false;
            }
        })
        socket.on('close', () => {
            sk.destroy();
        })
    })

    sk.on('data', (data) => {
        console.log('recv', data);
        socket.write(data);
    })

    sk.once('close', () => {
        console.log('closed');
        socket.destroy();
    })

    sk.once('error', (err) => {
        console.log(err);
        socket.destroy();
    })

})

sr.listen(5800);