#version 120
uniform vec3 color1; 
uniform vec3 color2;

uniform float time;
uniform float spikesPct;
uniform float spikesFrq;

varying vec2 texCoord;
varying float angle;

float rand(vec2 co){
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float rand(float c){
	return rand(vec2(c,1.0));
}

float noise2f( in vec2 p ){
	vec2 ip = vec2(floor(p));
	vec2 u = fract(p);
	u = u*u*(3.0-2.0*u);
	
	float res = mix(
		mix(rand(ip),  rand(ip+vec2(1.0,0.0)),u.x),
		mix(rand(ip+vec2(0.0,1.0)),   rand(ip+vec2(1.0,1.0)),u.x),
		u.y);
	return res*res;
}

float fbm(vec2 c) {
	float f = 0.0;
	float w = 1.0;
	for (int i = 0; i < 8; i++) {
		f+= w*noise2f(c);
		c*=2.0;
		w*=0.5;
	}
	return f;
}

void main(){
	texCoord = gl_MultiTexCoord0.xy;

	vec4 vert = gl_Vertex;
	float elevation = noise2f(texCoord*(-cos(time*0.01)*sin(time*0.02)*0.05)*spikesFrq)*spikesPct;
	vert.x += noise2f(texCoord*(cos(time*0.001)*-sin(time*0.005)*0.05))*40.0;
	vert.y += noise2f(texCoord*(-cos(time*0.005)*sin(time*0.001)*0.1))*30.0;
	vert.z = (elevation*100.0);

	gl_Position = gl_ModelViewProjectionMatrix * vert;

	vec3 color = mix(color1,color2,elevation);
	gl_FrontColor = vec4(vec3(color),0.2+elevation);//offSet;
}
