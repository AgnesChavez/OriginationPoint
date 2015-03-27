//const int octaves = 5;  

#version 120

const vec3 rDot = vec3(12.9898, 78.233, 42.43);  
const float rScale = 43758.5453;  

uniform int octaves;  
uniform float seed;  
uniform float[8] weights;  
uniform float[8] scaling;  
uniform float normalization;  
uniform float zPos;  


float pi = (asin(1.) * 2.0);  
vec3 fractPos,floorPos;  
vec4 bot,top,zMix;  
vec2 yMix;  

float rand(vec3 coord) {  
    return fract(sin(dot(coord.xyz, rDot)) * rScale);  
}  

float bilinearRand(vec3 pos) {  
	fractPos = (1. - cos(fract(pos)*pi))*0.5;  
	floorPos = floor(pos);  

	bot = vec4(  
        rand(floorPos),  
        rand(floorPos + vec3(1,0,0)),  
        rand(floorPos + vec3(0,1,0)),  
        rand(floorPos + vec3(1,1,0)));  

    top = vec4(  
        rand(floorPos + vec3(0,0,1)),  
        rand(floorPos + vec3(1,0,1)),  
        rand(floorPos + vec3(0,1,1)),  
        rand(floorPos + vec3(1,1,1)));  

    zMix = mix(bot,top, fractPos.z);  
    yMix = mix(zMix.xy, zMix.zw, fractPos.y);  
    return mix(yMix.x,yMix.y, fractPos.x);  
}  

float noise(vec3 pos) {  
	float total = 0.;  
	for(int i = 0; i < octaves; i++)  
		total += weights[i] * bilinearRand((pos + seed * float(i + 1)) * scaling[i]);  
	return total * normalization;  
}  

void main(void){  
	gl_FragColor = vec4(vec3(noise(vec3(gl_TexCoord[0].xy,zPos))), 1.);  
}  