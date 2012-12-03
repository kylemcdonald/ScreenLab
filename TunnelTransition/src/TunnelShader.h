#pragma once

string tunnel_shader_vs = "\
#define PI (3.1415926536)\n\
#define TWO_PI (6.2831853072)\n\
\n\
uniform float time;\n\
uniform float fogNear, fogFar;\n\
\n\
varying vec3 position, normal;\n\
varying float randomOffset;\n\
varying vec2 texCoord;\n\
varying float fogMult;\n\
\n\
const float pulseWidth = 100.;\n\
const float pulseSize = 1.;\n\
const float pulseSpeed = 1.;\n\
const float segments = 128.;\n\
\n\
float rand(vec2 co){\n\
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);\n\
}\n\
\n\
float smoothStep(float x) {\n\
	return 3.*(x*x)-2.*(x*x*x);\n\
}\n\
\n\
void main() {\n\
	randomOffset = rand(gl_Vertex.xy + gl_Vertex.yz);\n\
	gl_FrontColor = gl_Color;\n\
	gl_TexCoord[0] = gl_MultiTexCoord0;\n\
	texCoord = gl_TexCoord[0].xy;\n\
	gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex);// + offset);\n\
	position = gl_Vertex.xyz;\n\
	normal = gl_Normal.xyz;\n\
	fogMult = 1. - clamp((gl_Position.z - fogNear) / (fogFar - fogNear), 0., 1.);\n\
}\n\
";

string tunnel_shader_fs = "\
#define PI (3.1415926536)\n\
#define TWO_PI (6.2831853072)\n\
\n\
uniform float time;\n\
uniform int segmentSubdivision;\n\
uniform float randomDissolve;\n\
\n\
uniform float baseBlend;\n\
uniform float dissolveBlend;\n\
uniform float pulseBlend;\n\
\n\
varying float fogMult;\n\
varying vec3 position, normal;\n\
varying float randomOffset;\n\
varying vec2 texCoord;\n\
\n\
const vec4 on = vec4(vec3(1.), .75);\n\
const vec4 off = vec4(vec3(0.), .75);\n\
const vec4 red = vec4(.905, .192, .137, 1.);\n\
const vec4 green = vec4(.106, .694, .208, 1.);\n\
const vec4 yellow = vec4(.905, .682, .137, 1.);\n\
const vec4 blue = vec4(.160, .231, .620, 1.);\n\
\n\
const float dissolveSpeed = 100.;\n\
const float dissolveScale = 1000.;\n\
\n\
void main() {\n\
	if(fogMult == 0.) {\n\
		discard;\n\
	}\n\
\n\
	gl_FragColor =\n\
		baseBlend *\n\
		on;\n\
\n\
	float localTime = randomDissolve * randomOffset + time;\n\
	gl_FragColor += \n\
		dissolveBlend *\n\
		((mod(position.x + (localTime * dissolveSpeed), dissolveScale) > dissolveScale / 2.) ?\n\
		on : off);\n\
	gl_FragColor += \n\
		pulseBlend *\n\
		on * mod(-time + (texCoord.x) / 10., 2.);\n\
	\n\
	if(mod(texCoord.x, float(segmentSubdivision)) == 0.) {\n\
		gl_FragColor = yellow;\n\
	}\n\
\n\
	gl_FragColor *= fogMult;\n\
}\n\
";