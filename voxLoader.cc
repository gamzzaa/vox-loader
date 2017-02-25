#include <iostream>
#include <fstream>
#include "voxLoader.h"

VoxLoader::VoxLoader(const std::string& voxFile)
{
    std::cout << "Loading " << voxFile << " file." << std::endl;

    std::ifstream file(voxFile, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        file.seekg(0, std::ios::beg);
        file.read((char*)vox.header, 4);
        file.read((char*)&vox.version, sizeof(int));

        std::cout << "The header is loaded." << std::endl;
    }

    if (vox.header[0] == 'V' && vox.header[1] == 'O' && vox.header[2] == 'X' &&
        vox.header[3] == ' ')
    {
        std::cout << "VOX file version " << vox.version << std::endl;
    }
    else
    {
        file.close();
        // todo mborowiec: throw exception here
    }

    // everything is good so far, read the rest of file
    if (file.is_open())
    {
        vox.main = new CHUNK;
        file.read((char*)vox.main->chId, 4);
        file.read((char*)&vox.main->contentSize, 4);
        file.read((char*)&vox.main->sizeOfChilds, 4);

        std::cout << "Main chunk " << vox.main->chId
                  << ", content size = " << vox.main->contentSize << "B"
                  << ", childs size = " << vox.main->sizeOfChilds << "B"
                  << std::endl;
    }

    file.close();
}

VoxLoader::~VoxLoader() {}
