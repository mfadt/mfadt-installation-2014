#version 120
uniform vec3 color1; 
uniform vec3 color2;

uniform float time;

uniform float spikesPct;
uniform float spikesFrq;

uniform float AlphaPct;

uniform float falloffDist;
uniform float falloffExpo;
uniform float falloffScl;

float nearClip = 1.0;
float linearizeDepth( in float d ) {
	float dist = 100.0+falloffDist*5000.0;
	return (2.0 * nearClip) / (dist + nearClip - d * (dist - nearClip));
}

void main(){
	float expo = 0.6 + falloffExpo * 10.0;
	float scl = 0.5 + falloffScl * 2.0;
	float alpha = pow((1.0 - linearizeDepth( gl_FragCoord.z )) * scl, expo);
	gl_FragColor = vec4( gl_Color.xyz, gl_Color.w * alpha * AlphaPct);
}