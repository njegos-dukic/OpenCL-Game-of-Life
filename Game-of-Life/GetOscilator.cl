struct Pixel
{
	unsigned char r, g, b;
};

bool isVertical(__global unsigned char* input, int col, int row, const unsigned int width)
{
	// 1. red
	if (input[(row - 2) * width + (col - 2)] == 0)
		return false;

	if (input[(row - 2) * width + (col - 1)] == 0) 
		return false;

	if (input[(row - 2) * width + (col)] == 0) 
		return false;

	if (input[(row - 2) * width + (col + 1)] == 0) 
		return false;

	if (input[(row - 2) * width + (col + 2)] == 0) 
		return false;

	// 2. red
	if (input[(row - 1) * width + (col - 2)] == 0) 
		return false;

	if (input[(row - 1) * width + (col - 1)] == 0) 
		return false;

	if (input[(row - 1) * width + (col)] == 255) 
		return false;

	if (input[(row - 1) * width + (col + 1)] == 0) 
		return false;

	if (input[(row - 1) * width + (col + 2)] == 0) 
		return false;

	// 3. red
	if (input[(row) * width + (col - 2)] == 0) 
		return false;

	if (input[(row) * width + (col - 1)] == 0)
		return false;

	if (input[(row) * width + (col)] == 255) 
		return false;

	if (input[(row) * width + (col + 1)] == 0)  
		return false;

	if (input[(row) * width + (col + 2)] == 0)  
		return false;

	// 4. red
	if (input[(row + 1) * width + (col - 2)] == 0) 
		return false;

	if (input[(row + 1) * width + (col - 1)] == 0) 
		return false;

	if (input[(row + 1) * width + (col)] == 255) 
		return false;

	if (input[(row + 1) * width + (col + 1)] == 0)
		return false;

	if (input[(row + 1) * width + (col + 2)] == 0)  
		return false;
	
	// 5. red
	if (input[(row + 2) * width + (col - 2)] == 0) 
		return false;

	if (input[(row + 2) * width + (col - 1)] == 0) 
		return false;

	if (input[(row + 2) * width + (col)] == 0) 
		return false;

	if (input[(row + 2) * width + (col + 1)] == 0)
		return false;

	if (input[(row + 2) * width + (col + 2)] == 0) 
		return false;
	
	return true;
}

bool isHorizontal(__global unsigned char* input, int col, int row, const unsigned int width)
{
	// 1. red
	if (input[(row - 2) * width + (col - 2)] == 0)
		return false;

	if (input[(row - 2) * width + (col - 1)] == 0) 
		return false;

	if (input[(row - 2) * width + (col)] == 0) 
		return false;

	if (input[(row - 2) * width + (col + 1)] == 0) 
		return false;

	if (input[(row - 2) * width + (col + 2)] == 0) 
		return false;

	// 2. red
	if (input[(row - 1) * width + (col - 2)] == 0) 
		return false;

	if (input[(row - 1) * width + (col - 1)] == 0) 
		return false;

	if (input[(row - 1) * width + (col)] == 0) 
		return false;

	if (input[(row - 1) * width + (col + 1)] == 0) 
		return false;

	if (input[(row - 1) * width + (col + 2)] == 0) 
		return false;

	// 3. red
	if (input[(row) * width + (col - 2)] == 0) 
		return false;

	if (input[(row) * width + (col - 1)] == 255)
		return false;

	if (input[(row) * width + (col)] == 255) 
		return false;

	if (input[(row) * width + (col + 1)] == 255)  
		return false;

	if (input[(row) * width + (col + 2)] == 0)  
		return false;

	// 4. red
	if (input[(row + 1) * width + (col - 2)] == 0) 
		return false;

	if (input[(row + 1) * width + (col - 1)] == 0) 
		return false;

	if (input[(row + 1) * width + (col)] == 0) 
		return false;

	if (input[(row + 1) * width + (col + 1)] == 0)
		return false;

	if (input[(row + 1) * width + (col + 2)] == 0)  
		return false;
	
	// 5. red
	if (input[(row + 2) * width + (col - 2)] == 0) 
		return false;

	if (input[(row + 2) * width + (col - 1)] == 0) 
		return false;

	if (input[(row + 2) * width + (col)] == 0) 
		return false;

	if (input[(row + 2) * width + (col + 1)] == 0)
		return false;

	if (input[(row + 2) * width + (col + 2)] == 0) 
		return false;
	
	return true;
}

__kernel void centersDetection(__global unsigned char* input, __global unsigned char* centers, const unsigned int width, const unsigned int height)
{
    int i = get_global_id(0);
	int j = get_global_id(1);

	int pos = j * width + i;

	centers[pos] = 255 * isVertical(input, i, j, width) + 255 * isHorizontal(input, i, j, width); 
}

__kernel void drawOscilators(__global struct Pixel* output, __global unsigned char* input, __global bool* centers, const unsigned int width, const unsigned int height)
{
	int i = get_global_id(0);
	int j = get_global_id(1);

	int pos = j * width + i;

	if (!(output[pos].r == 255 && output[pos].g == 0 && output[pos].b == 0))
		output[pos].r = output[pos].g = output[pos].b = input[pos];

	else
		output[pos].r = output[pos].g = output[pos].b = input[pos];

	bool isHorizontalIn = isHorizontal(input, i, j, width);
	bool isVerticalIn = isVertical(input, i, j, width);

	if (centers[pos])
	{
		if (isHorizontalIn)
		{
			output[j * width + i].r = 255;
			output[j * width + i].g = 0;
			output[j * width + i].b = 0;

			output[j * width + i - 1].r = 255;
			output[j * width + i - 1].g = 0;
			output[j * width + i - 1].b = 0;

			output[j * width + i + 1].r = 255;
			output[j * width + i + 1].g = 0;
			output[j * width + i + 1].b = 0;
		}

		else if (isVerticalIn)
		{
			output[j * width + i].r = 255;
			output[j * width + i].g = 0;
			output[j * width + i].b = 0;

			output[(j + 1) * width + i].r = 255;
			output[(j + 1) * width + i].g = 0;
			output[(j + 1) * width + i].b = 0;

			output[(j - 1) * width + i].r = 255;
			output[(j - 1) * width + i].g = 0;
			output[(j - 1) * width + i].b = 0;
		}
	}
}