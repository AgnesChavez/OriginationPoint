uniform vec2 resolution;
uniform float w1;
uniform float w2;
uniform float w3;
uniform float w4;


float hash( vec2 p )
{
	float h = dot(p,vec2(127.1,311.7));
	
    return -1.0 + 2.0*fract(sin(h)*43758.5453123);
}

float noise( in vec2 p )
{
    vec2 i = floor( p );
    vec2 f = fract( p );
	
	vec2 u = f*f*(3.0-2.0*f);

    return mix( mix( hash( i + vec2(0.0,0.0) ), 
                     hash( i + vec2(1.0,0.0) ), u.x),
                mix( hash( i + vec2(0.0,1.0) ), 
                     hash( i + vec2(1.0,1.0) ), u.x), u.y);
}

void main(void) {
    vec2 p = gl_FragCoord.xy / resolution.xy;
	vec2 uv = p*vec2(resolution.x/resolution.y,1.0);
    float f = 0.0;
    uv *= 5.0;
    mat2 m = mat2( 1.6,  1.2, -1.2,  1.6 );
	f  = w1*noise( uv ); uv = m*uv;
	f += w2*noise( uv ); uv = m*uv;
	f += w3*noise( uv ); uv = m*uv;
	f += w4*noise( uv ); uv = m*uv;

    f = 0.5 + 0.5*f;
    gl_FragColor = vec4( f, f, f, 1.0 );
}