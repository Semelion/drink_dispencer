import React, { useState } from 'react';
import client from './mqtt_service';
// import Paho from 'paho-mqtt';

function send_val() {
  const input = document.getElementById("waterVolume");
  // const mess = new Paho.Message(input.value);
  // mess.destinationName = "semelion/value"
  // client.send(mess);
  client.publish("semelion/value", input.value)
  console.log(input.value);
}

function Button() {
    return (<button onClick={send_val}>Налить</button>)
}

export default Button
