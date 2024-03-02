clientID = "clientID-"+parseInt(Math.random() * 100);

host = "broker.emqx.io";
port = 8083;


client = new Paho.MQTT.Client(host, port,clientID);

client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

client.connect({
    onSuccess: onConnect
//        userName: userId,
//       passwordId: passwordId
});

function onConnect(){
    console.log("connect")
}



function onConnectionLost(responseObject){
    console.log("<span> ERROR: Connection is lost.</span><br>");
    if(responseObject !=0){
        console.log(responseObject.errorMessage );
    }

    client.connect({
        onSuccess: onConnect
    //        userName: userId,
    //       passwordId: passwordId
    });
}

function onMessageArrived(message){
    console.log("OnMessageArrived: "+message.payloadString);
}

function startDisconnect(){
    client.disconnect();
}

function publishMessage(){
  Message = new Paho.MQTT.Message(document.getElementById("waterVolume").value);
  Message.destinationName = "semelion/value";

  client.send(Message);
  console.log(Message);
}
