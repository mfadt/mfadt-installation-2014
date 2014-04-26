$(document).ready( function() {
	$('#radius').slider({
		min: 10,
		max: 250,
	});

	$('#radius').on( 'slide', function( evt, ui ) {
		var value = $('#radius').slider('values', 0);
		$('#radius h3').text( value );
		$('#circle').css( 'width', value );
		$('#circle').css( 'height', value );
		$('#circle').css( 'border-radius', value );
	});

	$('#angle').slider({
		min: 0,
		max: 90
	});

	$('#angle').on( 'slide', function( evt, ui ) {
		var value = $('#angle').slider('values', 0);
		$('#angle h3').text( value );
		$('#circle').css( 'transform', 'rotateX(' + value + 'deg)' );
		$('#circle').css( '-webkit-transform', 'rotateX(' + value + 'deg)' );
	});
});

// var container, stats;

// var camera, scene, renderer;

// var mesh, geometry, materials;

// init();
// animate();

// function init() {
// 	container = document.getElementById('container');

// 	camera = new THREE.PerspectiveCamera( 75, window.innerWidth / window.innerHeight, 0.1, 10000 );
// 	camera.position.set(0,100,300);

// 	controls = new THREE.TrackballControls( camera );
// 	controls.addEventListener( 'change', render );

// 	//Stats
// 	stats = new Stats();
// 	stats.domElement.style.position = 'absolute';
// 	stats.domElement.style.top = '0px';
// 	container.appendChild( stats.domElement );

// 	scene = new THREE.Scene();

// 	var spotLight = new THREE.PointLight( 0xffffff, 1.0, 1000 );
// 	spotLight.position.set( 0, 0, 500 );
// 	scene.add(spotLight);

// 	var hemiLight = new THREE.HemisphereLight( 0xffffff );
// 	scene.add(hemiLight);

// 	renderer = new THREE.CanvasRenderer();
// 	renderer.setClearColor( 0x111111 );
// 	renderer.setSize( window.innerWidth, window.innerHeight );

// 	container.appendChild( renderer.domElement );

// 	var material = new THREE.MeshBasicMaterial( { color: 0xFF0000 } );
// 	var geometry = new THREE.RingGeometry( 100, 103, 12, 12 );
// 	mesh = new THREE.Mesh( geometry, material );
// 	mesh.rotation.x = - Math.PI / 2;
// 	scene.add( mesh );

// }

// function animate() {

// 	requestAnimationFrame( animate );
// 	controls.update();

	

// 	stats.update();

// 	render();
// }

// function render() {
// 	camera.lookAt( scene.position );

// 	renderer.render( scene, camera );

// }