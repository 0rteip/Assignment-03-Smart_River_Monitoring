const ws = new WebSocket('ws://localhost:8080');
const modeP = document.getElementById('state');
const openingP = document.getElementById('opening');
const valveButton = document.getElementById('valveButton');
const valveInput = document.getElementById('openingInput');

const xValues = [];
const yValues = [];
const maxElements = 10;

function updateChart(newValue) {
    xValues.push(new Date().toLocaleString());
    yValues.push(parseFloat(newValue));

    if (xValues.length > maxElements) {
        xValues.shift();
        yValues.shift();
    }

    new Chart("myChart", {
        type: "line",
        data: {
            labels: xValues,
            datasets: [{
                data: yValues,
                borderColor: "red",
                fill: false
            }]
        },
        options: {
            legend: { display: false }
        }
    });
}

ws.onopen = function () {
    console.log('Connessione WebSocket stabilita');
};

ws.onmessage = function (event) {
    const data = JSON.parse(event.data);
    console.log('Messaggio ricevuto', data);

    if (data.topic === 'river-level') {
        updateChart(data.message);
    } else if (data.topic === 'description') {
        modeP.innerHTML = data.message;
    } else if (data.topic === 'valve-opening') {
        openingP.innerHTML = data.message;
    } else {
        console.log('Messaggio non gestito', data);
    }
};

ws.onclose = function () {
    console.log('Connessione WebSocket chiusa');
};

valveButton.onclick = function () {
    const opening = parseFloat(valveInput.value);
    if (opening >= 0 && opening <= 100) {
        ws.send(JSON.stringify({ topic: 'valve-opening', message: opening }));

    } else {
        alert('Inserire un valore compreso tra 0 e 100');
    }
}
