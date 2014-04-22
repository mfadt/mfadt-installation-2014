var WebSocketServer = require('ws').Server;
var wss = new WebSocketServer({port: 8080});

var radius = 200;
var angle = 0;

var OF;
var clients = [];

wss.on('connection', function(ws) {

	console.log("[WS] :: A new websocket connection was made!");

    ws.on('message', function(msg) {
        console.log('received: %s', msg);
        // ws.send(msg + "\n");

        var parts = msg.split(",");
        var event = parts[0];

        if (event == "init-of") {
        	OF = ws;
        	ws.send("Hello, openFrameworks.");
        }

        if (event == "init-client") {
        	clients.push(ws);
        	ws.send("Hello, browser.");
        }

        if (event == "radius") {
        	if (OF) OF.send("radius," + parts[1]);
        }

        if (event == "angle") {
        	if (OF) OF.send("angle," + parts[1]);
        }
    });

    ws.on('close', function() {
    	console.log("[WS] :: A websocket was closed.");

    	if (ws == OF) {
    		OF = null;
    	} else {
	    	clients.splice(clients.indexOf(this), 1);
    	}

    })

});