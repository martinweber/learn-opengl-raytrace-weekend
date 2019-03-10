#version 450

uniform float frameTime;
uniform int renderWidth;
uniform int renderHeight;
writeonly uniform image2D destTex;

layout (local_size_x = 16, local_size_y = 16) in;

//highp float rand(vec2 co)
//{
//    highp float a = 12.9898;
//    highp float b = 78.233;
//    highp float c = 43758.5453;
//    highp float dt= dot(co.xy ,vec2(a,b));
//    highp float sn= mod(dt,3.14);
//    return fract(sin(sn) * c);
//}
//

vec4 dir_to_color(in vec3 direction)
{
	vec3 unit_dir = normalize(direction);
	float t = 0.5 * (unit_dir.y + 1.0);
	return mix(vec4(1.0), vec4(0.5, 0.7, 1.0, 1.0), t); // color blend
}

void main()
{
	ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);

//	float localCoef = length(vec2(ivec2(gl_LocalInvocationID.xy)-8)/8.0);
//	float globalCoef = sin(float(gl_WorkGroupID.x + gl_WorkGroupID.y) * 0.1 + frameTime) * 0.5;
//	imageStore(destTex, storePos, vec4(1.0 - globalCoef*localCoef, 0.0, 0.0, 0.0));
//	// imageStore(destTex, storePos, vec4(rand(vec2(globalCoef, localCoef)), 0.0, 0.0, 0.0));
//
	const vec3 lowerLeft = vec3(-2.0, -1.0, -1.0);
	const vec3 horizontal = vec3(4.0, 0.0, 0.0);
	const vec3 vertical = vec3(0.0, 2.0, 0.0);
	const vec3 origin = vec3(0.0, 0.0, 0.0);

	const float u = float(storePos.x) / float(renderWidth);
	const float v = float(storePos.y) / float(renderHeight);

	vec3 direction = lowerLeft + u*horizontal + v*vertical;
	imageStore(destTex, storePos, dir_to_color(direction));
}
