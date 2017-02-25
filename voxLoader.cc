#include <iostream>
#include "voxLoader.h"

VoxLoader::VoxLoader(const std::string& voxFile)
{
    std::cout << "File " << voxFile << " loaded!" << std::endl;
}

VoxLoader::~VoxLoader() {}
