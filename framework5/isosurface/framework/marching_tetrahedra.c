/* Computer Graphics, Assignment, Volume rendering with cubes/points/isosurface
 *
 * Student name ....
 * Student email ...
 * Collegekaart ....
 * Date ............
 * Comments ........
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
    /* Initially, simply return the midpoint between p1 and p2.
       So no real interpolation is done yet */
	
	// unsigned char v = v1+v2;
	
	return v3_add(v3_multiply(p1, 0.5), v3_multiply(p2, 0.5));
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
	if(volume[(int)c.value[v0]] > isovalue) b0 = 1;
	if(volume[(int)c.value[v1]] > isovalue) b1 = 1;
	if(volume[(int)c.value[v2]] > isovalue) b2 = 1;
	if(volume[(int)c.value[v3]] > isovalue) b3 = 1;
	
	// 0001 or 1110
    if((!b0 && !b1 && !b2 && b3) || (b0 && b1 && b2 && !b3))
    {
        triangles[0].p[0] = interpolate_points(isovalue, c.p[v3], c.p[v0], c.value[v3], c.value[v0]);
        triangles[0].p[1] = interpolate_points(isovalue, c.p[v3], c.p[v1], c.value[v3], c.value[v1]);
        triangles[0].p[2] = interpolate_points(isovalue, c.p[v3], c.p[v2], c.value[v3], c.value[v2]);
        
        return 1;
    }
    
    // 0010 or 1101
    else if((!b0 && !b1 && b2 && !b3) || (b0 && b1 && !b2 && b3))
    {
        triangles[0].p[0] = interpolate_points(isovalue, c.p[v2], c.p[v0], c.value[v2], c.value[v0]);
        triangles[0].p[1] = interpolate_points(isovalue, c.p[v2], c.p[v1], c.value[v2], c.value[v1]);
        triangles[0].p[2] = interpolate_points(isovalue, c.p[v2], c.p[v3], c.value[v2], c.value[v3]);
       
        return 1;
    }
    
    // 0100 or 1011
    else if((!b0 && b1 && !b2 && !b3) || (b0 && !b1 && b2 && b3))
    {
        triangles[0].p[0] = interpolate_points(isovalue, c.p[v1], c.p[v0], c.value[v1], c.value[v0]);
        triangles[0].p[1] = interpolate_points(isovalue, c.p[v1], c.p[v2], c.value[v1], c.value[v2]);
        triangles[0].p[2] = interpolate_points(isovalue, c.p[v1], c.p[v3], c.value[v1], c.value[v3]);
        
        return 1;
    }
    
	// 1000 or 0111
	else if((b0 && !b1 && !b2 && !b3) || (!b0 && b1 && b2 && b3))
    {
        triangles[0].p[0] = interpolate_points(isovalue, c.p[v0], c.p[v1], c.value[v0], c.value[v1]);
        triangles[0].p[1] = interpolate_points(isovalue, c.p[v0], c.p[v2], c.value[v0], c.value[v2]);
        triangles[0].p[2] = interpolate_points(isovalue, c.p[v0], c.p[v3], c.value[v0], c.value[v3]);
        
        return 1;
    }
    
    // 1100 or 0011
    else if((b0 && b1 && !b2 && !b3) || (!b0 && !b1 && b2 && b3))
    {
        triangles[0].p[0] = interpolate_points(isovalue, c.p[v0], c.p[v1], c.value[v0], c.value[v1]);
        triangles[0].p[1] = interpolate_points(isovalue, c.p[v0], c.p[v2], c.value[v0], c.value[v2]);
        triangles[0].p[2] = interpolate_points(isovalue, c.p[v0], c.p[v3], c.value[v0], c.value[v3]);
       
        triangles[1].p[0] = interpolate_points(isovalue, c.p[v1], c.p[v0], c.value[v1], c.value[v0]);
        triangles[1].p[1] = interpolate_points(isovalue, c.p[v1], c.p[v2], c.value[v1], c.value[v2]);
        triangles[1].p[2] = interpolate_points(isovalue, c.p[v1], c.p[v3], c.value[v1], c.value[v3]);
       
        return 2;
    }
    
    // 1010 or 0101
    else if((b0 && !b1 && b2 && !b3) || (!b0 && b1 && !b2 && b3))
    {
        triangles[0].p[0] = interpolate_points(isovalue, c.p[v0], c.p[v1], c.value[v0], c.value[v1]);
        triangles[0].p[1] = interpolate_points(isovalue, c.p[v0], c.p[v2], c.value[v0], c.value[v2]);
        triangles[0].p[2] = interpolate_points(isovalue, c.p[v0], c.p[v3], c.value[v0], c.value[v3]);
        
        triangles[1].p[0] = interpolate_points(isovalue, c.p[v2], c.p[v0], c.value[v2], c.value[v0]);
        triangles[1].p[1] = interpolate_points(isovalue, c.p[v2], c.p[v1], c.value[v2], c.value[v1]);
        triangles[1].p[2] = interpolate_points(isovalue, c.p[v2], c.p[v3], c.value[v2], c.value[v3]);
        
        return 2;
    }
    
    // 0110 or 1001
    else if((!b0 && b1 && b2 && !b3) || (b0 && !b1 && !b2 && b3))
    {
		
        triangles[0].p[0] = interpolate_points(isovalue, c.p[v1], c.p[v0], c.value[v1], c.value[v0]);
        triangles[0].p[1] = interpolate_points(isovalue, c.p[v1], c.p[v2], c.value[v1], c.value[v2]);
        triangles[0].p[2] = interpolate_points(isovalue, c.p[v1], c.p[v3], c.value[v1], c.value[v3]);
        
        triangles[1].p[0] = interpolate_points(isovalue, c.p[v2], c.p[v0], c.value[v2], c.value[v0]);
        triangles[1].p[1] = interpolate_points(isovalue, c.p[v2], c.p[v1], c.value[v2], c.value[v1]);
        triangles[1].p[2] = interpolate_points(isovalue, c.p[v2], c.p[v3], c.value[v2], c.value[v3]);
        
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

/*
int
generate_cell_triangles(triangle *triangles, cell c, unsigned char isovalue)
{
	int tht = 0;
	
	// T1
    tht += generate_tetrahedron_triangles(&triangles[tht], isovalue, c, 0,1,3,2);
    
    // T2
    tht += generate_tetrahedron_triangles(&triangles[tht], isovalue, c, 0,1,5,4);
    
    // T3
    tht += generate_tetrahedron_triangles(&triangles[tht], isovalue, c, 2,0,4,6);
    
    // T4
    tht += generate_tetrahedron_triangles(&triangles[tht], isovalue, c, 4,6,7,5);
    
    // T5
    tht += generate_tetrahedron_triangles(&triangles[tht], isovalue, c, 3,2,6,7);
    
    // T6
    tht += generate_tetrahedron_triangles(&triangles[tht], isovalue, c, 1,3,7,5);
    
    return tht;
}*/


int
generate_cell_triangles(triangle *triangles, cell c, unsigned char isovalue)
{
	int tht = 0;
	
	// matches figure 9
	
	// T1
    tht += generate_tetrahedron_triangles(&triangles[tht], isovalue, c, 0,1,3,7);
    
    // T2
    tht += generate_tetrahedron_triangles(&triangles[tht], isovalue, c, 0,2,6,7);
    
    // T3
    tht += generate_tetrahedron_triangles(&triangles[tht], isovalue, c, 0,1,5,7);
    
    // T4
    tht += generate_tetrahedron_triangles(&triangles[tht], isovalue, c, 0,2,3,7);
    
    // T5
    tht += generate_tetrahedron_triangles(&triangles[tht], isovalue, c, 0,4,5,7);
    
    // T6
    tht += generate_tetrahedron_triangles(&triangles[tht], isovalue, c, 0,4,6,7);
    
    return tht;
}
