const ws = new WebSocket('ws://localhost:8080');

// Callback chiamata quando la connessione WebSocket viene stabilita
ws.onopen = function () {
    console.log('Connessione WebSocket stabilita');
};

// Callback chiamata quando il client riceve un messaggio WebSocket dal server
ws.onmessage = function (event) {
    const data = JSON.parse(event.data);
    console.log('Messaggio ricevuto:', data.message);
    // Qui puoi fare qualsiasi cosa tu voglia con i dati ricevuti, ad esempio aggiornare l'interfaccia utente
    aggiornaInterfacciaUtente(data.message);
};

// Callback chiamata quando la connessione WebSocket viene chiusa
ws.onclose = function () {
    console.log('Connessione WebSocket chiusa');
};

function aggiornaInterfacciaUtente(message) {
    // Qui puoi aggiornare l'interfaccia utente con i dati ricevuti
    // Ad esempio, puoi visualizzare i dati in un elemento HTML
    const messagesDiv = document.getElementById('messages');
    messagesDiv.innerHTML += '<p>' + message + '</p>';
}
