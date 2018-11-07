/*
 * Student name .... Kaj Meijer                 Lotte Philippus
 * Student email ... k.d.meijer17@gmail.com     lotte.philippus@gmail.com
 * Collegekaart .... 10509534                   11291168
 * Date ............ 7-11-2018
 * Comments ........ None
 */

function myOrtho(left, right, bottom, top, near, far) {

    // We don't need x, y and z in this case because the middle point is in the
    // origin. This means that left = -right, top = -bottom and far = -near,
    // resulting in the identity matrix.
    var x = (left+right) == 0 ? 0 : (left-right)/2;
    var y = (top+bottom) == 0 ? 0 : (top-bottom)/2;
    var z = (far+near) == 0 ? 0 : (far-near)/2;

    var u = 2/(left-right);
    var v = 2/(top-bottom);
    var w = 2/(far-near);

    // Matrix for scaling. There needs to be a - before u and w, otherwise the
    // x and z axes are mirrored.
    var mat1 = [
        -u ,0.0, 0.0, 0.0,
        0.0, v, 0.0, 0.0,
        0.0, 0.0, -w, 0.0,
        0.0, 0.0, 0.0, 1.0
    ];

    // Matrix for translation to the origin
    var mat2 = [
        1.0 ,0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        -x, -y, -z, 1.0
    ];

    var mat = m4.multiply(mat1, mat2)

    return mat;
}
