const ws = new WebSocket('ws://localhost:8080');
// const messagesDiv = document.getElementById('messages');
const modeP = document.getElementById('mode');
const openingP = document.getElementById('opening');

// Callback chiamata quando la connessione WebSocket viene stabilita
ws.onopen = function () {
    console.log('Connessione WebSocket stabilita');
};

// Callback chiamata quando il client riceve un messaggio WebSocket dal server
ws.onmessage = function (event) {
    const data = JSON.parse(event.data);
    // Qui puoi fare qualsiasi cosa tu voglia con i dati ricevuti, ad esempio aggiornare l'interfaccia utente
    if (data.topic === 'river-level') {
        aggiornaInterfacciaUtente(data.message);
    } else if (data.topic === 'current_state') {
        modeP.innerHTML = data.description;
        openingP.innerHTML = data.valveOpening;
    } else {
        console.log('Messaggio non gestito', data);
    }
};

const xValues = [];
const yValues = [];
const maxElements = 10;

function aggiornaInterfacciaUtente(message) {
    // const l = document.createElement('li');
    // l.innerHTML = message
    // messagesDiv.appendChild(l);


    xValues.push(Date.now());
    yValues.push(parseFloat(message));

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

// Callback chiamata quando la connessione WebSocket viene chiusa
ws.onclose = function () {
    console.log('Connessione WebSocket chiusa');
};
