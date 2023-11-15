import * as ws from 'ws';


const c = new ws.WebSocket('wss://gw-planetpigth.aladinfun.com/gate/ws');

c.onopen = function () {
    console.log('open');
}

c.onmessage = function (data) {
    console.log(data.data);
}