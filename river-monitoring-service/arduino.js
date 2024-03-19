const { SerialPort } = require('serialport')
const { ByteLengthParser } = require('@serialport/parser-byte-length')
// const { ReadlineParser } = require('@serialport/parser-readline')

const path = '/dev/ttyACM0'
const baudRate = 115200

// Create a port
const port = new SerialPort({
    path: path,
    baudRate: baudRate,
})
// const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }));


// port.on('readable', function () {
//     const data = port.read();
//     console.log('Data:', data.toString())
// })


// port.on('data', (data) => {
//     console.log(data.toString());
// });

const parser = port.pipe(new ByteLengthParser({ length: 1 }))
parser.on('data', function (data) {
    console.log('Data:', data.toString())
}) // will have 8 bytes per data event
