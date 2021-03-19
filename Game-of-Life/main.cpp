#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

#include <iostream>
#include "getIteration.h"
#include "setFromImage.h"
#include "getSegment.h"
#include "utils.h"
#include "getOscilator.h"

int dimension = 1024;
int maxDimension = 1024;


int main()
{
	printf("Unesite velicinu: ");
	scanf("%d", &dimension);
	// Set input to "Input.pgm"

	std::cout << "\nWorld size: " << dimension << " x " << dimension << std::endl << "=======================\n\n";
	
	unsigned char* buffer = new unsigned char[dimension * dimension];
	for (int i = 0; i < dimension * dimension; i++)
		buffer[i] = 255;

	setFromImage(buffer, dimension);
	// setFromImage(buffer, dimension);
	getIteration(buffer, dimension);
	// getSegment(buffer, dimension);
	getOscilator(buffer, dimension, 350);
}

