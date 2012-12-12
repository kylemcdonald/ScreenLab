#define PI (3.1415926536)
#define TWO_PI (6.2831853072)

uniform float time;
uniform int segmentSubdivision;
uniform float randomDissolve;

uniform float baseBlend;
uniform float dissolveBlend;
uniform float pulseBlend;

varying float fogMult;
varying vec3 position;
//varying vec3 normal;
varying float randomOffset;
varying vec2 texCoord;

const vec4 on = vec4(vec3(1.), .75);
const vec4 off = vec4(vec3(0.), .75);
const vec4 red = vec4(.905, .192, .137, 1.);
const vec4 green = vec4(.106, .694, .208, 1.);
const vec4 yellow = vec4(.905, .682, .137, 1.);
const vec4 blue = vec4(.160, .231, .620, 1.);

const float dissolveSpeed = 100.;
const float dissolveScale = 1000.;

void main() {
	if(fogMult == 0.) {
		discard;
	}

	gl_FragColor =
		baseBlend *
		on;

	float localTime = randomDissolve * randomOffset + time;
	gl_FragColor += 
		dissolveBlend *
		((mod(position.x + (localTime * dissolveSpeed), dissolveScale) > dissolveScale / 2.) ?
		on : off);
	gl_FragColor += 
		pulseBlend *
		on * mod(-time + (texCoord.x) / 10., 2.);
	
	if(mod(texCoord.x, float(segmentSubdivision)) == 0.) {
		gl_FragColor = yellow;
	}

	gl_FragColor *= fogMult;
}
