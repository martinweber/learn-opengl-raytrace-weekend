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

vec4 color(in Ray ray)
{
	const float minRayLength = 0.0;
    const float maxRayLength = 10000.0;
	HitRecord hit;

	const bool hasHit = world_hit(ray, minRayLength, maxRayLength, hit);
	if (hasHit)
	{
		return 0.5 * vec4(hit.normal.x + 1.0, hit.normal.y + 1.0, hit.normal.z + 1.0, 2.0);
	}

	const vec3 unit_dir = normalize(ray.direction);
	const float t = 0.5 * (unit_dir.y + 1.0);
	return mix(vec4(1.0), vec4(0.5, 0.7, 1.0, 1.0), t); // color blend
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
