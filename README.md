# Game of Life

<img src = "https://lh3.googleusercontent.com/iZwB2p3rX7D7h-4QWSmKXy-_4FBRVCB8A8vrGwQduZzqfU-1ZMewoumgw3HOTiOsrN3Ax_vnOuUGRFnWuIbGoOWpAnpCoxGqvxg=s850" width = 500>

- Conway's Game of Life implemented in OpenCL with C++ Wrapper.
- See rules: [Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life).
- Input file must be set to: **Input.pgm** and located in the *.\Example\Input.pgm*.
- Required to create folder: *.\Example\Output*.
- Dimension can be set from code: ```"main.cpp" -> int dimension;```

- All functionalities are separated in following headers:
  - ```getIteration.h```
  - ```getOscilator.h```
  - ```getSegment.h```
  - ```setFromImage.h```

- Example API usage shown below:

```C++
int main()
{
	// Set input to "Input.pgm"

	std::cout << "World size: " << dimension << " x " << dimension << std::endl << "=======================\n\n";
	
	unsigned char* buffer = new unsigned char[dimension * dimension];
	for (int i = 0; i < dimension * dimension; i++)
		buffer[i] = 255;

	setFromImage(buffer, dimension);
	getIteration(buffer, dimension);
	getSegment(buffer, dimension);
	getOscilator(buffer, dimension, 350);
}
```

- Header ```Info.h``` enables preview of OpenCL Platforms and devices.
- Usage example shown below:
  
--- 
<img src = ..\assets\UseCase.PNG>

---

- In *.\Example\Output* folder, you can find the following:
  
  - All iterations from first to specified saved as **IterationX.pgm**.
  - Desired inverted segment as **Segment.pgm**.
  - **Colored OscilatorX.ppm** that represents the next iteration after the specified and has [Blinker](https://www.conwaylife.com/wiki/Blinker) colored in red. Number of iterations that will be run with oscilator is represent by third argument of ```getOscilator()```.

