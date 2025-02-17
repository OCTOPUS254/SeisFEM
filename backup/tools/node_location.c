int node_location( int node_num, double edge_size, double **node_xy, double x, double y )
/******************************************************************************/
/*
  Purpose:

    node_location return node index according to the given (x,z).

*/


{

	int i, flag, node;
	
	flag = 0;
	for(i = 0; i < node_num ; i++)
	{
		if( fabs( node_xy[0][i] - x ) <= edge_size/2.0 && fabs( node_xy[1][i] - y ) <= edge_size/2.0 )
		{
			node = i;
			flag ++;
		}
	}
	
	if(flag == 0) 
	{
		node = -1;
		printf("Error: can not find node index of (%f,%f),	set node to be -1\n", x, y);
	} 
	
	return node;

}
