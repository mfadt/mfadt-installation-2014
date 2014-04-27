var socket = new WebSocket("ws://macaroni.local:8081/");

//Generate a random color for our light.
var red = Math.floor(Math.random()*150) + 100;
var green = Math.floor(Math.random()*150) + 100;
var blue = Math.floor(Math.random()*150) + 100;
var color = "rgb(" + red + "," + green + "," + blue + ")";
document.body.style.background = color;



socket.addEventListener('open', function(evt) {
	console.log('[WS] :: Socket opened!');
	socket.send('init-client,' + red + ',' + blue + ',' + green);
});

socket.addEventListener('message', function(evt) {
	console.log('[WS] :: Message received!');

	//var jsonMessage = JSON.parse(evt.data);

	// if (typeof jsonMessage == 'string') {
	// 	jsonMessage = JSON.parse(jsonMessage);
	// }
	console.log (evt.data);
});


var tiltLR = document.getElementById('doTiltLR');
var tiltFB = document.getElementById('doTiltFB');
var dir = document.getElementById('doDirection');
var accX = document.getElementById('accX');
var accY = document.getElementById('accY');
var accZ = document.getElementById('accZ');


//Device orientation/rotation
if (window.DeviceOrientationEvent) {
	document.getElementById("doEvent").innerHTML = "DeviceOrientation";

  // Listen for the event and handle DeviceOrientationEvent object
  window.addEventListener('deviceorientation', function( eventData ) {

  	var LR = eventData.gamma;
  	tiltLR.innerHTML = LR;

  	var FB = eventData.beta;
  	tiltFB.innerHTML = FB;

  	var D = eventData.alpha;
  	dir.innerHTML = D;

  	sendOrientation( LR, FB, D );
  }, false);
}

if (window.DeviceMotionEvent) {
	window.addEventListener('devicemotion', function( eventData) {
  		var acceleration = eventData.acceleration;
  		accX.innerHTML = acceleration.x;
  		accY.innerHTML = acceleration.y;
  		accZ.innerHTML = acceleration.z;
	});
}

function sendOrientation( LR, FB, D ) {
	socket.send( 'orientation,' + LR + ',' + FB + ',' + D );
}
