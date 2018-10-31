/* Computer Graphics assignment, Triangle Rasterization
 * Filename ........ trirast.c
 * Description ..... Implements triangle rasterization
 * Created by ...... Paul Melis
 *
 * Student name ....
 * Student email ...
 * Collegekaart ....
 * Date ............
 * Comments ........
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "types.h"

/*
 * Rasterize a single triangle.
 * The triangle is specified by its corner coordinates
 * (x0,y0), (x1,y1) and (x2,y2).
 * The triangle is drawn in color (r,g,b).
 */

// Get the lower number
int minimum(int i, int j) {
    return i > j ? j : i;
}

// Get the higher number
int maximum(int i, int j) {
    return i < j ? j : i; 
}

void
draw_triangle(float x0, float y0, float x1, float y1, float x2, float y2,
    byte r, byte g, byte b)
{
    // get the maximum and minimum values of the triangle (no point can be outside this area)
    int xminimum, xmaximum, yminimum, ymaximum, x, y; 
    float alpha, beta, gamma;
    xmaximum = maximum(maximum(x0, x1), maximum(x0, x2));
    ymaximum = maximum(maximum(y0, y1), maximum(y0, y2));

    // loop through the point 
    for(y = 0; y <= ymaximum; y++) {
        for(x = 0; x <= xmaximum; x++) { 
            
            // calculate alpha, beta and gamma
            alpha = ((y1 - y2) * x + (x2 - x1) * y + x1*y2 - x2*y1)/((y1 - y2) * x0 + (x2 - x1) * y0 + x1*y2 - x2*y1);
            
            beta = ((y2 - y0)* x + (x0 - x2) * y + x2*y0 - x0*y2)/((y2 - y0)*x1 + (x0 - x2) * y1 + x2*y0 - x0*y2);  

            gamma = ((y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0)/((y0 - y1) * x2 + (x1 - x0) * y2 + x0 * y1 - x1 * y0); 

            // To get rid of the black pixels we also allow alpha, beta and gamma to be zero
            if(alpha >= 0 && beta >= 0 && gamma >= 0) {
                if(
                (alpha > 0 || ((y1 - y2) * -1 + (x2 - x1) * -1 + x1 * y2 - x2 * y1)*((y1 - y2) * x0 + (x2 - x1) * y0 + x1 * y2 - x2 * y1) > 0) &&
                (beta > 0  || ((y2 - y0) * -1 + (x0 - x2) * -1 + x2 * y0 - x0 * y2)*((y2 - y0) * x1 + (x0 - x2) * y1 + x2 * y0 - x0 * y2) > 0) &&
                (gamma > 0 || ((y0 - y1) * -1 + (x1 - x0) * -1 + x0 * y1 - x1 * y0)*((y0 - y1) * x2 + (x1 - x0) * y2 + x0 * y1 - x1 * y0) > 0)) {
                    
                    PutPixel(x, y, r, g, b);
                }
            }
        }
    }

}

void
draw_triangle_optimized(float x0, float y0, float x1, float y1, float x2, float y2,
    byte r, byte g, byte b)
{
    // get the maximum and minimum values of the triangle (no point can be outside this area)
    int xminimum, xmaximum, yminimum, ymaximum, x, y, alpha, beta, gamma;
    xminimum = minimum(minimum(x0, x1), minimum(x0, x2));
    xmaximum = maximum(maximum(x0, x1), maximum(x0, x2));
    yminimum = minimum(minimum(y0, y1), minimum(y0, y2));
    ymaximum = maximum(maximum(y0, y1), maximum(y0, y2));

    // calculate as many things outside the for-loop as possible 
    // differences
    int y1_y2_diff = y1 - y2;
    int y2_y0_diff = y2 - y0;
    int y0_y1_diff = y0 - y1;
    int x2_x1_diff = x2 - x1;
    int x0_x2_diff = x0 - x2;
    int x1_x0_diff = x1 - x0;
    
    // cross products
    int cross_1_2 = x1*y2 - x2*y1;
    int cross_0_2 = x2*y0 - x0*y2;
    int cross_0_1 = x0*y1 - x1*y0;
    
    // the lower part of the divisions
    int fa = y1_y2_diff * x0 + x2_x1_diff * y0 + cross_1_2;
    int fb = y2_y0_diff*x1 + x0_x2_diff * y1 + x2*y0 - x0*y2;
    int fg = y0_y1_diff * x2 + x1_x0_diff * y2 + cross_0_1;
    
    int f12 = ((y1 - y2) * -1 + (x2 - x1) * -1 + x1 * y2 - x2 * y1)*fa;
    int f02 = ((y2 - y0) * -1 + (x0 - x2) * -1 + x2 * y0 - x0 * y2)*fb;
    int f01 = ((y0 - y1) * -1 + (x1 - x0) * -1 + x0 * y1 - x1 * y0)*fg;
    
    // go through all points
    // @todo add continues if we know that we won't draw the next points in the line
    for(y = yminimum; y <= ymaximum; y++) {
        for(x = xminimum; x <= xmaximum; x++) {
            
            // calculate alpha
            alpha = (y1_y2_diff * x + x2_x1_diff * y + cross_1_2); 
            if(fa < 0)
                alpha *= -1;
            
            // calculate beta
            beta = (y2_y0_diff*x + x0_x2_diff * y + cross_0_2);  
            if(fb < 0)
                beta *= -1;
            
            // calculate gamma
            gamma = (y0_y1_diff * x + x1_x0_diff * y + cross_0_1); 
            if(fg < 0)
                gamma *= -1;
            
            // draw the pixel of the triangle
            if(alpha >= 0 && beta >= 0 && gamma >= 0) {
                if (
                (alpha > 0 || f12 > 0) &&
                (beta > 0  || f02 > 0) &&
                (gamma > 0 || f01 > 0)) {
                    PutPixel(x, y, r, g, b);
                }
            }
        }
    }
}
