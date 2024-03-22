const { SerialPort } = require('serialport')
const mqtt = require('mqtt');
const WebSocket = require('ws');
const { ReadlineParser } = require('@serialport/parser-readline')

const path = '/dev/ttyACM0'
const baudRate = 115200
const wssPort = 8080

const mqttClient = mqtt.connect('mqtt://broker.mqtt-dashboard.com');
const wss = new WebSocket.Server({ port: wssPort });

// Create a port
const port = new SerialPort({
    path: path,
    baudRate: baudRate,
})
const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }));

const Modality = Object.freeze({
    AUTO: 0,
    MANUAL: 1,
});

const Frequecy = Object.freeze({
    F1: 5000,
    F2: 1000
})

const AlarmState = Object.freeze({
    ALARM_TOO_LOW: { valveOpening: 0, frequecy: Frequecy.F1 },
    NORMAL: { valveOpening: 25, frequecy: Frequecy.F1 },
    PRE_ALARM_TOO_HIGH: { valveOpening: 25, frequecy: Frequecy.F2 },
    ALARM_TOO_HIGH: { valveOpening: 50, frequecy: Frequecy.F2 },
    ALARM_TOO_HIGH_CRITIC: { valveOpening: 100, frequecy: Frequecy.F2 },
})

const WaterLevel = Object.freeze({
    WL1: 0.14,
    WL2: 0.10,
    WL3: 0.08,
    WL4: 0.04
})

const valve_prefix = 'vl:'
const mode_prefix = 'md:'

let currentAlarmState = AlarmState.NORMAL

function sendMessage(message) {
    port.write(message, function (err) {
        if (err) {
            return console.log('Error on write: ', err.message)
        }
    })
}

function sendFrequency(frequency) {
    mqttClient.publish("frequency", frequency.toString());
}

function parseData(data) {
    if (data.startsWith(valve_prefix)) {
        const valve = data.slice(valve_prefix.length)
        console.log('Valve opening:', valve + '%')
    }
    else if (data.startsWith(mode_prefix)) {
        const mode = data.slice(mode_prefix.length)

        if (mode === Modality.MANUAL.toString()) {
            console.log('Manual mode')

        } else if (mode === Modality.AUTO.toString()) {
            console.log('Automatic mode')
        }

    } else {
        console.log(data)
    }
}

function sendMode(mode) {
    if (currentAlarmState !== mode) {
        sendMessage(valve_prefix + mode.valveOpening + "\n")
        sendFrequency(mode.frequecy)
        sendWsData('alarm_state', currentAlarmState)
        sendWsData('opening', mode.valveOpening)
        currentAlarmState = mode
    }
}

function checkLevel(level) {
    if (level > WaterLevel.WL1) {
        sendMode(AlarmState.ALARM_TOO_LOW)
    } else if (WaterLevel.WL1 > level && level > WaterLevel.WL2) {
        sendMode(AlarmState.NORMAL)
    } else if (WaterLevel.WL2 > level && level >= WaterLevel.WL3) {
        sendMode(AlarmState.PRE_ALARM_TOO_HIGH)
    } else if (WaterLevel.WL3 > level && level >= WaterLevel.WL4) {
        sendMode(AlarmState.ALARM_TOO_HIGH)
    } else if (level < WaterLevel.WL4) {
        sendMode(AlarmState.ALARM_TOO_HIGH_CRITIC)
    }
}

function sendWsData(topic, message) {
    wss.clients.forEach(function each(client) {
        if (client.readyState === WebSocket.OPEN) {
            client.send(JSON.stringify({ topic: topic, message: message.toString() }));
        }
    });
}

// const parser = port.pipe(new ByteLengthParser({ length: 1 }))
parser.on('data', function (data) {
    parseData(data)
}) // will have 8 bytes per data event


mqttClient.on('connect', function () {
    console.log('Connected to MQTT');
    mqttClient.subscribe('river-level');
    mqttClient.subscribe('frequency');

});

mqttClient.on('message', function (topic, message) {
    if (topic === 'river-level') {
        checkLevel(parseFloat(message.toString()))
        sendWsData(topic, message)
    }
});


wss.on('connection', function connection(ws) {
    console.log('Client connected');
});
