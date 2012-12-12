#define PI (3.1415926536)
#define TWO_PI (6.2831853072)

uniform float time;
uniform float fogNear, fogFar;

varying vec3 position, normal;
varying float randomOffset;
varying vec2 texCoord;
varying float fogMult;

const float pulseWidth = 100.;
const float pulseSize = 1.;
const float pulseSpeed = 1.;
const float segments = 128.;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float smoothStep(float x) {
	return 3.*(x*x)-2.*(x*x*x);
}

void main() {
	randomOffset = rand(gl_Vertex.xy + gl_Vertex.yz);
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	texCoord = gl_TexCoord[0].xy;
	//float stretch = 0.;
	//float stretch = ((sin(time * pulseSpeed + TWO_PI * texCoord.x / pulseWidth) + 1.)) / 2.;
	//float stretch = (pulseWidth - clamp(abs(texCoord.x - mod(time * pulseSpeed, 256.)), 0., pulseWidth)) / pulseWidth;
	//vec4 offset = vec4(gl_Normal.xyz, 0.) * smoothStep(stretch) * pulseSize;
	gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex);// + offset);
	position = gl_Vertex.xyz;
	//normal = gl_Normal.xyz;

	fogMult = 1. - clamp((gl_Position.z - fogNear) / (fogFar - fogNear), 0., 1.);
}
