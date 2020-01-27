const maxApi = require("max-api");
const mqtt = require('mqtt')

//For MQTT
var options = {qos: 1, keepalive:0,reconnectPeriod:5000}

maxApi.post("The node script was started");

//Free mqtt broker
var client  = mqtt.connect('mqtt://broker.hivemq.com:1883 ',{
});

//Called when client is connected
client.on('connect', function () {

    maxApi.post('Status: Subscriber is connected to broker');
});

client.on('message', function (topic, message) {
    // message is Buffer
    console.log(message.toString())
    maxApi.post('Recieved topic:' + topic.toString() + ' and payload' + message.toString());
    maxApi.outletBang();
})


