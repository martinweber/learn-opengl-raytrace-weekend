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

vec4 normal_color(in vec3 normal)
{
	return vec4(normal.x + 1.0, normal.y + 1.0, normal.z + 1.0, 2.0);
}

vec4 sky_color(in vec3 direction)
{
	const vec3 unit_dir = normalize(direction);
	const float t = 0.5 * (unit_dir.y + 1.0);
	return mix(vec4(1.0), vec4(0.3, 0.5, 0.9, 1.0), t); // color blend
}

vec4 diffuse()
{
	return vec4( 0.3, 0.3, 0.3, 1.0);
}

vec4 color(in Ray ray)
{
	const float minRayLength = 0.001;
    const float maxRayLength = 10000.0;
	HitRecord hit;

	const bool hasHit = world_hit(ray, minRayLength, maxRayLength, hit);
	if (hasHit)
	{
		//return 0.5 * vec4(hit.normal.x + 1.0, hit.normal.y + 1.0, hit.normal.z + 1.0, 2.0);
		vec3 target = hit.pos + hit.normal + random_in_unit_sphere(hit.pos);
		// secondary ray
		Ray secondaryRay = { hit.pos, target - hit.pos };

		if( world_hit(secondaryRay, minRayLength, maxRayLength, hit) )
		{
			return 0.5 * diffuse();
		}
		else
		{
			return sky_color(secondaryRay.direction);
		}
	}

	return sky_color(ray.direction);
}

void main()
{
	ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);

	const float aspect_ratio = float(renderWidth) / float(renderHeight);

	const vec3 lowerLeft = vec3(-aspect_ratio, -1.0, -1.0);
	const vec3 horizontal = vec3(2.0 * aspect_ratio, 0.0, 0.0);
	const vec3 vertical = vec3(0.0, aspect_ratio, 0.0);
	const vec3 origin = vec3(0.0, 0.0, 0.0);

	const float u = float(storePos.x) / float(renderWidth);
	const float v = float(storePos.y) / float(renderHeight);

	Ray ray;
	ray.origin = vec3(0.0, 0.0, 0.0);
	ray.direction = lowerLeft + u*horizontal + v*vertical;
	imageStore(destTex, storePos, color(ray));
}
