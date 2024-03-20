const { SerialPort } = require('serialport')
const { ReadlineParser } = require('@serialport/parser-readline')

const path = '/dev/ttyACM0'
const baudRate = 115200

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

const AlarmState = Object.freeze({
    ALARM_TOO_LOW: 0,
    NORMAL: 25,
    PRE_ALARM_TOO_HIGH: 25,
    ALARM_TOO_HIGH: 50,
    ALARM_TOO_HIGH_CRITIC: 100,
})


const valve_prefix = 'vl:'
const mode_prefix = 'md:'

function sendMessage(message) {
    port.write(message, function (err) {
        if (err) {
            return console.log('Error on write: ', err.message)
        }
    })
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

// const parser = port.pipe(new ByteLengthParser({ length: 1 }))
parser.on('data', function (data) {
    parseData(data)
}) // will have 8 bytes per data event


// random from alarmstate
setInterval(() => {
    const rand = Math.floor(Math.random() * Object.keys(AlarmState).length)
    sendMessage('vl:' + AlarmState[Object.keys(AlarmState)[rand]] + "\n")
}, 10000)
