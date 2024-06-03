import Paho from 'paho-mqtt';

function getRandomInt(max) {
  return Math.floor(Math.random() * max);
}

client_id = "user_serves" + getRandomInt(100);

let client = new Paho.Client("broker.emqx.io", Number(8084), );
client.connect({onSuccess: () => {console.log("Connected")}});

export default client
