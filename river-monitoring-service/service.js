const { SerialPort } = require('serialport')
const mqtt = require('mqtt');
const fs = require('fs');
const WebSocket = require('ws');
const { ReadlineParser } = require('@serialport/parser-readline');
const { parse } = require('path');

let isManual = false;
const data = JSON.parse(fs.readFileSync('data.json', 'utf8'));
const {
    Modality,
    Frequecy,
    AlarmState,
    WaterLevel,
    valve_prefix,
    mode_prefix,
    path,
    baudRate,
    wssPort } = data;

const mqttClient = mqtt.connect('mqtt://broker.mqtt-dashboard.com');
const wss = new WebSocket.Server({ port: wssPort });

// Create a port
const port = new SerialPort({
    path: path,
    baudRate: baudRate,
})
const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }));

let currentAlarmState = AlarmState.NORMAL

function sendSerialMessage(message) {
    port.write(message, function (err) {
        if (err) {
            return console.log('Error on write: ', err.message)
        }
    })
}

function sendFrequency(frequency) {
    mqttClient.publish("frequency", frequency.toString());
}

function sendWsData(topic, message) {
    console.log('Sending data to clients:', topic, message)
    wss.clients.forEach(function each(client) {
        if (client.readyState === WebSocket.OPEN) {
            client.send(JSON.stringify({
                topic: topic,
                message: message
            }));
        }
    });
}

function parseData(data) {
    console.log('Received data:', data)
    if (data.startsWith(valve_prefix)) {
        const valve = data.slice(valve_prefix.length)
        console.log('Valve opening:', valve + '%')
        sendWsData("valve-opening", valve)
    }
    else if (data.startsWith(mode_prefix)) {
        const mode = data.slice(mode_prefix.length)

        if (mode === Modality.MANUAL.toString()) {
            console.log('Manual mode')
            isManual = true;

        } else if (mode === Modality.AUTO.toString()) {
            console.log('Automatic mode')
            isManual = false;
        }

        sendWsData("modality", mode)

    } else {
        console.log(data)
    }
}

function sendMode(mode, force = false) {
    if (currentAlarmState !== mode || force) {
        console.log('Sending mode:', mode)
        currentAlarmState = mode
        sendSerialMessage(valve_prefix + currentAlarmState.valveOpening + "\n")
        sendFrequency(currentAlarmState.frequecy)
        sendWsData("description", currentAlarmState.description)
        if (!isManual) {
            sendWsData("valve-opening", currentAlarmState.valveOpening)
        }
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

parser.on('data', function (data) {
    parseData(data)
})


mqttClient.on('connect', function () {
    console.log('Connected to MQTT');
    mqttClient.subscribe('river-level');
    mqttClient.subscribe('frequency');
});

mqttClient.on('message', function (topic, message) {
    if (topic === 'river-level') {
        checkLevel(parseFloat(message.toString()))
        sendWsData(topic, parseFloat(message.toString()))
    }
});

wss.on('connection', function connection(ws) {
    console.log('Client connected');
    sendMode(currentAlarmState, true);

    ws.on('message', function incoming(message) {
        const data = JSON.parse(message);
        if (data.topic === 'valve-opening') {
            sendSerialMessage(valve_prefix + data.message + "\n")
            // currentAlarmState.valveOpening = data.message
            const currentAlarmStateCopy = { ...currentAlarmState }
            currentAlarmStateCopy.valveOpening = data.message
            sendMode(currentAlarmStateCopy, true)
        }

    });
});
