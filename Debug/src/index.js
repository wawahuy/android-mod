
import net from 'net';
import crypto from  'crypto';

const server = net.createServer();
server.listen(1235);

const fin = {
    end: 0x01,
    continue: 0x00,
};

const opcode = {
    key:  0b00000001,
    json: 0b00000011,
};

const frameSize = 1024;
const headerSize = 32; // fin opcode 2size 16iv 12pad ....

const key = Buffer.from([0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8]);

function sendKey(socket) {
    const dataMaxSize = frameSize - headerSize; 

    const header = Buffer.alloc(headerSize);
    header[0] = fin.end;
    header[1] = opcode.key;
    header.writeInt16LE(key.length, 2);
    socket.write(header);

    const sub = Buffer.alloc(dataMaxSize);
    key.copy(sub);
    socket.write(sub);
}

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

const SOCKET_BUFFER_SIZE = 1024;
const HEADER_SIZE = 32;
const OFFSET_FIN = 0;
const OFFSET_OPCODE = 1;
const OFFSET_SIZE = 2;
const OFFSET_IV = 4;
const BYTE_FIN_END = 0x01;
const BYTE_OPCODE_KEY = 0b00000001;
const BYTE_OPCODE_JSON = 0b00000011;

let allData = null;
let offsetAllData = 0;

function handleOpcodeJson(data, fs) {
  const dataSize = data.length;
  try {
    const jsonString = data.toString('utf-8', 0, dataSize - 1);
    const js = JSON.parse(jsonString);
    console.log(js);
    fs.stage = 'Continue';
  } catch (error) {
    console.error('JSON Parse Error:', error);
    fs.stage = 'Error';
  }
}

function handleFrame(buffer, fs) {
const dataMaxSize = frameSize - headerSize; 
  const byteFin = buffer[OFFSET_FIN];
  const byteOpcode = buffer[OFFSET_OPCODE];
  const dataSize = buffer.readUInt16LE(OFFSET_SIZE);
  const iv = buffer.slice(OFFSET_IV, OFFSET_IV + 16);
  const data = buffer.slice(HEADER_SIZE, SOCKET_BUFFER_SIZE);
  const isFinEnd = (byteFin & BYTE_FIN_END) === BYTE_FIN_END;
  console.log("byteFin", byteFin);
  console.log("byteOpcode", byteOpcode);
  console.log("size", buffer.slice(OFFSET_SIZE, OFFSET_SIZE + 2));
  console.log("size", dataSize);
  console.log("iv", iv);
  console.log("data", data.slice(0, 2), data.slice(dataMaxSize - 2, dataMaxSize));

  if (key === null) {
    console.error('Not key');
    fs.stage = 'Error';
    return;
  }



  const decipher = crypto.createDecipheriv('aes-128-cbc', key, iv);
  decipher.setAutoPadding(false);
  let decryptedData = Buffer.concat([decipher.update(data), decipher.final()]);
  decryptedData = decryptedData.slice(0, dataSize + 1);
  console.log(decryptedData.toString('utf-8'));

  if (offsetAllData === 0) {
    allData = decryptedData;
  } else {
    allData = Buffer.concat([allData, decryptedData]);
  }
  offsetAllData += dataSize;

  if (isFinEnd && byteOpcode === BYTE_OPCODE_JSON) {
    handleOpcodeJson(allData, fs);
    allData = null;
    offsetAllData = 0;
  }
}


server.on('connection', (socket) => {
    sendKey(socket);
    let frameSession = { stage: 'Continue' };
    const buffer = Buffer.alloc(SOCKET_BUFFER_SIZE);
    let totalBytesReceived = 0;

    function pushData(data) {
        let irem = -1;
        if (totalBytesReceived + data.length > SOCKET_BUFFER_SIZE) {
            data.subarray(0, SOCKET_BUFFER_SIZE - totalBytesReceived).copy(buffer, totalBytesReceived);
            irem = SOCKET_BUFFER_SIZE - totalBytesReceived;
            totalBytesReceived = SOCKET_BUFFER_SIZE;
        } else {
            data.copy(buffer, totalBytesReceived);
            totalBytesReceived += data.length;
        }
        console.log(totalBytesReceived);
    
        if (totalBytesReceived == SOCKET_BUFFER_SIZE) {
            console.log('handleFrame', buffer.length);
          handleFrame(buffer, frameSession);
          console.log('handleFrame done!');
          if (frameSession.stage === 'Error') {
            console.error('Frame false!');
            socket.destroy();
          }

          totalBytesReceived = 0;
        }

        if (irem > -1) {
            const sub = data.subarray(irem, data.length);
            pushData(sub);
        }
    }

    socket.on('data', (data) => {
        pushData(data);
    });

    socket.once('error', (e) => {
        console.log(e);
    })
}).on('error', () => {
    console.log('sk err');
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

// Slingshot::OnPointerUp
// addr: 01D4766C - NOP - 1F 20 03 D5 (1 cham)
// addr: 01D47618 - NOP - 1F 20 03 D5 (x8)
// addr: 01D47620 - NOP - 1F 20 03 D5 (nhanh)
