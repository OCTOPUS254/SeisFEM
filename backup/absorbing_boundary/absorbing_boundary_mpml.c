void absorbing_boundary_mpml(int node_num, int element_num, int element_order, int *element_node, double **node_xy,
							 int mpml_nx, int mpml_ny, double h, double xmin, double xmax, double ymin, double ymax, double vp_max,
							 int use_mpml_xmin, int use_mpml_xmax, int use_mpml_ymin, int use_mpml_ymax,
							 double *mpml_dx, double *mpml_dy, double *mpml_dxx, double *mpml_dyy, double *mpml_dxx_pyx, double *mpml_dyy_pxy)
/******************************************************************************/
/*
  Purpose:

   absorbing_boundary_mpml is used to set mpml absorbing boundary condition and give mpml_dx[node_num],
   mpml_dy[node_num], mpml_dxx[node_num], mpml_dyy[node_num], mpml_dxx_pyx[node_num], mpml_dyy_pxy[node_num] 
   for assembing the matrices. 
   
   attention: the mpml_dx[node_num], mpml_dy[node_num], mpml_dxx[node_num], mpml_dyy[node_num] can aslo
   be set in the assblebing process by giving their values according their x_val and y_val location,
   which allows to set diffreent values even in the same element.  Because their values can be defined
   on the qudrature rule points instead of the element control points.
  
   Output:
     mpml_dx, mpml_dy, mpml_dxx, mpml_dyy, mpml_dxx_pyx, mpml_dyy_pxy
   
  
           +--------------------+   ---> x
           | 12 |    2     |23  |
           |--------------------|
           |    |          |    |
           |    |          |    |
           |    |          |    |
           |  1 |          |  3 |
           |    |          |    |
           |    |          |    |
           |    |          |    |
           |    |          |    |
           |--------------------|
           | 14 |    4     | 34 |
           +--------------------+
           |       
           | y
           +
*/

{

	int p, order, element;
	double x, y;
	double d0_x, d0_y;
	double pxy, pyx;
	double Rcoef;
	double xmin_mpml, xmax_mpml, ymin_mpml, ymax_mpml, mpml_x_thick, mpml_y_thick;

	pxy = 0.15;
	pyx = 0.15;
	Rcoef = 0.0001;
	mpml_x_thick = h * mpml_nx;
	mpml_y_thick = h * mpml_ny;
	xmin_mpml = xmin + mpml_x_thick;
	xmax_mpml = xmax - mpml_x_thick;
	ymin_mpml = ymin + mpml_y_thick;
	ymax_mpml = ymax - mpml_y_thick;

	d0_x = 3.0 * vp_max * log10(1.0 / Rcoef) / (2.0 * mpml_x_thick);
	d0_y = 3.0 * vp_max * log10(1.0 / Rcoef) / (2.0 * mpml_y_thick);

	for (element = 0; element < element_num * element_order; element = element + element_order)
	{

		for (order = 0; order < element_order; order++)
		{
			// p is the current node and set pml_mpml_dx, pml_mpml_dy, pml_mpml_dxx, pml_mpml_dyy according to its coordinates(x,y).
			p = element_node[order + element] - 1; // convert to 0-based index
			x = node_xy[0][p];
			y = node_xy[1][p];

			// area 1
			if ((x <= xmin_mpml) && (y >= ymin_mpml) && (y <= ymax_mpml) && (use_mpml_xmin == 1))
			{
				mpml_dx[p] = d0_x * ((xmin_mpml - x) / mpml_x_thick) * ((xmin_mpml - x) / mpml_x_thick);
				mpml_dy[p] = pyx * mpml_dx[p];
				mpml_dxx[p] = -d0_x * 2 * (xmin_mpml - x) / (mpml_x_thick * mpml_x_thick);
				mpml_dyy[p] = 0.0;
				mpml_dxx_pyx[p] = pyx * mpml_dxx[p];
				mpml_dyy_pxy[p] = pxy * mpml_dyy[p];
			}

			// area 3
			else if ((x >= xmax_mpml) && (y >= ymin_mpml) && (y <= ymax_mpml) && (use_mpml_xmax == 1))
			{
				mpml_dx[p] = d0_x * ((x - xmax_mpml) / mpml_x_thick) * ((x - xmax_mpml) / mpml_x_thick);
				mpml_dy[p] = pyx * mpml_dx[p];
				mpml_dxx[p] = d0_x * 2 * (x - xmax_mpml) / (mpml_x_thick * mpml_x_thick);
				mpml_dyy[p] = 0.0;
				mpml_dxx_pyx[p] = pyx * mpml_dxx[p];
				mpml_dyy_pxy[p] = pxy * mpml_dyy[p];
			}

			// area 2
			else if ((y <= ymin_mpml) && (x >= xmin_mpml) && (x <= xmax_mpml) && (use_mpml_ymin == 1))
			{
				mpml_dy[p] = d0_y * ((ymin_mpml - y) / mpml_y_thick) * ((ymin_mpml - y) / mpml_y_thick);
				mpml_dx[p] = pxy * mpml_dy[p];
				mpml_dxx[p] = 0.0;
				mpml_dyy[p] = -d0_y * 2 * (ymin_mpml - y) / (mpml_y_thick * mpml_y_thick);
				mpml_dxx_pyx[p] = pyx * mpml_dxx[p];
				mpml_dyy_pxy[p] = pxy * mpml_dyy[p];
			}

			// area 4
			else if ((y >= ymax_mpml) && (x >= xmin_mpml) && (x <= xmax_mpml) && (use_mpml_ymax == 1))
			{
				mpml_dy[p] = d0_y * ((y - ymax_mpml) / mpml_y_thick) * ((y - ymax_mpml) / mpml_y_thick);
				mpml_dx[p] = pxy * mpml_dy[p];
				mpml_dxx[p] = 0.0;
				mpml_dyy[p] = d0_y * 2 * (y - ymax_mpml) / (mpml_y_thick * mpml_y_thick);
				mpml_dxx_pyx[p] = pyx * mpml_dxx[p];
				mpml_dyy_pxy[p] = pxy * mpml_dyy[p];
			}

			// area 12
			else if ((x <= xmin_mpml) && (y <= ymin_mpml) && ((use_mpml_xmin == 1) || (use_mpml_ymin == 1)))
			{
				mpml_dx[p] = d0_x * ((xmin_mpml - x) / mpml_x_thick) * ((xmin_mpml - x) / mpml_x_thick) + pxy * d0_y * ((ymin_mpml - y) / mpml_y_thick) * ((ymin_mpml - y) / mpml_y_thick);
				mpml_dy[p] = d0_y * ((ymin_mpml - y) / mpml_y_thick) * ((ymin_mpml - y) / mpml_y_thick) + pyx * d0_x * ((xmin_mpml - x) / mpml_x_thick) * ((xmin_mpml - x) / mpml_x_thick);
				mpml_dxx[p] = -d0_x * 2 * (xmin_mpml - x) / (mpml_x_thick * mpml_x_thick);
				mpml_dyy[p] = -d0_y * 2 * (ymin_mpml - y) / (mpml_y_thick * mpml_y_thick);
				mpml_dxx_pyx[p] = pyx * mpml_dxx[p];
				mpml_dyy_pxy[p] = pxy * mpml_dyy[p];
			}

			// area 14
			else if ((x <= xmin_mpml) && (y >= ymax_mpml) && ((use_mpml_xmin == 1) || (use_mpml_ymax == 1)))
			{

				if (use_mpml_ymax == 1)
				{
					mpml_dx[p] = d0_x * ((xmin_mpml - x) / mpml_x_thick) * ((xmin_mpml - x) / mpml_x_thick) +
								 pxy * d0_y * ((y - ymax_mpml) / mpml_y_thick) * ((y - ymax_mpml) / mpml_y_thick);
					mpml_dy[p] = d0_y * ((y - ymax_mpml) / mpml_y_thick) * ((y - ymax_mpml) / mpml_y_thick) +
								 pyx * d0_x * ((xmin_mpml - x) / mpml_x_thick) * ((xmin_mpml - x) / mpml_x_thick);
					mpml_dyy[p] = d0_y * 2 * (y - ymax_mpml) / (mpml_y_thick * mpml_y_thick);
				}
				else
				{
					mpml_dx[p] = d0_x * ((xmin_mpml - x) / mpml_x_thick) * ((xmin_mpml - x) / mpml_x_thick);	   //  modified here, for surface wave
					mpml_dy[p] = pyx * d0_x * ((xmin_mpml - x) / mpml_x_thick) * ((xmin_mpml - x) / mpml_x_thick); //  modified here, for surface wave
					mpml_dyy[p] = 0.0;																			   //  modified here, for surface wave
				}
				mpml_dxx[p] = -d0_x * 2 * (xmin_mpml - x) / (mpml_x_thick * mpml_x_thick);
				mpml_dxx_pyx[p] = pyx * mpml_dxx[p];
				mpml_dyy_pxy[p] = pxy * mpml_dyy[p];
			}

			// area 23
			else if ((x >= xmax_mpml) && (y <= ymin_mpml) && ((use_mpml_xmax == 1) || (use_mpml_ymin == 1)))
			{
				mpml_dx[p] = d0_x * ((x - xmax_mpml) / mpml_x_thick) * ((x - xmax_mpml) / mpml_x_thick) +
							 pxy * d0_y * ((ymin_mpml - y) / mpml_y_thick) * ((ymin_mpml - y) / mpml_y_thick);
				mpml_dxx[p] = d0_x * 2 * (x - xmax_mpml) / (mpml_x_thick * mpml_x_thick);
				mpml_dy[p] = d0_y * ((ymin_mpml - y) / mpml_y_thick) * ((ymin_mpml - y) / mpml_y_thick) +
							 pyx * d0_x * ((x - xmax_mpml) / mpml_x_thick) * ((x - xmax_mpml) / mpml_x_thick);
				mpml_dyy[p] = -d0_y * 2 * (ymin_mpml - y) / (mpml_y_thick * mpml_y_thick);
				mpml_dxx_pyx[p] = pyx * mpml_dxx[p];
				mpml_dyy_pxy[p] = pxy * mpml_dyy[p];
			}

			//    area 34
			else if ((x >= xmax_mpml) && (y >= ymax_mpml) && ((use_mpml_xmax == 1) || (use_mpml_ymax == 1)))
			{

				if (use_mpml_ymax == 1)
				{
					mpml_dx[p] = d0_x * ((x - xmax_mpml) / mpml_x_thick) * ((x - xmax_mpml) / mpml_x_thick) +
								 pxy * d0_y * ((y - ymax_mpml) / mpml_y_thick) * ((y - ymax_mpml) / mpml_y_thick);
					mpml_dy[p] = d0_y * ((y - ymax_mpml) / mpml_y_thick) * ((y - ymax_mpml) / mpml_y_thick) +
								 pyx * d0_x * ((x - xmax_mpml) / mpml_x_thick) * ((x - xmax_mpml) / mpml_x_thick);
					mpml_dyy[p] = d0_y * 2 * (y - ymax_mpml) / (mpml_y_thick * mpml_y_thick);
				}
				else
				{
					mpml_dx[p] = d0_x * ((x - xmax_mpml) / mpml_x_thick) * ((x - xmax_mpml) / mpml_x_thick);	   //  modified here, for surface wave
					mpml_dy[p] = pyx * d0_x * ((x - xmax_mpml) / mpml_x_thick) * ((x - xmax_mpml) / mpml_x_thick); //  modified here, for surface wave
					mpml_dyy[p] = 0.0;																			   //  modified here, for surface wave
				}
				mpml_dxx[p] = d0_x * 2 * (x - xmax_mpml) / (mpml_x_thick * mpml_x_thick);
				mpml_dxx_pyx[p] = pyx * mpml_dxx[p];
				mpml_dyy_pxy[p] = pxy * mpml_dyy[p];
			}

			else
			{
				mpml_dx[p] = 0.0;
				mpml_dy[p] = 0.0;
				mpml_dxx[p] = 0.0;
				mpml_dyy[p] = 0.0;
				mpml_dxx_pyx[p] = 0.0;
				mpml_dyy_pxy[p] = 0.0;
			}
		}
	}
}