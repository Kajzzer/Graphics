/*
 * Student names:
 * Student numbers:
 *
 */

function myOrtho(left, right, bottom, top, near, far) {

    // We don't need these in this case because the middle point is in the
    // origin. This means that left = right, top = bottom and far = near.
    // var x = (left-right)/2;
    // var y = (top-bottom)/2;
    // var z = (far-near)/2;

    var u = 2/(left-right);
    var v = 2/(top-bottom);
    var w = 2/(far-near);

    var mat = [
        -u ,0.0, 0.0, 0.0,
        0.0, v, 0.0, 0.0,
        0.0, 0.0, -w, 0.0,
        0.0, 0.0, 0.0, 1.0
    ];

    // var mat3 = [
    //     1.0 ,0.0, 0.0, 0.0,
    //     0.0, 1.0, 0.0, 0.0,
    //     0.0, 0.0, 1.0, 0.0,
    //     -x, -y, -z, 1.0
    // ];

    // var mat = m4.multiply(mat2, mat3)

    return mat;
}
