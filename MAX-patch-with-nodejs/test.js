const maxApi = require("max-api");
const io = require("socket.io-client");

// Start a socket connection to the server
// Some day we would run this server somewhere else
socket = io.connect('http://localhost:3000');


//Different handlers for the maxApi
const handlers = {
    my_message: () => {
      console.log("got my_message");
      maxApi.post("Got a message");
    },
    my_message_with_args: (arg1, arg2) => {
      console.log("got my arged message: ${arg1}, ${arg2} ");
    },
    '/test': () =>{
        maxApi.post("Testing if /test is recieved");
        maxApi.outletBang();
    },
    '/test': (arg1) =>{
        maxApi.post("Testing if /test is recieved");
        maxApi.post("got my arged message: " + arg1);
        maxApi.outletBang();

      // Send that object to the socket
      socket.emit('OSC',arg1);

    }
  };


maxApi.post("The node script was started");

maxApi.addHandler('/test', () =>{
    maxApi.post("Got a test from Arduino");
    maxApi.outlet(22);
    console.log('logging from console');
});

maxApi.addHandlers(handlers);
