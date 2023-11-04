// fin opcode 2size 16iv 12pad .... (32 byte header)

export const ByteFin = {
    end: 0x01,
    continue: 0x00,
};

export const ByteOpcode = {
    key:  0b00000001,
    json: 0b00000011,
};

export const FrameSize = 1024;
export const HeaderSize = 32;
export const DataSize = FrameSize - HeaderSize;
export const HEADER_SIZE = 32;
export const OFFSET_FIN = 0;
export const OFFSET_OPCODE = 1;
export const OFFSET_SIZE = 2;
export const OFFSET_IV = 4;

// AES-128-CBC
export const ENC_SIZE_KEY = 16; // 16 byte
export const ENC_SIZE_IV = 16;

export enum FrameSessionStage {
    Continue,
    Error,
}

export class FrameSession {
    stage: FrameSessionStage;
}