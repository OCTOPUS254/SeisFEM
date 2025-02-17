void shape_t6(double r, double s, double t[], double dtdr[], double dtds[] )

/*
*****************************************************************************

 SHAPE_T6 evaluates shape functions for a 6 node reference triangle.

  Reference Element T6:

    |
    1  3
    |  |\
    |  | \
    S  6  5
    |  |   \
    |  |    \
    0  1--4--2
    |
    +--0--R--1-->

*/

{
  	t[0] = 2.000 *     ( 1.000 - r - s ) * ( 0.500 - r - s );
  	t[1] = 2.000 * r * ( r - 0.500 );
  	t[2] = 2.000 * s * ( s - 0.500 );
  	t[3] = 4.000 * r * ( 1.000 - r - s );
 	t[4] = 4.000 * r * s;
 	t[5] = 4.000 * s * ( 1.000 - r - s );

  	dtdr[0] = - 3.000 + 4.000 * r + 4.000 * s;
	dtdr[1] = - 1.000 + 4.000 * r;
  	dtdr[2] =   0.000;
  	dtdr[3] =   4.000 - 8.000 * r - 4.000 * s;
 	dtdr[4] =                       4.000 * s;
  	dtdr[5] =                     - 4.000 * s;

 	dtds[0] = - 3.000 + 4.000 * r + 4.000 * s;
 	dtds[1] =   0.000;
 	dtds[2] = - 1.000             + 4.000 * s;
	dtds[3] =         - 4.000 * r;
	dtds[4] =           4.000 * r;
 	dtds[5] =   4.000 - 4.000 * r - 8.000 * s;

}
