/*
 * Student name .... Kaj Meijer                 Lotte Philippus
 * Student email ... k.d.meijer17@gmail.com     lotte.philippus@gmail.com
 * Collegekaart .... 10509534                   11291168
 * Date ............ 7-11-2018
 * Comments ........ None
 */

function myLookAt(eyeX, eyeY, eyeZ,
                  centerX, centerY, centerZ,
                  upX, upY, upZ) {

	// initialize the vectors for the orthonormal basis
	var n = [0, 0, 0];
	var u = [0, 0, 0];
	var v = [0, 0, 0];

	// get the first vector/axis of the orthonormal basis
	n[0] = centerX + eyeX;
	n[1] = centerY + eyeY;
	n[2] = centerZ + eyeZ;

	// get the length of that vector
	var n_length = Math.sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);

	// normalize the vector
	n[0] = n[0]/n_length;
	n[1] = n[1]/n_length;
	n[2] = n[2]/n_length;

	// Second axis of the orthonormal basis
	// using the given up vector and first axis
	u[0] = (upY*n[2]) - (upZ*n[1]);
	u[1] = (upZ*n[0]) - (upX*n[2]);
	u[2] = (upX*n[1]) - (upY*n[0]);

	// length
	var u_length = Math.sqrt(u[0]*u[0] + u[1]*u[1] + u[2]*u[2]);

	// normalize
	u[0] = u[0]/u_length;
	u[1] = u[1]/u_length;
	u[2] = u[2]/u_length;

	// get the last axis with the cross product between the first 2
	// it's already normalized, because the other veectors are
	v[0] = (n[1]*u[2]) - (n[2]*u[1]);
	v[1] = (n[2]*u[0]) - (n[0]*u[2]);
	v[2] = (n[0]*u[1]) - (n[1]*u[0]);

	// create the rotation matrix
    var R = [
		u[0], v[0], n[0], 0.0,
        u[1], v[1], n[1], 0.0,
        u[2], v[2], n[2], 0.0,
        0.0, 0.0, 0.0, 1.0
    ];

    // create the translation matrix
    var T = [
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        -eyeX, -eyeY, -eyeZ, 1.0
    ];

    // identity matrix
    var mat = m4.identity();

    // multiply R and T (we can change the order) and store in mat
    mat = m4.multiply(R, T);
    // m4.transpose(mat,mat);

    return mat;
}
