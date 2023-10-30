import { WebSocket } from 'ws';

const host = "192.168.1.42";
// const ports = [1234, 1235, 1236];
const ports = [1235];

const command = {
    VIl2CppWatch: 1,
    VIl2CppMapList: 2,
    EIl2CppWrite: 100,
    Reset: 1000,
}

const commandStr = Object.keys(command).reduce((val, item) => {
    val[command[item]] = item;
    return val;
}, {});

function sendCommand(socket, command) {
    console.log('command:', commandStr[command.cmd] || 'none', command);
    return socket.send(JSON.stringify(command));
}

function getCommandVIl2CppWatch(offset, size) {
    return { 
        cmd: command.VIl2CppWatch,
        offset,
        size,
    };
}

function getCommandVIl2CppMapList() {
    return { 
        cmd: command.VIl2CppMapList,
    };
}

function getCommandEIl2CppWrite(offset, bytes) {
    return { 
        cmd: command.EIl2CppWrite,
        offset,
        bytes,
    };
}


function getCommandReset() {
    return { 
        cmd: command.Reset,
    };
}

function connect() {
    const port = ports.shift();
    ports.push(port);
    console.log('connecting...', port);

    const socket = new WebSocket(`ws://${host}:${port}`);
    socket.on('open', () => {
        console.log('connected!');
        onConnected(socket);
    });
    socket.on('error', (err) => {
        console.log(err);
    });
    socket.on('close', () => {
        console.log('closed!');
        setTimeout(() => connect(), 100);
    });
}

function onConnected(socket) {
    sendCommand(socket, getCommandReset());
    sendCommand(socket, getCommandVIl2CppMapList());
    // sendCommand(socket, getCommandVIl2CppWatch('223C4F0', 10));
    // sendCommand(socket, getCommandEIl2CppWrite('223C4F0', [0x00, 0x00, 0x00]));

    // const nop = [0x1f, 0x20, 0x03, 0xd5];
    // const s = 68 / nop.length + 1;
    // sendCommand(socket, getCommandVIl2CppWatch('223C524', 10));
    // sendCommand(socket, getCommandVIl2CppWatch('223C568', 10));
    // sendCommand(socket, getCommandEIl2CppWrite('223C524', Array.from({ length: s }).reduce((val) => {
    //     val = val.concat(nop);
    //     return val;
    // }, [])));

}

// CalculateHitResult 0x223C4F0
// Jumb 0x223C604 -> 0x223C618 
// Jumb 0x223C64c -> 0x223C710

// op: B jum_size + 14
//       00 00 00
// jump dst - current addr / 4
// ex: 
// addr: 0x223C64c - B libil2cpp.so + 0x223C710
// jum_size = 0x223C710 - 0x223C64c / 4 = 31 => 31 00 00 14

//
// addr: 0x223C604 - B libil2cpp.so + 0x223C618 - 05 00 00 14
// addr: 0x223C64c - B libil2cpp.so + 0x223C710 - 31 00 00 14

connect();
