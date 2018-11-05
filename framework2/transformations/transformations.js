/*
 * Student names:
 * Student numbers:
 *
 */

function myTranslate(x, y, z) {
    // Translate by x, y and z.
    var mat = [
               1.0, 0.0, 0.0, 0.0,
               0.0, 1.0, 0.0, 0.0,
               0.0, 0.0, 1.0, 0.0,
               x, y, z, 1.0
              ];

   return mat;
}

function myScale(x, y, z) {
    // Scale by x, y and z.
    var mat = [
               x, 0.0, 0.0, 0.0,
               0.0, y, 0.0, 0.0,
               0.0, 0.0, z, 0.0,
               0.0, 0.0, 0.0, 1.0
              ];

   return mat;
}

function myRotate(angle, x, y, z) {
    // Rotate by angle around [x, y, z]^T.

    //
    // 1. Create the orthonormal basis
    //
    
    var w = [0, 0, 0];
    var t = [0, 0, 0];
    var u = [0, 0, 0];
    var v = [0, 0, 0];
    
	var w_length = Math.sqrt((x*x) + (y*y) + (z*z));

    // First axis of the basis
    w[0] = x/w_length;
    w[1] = y/w_length;
    w[2] = z/w_length;

    // Compute the value of t, based on w
	
	// copy w into t
    t[0] = w[0];
    t[1] = w[1];
    t[2] = w[2];

	// get the absolute values of w
    var ab0 = Math.abs(w[0]);
    var ab1 = Math.abs(w[1]);
    var ab2 = Math.abs(w[2]);

	// change the smallest absolute value to 1.0
    if(ab0 < ab1 && ab0 < ab2)
        t[0] = 1.0;
    else if(ab1 < ab2)
        t[1] = 1.0;
    else 
        t[2] = 1.0;

    // Compute u = t x w
    u[0] = (t[1]*w[2]) - (t[2]*w[1]);
    u[1] = (t[2]*w[0]) - (t[0]*w[2]);
    u[2] = (t[0]*w[1]) - (t[1]*w[0]);

    // Normalize u
    // Second base of the basis with a 90 degrees angle on w
    var u_length = Math.sqrt((u[0]*u[0]) + (u[1]*u[1]) + (u[2]*u[2]));
    u[0] = u[0]/u_length;
    u[1] = u[1]/u_length;
    u[2] = u[2]/u_length;

    // Compute v = w x u

    // Third axis of the basis with a 90 degrees angle on w and u
    // Since they are both normalized, v is normalized as well
    v[0] = (w[1]*u[2]) - (w[2]*u[1]);
    v[1] = (w[2]*u[0]) - (w[0]*u[2]);
    v[2] = (w[0]*u[1]) - (w[1]*u[0]);
    
    //
    // 2. Set up the three matrices making up the rotation
    //
    var A = [
			u[0], u[1], u[2], 0.0,
			v[0], v[1], v[2], 0.0,
			w[0], w[1], w[2], 0.0,
			0.0, 0.0, 0.0, 1.0
            ]; 

    var B = [
			Math.cos(angle), Math.sin(angle), 0.0, 0.0,
			-Math.sin(angle), Math.cos(angle), 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0
            ];

    var C = [
			u[0], v[0], w[0], 0.0,
			u[1], v[1], w[1], 0.0,
			u[2], v[2], w[2], 0.0,
			0.0, 0.0, 0.0, 1.0
            ];


    var mat = m4.multiply(A, m4.multiply(B, C));
    return mat;
}
