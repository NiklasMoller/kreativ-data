var express = require('express');
var app = express();
var http = require('http').createServer(app);

const fs = require('fs');


//-------- Starting Server with Express
let port = process.env.PORT; //To run on Heroku
if (port == null || port == "") { //To run on local host
    port = 3000;
}

http.listen(port, function(){
    console.log('listening on *:3000');
});


app.get('/video', function(req, res) {
  const path = 'test.mp4'
  const stat = fs.statSync(path)
  const fileSize = stat.size
  const range = req.headers.range

  if (range) {
    const parts = range.replace(/bytes=/, "").split("-")
    const start = parseInt(parts[0], 10)
    const end = parts[1]
      ? parseInt(parts[1], 10)
      : fileSize-1
    const chunksize = (end-start)+1
    const file = fs.createReadStream(path, {start, end})
    const head = {

      'Content-Range': `bytes ${start}-${end}/${fileSize}`,
      'Accept-Ranges': 'bytes',
      'Content-Length': chunksize,
      'Content-Type': 'video/mp4',
    }

    res.writeHead(206, head);
    file.pipe(res);

  } else {

    const head = {
      'Content-Length': fileSize,
      'Content-Type': 'video/mp4',
    }

    res.writeHead(200, head)

    fs.createReadStream(path).pipe(res)
  }

});
