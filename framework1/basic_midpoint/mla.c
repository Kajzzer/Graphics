/* Computer Graphics, Assignment 1, Bresenham's Midpoint Line-Algorithm
 *
 * Filename ........ mla.c
 * Description ..... Midpoint Line Algorithm
 * Created by ...... Jurgen Sturm
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

#include "SDL.h"
#include "init.h"

/*
 * Midpoint Line Algorithm
 *
 * As you probably will have figured out, this is the part where you prove
 * your programming skills. The code in the mla function should draw a direct
 * line between (x0,y0) and (x1,y1) in the specified color.
 *
 * Until now, the example code below draws only a horizontal line between
 * (x0,y0) and (x1,y0) and a vertical line between (x1,y1).
 *
 * And here the challenge begins..
 *
 * Good luck!
 *
 *
 */
void mla(SDL_Texture *t, int x0, int y0, int x1, int y1, Uint32 colour) {
  
    // everything will be drawn from left to right, so we want to have the start point
    // to the left side of the end point
    if(x0 > x1) 
    {
        mla(t, x1, y1, x0, y0, colour);
        return;
    }

    // initialize the variables
    int x, y, dx, dy, d, slope;

    // draw the begin and end points
    PutPixel(t,x0,y0,colour);
    PutPixel(t,x1,y1,colour);

    // get the current coordinates
    y = y0;
    x = x0;
    
    // set the normalized vector values that are used for the calculation
    // of d during the creation of the line       
    dx = x1 - x0; 
    dy = y0 - y1;
    
    // d is 0 because the beginning of the line should be on the line
    d = 0;
    
    // set the standard slope to 1 
    slope = 1;

    // if delta y is bigger than 0, the startpoint is higher than the endpoint, so the direction
    // in which y is drawn needs to be reversed
    if(dy > 0) {
        slope = -1;
        dy = -dy;
    } 

    // determine the behaviour of the line by comparing delta x and delta y
    // we want a custom behaviour for the more vertical lines
    if((abs(dx)) >= abs(dy)){

        // go through the points from x0 to x1
        for(x = x0; x != x1; x+=1) {
            PutPixel(t, x, y, colour);
            
            // if d is smaller than 0, we decide to add the slope to the y coordinate
            if(d < 0){
                // not just +1 because we use -1 for lines that go underneath the middle point 
                y += slope;
                d += dx + dy;
            } else {
                d += dy; 
            }  
        }
        
    // do the same we did for the more horizontal lines, but for the y values
    } else {
        
        // go through the points from y0 to y1
        for(y = y0; y != y1; y+=slope) {
            PutPixel(t, x, y, colour);
            if(d < 0){
                // always one, because we only draw from left to right
                x += 1;
                d += -(dx + dy);
            } else {
                d -= dx; 
            }  
        }
    }
    
    // return to the function :D
    return;
}
