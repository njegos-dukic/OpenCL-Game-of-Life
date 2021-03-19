#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

#include <CL/cl.h>
#include <iostream>
#include <string>
#include "utils.h"
#include "getIteration.h"

void drawOscilators(unsigned char*& world, unsigned char* centersBuffer, int dimension, int i)
{
	unsigned int width = dimension;
	unsigned int height = dimension;

	int imageSize = width * height;
	int ppmImageSize = width * height * sizeof(Pixel);

	Pixel* ppmImage = new Pixel[imageSize];

	cl_mem input;
	cl_mem output;
	cl_mem centers;

	cl_platform_id cpPlatform;      
	cl_device_id device_id;         
	cl_context context;             
	cl_command_queue queue;         
	cl_program program;             
	cl_kernel kernel;               

	size_t globalSize[2], localSize[2];
	cl_int err;

	localSize[0] = localSize[1] = 8;

	globalSize[0] = dimension;
	globalSize[1] = dimension;

	err = clGetPlatformIDs(1, &cpPlatform, NULL);
	err = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);

	queue = clCreateCommandQueue(context, device_id, 0, &err);

	char* kernelSource = readKernelSource("GetOscilator.cl");
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

	kernel = clCreateKernel(program, "drawOscilators", &err);

	input = clCreateBuffer(context, CL_MEM_READ_WRITE, imageSize, NULL, NULL);
	output = clCreateBuffer(context, CL_MEM_READ_WRITE, ppmImageSize, NULL, NULL);
	centers = clCreateBuffer(context, CL_MEM_READ_WRITE, imageSize, NULL, NULL);

	err = clEnqueueWriteBuffer(queue, input, CL_TRUE, 0, imageSize, world, 0, NULL, NULL);
	err = clEnqueueWriteBuffer(queue, centers, CL_TRUE, 0, imageSize, centersBuffer, 0, NULL, NULL);

	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &output); // Pixel
	err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &input); // World
	err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &centers);
	err |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &dimension);
	err |= clSetKernelArg(kernel, 4, sizeof(unsigned int), &dimension);

	err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalSize, localSize, 0, NULL, NULL);

	clFinish(queue);

	clEnqueueReadBuffer(queue, output, CL_TRUE, 0, ppmImageSize, ppmImage, 0, NULL, NULL);

	clFinish(queue);

	const std::string outFile = std::string("Example\\Output\\Colored Oscilator") + std::to_string(i + 1) + std::string(".ppm");
	writeColorImage(outFile.c_str(), ppmImage, width, height);

	// writeColorImage("Example\\Output\\Colored Oscilator.ppm", ppmImage, width, height);

	clReleaseMemObject(input);
	clReleaseMemObject(output);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	free(kernelSource);
}

void getOscilator(unsigned char*& world, int dimension, int iterations)
{
	std::cout << "\nStarted Blinker detection for: " << iterations << " iterations.\n";
	unsigned int width = dimension;
	unsigned int height = dimension;

	int imageSize = width * height;
	unsigned char* centersBuffer = new unsigned char[imageSize];

	for (int i = 0; i < imageSize; i++)
		centersBuffer[i] = 0;

	cl_mem input;
	cl_mem centers;

	cl_platform_id cpPlatform;
	cl_device_id device_id;
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_kernel kernel;

	size_t globalSize[2], localSize[2];
	cl_int err;

	localSize[0] = localSize[1] = 8;

	globalSize[0] = dimension;
	globalSize[1] = dimension;

	err = clGetPlatformIDs(1, &cpPlatform, NULL);
	err = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);

	queue = clCreateCommandQueue(context, device_id, 0, &err);

	char* kernelSource = readKernelSource("GetOscilator.cl");
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

	kernel = clCreateKernel(program, "centersDetection", &err);

	input = clCreateBuffer(context, CL_MEM_READ_WRITE, imageSize, NULL, NULL);
	centers = clCreateBuffer(context, CL_MEM_READ_WRITE, imageSize, NULL, NULL);

	for (int i = 0; i < iterations; i++)
	{
	err = clEnqueueWriteBuffer(queue, input, CL_TRUE, 0, imageSize, world, 0, NULL, NULL);
	err = clEnqueueWriteBuffer(queue, centers, CL_TRUE, 0, imageSize, centersBuffer, 0, NULL, NULL);

	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
	err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &centers);
	err |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &dimension);
	err |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &dimension);

	err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalSize, localSize, 0, NULL, NULL);
	clFinish(queue);

	clEnqueueReadBuffer(queue, centers, CL_TRUE, 0, imageSize, centersBuffer, 0, NULL, NULL);
	clFinish(queue);

	getSingleIteration(world, dimension);
	drawOscilators(world, centersBuffer, dimension, i);
	}


	clReleaseMemObject(input);
	clReleaseMemObject(centers);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);

	// free(kernelSource);
	free(centersBuffer);
}

