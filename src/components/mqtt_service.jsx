// import Paho from 'paho-mqtt';
import mqtt from 'mqtt'

// function getRandomInt(max) {
//   return Math.floor(Math.random() * max);
// }
//
// const client_id = "user_serves" + getRandomInt(100);
//
// const mqtt = require("mqtt");
// let client = mqtt.connect("wss://broker.emqx.io:8084");
// // let client = new Paho.Client("broker.emqx.io", Number(8084), client_id);
// // client.connect({onSuccess: () => {console.log("Connected")}});
//
// client.on("connect", () => {
//   console.log("connect");
// });
//
//
// export default client


const client = mqtt.connect('broker.emqx.io', {
  // log: console.log.bind(console),
  host: 'broker.emqx.io',
  keepalive: 30,
  port: 8084,
  path: '/mqtt',
  clientId: 'mqttjs_' + Math.random().toString(16).substr(2, 8),
  protocol: 'wss'

});

client.on("connect", () => {
  console.log("connected");

});

client.on("message", (topic, message) => {
  console.log('message', topic, message.toString());
  // message is Buffer
  // messages.value.push(message.toString());
});

client.on("close", () => {
  console.log("close");
});


export default client
