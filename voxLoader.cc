#include <iostream>
#include <fstream>
#include "voxLoader.h"

VoxLoader::VoxLoader(const std::string& voxFile)
{
    std::cout << "File " << voxFile << " loaded." << std::endl;
    std::cout << "Vox size " << sizeof(vox) << std::endl;

    std::ifstream file (voxFile, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        file.seekg (0, std::ios::beg);
        file.read ((char*)vox.header, 4);
        file.read((char*)&vox.version, sizeof(int));
        file.close();

        std::cout << "The header is loaded." << std::endl;
    }

    if (vox.header[0] == 'V' && vox.header[1] == 'O' && vox.header[2] == 'X' &&
        vox.header[3] == ' ')
    {
        std::cout << "VOX file version " << vox.version << std::endl;
    }
}

VoxLoader::~VoxLoader() {}
