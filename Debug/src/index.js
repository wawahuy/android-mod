
import net from 'net';

const server = net.createServer();
server.listen(1235);

server.on('connection', (socket) => {
    socket.write("abc");
    socket.write(Buffer.alloc(1021));
    console.log(socket);
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
