/* Computer Graphics assignment, Triangle Rasterization
 * Filename ........ trirast.c
 * Description ..... Implements triangle rasterization
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
    int xmaximum, ymaximum, x, y; 
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
    int xminimum, xmaximum, yminimum, ymaximum, x, y, ay, by, gy, ax, bx, gx, begin_of_triangle, end_of_triangle;
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
    int cross_2_0 = x2*y0 - x0*y2;
    int cross_0_1 = x0*y1 - x1*y0;
    
    // the lower part of the divisions
    int fa = y1_y2_diff * x0 + x2_x1_diff * y0 + cross_1_2;
    int fb = y2_y0_diff * x1 + x0_x2_diff * y1 + cross_2_0;
    int fg = y0_y1_diff * x2 + x1_x0_diff * y2 + cross_0_1;
    
    // the off-screen point checks 
    int f12 = (y1_y2_diff * -1 + x2_x1_diff * -1 + cross_1_2)*fa;
    int f02 = (y2_y0_diff * -1 + x0_x2_diff * -1 + cross_2_0)*fb;
    int f01 = (y0_y1_diff * -1 + x1_x0_diff * -1 + cross_0_1)*fg;
    
    //  initial alpha for the loops
    ay = (y1_y2_diff * (xminimum-1) + x2_x1_diff * (yminimum-1) + cross_1_2); 
    
    // initial beta for the loops
    by = (y2_y0_diff * (xminimum-1) + x0_x2_diff * (yminimum-1) + cross_2_0);  
        
    // initial gamma for the loops    
    gy = (y0_y1_diff * (xminimum-1) + x1_x0_diff * (yminimum-1) + cross_0_1); 

    // revert alpha
    if (fa < 0){
        ay *=-1;
        x2_x1_diff *=-1;
        y1_y2_diff *=-1;
    }

    // revert beta
    if (fb < 0) {
        by*=-1;
        x0_x2_diff *=-1;
        y2_y0_diff *=-1;
    }
    
    // revert gamma
    if (fg < 0) {
        gy*=-1;
        x1_x0_diff *=-1;
        y0_y1_diff *=-1;
    }


    // go through all points
    for(y = yminimum; y <= ymaximum; y++) {
        
        // increment y
        ay += x2_x1_diff;
        by += x0_x2_diff;
        gy += x1_x0_diff;
        
        // reset the x-incremented values for the current y
        ax = ay;
        bx = by;
        gx = gy;
        
        // reset the bools
        begin_of_triangle = 0;
        end_of_triangle = 0;
        
        for(x = xminimum; x <= xmaximum; x++) {
            
            // skip if you have had the beginning and end of the triangle
            if (begin_of_triangle && end_of_triangle) break;
            
            // increment x
            ax += y1_y2_diff; 
            bx += y2_y0_diff;  
            gx += y0_y1_diff; 
            
            // draw the pixel of the triangle
            if(ax >= 0 && bx >= 0 && gx >= 0) {
                
                // off-screen point check
                if (
                (ax > 0 || f12 > 0) &&
                (bx > 0  || f02 > 0) &&
                (gx > 0 || f01 > 0)) {
                    PutPixel(x, y, r, g, b);
                    
                    // we've reached the beginning of the triangle
                    if(!begin_of_triangle) begin_of_triangle = 1;
                }
            }else {
                
                // we have reached the end of the triangle
                if(begin_of_triangle) end_of_triangle = 1;
            }
        }
    }
}

