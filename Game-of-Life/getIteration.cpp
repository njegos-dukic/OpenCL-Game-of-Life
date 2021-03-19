#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

#include <CL/cl.h>
#include <iostream>
#include <string>
#include "utils.h"

static void setIteration(unsigned char*& buffer, unsigned char*& next, int& dimension)
{
	int imageSize = dimension * dimension;

	cl_mem d_world;
	cl_mem d_next;

	cl_platform_id cpPlatform;        
	cl_device_id device_id;           
	cl_context context;               
	cl_command_queue queue;           
	cl_program program;               
	cl_kernel kernel;                 

	size_t globalSize[2], localSize[2];
	cl_int err;

	localSize[0] = localSize[1] = 8;
	globalSize[0] = dimension; // (size_t)ceil(dimension / (float)(int)localSize)* ((int)localSize); 
	globalSize[1] = dimension; // (size_t)ceil(dimension / (float)(int)localSize)* ((int)localSize); 

	err = clGetPlatformIDs(1, &cpPlatform, NULL);
	err = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);

	context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
	queue = clCreateCommandQueue(context, device_id, 0, &err);

	char* kernelSource = readKernelSource("GetIteration.cl");
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

	kernel = clCreateKernel(program, "setNextIteration", &err);

	size_t kernelWorkGroupSize = 0;
	clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &kernelWorkGroupSize, nullptr);

	d_world = clCreateBuffer(context, CL_MEM_READ_WRITE, imageSize, NULL, NULL);
	d_next = clCreateBuffer(context, CL_MEM_READ_WRITE, imageSize, NULL, NULL);

	err = clEnqueueWriteBuffer(queue, d_world, CL_TRUE, 0, imageSize, buffer, 0, NULL, NULL);
	err = clEnqueueWriteBuffer(queue, d_next, CL_TRUE, 0, imageSize, next, 0, NULL, NULL);

	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_world);
	err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_next);
	err |= clSetKernelArg(kernel, 2, sizeof(int), &dimension);
	err |= clSetKernelArg(kernel, 3, sizeof(int), &dimension);

	err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalSize, localSize, 0, NULL, NULL);
	clFinish(queue);

	clEnqueueReadBuffer(queue, d_world, CL_TRUE, 0, imageSize, buffer, 0, NULL, NULL);
	clFinish(queue);

	clReleaseMemObject(d_world);
	clReleaseMemObject(d_next);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	free(kernelSource);

	return;
}

void getIteration(unsigned char*& buffer, int& dimension)
{
	unsigned char* next = new unsigned char[dimension * dimension];
	for (int i = 0; i < dimension * dimension; i++)
		next[i] = 255;

	int iterations = 0;
	int imageSize = dimension * dimension;
	std::cout << "\nPlease set desired iteration: ";
	std::cin >> iterations;
	
	cl_mem d_world;
	cl_mem d_next;

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

	char* kernelSource = readKernelSource("GetIteration.cl");
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

	kernel = clCreateKernel(program, "getNextIteration", &err);

	size_t kernelWorkGroupSize = 0;
	clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &kernelWorkGroupSize, NULL);

	d_world = clCreateBuffer(context, CL_MEM_READ_WRITE, imageSize, NULL, NULL);
	d_next = clCreateBuffer(context, CL_MEM_READ_WRITE, imageSize, NULL, NULL);

	err = clEnqueueWriteBuffer(queue, d_next, CL_TRUE, 0, imageSize, next, 0, NULL, NULL);

	for (int i = 0; i < iterations; i++)
	{
		err = clEnqueueWriteBuffer(queue, d_world, CL_TRUE, 0, imageSize, buffer, 0, NULL, NULL);

		err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_world);
		err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_next);
		err |= clSetKernelArg(kernel, 2, sizeof(int), &dimension);
		err |= clSetKernelArg(kernel, 3, sizeof(int), &dimension);

		err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalSize, localSize, 0, NULL, NULL);
		clFinish(queue);

		clEnqueueReadBuffer(queue, d_next, CL_TRUE, 0, imageSize, next, 0, NULL, NULL);
		clFinish(queue);

		setIteration(buffer, next, dimension);

		const std::string outFile = std::string("Example\\Output\\Iteration") + std::to_string(i + 1) + std::string(".pgm");
		writeImage(outFile.c_str(), buffer, dimension, dimension);
	}


	clReleaseMemObject(d_world);
	clReleaseMemObject(d_next);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	free(kernelSource);

	return;
}

void getSingleIteration(unsigned char*& buffer, int& dimension)
{
	unsigned char* next = new unsigned char[dimension * dimension];
	for (int i = 0; i < dimension * dimension; i++)
		next[i] = 255;

	int imageSize = dimension * dimension;

	cl_mem d_world;
	cl_mem d_next;

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

	char* kernelSource = readKernelSource("GetIteration.cl");
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

	kernel = clCreateKernel(program, "getNextIteration", &err);

	size_t kernelWorkGroupSize = 0;
	clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &kernelWorkGroupSize, nullptr);

	d_world = clCreateBuffer(context, CL_MEM_READ_WRITE, imageSize, NULL, NULL);
	d_next = clCreateBuffer(context, CL_MEM_READ_WRITE, imageSize, NULL, NULL);

	err = clEnqueueWriteBuffer(queue, d_world, CL_TRUE, 0, imageSize, buffer, 0, NULL, NULL);
	err = clEnqueueWriteBuffer(queue, d_next, CL_TRUE, 0, imageSize, next, 0, NULL, NULL);

	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_world);
	err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_next);
	err |= clSetKernelArg(kernel, 2, sizeof(int), &dimension);
	err |= clSetKernelArg(kernel, 3, sizeof(int), &dimension);

	err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalSize, localSize, 0, NULL, NULL);
	clFinish(queue);

	clEnqueueReadBuffer(queue, d_next, CL_TRUE, 0, imageSize, next, 0, NULL, NULL);
	clFinish(queue);

	setIteration(buffer, next, dimension);

	// const std::string outFile = std::string("Example\\Output\\Single Iteration.pgm");
	// writeImage(outFile.c_str(), buffer, dimension, dimension);

	clReleaseMemObject(d_world);
	clReleaseMemObject(d_next);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	free(kernelSource);

	return;
}
