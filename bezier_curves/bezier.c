/* Computer Graphics, Assignment, Bezier curves
 * Filename ........ bezier.c
 * Description ..... Bezier curves
 * Date ............ 22.07.2009
 * Created by ...... Paul Melis
 *
 * Student name .... Kaj Meijer                 Lotte Philippus
 * Student email ... k.d.meijer17@gmail.com     lotte.philippus@gmail.com
 * Collegekaart .... 10509534                   11291168
 * Date ............ 2-11-2018
 * Comments ........ None
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include <math.h>
#include "bezier.h"
#include <stdio.h>

/* Given a Bezier curve defined by the 'num_points' control points
 * in 'p' compute the position of the point on the curve for parameter
 * value 'u'.
 *
 * Return the x and y values of the point by setting *x and *y,
 * respectively.
 */

// calculates the factorial value of n
long factorial(int n)
{
	int i;
	long fac = 1;

	for(i=1;i<=n; i++)
	{
		fac *= i;
	}

	return fac;
}

// calculate the binomial distribution
float binomial_distribution(int n, int k) 
{
	return factorial(n)/(factorial(k)*factorial(n-k));
}

void
evaluate_bezier_curve(float *x, float *y, control_point p[], int num_points, float u)
{
	// reset the values of x and y
    *x = 0.0;
    *y = 0.0;
    
    // get the total number of points (-1 for the index)
    int n = num_points-1;

	float temp;
	
	// calculate the x and y values of 
	for(int i = 0; i <= n; i++)
	{
		// store the calculation we need for both points
		temp = binomial_distribution(n,i)*pow(u,i)*pow(1-u,n-i);
		
		// calculate the x and y value compared to the controle points
		*x += temp * (p[i].x);
		*y += temp * (p[i].y);
	}
}

/* Draw a Bezier curve defined by the control points in p[], which
 * will contain 'num_points' points.
 *
 *
 * The 'num_segments' parameter determines the "discretization" of the Bezier
 * curve and is the number of straight line segments that should be used
 * to approximate the curve.
 *
 * Call evaluate_bezier_curve() to compute the necessary points on
 * the curve.
 *
 * You will have to create a GLfloat array of the right size, filled with the
 * vertices in the appropriate format and bind this to the buffer.
 */

void
draw_bezier_curve(int num_segments, control_point p[], int num_points)
{
	// the buffer
    GLuint buffer[1];

	int max_points = num_segments+1;

	// the points of the curve
	// we store them as x0, y0, x1, y1, ...,xn, yn
	GLfloat points[2*max_points];
	
	// the size of the steps
	float du = 1.0 / num_segments;
    
    // the x and y value of a point
    float x, y;
    
    // the index of a point
    int i = 0;
    
    /* Write your own code to create and fill the array here. */
	for (float u = 0.0; u <= 1.0; u += du) {
		
		// calculate the curve and store the x and y values
        evaluate_bezier_curve(&x, &y, p, num_points, u);
        points[i] = x;
        points[i+1] = y;
        
        // increment the index
        i += 2;
    }

    // This creates the VBO and binds an array to it.
    glGenBuffers(1, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, *buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points),
                 points, GL_STATIC_DRAW);

    // This tells OpenGL to draw what is in the buffer as a Line Strip.
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glDrawArrays(GL_LINE_STRIP, 0, max_points);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, buffer);
}

/* Find the intersection of a cubic Bezier curve with the line X=x.
   Return 1 if an intersection was found and place the corresponding y
   value in *y.
   Return 0 if no intersection exists.
*/

int
intersect_cubic_bezier_curve(float *y, control_point p[], float x)
{
	float dx, dy, u, du;

	// a small value for the step size
	du = 0.001;
	
	// check if the x value is between the start and end point
	if (x < p[0].x && x > p[3].x) {
        return 0;
    }

	// evaluate for every step
	for(u = 0.0; u < 1.0; u+= du)
	{
		// calculate the curve for the current step
		evaluate_bezier_curve(&dx, &dy, p, 4, u);
		
		// change y if the difference is smaller than the step size
		if(fabs(dx-x) < du)
		{
			*y = dy;
			return 1;
		}
	}
	
    return 0;
}
