#version 330

in	vec2	  v_TexCoord;	// varying variable for passing texture coordinate from vertex shader

uniform int       u_Wsize;	  // filter width value
uniform int       u_Hsize;        // filter height value
uniform float	  u_HStep;        //horizontal step size
uniform float     u_VStep;        //vertical step size
uniform float     u_filter[900];  //
uniform	sampler2D u_Sampler;	  // uniform variable for the texture image


void main() {
	
	vec3 avg = vec3(0.0);
	vec2 tc  = v_TexCoord;
	int  w2  = u_Wsize / 2;
        int  h2  = u_Hsize / 2;
        for(int i=-w2; i<=w2; ++i){
            for(int j= -h2; j<=h2;++j){
                avg += u_filter[(j + w2) * u_Wsize + (i + h2)] *
                        texture2D(u_Sampler,vec2(tc.x + i*u_HStep, tc.y + j*u_VStep)).rgb;
            }
        }
        //avg = avg / (u_Wsize * u_Hsize);
	gl_FragColor = vec4(avg, 1.0);
}
