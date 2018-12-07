/* Computer Graphics, Assignment, Volume rendering with cubes/points/isosurface
 *
 * Student name .... Kaj Meijer                 Lotte Philippus
 * Student email ... k.d.meijer17@gmail.com     lotte.philippus@gmail.com
 * Collegekaart .... 10509534                   11291168
 * Date ............ 7-12-2018
 * Comments ........ None
 *
 * (always fill in these fields before submitting!!)
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "marching_tetrahedra.h"

/* Compute a linearly interpolated position where an isosurface cuts
   an edge between two vertices (p1 and p2), each with their own
   scalar value (v1 and v2) */

static vec3
interpolate_points(unsigned char isovalue, vec3 p1, vec3 p2, unsigned char v1, unsigned char v2)
{
	// Cast the chars to floats
	float v1_float = (float) v1;
	float v2_float = (float) v2;
	float iso = (float) isovalue;
	float ratio;

	// Calculate the ratio between the end point and the begin point, and the end
	// point and the iso value
    ratio = (iso - v2_float) / (v1_float - v2_float);

    return v3_add(v3_multiply(p1, ratio), v3_multiply(p2, 1-ratio));
}

/*	Calculate the corners of the triangle for the cases where only one point is
	higher or lower than the isovalue, then calculate the normal vector.

	Return the calculated triangle.
*/
static triangle
create_triangle(unsigned char isovalue, cell c, int v0, int v1, int v2, int v3)
{
	triangle t;

	t.p[0] = interpolate_points(isovalue, c.p[v0], c.p[v1], c.value[v0], c.value[v1]);
	t.p[1] = interpolate_points(isovalue, c.p[v0], c.p[v2], c.value[v0], c.value[v2]);
	t.p[2] = interpolate_points(isovalue, c.p[v0], c.p[v3], c.value[v0], c.value[v3]);

	// Calculate the normalized normal vector
	vec3 normal = v3_normalize(v3_crossprod(v3_subtract(t.p[0], t.p[2]), v3_subtract(t.p[1], t.p[2])));
	t.n[0] = normal;
	t.n[1] = normal;
	t.n[2] = normal;

	return t;
}

/*	Calculate the corners of the triangle for the cases where two points are
	higher or lower than the isovalue, then calculate the normal vector.

	Return the calculated triangle. 
*/
static triangle
create_triangle_square(unsigned char isovalue, cell c, int v0, int v1, int v2, int v3)
{
	triangle t;

	t.p[0] = interpolate_points(isovalue, c.p[v0], c.p[v1], c.value[v0], c.value[v1]);
    t.p[1] = interpolate_points(isovalue, c.p[v1], c.p[v2], c.value[v1], c.value[v2]);
    t.p[2] = interpolate_points(isovalue, c.p[v2], c.p[v3], c.value[v2], c.value[v3]);

	// Calculate the normalized normal vector
	vec3 normal = v3_normalize(v3_crossprod(v3_subtract(t.p[0], t.p[2]), v3_subtract(t.p[1], t.p[2])));
	t.n[0] = normal;
	t.n[1] = normal;
	t.n[2] = normal;
	return t;
}

/* Using the given iso-value generate triangles for the tetrahedron
   defined by corner vertices v0, v1, v2, v3 of cell c.

   Store the resulting triangles in the "triangles" array.

   Return the number of triangles created (either 0, 1, or 2).

   Note: the output array "triangles" should have space for at least
         2 triangles.
*/

static int
generate_tetrahedron_triangles(triangle *triangles, unsigned char isovalue, cell c, int v0, int v1, int v2, int v3)
{
	int b0 = 0, b1 = 0, b2 = 0, b3 = 0;

	// calculate the bit values by comparing the vertex data values with the iso-value
	if(c.value[v0] > isovalue) b0 = 1;
	if(c.value[v1] > isovalue) b1 = 1;
	if(c.value[v2] > isovalue) b2 = 1;
	if(c.value[v3] > isovalue) b3 = 1;

	// 1000 or 0111
    if((b0 && !b1 && !b2 && !b3) || (!b0 && b1 && b2 && b3))
    {
		*triangles = create_triangle(isovalue, c, v0, v1, v2, v3);

        return 1;
    }

    // 0100 or 1011
    else if((!b0 && b1 && !b2 && !b3) || (b0 && !b1 && b2 && b3))
    {
        *triangles = create_triangle(isovalue, c, v1, v0, v2, v3);

        return 1;
    }

    // 0010 or 1101
    else if((!b0 && !b1 && b2 && !b3) || (b0 && b1 && !b2 && b3))
    {
        *triangles = create_triangle(isovalue, c, v2, v3, v0, v1);

        return 1;
    }

	// 0001 or 1110
	else if((!b0 && !b1 && !b2 && b3) || (b0 && b1 && b2 && !b3))
    {
        *triangles = create_triangle(isovalue, c, v3, v0, v1, v2);

        return 1;
    }

    // 0011 or 1100
    else if((!b0 && !b1 && b2 && b3) || (b0 && b1 && !b2 && !b3))
    {
        *triangles = create_triangle_square(isovalue, c, v2, v0, v3, v1);
		*(triangles+1) = create_triangle_square(isovalue, c, v3, v1, v2, v0);

        return 2;
    }

    // 0101 or 1010
    else if((!b0 && b1 && !b2 && b3) || (b0 && !b1 && b2 && !b3))
    {
        *triangles = create_triangle_square(isovalue, c, v2, v3, v0, v1);
		*(triangles+1) = create_triangle_square(isovalue, c, v3, v2, v1, v0);

        return 2;
    }

    // 1001 or 0110
    else if((b0 && !b1 && !b2 && b3) || (!b0 && b1 && b2 && !b3))
    {
        *triangles = create_triangle_square(isovalue, c, v3, v1, v0, v2);
		*(triangles+1) = create_triangle_square(isovalue, c, v1, v3, v2, v0);

        return 2;
    }

    return 0;
}

/* Generate triangles for a single cell for the given iso-value. This function
   should produce at most 6 * 2 triangles (for which the "triangles" array should
   have enough space).

   Use calls to generate_tetrahedron_triangles().

   Return the number of triangles produced
*/

int
generate_cell_triangles(triangle *triangles, cell c, unsigned char isovalue)
{
	int tht = 0;

	// matches figure 9

	// T1
    tht += generate_tetrahedron_triangles(triangles+tht, isovalue, c, 0,1,3,7);

    // T2
    tht += generate_tetrahedron_triangles(triangles+tht, isovalue, c, 0,2,6,7);

    // T3
    tht += generate_tetrahedron_triangles(triangles+tht, isovalue, c, 0,1,5,7);

    // T4
    tht += generate_tetrahedron_triangles(triangles+tht, isovalue, c, 0,2,3,7);

    // T5
    tht += generate_tetrahedron_triangles(triangles+tht, isovalue, c, 0,4,5,7);

    // T6
    tht += generate_tetrahedron_triangles(triangles+tht, isovalue, c, 0,4,6,7);

    return tht;
}
