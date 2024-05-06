import Paho from 'paho-mqtt';

let client = new Paho.Client("broker.emqx.io", Number(8083), 'pupka_zalupka');
client.connect({onSuccess: () => {console.log("Connected")}});

export default client