import { WebSocket } from 'ws';

const host = "192.168.1.42";
const ports = [1234];

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
        setTimeout(() => connect(), 1000);
    });
}

function onConnected(socket) {
    sendCommand(socket, getCommandReset());
    sendCommand(socket, getCommandVIl2CppWatch('1FF656C', 5));
    sendCommand(socket, getCommandEIl2CppWrite('1FF656C', [0x01, 0x02, 0x30]));
    sendCommand(socket, getCommandVIl2CppWatch('1FF656C', 5));
    sendCommand(socket, getCommandEIl2CppWrite('1FF656C', [0x01, 0x22, 0x40]));
}

connect();
