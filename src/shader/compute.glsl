#version 450

uniform float frameTime;
uniform int renderWidth;
uniform int renderHeight;
writeonly uniform image2D destTex;

layout (local_size_x = 16, local_size_y = 16) in;

struct Ray 
{
	vec3 origin;
	vec3 direction;
};

struct Sphere
{
	vec3 center;
	float radius;
};

struct HitRecord
{
	float t;
	vec3 pos;
	vec3 normal;
};

const int sphereCount = 2;

Sphere allSpheres[sphereCount] = {
	Sphere(vec3(0, 0, -1), 0.5),
	Sphere(vec3(0, -100.5, -1), 100)
};

vec3 point_at_parameter(in Ray ray, in float t)
{
	return ray.origin + t * ray.direction;
}

bool sphere_hit(in Sphere sphere, in Ray ray, in float t_min, in float t_max, out HitRecord hit)
{
	const vec3 oc = ray.origin - sphere.center;
	const float a = dot(ray.direction, ray.direction);
	const float b = dot(oc, ray.direction);
	const float c = dot(oc, oc) - sphere.radius*sphere.radius;
	
	const float discriminant = b*b - a*c;
	if (discriminant > 0.0)
	{
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min)
		{
			hit.t = temp;
			hit.pos = point_at_parameter(ray, hit.t);
			hit.normal = (hit.pos - sphere.center) / sphere.radius;
			return true;
		}

		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min)
		{
			hit.t = temp;
			hit.pos = point_at_parameter(ray, hit.t);
			hit.normal = (hit.pos - sphere.center) / sphere.radius;
			return true;
		}
	}

	return false;
}

highp float rand(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt= dot(co.xy ,vec2(a,b));
    highp float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}

vec3 random_in_unit_sphere(in vec3 pos)
{
	vec3 p = vec3(0);
	do
	{
		p = 2.0 * vec3(rand(pos.xy + p.xy), rand(pos.yz + p.yz), rand(pos.xz + p.xz)) - vec3(1,1,1);
	} 
	while (length(p)*length(p) >= 1.0);
	return p;
}

bool world_hit(in Ray ray, in float t_min, in float t_max, out HitRecord hit)
{
	HitRecord temp_hit;
	bool hit_anything = false;
	float closest_so_far = t_max;

	for(int i=0; i < sphereCount; ++i)
	{
		if (sphere_hit(allSpheres[i], ray, t_min, closest_so_far, temp_hit))
		{
			hit_anything = true;
			closest_so_far = temp_hit.t;
			hit = temp_hit;
		}
	}

	return hit_anything;
}

vec3 normal_color(in vec3 normal)
{
	return vec3(normal.x + 1.0, normal.y + 1.0, normal.z + 1.0);
}

vec3 sky_color(in vec3 direction)
{
	const vec3 unit_dir = normalize(direction);
	const float t = 0.5 * (unit_dir.y + 1.0);
	return mix(vec3(1.0), vec3(0.3, 0.5, 0.9), t); // color blend
}

vec3 diffuse()
{
	return vec3( 0.3, 0.3, 0.3);
}

vec4 color(in Ray ray)
{
	const float minRayLength = 0.001;
    const float maxRayLength = 100000.0;
	HitRecord hit;

	bool hasHit = world_hit(ray, minRayLength, maxRayLength, hit);
	if (hasHit)
	{
		// secondary ray
		const int MAX_SECONDARY_RAYS = 16;
		vec3 rayColor[MAX_SECONDARY_RAYS];

		int ray_count = 0;
		do
		{
			vec3 target = hit.pos + hit.normal + random_in_unit_sphere(hit.pos);
			Ray secondaryRay = { hit.pos, target - hit.pos };

			hasHit = world_hit(secondaryRay, minRayLength, maxRayLength, hit);
			if (hasHit)	
			{
				rayColor[ray_count] = vec3(0);
				ray_count++;
			}
			else 
				rayColor[ray_count] = sky_color(secondaryRay.direction).rgb;

		}
		while( hasHit && ray_count < MAX_SECONDARY_RAYS);

		vec3 color = vec3(0);
		do
		{
			color = 0.5 * (color + rayColor[ray_count]);
			ray_count--;
		}
		while(ray_count >= 0);

		return vec4(color, 1.0);
	}

	return vec4(sky_color(ray.direction), 1.0);
}

void main()
{

	ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);

	const float aspect_ratio = float(renderWidth) / float(renderHeight);

	const vec3 lowerLeft = vec3(-aspect_ratio, -1.0, -1.0);
	const vec3 horizontal = vec3(2.0 * aspect_ratio, 0.0, 0.0);
	const vec3 vertical = vec3(0.0, aspect_ratio, 0.0);
	const vec3 origin = vec3(0.0, 0.0, 0.0);

	vec4 col = vec4(0);

	Ray ray;
	ray.origin = vec3(0.0, 0.0, 0.0);

	const int TOTAL_SAMPLES = 4;
	for(int s = 0; s < TOTAL_SAMPLES; ++s)
	{
		float rx = rand(vec2(float(storePos.x+s), float(storePos.y+s)));
		float ry = rand(vec2(float(storePos.y+s), float(storePos.x+s)));

		const float u = (float(storePos.x) + rx) / float(renderWidth);
		const float v = (float(storePos.y) + ry) / float(renderHeight);

		ray.direction = lowerLeft + u*horizontal + v*vertical;
		col += color(ray);
	}

	imageStore(destTex, storePos, col/TOTAL_SAMPLES);
}
