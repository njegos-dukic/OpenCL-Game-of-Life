__kernel void getNextIteration(__global unsigned char* world, __global unsigned char* next, const int width, const int height)
{
	int col = get_global_id(0);
	int row = get_global_id(1);

	if (col == 0 || col == width - 1 || row == 0 || row == height - 1)
	{
		return;
	}

    int pos = (row) * width + (col);

	int liveNeighbours = 0;
	int isAlive = !world[pos]; 

    liveNeighbours += !(world[(row + 1) * width + (col - 1)]);
	liveNeighbours += !(world[(row + 1) * width + (col)]);
	liveNeighbours += !(world[(row + 1) * width + (col + 1)]);

	liveNeighbours += !(world[(row) * width + (col - 1)]);
	liveNeighbours += !(world[(row) * width + (col + 1)]);

	liveNeighbours += !(world[(row - 1) * width + (col - 1)]);
	liveNeighbours += !(world[(row - 1) * width + (col)]);
	liveNeighbours += !(world[(row - 1) * width + (col + 1)]);
    
    int willLive = (isAlive && liveNeighbours == 2) || liveNeighbours == 3;

	if (willLive)
	{
		next[pos] = 0;
	}
	else
	{
		next[pos] = 255;
	}
}

__kernel void setNextIteration(__global unsigned char* world, __global unsigned char* next, const int width, const int height)
{
	int col = get_global_id(0);
	int row = get_global_id(1);

    int pos = (row) * width + (col);
	world[pos] = next[pos];
}
