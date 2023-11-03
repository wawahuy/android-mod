
import net from 'net';
import crypto from  'crypto';

const server = net.createServer();
server.listen(1235);

const fin = {
    end: 0x01,
    continue: 0x00,
};

const opcode = {
    json: 0b00000001,
};

const frameSize = 1024;
const headerSize = 32; // fin opcode 2size 16iv 12pad ....

const key = Buffer.from([0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8]);

function sendJson(socket, json) {
    const dataMaxSize = frameSize - headerSize; 
    const dataStr = JSON.stringify(json);

    const data = Buffer.alloc(dataStr.length + 1);
    data.write(dataStr, 'utf-8');
    data.writeUInt8(0, dataStr.length);

    const dataLength = data.length;

    let f = fin.continue;
    for (let i = 0; i < dataLength; i += dataMaxSize) {
        let e = i + dataMaxSize;
        if (e >= dataLength) {
            e = dataLength;
            f = fin.end;
        }

        let sub = data.subarray(i, e);

        const iv = Buffer.alloc(16);
        const header = Buffer.alloc(headerSize);
        header[0] = f;
        header[1] = opcode.json;
        header.writeInt16LE(sub.length, 2);
        
        for(let i = 0; i < 16; i++) {
            header.writeInt8(Math.floor(Math.random() * 125), i + 4);
            iv.writeInt8(header.at(i + 4), i);
        }

        console.log(header)
        socket.write(header);

        if (f == fin.end) {
            const tmp = sub;
            sub = Buffer.alloc(dataMaxSize);
            tmp.copy(sub);
        }

        const cipher = crypto.createCipheriv('aes-128-cbc', key, iv);
        cipher.setAutoPadding(false);
        let encryptedData = cipher.update(sub);
        console.log(encryptedData);

        const decipher = crypto.createDecipheriv('aes-128-cbc', key, iv);
        decipher.setAutoPadding(false);
        let decryptedData = Buffer.concat([decipher.update(encryptedData), decipher.final()]);
        console.log('Old Data (Buffer):', sub);
        console.log('Decrypted Data (Buffer):', decryptedData);

        socket.write(encryptedData);
    }
}

server.on('connection', (socket) => {
    sendJson(socket, { oq: true });
    // socket.write(Buffer.from([
    //     fin.end,
    //     opcode.json,
    //     ...keys
    // ]));

    // socket.write(Buffer.alloc(1017));
    // console.log(socket);

    // sendJson(socket, Array.from({length: 2000 }).map(() => ({ test: 1 })));
});


// CalculateHitResult 0x223C4F0
// Jumb 0x223C604 -> 0x223C618 
// Jumb 0x223C64c -> 0x223C710

// op: B jum_size + 14
//       00 00 00
// jump dst - current addr / 4
// ex: 
// addr: 0x223C64c - B libil2cpp.so + 0x223C710
// jum_size = 0x223C710 - 0x223C64c / 4 = 31 => 31 00 00 14

// PineTree::CalculateHitResult
// addr: 0x223C604 - B libil2cpp.so + 0x223C618 - 05 00 00 14
// addr: 0x223C64c - B libil2cpp.so + 0x223C710 - 31 00 00 14

// Slingshot::Shot
// addr: 01e47d70 - B libil2cpp.so + 01e47ef4 - 61 00 00 14
