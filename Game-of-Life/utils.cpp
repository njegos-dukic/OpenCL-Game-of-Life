#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"

char* readKernelSource(const char* filename)
{
	char* kernelSource = nullptr;
	long length;
	FILE* f = fopen(filename, "r");
	if (f)
	{
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		kernelSource = (char*)calloc(length, sizeof(char));
		if (kernelSource)
			fread(kernelSource, 1, length, f);
		fclose(f);
	}
	return kernelSource;
}

void readImage(const char* filename, unsigned char*& array, int& width, int& height)
{
	FILE* fp = fopen(filename, "rb");
	if (!fscanf(fp, "P5\n%d %d\n255\n", &width, &height))
	{
		throw "Error";
	}
	unsigned char* image = new unsigned char[(size_t)width * height];
	fread(image, sizeof(unsigned char), (size_t)width * (size_t)height, fp);
	fclose(fp);
	array = image;
}

void writeImage(const char* filename, const unsigned char* array, const int width, const int height)
{
	FILE* fp = fopen(filename, "wb");
	fprintf(fp, "P5\n%d %d\n255\n", width, height);
	fwrite(array, sizeof(unsigned char), (size_t)width * (size_t)height, fp);
	fclose(fp);
}

void writeColorImage(const char* filename, const Pixel* array, const int width, const int height)
{
	FILE* fp = fopen(filename, "wb");
	fprintf(fp, "P6\n%d %d\n255\n", width, height);
	fwrite(array, sizeof(Pixel), (size_t)width * (size_t)height, fp);
	fclose(fp);
}