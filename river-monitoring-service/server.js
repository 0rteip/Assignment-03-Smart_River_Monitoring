const mqtt = require('mqtt');
const WebSocket = require('ws');

const mqttClient = mqtt.connect('mqtt://broker.mqtt-dashboard.com');
const wss = new WebSocket.Server({ port: 8080 });

mqttClient.on('connect', function () {
    console.log('Connected to MQTT');
    mqttClient.subscribe('river-level');
    mqttClient.subscribe('frequency');

});

mqttClient.on('message', function (topic, message) {
    console.log(message.toString())
    if (topic === 'river-level')
    wss.clients.forEach(function each(client) {
        if (client.readyState === WebSocket.OPEN) {
            client.send(JSON.stringify({ topic: topic, message: message.toString() }));
        }
    });
});

wss.on('connection', function connection(ws) {
    console.log('Client connected');
});

function sendFrequency() {
    mqttClient.publish("frequecy", "freq");
}

