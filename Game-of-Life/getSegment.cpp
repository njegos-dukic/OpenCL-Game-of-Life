#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

#include <CL/cl.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "utils.h"

void getSegment(unsigned char*& world, int dimension)
{
	int x, y, sWidth, sHeight;
	std::cout << "\nGrab segment from: (x, y)\n   x = ";
	std::cin >> x;
	std::cout << "   y = ";
	std::cin >> y;
	std::cout << "\nEnter segment width: ";
	std::cin >> sWidth;
	std::cout << "Enter segment height: ";
	std::cin >> sHeight;

	unsigned char* segment = new unsigned char[sWidth * sHeight];
	for (int i = 0; i < sWidth * sHeight; i++)
		segment[i] = 255;

	int segmentSize = sWidth * sHeight;
	int worldSize = dimension * dimension;

	cl_mem d_world;
	cl_mem d_segment;

	cl_platform_id cpPlatform;       
	cl_device_id device_id;          
	cl_context context;              
	cl_command_queue queue;          
	cl_program program;              
	cl_kernel kernel;                

	size_t globalSize[2], localSize[2];
	cl_int err;

	localSize[0] = localSize[1] = 8;

	globalSize[0] = sWidth;
	globalSize[1] = sHeight;

	err = clGetPlatformIDs(1, &cpPlatform, NULL);
	err = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
	queue = clCreateCommandQueue(context, device_id, 0, &err);

	char* kernelSource = readKernelSource("GetSegment.cl");
	program = clCreateProgramWithSource(context, 1, (const char**)&kernelSource, NULL, &err);
	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

	if (err)
	{
		size_t log_size;
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
		char* log = (char*)malloc(log_size);
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
		printf("%s\n", log);
		free(log);
	}

	kernel = clCreateKernel(program, "getSegment", &err);
	size_t kernelWorkGroupSize = 0;
	clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &kernelWorkGroupSize, nullptr);

	d_world = clCreateBuffer(context, CL_MEM_READ_WRITE, worldSize, NULL, NULL);
	d_segment = clCreateBuffer(context, CL_MEM_READ_WRITE, segmentSize, NULL, NULL);

	err = clEnqueueWriteBuffer(queue, d_world, CL_TRUE, 0, worldSize, world, 0, NULL, NULL);
	err = clEnqueueWriteBuffer(queue, d_segment, CL_TRUE, 0, segmentSize, segment, 0, NULL, NULL);

	clFinish(queue);

	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_world);
	err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_segment);
	err |= clSetKernelArg(kernel, 2, sizeof(int), &dimension);
	err |= clSetKernelArg(kernel, 3, sizeof(int), &sWidth);
	err |= clSetKernelArg(kernel, 4, sizeof(int), &sHeight);
	err |= clSetKernelArg(kernel, 5, sizeof(int), &x);
	err |= clSetKernelArg(kernel, 6, sizeof(int), &y);

	clFinish(queue);

	err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalSize, localSize, 0, NULL, NULL);

	clFinish(queue);

	clEnqueueReadBuffer(queue, d_segment, CL_TRUE, 0, segmentSize, segment, 0, NULL, NULL);

	clFinish(queue);

	const std::string outFile = std::string("Example\\Output\\Segment.pgm");
	writeImage(outFile.c_str(), segment, sWidth, sHeight);

	clReleaseMemObject(d_world);
	clReleaseMemObject(d_segment);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	free(kernelSource);

	return;
}