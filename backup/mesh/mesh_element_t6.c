
int mesh_t6_element ( int nelemx, int nelemy, int *element_node )

/******************************************************************************/
/*
  Purpose: 

    MESH_T6_ELEMENT produces a grid of pairs of 6 node triangles.

  Example:

    Input:

      NELEMX = 3, NELEMY = 2

    Output:

      ELEMENT_NODE = 
         1,  3, 15,  2,  9,  8;
        17, 15,  3, 16,  9, 10;
         3,  5, 17,  4, 11, 10;
        19, 17,  5, 18, 11, 12;
         5,  7, 19,  6, 13, 12;
        21, 19,  7, 20, 13, 14;
        15, 17, 29, 16, 23, 22;
        31, 29, 17, 30, 23, 24;
        17, 19, 31, 18, 25, 24;
        33, 31, 19, 32, 25, 26;
        19, 21, 33, 20, 27, 26;
        35, 33, 21, 34, 27, 28.

  Grid:

   29-30-31-32-33-34-35
    |\ 8  |\10  |\12  |
    | \   | \   | \   |
   22 23 24 25 26 27 28
    |   \ |   \ |   \ |
    |  7 \|  9 \| 11 \|
   15-16-17-18-19-20-21
    |\ 2  |\ 4  |\ 6  |
    | \   | \   | \   |
    8  9 10 11 12 13 14
    |   \ |   \ |   \ |
    |  1 \|  3 \|  5 \|
    1--2--3--4--5--6--7

  Element T6:

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
  int c;
  int e;
  int element;
  int element_order = 6;
  int i;
  int j;
  int n;
  int ne;
  int nw;
  int s;
  int se;
  int sw;
  int w;

 
/*
  Node labeling:

    NW---N--NE
     | \     |
     W   C   E
     |    \  |
    SW---S--SE
*/
  element = 0;
 
  for ( j = 1; j <= nelemy; j++ )
  {
    for ( i = 1; i <= nelemx; i++ )
    {
      sw = 2 * ( j - 1 )  * ( 2 * nelemx + 1 ) + 2 * ( i - 1 ) + 1;
      w  = sw +               2 * nelemx + 1;
      nw = sw +         2 * ( 2 * nelemx + 1 );

      s  = sw + 1;
      c  = sw + 1 +           2 * nelemx + 1;
      n  = sw + 1 +     2 * ( 2 * nelemx + 1 );

      se = sw + 2;
      e  = sw + 2 +           2 * nelemx + 1;
      ne = sw + 2 +     2 * ( 2 * nelemx + 1 );

      element_node[0 + element * element_order] = sw;
      element_node[1 + element * element_order] = se;
      element_node[2 + element * element_order] = nw;
      element_node[3 + element * element_order] = s;
      element_node[4 + element * element_order] = c;
      element_node[5 + element * element_order] = w;
      element = element + 1;

      element_node[0 + element * element_order] = ne;
      element_node[1 + element * element_order] = nw;
      element_node[2 + element * element_order] = se;
      element_node[3 + element * element_order] = n;
      element_node[4 + element * element_order] = c;
      element_node[5 + element * element_order] = e;
      element = element + 1;
    }
  }

}
