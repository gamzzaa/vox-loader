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

        SIZE_CHUNK* size = new SIZE_CHUNK;  // todo mborowiec: change type
        vox.main->childs.push_back(size);
        file.read(reinterpret_cast<char *>(size->chId), 4);
        file.read(reinterpret_cast<char *>(&(size->contentSize)), 4);
        file.read(reinterpret_cast<char *>(&(size->sizeOfChilds)), 4);

        // read content
        if (size->contentSize > 0)
        {
            file.read(reinterpret_cast<char *>(&size->xSize), 4);
            file.read(reinterpret_cast<char *>(&size->ySize), 4);
            file.read(reinterpret_cast<char *>(&size->zSize), 4);
        }

        std::cout << "Child chunk " << vox.main->childs.at(0)->chId
        << ", content size " << vox.main->childs.at(0)->contentSize << "B"
        << ", size of childs " << vox.main->childs.at(0)->sizeOfChilds << "B"
        << ", x " << (static_cast<SIZE_CHUNK*>(vox.main->childs.at(0)))->xSize
        << ", y " << (static_cast<SIZE_CHUNK*>(vox.main->childs.at(0)))->ySize
        << ", z " << (static_cast<SIZE_CHUNK*>(vox.main->childs.at(0)))->zSize
        << std::endl;

        XYZI_CHUNK* xyzi = new XYZI_CHUNK;
        vox.main->childs.push_back(xyzi);
        file.read(reinterpret_cast<char *>(xyzi->chId), 4);
        file.read(reinterpret_cast<char *>(&(xyzi->contentSize)), 4);
        file.read(reinterpret_cast<char *>(&(xyzi->sizeOfChilds)), 4);

        if (xyzi->contentSize > 0)
        {
            file.read(reinterpret_cast<char *>(&xyzi->numVoxels), 4);

            for (unsigned int i = 0; i < xyzi->numVoxels; i++)
            {
                VOXEL* voxel = new VOXEL;
                file.read(reinterpret_cast<char *>(&voxel->x), 1);
                file.read(reinterpret_cast<char *>(&voxel->y), 1);
                file.read(reinterpret_cast<char *>(&voxel->z), 1);
                file.read(reinterpret_cast<char *>(&voxel->colorIndex), 1);

                xyzi->voxels.push_back(voxel);

                if (voxel->x > size->xSize)
                    std::cout << "x coord is not correct " << voxel->x << std::endl;

                if (voxel->y > size->ySize)
                    std::cout << "y coord is not correct " << voxel->y << std::endl;

                if (voxel->z > size->zSize)
                    std::cout << "z coord is not correct " << voxel->z << std::endl;
            }
        }

        std::cout << "Child chunk " << vox.main->childs.at(1)->chId
        << ", content size " << vox.main->childs.at(1)->contentSize << "B"
        << ", size of childs " << vox.main->childs.at(1)->sizeOfChilds << "B"
        << ", voxels " << (static_cast<XYZI_CHUNK*>(vox.main->childs.at(1)))->numVoxels
        << ": "
        << std::endl;

        XYZI_CHUNK* xyzi_temp = static_cast<XYZI_CHUNK*>(vox.main->childs.at(1));
        for (VOXEL* vox : xyzi_temp->voxels)
        {
            std::cout
            << "x = " << vox->x
            << ", y = " << vox->y
            << ", z = " << vox->z
            << ", colorIndex = " << vox->colorIndex
            << std::endl;
        }

/*
        << ", x " << (static_cast<XYZI_CHUNK*>(vox.main->childs.at(1)))->x
        << ", y " << (static_cast<XYZI_CHUNK*>(vox.main->childs.at(1)))->y
        << ", z " << (static_cast<XYZI_CHUNK*>(vox.main->childs.at(1)))->z
        << ", colorIndex " << (static_cast<XYZI_CHUNK*>(vox.main->childs.at(1)))->colorIndex
        << std::endl;
        */
    }

    file.close();
}

VoxLoader::~VoxLoader() {}
