#version 150

uniform sampler2DRect mask;
uniform sampler2DRect noi;

void main( void ) {
    vec4 color = texture( mask, gl_FragCoord.xy );
    vec4 noicol = texture( noi, gl_FragCoord.xy );

    if( color.r < 0.01 ) {
        gl_FragColor = vec4( 0, 0, 0, 0 );
    } else {
        gl_FragColor = vec4( noicol.rgb, color.a );
    }
}