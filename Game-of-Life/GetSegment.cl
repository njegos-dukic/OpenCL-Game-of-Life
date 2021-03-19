__kernel void getSegment(
__global unsigned char* world, 
__global unsigned char* segment, 
const int worldDimension, 
const int segmentWidth, 
const int segmentHeight, 
const int topLeftX, 
const int topLeftY)
{
	int col = get_global_id(0); 
	int row = get_global_id(1);

	if (col + topLeftX >= worldDimension || row + topLeftY >= worldDimension)
		return;

	int pos = (topLeftY + row) * worldDimension + (topLeftX + col);
	segment[row * segmentWidth + col] = !world[pos] * 255;
}