#version 450
uniform sampler2D srcTex;
in vec2 texCoord;
out vec4 color;

vec3 LinearToSRGB(in vec3 color)
{
    vec3 ret;
    if (any(greaterThan(color, vec3(0.0031308)))) ret = 1.055 * pow(color, vec3(1.0 / 2.4)) - vec3(0.055);
    if (color.r <= 0.0031308) { ret.r = color.r < 0.0 ? color.r : color.r * 12.92; }
    if (color.g <= 0.0031308) { ret.g = color.g < 0.0 ? color.g : color.g * 12.92; }
    if (color.b <= 0.0031308) { ret.b = color.b < 0.0 ? color.b : color.b * 12.92; }
    return ret;
}

void main()
{
	vec4 col = texture(srcTex, texCoord);
	color = vec4(LinearToSRGB(col.rgb), col.a);
}