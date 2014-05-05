var express = require('express');
var directory = require('serve-index');
var app = express();
var server = require('http').createServer(app)
var io = require('socket.io').listen(server);
var fs = require('fs');

var WWW_ROOT = '../www';

io.set('log level', 2);
server.listen(8080);

app.use(directory(WWW_ROOT));
app.use(express.static(WWW_ROOT));

io.sockets.on('connection', function (socket) {

	// load up json file and send it to new clients
	fs.readFile(WWW_ROOT + '/js/data.json', 'utf8', function (err, data) {
		if (err) console.log('Error reading data.json:', err);
		console.log(data);
		socket.emit('init', data);
	});

	socket.on('my other event', function (data) {
		console.log(data);
	});
});