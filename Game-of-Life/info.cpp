#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

#include <vector>
#include <CL/cl.hpp>
#include <iostream>

void getInfo()
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	// A platform is an OpenCL implementation. 
	// NVIDIA, AMD and Intel implementations would correspond to different platforms.
	std::cout << "Number of available OpenCL platforms: " << platforms.size() << std::endl;

	for (const cl::Platform& platform : platforms)
	{
		std::cout << "============================" << std::endl;

		std::vector<cl::Device> devices;
		platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

		const auto platformName = platform.getInfo<CL_PLATFORM_NAME>();
		const auto platformVendor = platform.getInfo<CL_PLATFORM_VENDOR>();
		const auto platformClVersion = platform.getInfo<CL_PLATFORM_VERSION>();

		std::cout << "Platform: " << platformName << std::endl;
		std::cout << "Vendor: " << platformVendor << std::endl;
		std::cout << "OpenCL Version: " << platformClVersion << std::endl;
		std::cout << "---------------------------" << std::endl;

		for (const cl::Device& device : devices)
		{
			const auto deviceType = device.getInfo<CL_DEVICE_TYPE>();
			const auto deviceVendor = device.getInfo<CL_DEVICE_VENDOR>();
			const auto deviceName = device.getInfo<CL_DEVICE_NAME>();
			const auto deviceClVersion = device.getInfo<CL_DEVICE_VERSION>();
			const auto deviceMaxWorkGroupSize = device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
			const auto deviceMemory = device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();

			std::cout << "Device type: " << deviceType << std::endl;
			std::cout << "Vendor: " << deviceVendor << std::endl;
			std::cout << "Device name: " << deviceName << std::endl;
			std::cout << "Device OpenCL version: " << deviceClVersion << std::endl;
			std::cout << "Device memory: " << (deviceMemory / 1024 / 1024) << "MB" << std::endl;
			std::cout << "Device max work-group size: " << deviceMaxWorkGroupSize << std::endl;
		}

		std::cout << "============================" << std::endl;
	}

	return;
}