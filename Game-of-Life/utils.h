#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

struct Pixel
{
	unsigned char r, g, b;
};

char* readKernelSource(const char* filename);
void readImage(const char* filename, unsigned char*& array, int& width, int& height);
void writeImage(const char* filename, const unsigned char* array, const int width, const int height);
void writeColorImage(const char* filename, const Pixel* array, const int width, const int height);