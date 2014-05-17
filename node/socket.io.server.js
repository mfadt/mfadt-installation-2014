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

var cameras = [
    'BACK',
    'CLOSE',
    'CLOSESIDE',
    'FAR',
    'FRONT',
    'RIGHT',
    'LEFT',
    'SUPERFAR',
    'TOPDOWN',
    'UNDER'
];
var cameraTimer;
var flockingTimer;
var flockingAmount = 0.8;

var queue = require(WWW_ROOT + '/js/data.json');
var queueTimer;

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
        console.log(msg);
		
        if (OF) {
            flockingAmount = 1;
            OF.send('flocking,1');
            OF.send(msg);
            setTimeout(reduceFlocking, 500);
        }
	});
});


var OF;
var wss = new WebSocketServer({ port: WEBSOCKET_PORT });

wss.on('connection', function(ws) {

	console.log("[WS] :: A new websocket connection was made!");

    ws.on('message', function(msg) {
        //console.log('received: %s', msg);
        // ws.send(msg + "\n");

        if (OF) OF.send(msg);
        
        var parts = msg.split(",");
        var event = parts[0];

        if (event == "init-of") {
        	OF = ws;
        	ws.send("Hello, openFrameworks.");
            cameraTimer = setInterval(setRandomCamera, 5 * 1000);
            queueTimer = setInterval(setRandomModel, 10 * 1000);
        }

        if (event == "load_model") {
            if (OF) {
                flockingAmount = 1;
                OF.send('flocking,1');
                OF.send(msg);
                setTimeout(reduceFlocking, 1000);
            }
        }
    
    });

    ws.on('close', function() {
    	console.log("[WS] :: A websocket was closed.");

    	if (ws == OF) {
    		OF = null;
            clearInterval(cameraTimer);
            clearInterval(queueTimer);
    	}
    })

});

var setRandomCamera = function() {
    var cam = cameras[parseInt(cameras.length * Math.random())];
    if (OF) OF.send('camera,' + cam);
}

var setRandomModel = function() {
    var data = queue[parseInt(queue.length * Math.random())];
    if (OF) OF.send('load_model,' + data.slug + ',' + data.people);
}

var reduceFlocking = function() {
    console.log('reduceFlocking');
    console.log('flockingAmount', flockingAmount);

    flockingAmount -= 0.1;
    if (flockingAmount < 0) return;

    if (OF) {
        var msg = 'flocking,' + flockingAmount;
        OF.send(msg);
        console.log(msg);
        setTimeout(reduceFlocking, 1000);
    }
}
