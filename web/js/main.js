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