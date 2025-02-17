
void stiffness_sparse_t3(int node_num, int element_num, int element_order, int *element_node, double **node_xy, int *stiffi, int *stiffj, double *stiffness, int stif_type)

/* stiffness_sparse_t3 computes the stiffness matrix, store the matrix in the coo format (i,j,value), using 3-node triangles, 3 ponits quadrature rule.

  Reference Element T3:

    |
    1  3
    |  |\
    |  | \
    S  |  \
    |  |   \
    |  |    \
    0  1-----2
    |
    +--0--R--1-->

   ELEMENT_NODE = 1, 2, 3
 
 List:
 
 I      Stiffness_TYPE
 -  --------------------------------------
 0  dphi_dx * dphi_dx + dphi_dy * dphi_dy
 1  dphi_dx * dphi_dx
 2  dphi_dy * dphi_dy
 3  dphi_dx * dphi_dy
 4  dphi_dy * dphi_dx
 5  phi     * dphi_dx
 6  phi     * dphi_dy
 
 */

{

    int quad_num = 12;
    int i, j, iq, jq, ip, jp, element, quad, coo_index;
    int p1, p2, p3;
    double r, s;
    double rtab[12], stab[12], weight[12];
    double w[3], dwdr[3], dwds[3];
    double phi[3], dphidx[3], dphidy[3];
    double x1, x2, x3, y1, y2, y3;
    double area, det;
    double drdx, drdy, dsdx, dsdy;
    double a, b, c, d, e, f, g, uu, vv, ww;

    a = 0.87382197101699600;
    b = 0.06308901449150200;
    c = 0.50142650965817900;
    d = 0.24928674517091000;
    e = 0.63650249912139900;
    f = 0.31035245103378500;
    g = 0.05314504984481600;
    uu = 0.05084490637020700;
    vv = 0.11678627572637900;
    ww = 0.08285107561837400;

    rtab[0] = a;
    rtab[1] = b;
    rtab[2] = b;
    rtab[3] = c;
    rtab[4] = d;
    rtab[5] = d;
    rtab[6] = e;
    rtab[7] = e;
    rtab[8] = f;
    rtab[9] = f;
    rtab[10] = g;
    rtab[11] = g;

    stab[0] = b;
    stab[1] = a;
    stab[2] = b;
    stab[3] = d;
    stab[4] = c;
    stab[5] = d;
    stab[6] = f;
    stab[7] = g;
    stab[8] = e;
    stab[9] = g;
    stab[10] = e;
    stab[11] = f;

    weight[0] = uu;
    weight[1] = uu;
    weight[2] = uu;
    weight[3] = vv;
    weight[4] = vv;
    weight[5] = vv;
    weight[6] = ww;
    weight[7] = ww;
    weight[8] = ww;
    weight[9] = ww;
    weight[10] = ww;
    weight[11] = ww;

    coo_index = 0; // for coo format sparse matrix index
    for (i = 0; i < element_num * element_order * element_order; i++)
    {
        stiffi[i] = 0;
        stiffj[i] = 0;
        stiffness[i] = 0.0;
    }

    for (element = 0; element < element_num * element_order; element = element + element_order)
    {
        p1 = element_node[0 + element] - 1;
        p2 = element_node[1 + element] - 1;
        p3 = element_node[2 + element] - 1;
        x1 = node_xy[0][p1];
        y1 = node_xy[1][p1];
        x2 = node_xy[0][p2];
        y2 = node_xy[1][p2];
        x3 = node_xy[0][p3];
        y3 = node_xy[1][p3];

        area = 0.5 * fabs(x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));

        if (area == 0.0)
        {
            printf("STIFFNESS_SPARSE_T3 - Fatal error!\n");
            printf("Zero area for element: %d\n", element);
            exit(1);
        }

        det = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
        drdx = (y3 - y1) / det;
        drdy = (x1 - x3) / det;
        dsdx = (y1 - y2) / det;
        dsdy = (x2 - x1) / det;

        // For each quadrature point in the element...

        for (quad = 0; quad < quad_num; quad++)
        {
            r = rtab[quad];
            s = stab[quad];
            shape_t3(r, s, w, dwdr, dwds);

            phi[0] = w[0];
            phi[1] = w[1];
            phi[2] = w[2];

            dphidx[0] = dwdr[0] * drdx + dwds[0] * dsdx;
            dphidx[1] = dwdr[1] * drdx + dwds[1] * dsdx;
            dphidx[2] = dwdr[2] * drdx + dwds[2] * dsdx;

            dphidy[0] = dwdr[0] * drdy + dwds[0] * dsdy;
            dphidy[1] = dwdr[1] * drdy + dwds[1] * dsdy;
            dphidy[2] = dwdr[2] * drdy + dwds[2] * dsdy;

            for (iq = 0; iq < element_order; iq++)
            {
                ip = element_node[iq + element] - 1; // c array from 0

                for (jq = 0; jq < element_order; jq++)
                {
                    jp = element_node[jq + element] - 1; // c array from 0

                    coo_index = element * element_order + iq * element_order + jq;
                    stiffi[coo_index] = ip;
                    stiffj[coo_index] = jp;
                    if( stif_type == 0 )
                        stiffness[coo_index] = stiffness[coo_index] + area * weight[quad] * (dphidx[iq] * dphidx[jq] + dphidy[iq] * dphidy[jq]);
                    else if(stif_type == 1)
                        stiffness[coo_index] = stiffness[coo_index] + area * weight[quad] * (dphidx[iq] * dphidx[jq]                          );
                    else if(stif_type == 2)
                        stiffness[coo_index] = stiffness[coo_index] + area * weight[quad] * (                        + dphidy[iq] * dphidy[jq]);
                    else if(stif_type == 3)
                        stiffness[coo_index] = stiffness[coo_index] + area * weight[quad] * (dphidx[iq] * dphidy[jq]                          );
                    else if(stif_type == 4)
                        stiffness[coo_index] = stiffness[coo_index] + area * weight[quad] * (                        + dphidy[iq] * dphidx[jq]  );
                    else if(stif_type == 5)
                        stiffness[coo_index] = stiffness[coo_index] + area * weight[quad] * (   phi[iq] * dphidx[jq]                          );
                    else if(stif_type == 6)
                        stiffness[coo_index] = stiffness[coo_index] + area * weight[quad] * (                        + phi[iq] * dphidy[jq]   );
                    else
                    {
                        stiffness[coo_index] = 0.0;
                        fprintf(stderr, "\n");
                        fprintf(stderr, "STIFFNESS_SPARSE_Q4 - Fatal error!\n");
                        fprintf(stderr, "  Illegal value of stif type = \"%s\".\n", stif_type);
                        exit(1);
                    }
                    
                }
            }
        }
    }
}
