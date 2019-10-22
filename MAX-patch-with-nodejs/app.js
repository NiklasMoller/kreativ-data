var express = require('express'); // Using express: http://expressjs.com/
var app = express(); //Create the app
var server = require('http').createServer(app); //'Http' allows Node.js to transfer data over the Hyper Text Transfer Protocol (HTTP).
// Creates HTTP server that listens to server ports and gives a response back to the client.
var io = require('socket.io').listen(server);

server.listen(process.env.PORT || 3000); //process.env.PORT is related to deploying on heroku




//Set up the server locally on port 3000. This is a shorter version of below
/*
app.listen(3000, () =>
    console.log('Example app listening on port 3000!'),
);
*/


// This call back just tells us that the server has started
function listen() {
  var host = server.address().address;
  var port = server.port;
  console.log('Example app listening at http://' + host + ':' + port);
}


app.get('/', function(req, res){
    res.sendFile(__dirname + '/index.html');
});


//Use the following code to serve images, CSS files, and JavaScript files in a directory named public:
//app.use(express.static('public'));




// Register a callback function to run when we have an individual connection
// This is run for each individual user that connects
// All events we emit goes in here
io.sockets.on('connection',
  // We are given a websocket object in our function
  function (socket) {

    console.log("We have a client connected!");

    // When this user emits, client side: socket.emit('otherevent',some data);
    socket.on('OSC',
      function(data) {
        // Data comes in as whatever was sent, including objects
        console.log("Received: OSC " + data);
        //connections.push(socket); //dont know wht this is for

        // Send it to all other clients
        socket.broadcast.emit('OSC', data);
        socket.broadcast.emit("Sending a message from server that was triggered by OSC")

        // This is a way to send to everyone including sender
        //io.sockets.emit('message', "this goes to everyone");

      }
    );

    socket.on('disconnect', function() {
      console.log("Client has disconnected");
    });
  }
);
