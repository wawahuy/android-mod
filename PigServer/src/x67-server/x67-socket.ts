import { Logger } from '@nestjs/common';
import * as events from 'events';
import * as net from 'net';
import * as crypto from 'crypto'; 
import { isNil, isNumber, isString } from 'lodash';
import { ByteFin, ByteOpcode, DataSize, ENC_SIZE_IV, ENC_SIZE_KEY, FrameSession, FrameSessionStage, FrameSize, HEADER_SIZE, HeaderSize, OFFSET_FIN, OFFSET_IV, OFFSET_OPCODE, OFFSET_SIZE } from './x67-config';
import { randomUint8 } from 'src/utils/common';
import { X67Command } from './x67-command';

declare interface X67Socket {
    // on(event: 'close', listener: () => void): this;
    // on(event: 'cmd', listener: (command: X67Command<any>, socket: X67Socket) => void): this;
    // once(event: 'close', listener: () => void): this;
    on(event: string, listener: Function): this;
    once(event: string, listener: Function): this;
}

class X67Socket extends events.EventEmitter {
    private _logger = new Logger("X67Socket");
    private _key = Buffer.alloc(ENC_SIZE_KEY);

    private _bytesReceived = Buffer.alloc(FrameSize);
    private _totalBytesReceived = 0;

    private _frameSession: FrameSession = { 
        stage: FrameSessionStage.Continue
    };
    private _allData = null;
    private _offsetAllData = 0;

    readonly id = crypto.randomUUID();
    readonly eventSystem = new events.EventEmitter;

    constructor(
        private _socket: net.Socket
    ) {
        super();

        this._logger.log(`client ${this.id} connected!`);
        this._socket.on("close", this.onClose.bind(this));
        this._socket.on('error', this.onError.bind(this));
        this._socket.on('data', this.onData.bind(this));
        this.establish();
    }

    private randomKey(buffer: Buffer, size: number) {
        for(let i = 0; i < size; i++) {
            buffer.writeUInt8(randomUint8(), i);
        }
    }

    private establish() {
        this.randomKey(this._key, ENC_SIZE_KEY);

        /**
         * SEND KEY
         */
        const header = Buffer.alloc(HeaderSize);
        header[0] = ByteFin.end;
        header[1] = ByteOpcode.key;
        header.writeInt16LE(ENC_SIZE_KEY, 2);
        this._socket.write(header);
    
        const sub = Buffer.alloc(DataSize);
        this._key.copy(sub);
        this._socket.write(sub);
    }

    private onError(err: Error) {
        this._logger.error(err);
        this.eventSystem.emit('close');
    }

    private onClose() {
        this._logger.log(`client ${this.id} closed!`);
        this.eventSystem.emit('close');
    }

    private onData(data: Buffer) {
        const stack = [data];


        while (stack.length) {
            const data = stack.pop();
            const dataSize = data.length;

            let dataAdded: Buffer = data;
            let offsetDataRedundancy = -1;
            if (this._totalBytesReceived + dataSize > FrameSize) {
                offsetDataRedundancy = FrameSize - this._totalBytesReceived;
                dataAdded = data.subarray(0, offsetDataRedundancy);
            }
            dataAdded.copy(this._bytesReceived, this._totalBytesReceived);
            this._totalBytesReceived += dataAdded.length;

            if (this._totalBytesReceived == FrameSize) {
                try {
                    this.handleFrame(this._bytesReceived, this._frameSession);
                } catch (err) {
                    this._logger.error(err);
                }
                if (this._frameSession.stage === FrameSessionStage.Error) {
                    this._logger.error(`${this.id} frame false!`);
                    this._socket.destroy();
                }
                this._totalBytesReceived = 0;
            }

            if (offsetDataRedundancy > -1) {
                const sub = data.subarray(offsetDataRedundancy, data.length);
                stack.push(sub);
            }
        }
    }

    private handleFrame(buffer: Buffer, frameSession: FrameSession) {
        const byteFin = buffer[OFFSET_FIN];
        const byteOpcode = buffer[OFFSET_OPCODE];
        const dataSize = buffer.readUInt16LE(OFFSET_SIZE);
        const iv = buffer.subarray(OFFSET_IV, OFFSET_IV + 16);
        const data = buffer.subarray(HEADER_SIZE, FrameSize);
        const isFinEnd = (byteFin & ByteFin.end) === ByteFin.end;
        // console.log("byteFin", byteFin);
        // console.log("byteOpcode", byteOpcode);
        // console.log("size", buffer.subarray(OFFSET_SIZE, OFFSET_SIZE + 2));
        // console.log("size", dataSize);
        // console.log("iv", iv);
        // console.log("data", data.subarray(0, 2), data.subarray(DataSize - 2, DataSize));
      
        const decipher = crypto.createDecipheriv('aes-128-cbc', this._key, iv);
        decipher.setAutoPadding(false);
        let decryptedData = Buffer.concat([decipher.update(data), decipher.final()]);
        decryptedData = decryptedData.subarray(0, dataSize + 1);
      
        if (this._offsetAllData === 0) {
            this._allData = decryptedData;
        } else {
            this._allData = Buffer.concat([this._allData, decryptedData]);
        }
        this._offsetAllData += dataSize;
      
        if (isFinEnd && byteOpcode === ByteOpcode.json) {
            this.handleOpcodeJson(this._allData, frameSession);
            this._allData = null;
            this._offsetAllData = 0;
        }
    }

    private handleOpcodeJson(data: Buffer, frameSession: FrameSession) {
        const dataSize = data.length;
        try {
            const jsonString = data.toString('utf-8', 0, dataSize - 1);
            const js = JSON.parse(jsonString);
            if (isString(js.command) || isNumber(js.command)) {
                this.handleCommand(js);
            }
            frameSession.stage = FrameSessionStage.Continue;
        } catch (error) {
            this._logger.error(error);
            frameSession.stage = FrameSessionStage.Error;
        }
    }

    private handleCommand(cmd: X67Command<any>) {
        // this._logger.log(cmd);
        this.emit(cmd.command, cmd.data);
        this.eventSystem.emit('command', cmd, this);
    }

    sendJson(json: Object) {
        const dataStr = JSON.stringify(json);
        const data = Buffer.alloc(dataStr.length + 1);
        data.write(dataStr, 'utf-8');
        data.writeUInt8(0, dataStr.length);
    
        const dataLength = data.length;
    
        let f = ByteFin.continue;
        for (let i = 0; i < dataLength; i += DataSize) {
            let e = i + DataSize;
            if (e >= dataLength) {
                e = dataLength;
                f = ByteFin.end;
            }
    
            let sub = data.subarray(i, e);
    
            const iv = Buffer.alloc(ENC_SIZE_IV);
            const header = Buffer.alloc(HeaderSize);
            header[OFFSET_FIN] = f;
            header[OFFSET_OPCODE] = ByteOpcode.json;
            header.writeInt16LE(sub.length, OFFSET_SIZE);
            
            for(let i = 0; i < ENC_SIZE_IV; i++) {
                header.writeUInt8(randomUint8(), i + OFFSET_IV);
                iv.writeUInt8(header.at(i + 4), i);
            }
            this._socket.write(header);
    
            if (f == ByteFin.end) {
                const tmp = sub;
                sub = Buffer.alloc(DataSize);
                tmp.copy(sub);
            }
    
            const cipher = crypto.createCipheriv('aes-128-cbc', this._key, iv);
            cipher.setAutoPadding(false);
            const encryptedData = cipher.update(sub);
            this._socket.write(encryptedData);
        }
    }

    command<T>(command: string, data: T) {
        const cmd = new X67Command<T>();
        cmd.command = command;
        cmd.data = data;
        this.sendJson(cmd);
    }
}

export default X67Socket;