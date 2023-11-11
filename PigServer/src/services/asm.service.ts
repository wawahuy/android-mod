import { Injectable, Logger } from '@nestjs/common';

@Injectable()
export class AsmService {
    private _logger = new Logger('AsmService');
    constructor() {
    }

    movAddress5Buffer(register = 'x9', addressBuffer: Buffer) {
        const ascode = `
            mov    ${register}, #0x${addressBuffer.subarray(3, 5).toString('hex')}
            movk   ${register}, #0x${addressBuffer.subarray(1, 3).toString('hex')} , LSL #16
            movk   ${register}, #0x${addressBuffer.subarray(0, 1).toString('hex')} , LSL #32
        `;
        this._logger.debug(ascode);
        return this.asm2hex(ascode);
    }

    asm2hex(asm: string) {
        const data = {
            arch: ["arm64"],
            asm,
            offset: ""
        }
        return fetch("https://armconverter.com/api/convert", {
            "headers": {
                "accept": "*/*",
                "accept-language": "en-US,en;q=0.9,vi;q=0.8",
                "content-type": "text/plain;charset=UTF-8",
                "sec-ch-ua": "\"Microsoft Edge\";v=\"119\", \"Chromium\";v=\"119\", \"Not?A_Brand\";v=\"24\"",
                "sec-ch-ua-mobile": "?0",
                "sec-ch-ua-platform": "\"Windows\"",
                "sec-fetch-dest": "empty",
                "sec-fetch-mode": "cors",
                "sec-fetch-site": "same-origin",
                "Referer": "https://armconverter.com/",
                "Referrer-Policy": "strict-origin-when-cross-origin"
            },
            "body": JSON.stringify(data),
            "method": "POST"
        })
        .then(rs => rs.json())
        .then(rs => rs.hex.arm64[1].replace(/\n/g, ''))
        .catch(err => {
            this._logger.error(err);
            return null;
        });
    }
}
