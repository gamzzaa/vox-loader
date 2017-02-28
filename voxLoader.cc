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
        file.read(reinterpret_cast<char *>(vox.header), 4);
        file.read(reinterpret_cast<char *>(&vox.version), sizeof(int));

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

    // everything is good so far, read the rest of the file
    if (file.is_open())
    {
        vox.main = new CHUNK;
        file.read(reinterpret_cast<char *>(vox.main->chId), 4);
        file.read(reinterpret_cast<char *>(&vox.main->contentSize), 4);
        file.read(reinterpret_cast<char *>(&vox.main->sizeOfChilds), 4);

        std::cout << "Main chunk " << vox.main->chId
                  << ", content size = " << vox.main->contentSize << "B"
                  << ", childs size = " << vox.main->sizeOfChilds << "B"
                  << std::endl;

        SIZE_CHUNK* size = new SIZE_CHUNK;
        vox.main->childs.push_back(size);
        file.read(reinterpret_cast<char *>(size->chId), 4);
        file.read(reinterpret_cast<char *>(&(size->contentSize)), 4);
        file.read(reinterpret_cast<char *>(&(size->sizeOfChilds)), 4);

        // read content
        if (size->contentSize > 0)
        {
            file.read(reinterpret_cast<char *>(&size->x), 4);
            file.read(reinterpret_cast<char *>(&size->y), 4);
            file.read(reinterpret_cast<char *>(&size->z), 4);
        }

        std::cout << "Child chunk " << vox.main->childs.at(0)->chId
        << ", content size " << vox.main->childs.at(0)->contentSize << "B"
        << ", size of childs " << vox.main->childs.at(0)->sizeOfChilds << "B"
        << ", x " << (static_cast<SIZE_CHUNK*>(vox.main->childs.at(0)))->x
        << ", y " << (static_cast<SIZE_CHUNK*>(vox.main->childs.at(0)))->y
        << ", z " << (static_cast<SIZE_CHUNK*>(vox.main->childs.at(0)))->z
        << std::endl;
    }

    file.close();
}

VoxLoader::~VoxLoader() {}
