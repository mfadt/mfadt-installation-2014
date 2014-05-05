var express = require('express');
var directory = require('serve-index');
var app = express();
var server = require('http').createServer(app)
var io = require('socket.io').listen(server);
var fs = require('fs');
var WebSocketServer = require('ws').Server;

var WWW_ROOT = '../www';
var SOCKET_IO_PORT = 8080;
var WEBSOCKET_PORT = 8081;

io.set('log level', 2);
server.listen(SOCKET_IO_PORT);

app.use(directory(WWW_ROOT));
app.use(express.static(WWW_ROOT));

io.sockets.on('connection', function (socket) {

	// console.log('new connection: ' + socket.id);

	// load up json file and send it to new clients
	fs.readFile(WWW_ROOT + '/js/data.json', 'utf8', function (err, data) {
		if (err) console.log('Error reading data.json:', err);
		// console.log(data);
		data = JSON.parse(data);
		socket.emit('init', data);
	});

	socket.on('load_model', function (data) {

		var msg = 'load_model,' + data.slug + ',' + data.people;
		if (OF) OF.send(msg);
	});
});


var OF;
var wss = new WebSocketServer({ port: WEBSOCKET_PORT });

wss.on('connection', function(ws) {

	console.log("[WS] :: A new websocket connection was made!");

    ws.on('message', function(msg) {
        //console.log('received: %s', msg);
        // ws.send(msg + "\n");

        // if (OF) OF.send(msg);
        
        var parts = msg.split(",");
        var event = parts[0];

        if (event == "init-of") {
        	OF = ws;
        	ws.send("Hello, openFrameworks.");
        }
    
    });

    ws.on('close', function() {
    	console.log("[WS] :: A websocket was closed.");

    	if (ws == OF) {
    		OF = null;
    	}
    })

});