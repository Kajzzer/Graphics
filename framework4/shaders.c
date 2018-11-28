/* Computer Graphics and Game Technology, Assignment Ray-tracing
 *
 * Student name .... Kaj Meijer                 Lotte Philippus
 * Student email ... k.d.meijer17@gmail.com     lotte.philippus@gmail.com
 * Collegekaart .... 10509534                   11291168
 * Date ............ 30-11-2018
 * Comments ........ None
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "shaders.h"
#include "perlin.h"
#include "v3math.h"
#include "intersection.h"
#include "scene.h"
#include "quat.h"
#include "constants.h"

// shade_constant()
//
// Always return the same color. This shader does no real computations
// based on normal, light position, etc. As such, it merely creates
// a "silhouette" of an object.

vec3
shade_constant(intersection_point ip)
{
    return v3_create(1, 0, 0);
}

vec3
shade_matte(intersection_point ip)
{
	vec3 l, offset;
	float light_angle, ray_angle,L, small;
	L = 0;
	small = 0.00003;
	offset = v3_multiply(ip.n, small);

	for (int i = 0; i < scene_num_lights;i++)
	{
		// get the vector to the light source
		l = v3_normalize(v3_subtract(scene_lights[i].position,ip.p));

		// get the light and ray angle of the ip
		light_angle = v3_dotprod(l, ip.n);

		// add the intensity of the current light source
		if(!shadow_check(v3_add(ip.p,offset), l)) L += scene_lights[i].intensity*fmax(0, light_angle);
	}
    return v3_create(L, L, L);
}

vec3
shade_blinn_phong(intersection_point ip)
{

    vec3 l, offset, cd, cs, h;
	float light_angle, ray_angle, small, L, K, kd, ks, a;

	L = 0;
	K = 0;
	small = 0.00003;
	kd = 0.8;
	ks = 0.5;
	a = 50;

	cd = v3_create(1,0,0);
	cs = v3_create(1,1,1);

	offset = v3_multiply(ip.n, small);

	for (int i = 0; i < scene_num_lights;i++)
	{
		// get the vector to the light source
		l = v3_normalize(v3_subtract(scene_lights[i].position,ip.p));

		// get the light and ray angle of the ip
		light_angle = v3_dotprod(l, ip.n);
		ray_angle = v3_dotprod(ip.i, ip.n);

		h = v3_normalize(v3_add(l, ip.i));

		// add the intensity of the current light source
		if(!shadow_check(v3_add(ip.p,offset), l))
		{
			L += scene_lights[i].intensity*fmax(0, light_angle);
			K += scene_lights[i].intensity*powf(v3_dotprod(ip.n,h),a);
		}
	}
	return v3_add(v3_multiply(cd,(scene_ambient_light+kd*L)), v3_multiply(cs,ks*K));
}

vec3
shade_reflection(intersection_point ip)
{
    vec3 l, offset, cd, cs, h, r, reflection, col, matte;
	float small = 0.00003;

	// get the standard matte shader color
	matte = shade_matte(ip);

	// get the offset
	offset = v3_multiply(ip.n, small);

	// calculate the r vector
	r = v3_normalize(v3_subtract(v3_multiply(ip.n, 2*v3_dotprod(ip.i, ip.n)), ip.i));

	// get the reflection color
	reflection =  ray_color(ip.ray_level + 1, v3_add(ip.p, offset), r);

	// return 75% matte shader color plus 25% reflection color
	return v3_add(v3_multiply(matte, 0.75), v3_multiply(reflection, 0.25));
}

// Returns the shaded color for the given point to shade.
// Calls the relevant shading function based on the material index.
vec3
shade(intersection_point ip)
{
  switch (ip.material)
  {
    case 0:
      return shade_constant(ip);
    case 1:
      return shade_matte(ip);
    case 2:
      return shade_blinn_phong(ip);
    case 3:
      return shade_reflection(ip);
    default:
      return shade_constant(ip);

  }
}

// Determine the surface color for the first object intersected by
// the given ray, or return the scene background color when no
// intersection is found
vec3
ray_color(int level, vec3 ray_origin, vec3 ray_direction)
{
    intersection_point  ip;

    // If this ray has been reflected too many times, simply
    // return the background color.
    if (level >= 3)
        return scene_background_color;

    // Check if the ray intersects anything in the scene
    if (find_first_intersection(&ip, ray_origin, ray_direction))
    {
        // Shade the found intersection point
        ip.ray_level = level;
        return shade(ip);
    }

    // Nothing was hit, return background color
    return scene_background_color;
}
