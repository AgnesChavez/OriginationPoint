uniform sampler2DRect tex;

void main (void){
	vec4 tex0 = texture2DRect(tex, gl_TexCoord[0].st);
	vec4 outCol = vec4( 1.0 - tex0.r, 1.0 - tex0.g, 1.0 - tex0.b, 1.0 );
	gl_FragColor = vec4(outCol); 
}