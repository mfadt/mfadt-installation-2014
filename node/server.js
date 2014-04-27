var http = require('http');
var fs = require('fs');
var path = require('path');
var url = require('url');
var WebSocketServer = require('ws').Server;

//Settings
var WWW_ROOT = './www/';
var HTTP_HOST = 'macaroni.local';
var HTTP_PORT = 8080;
var WS_PORT = 8081;

//Server
var mimeTypes = {
    "html": "text/html",
    "jpeg": "image/jpeg",
    "jpg": "image/jpeg",
    "png": "image/png",
    "js": "text/javascript",
    "css": "text/css"
};

http.createServer( function( req, res ) {

    var fileToLoad;

    if (req.url == '/') {
        fileToLoad = 'index.html';
    }
    else {
        fileToLoad = url.parse( req.url ).pathname.substr(1);
    }

    console.log( '[HTTP] :: Loading :: ' + WWW_ROOT + fileToLoad );

    var fileBytes;
    var httpStatusCode = 200;

    fs.exists( WWW_ROOT + fileToLoad, function( doesItExist) {

        if (!doesItExist) {
            console.log( '[HTTP] :: Error loading :: ' + WWW_ROOT + fileToLoad );

            httpStatusCode = 404;
            fileToLoad = 'error404.html';
        }

        var fileBytes = fs.readFileSync( WWW_ROOT + fileToLoad );

        var mimeType = mimeTypes[path.extname(fileToLoad).split('.')[1]];

        res.writeHead(httpStatusCode, {'Content-type':mimeType});
        res.end(fileBytes);
    });

}).listen(HTTP_PORT, HTTP_HOST);

var radius = 200;
var angle = 0;

var OF;
var clients = [];

var wss = new WebSocketServer({port: WS_PORT});

wss.on('connection', function(ws) {

	console.log("[WS] :: A new websocket connection was made!");

    ws.on('message', function(msg) {
        //console.log('received: %s', msg);
        // ws.send(msg + "\n");

        var parts = msg.split(",");
        var event = parts[0];

        // var jsonMessage = JSON.parse(msg.data);
        // console.log(jsonMessage);

        if (event == "init-of") {
        	OF = ws;
        	ws.send("Hello, openFrameworks.");
        }

        if (event == "init-client") {
            ws.id = 'client_' + Date.now();
        	clients.push(ws);
        	ws.send("Hello, browser.");

            if (OF) OF.send("new-user," + ws.id + ',' + parts[1] + ',' + parts[2] + ',' + parts[3] );
        }

        if (event == 'orientation') {
            if (OF) {
              OF.send('orientation,' + ws.id + ',' + parts[1] + ',' + parts[2] + ',' + parts[3]);
            } 

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
            if (OF) OF.send('close,' + ws.id);
	    	clients.splice(clients.indexOf(this), 1);
    	}

    })

});