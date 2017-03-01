#include <iostream>
#include <fstream>
#include "voxLoader.h"

VoxLoader::VoxLoader(const std::string& filename)
{
    std::cout << "Loading " << filename << " file." << std::endl;

    std::ifstream file(filename, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char *>(vox.header), 4);
        file.read(reinterpret_cast<char *>(&vox.version), sizeof(int));

        std::cout << "The header is loaded." << std::endl;
    }

    CheckHeader(vox.header);
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
        // read main chunk
        vox.main = new CHUNK;
        file.read(reinterpret_cast<char *>(vox.main->chId), 4);
        file.read(reinterpret_cast<char *>(&vox.main->contentSize), 4);
        file.read(reinterpret_cast<char *>(&vox.main->sizeOfChilds), 4);

        std::cout << "Main chunk " << vox.main->chId
                  << ", content size = " << vox.main->contentSize << "B"
                  << ", childs size = " << vox.main->sizeOfChilds << "B"
                  << std::endl;

        // skip content

        // start reading child chunks
        // MAIN
        //  |
        //  |- PACK
        //  |
        //  |- SIZE
        //  |- XYZI
        //  |- ...
        //  |

        Size* size = new Size;  // todo mborowiec: change type
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
        << ", x " << (static_cast<Size*>(vox.main->childs.at(0)))->xSize
        << ", y " << (static_cast<Size*>(vox.main->childs.at(0)))->ySize
        << ", z " << (static_cast<Size*>(vox.main->childs.at(0)))->zSize
        << std::endl;

        Xyzi* xyzi = new Xyzi;
        vox.main->childs.push_back(xyzi);
        file.read(reinterpret_cast<char *>(xyzi->chId), 4);
        file.read(reinterpret_cast<char *>(&(xyzi->contentSize)), 4);
        file.read(reinterpret_cast<char *>(&(xyzi->sizeOfChilds)), 4);

        if (xyzi->contentSize > 0)
        {
            file.read(reinterpret_cast<char *>(&xyzi->numVoxels), 4);

            for (unsigned int i = 0; i < xyzi->numVoxels; i++)
            {
                Voxel* voxel = new Voxel;
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
        << ", voxels " << (static_cast<Xyzi*>(vox.main->childs.at(1)))->numVoxels
        << ": "
        << std::endl;
/*
        Xyzi* xyzi_temp = static_cast<Xyzi*>(vox.main->childs.at(1));
        for (VOXEL* vox : xyzi_temp->voxels)
        {
            std::cout
            << "x = " << vox->x
            << ", y = " << vox->y
            << ", z = " << vox->z
            << ", colorIndex = " << vox->colorIndex
            << std::endl;
        }
*/
        RGBA_CHUNK* rgba = new RGBA_CHUNK;  // todo mborowiec: change type
        vox.main->childs.push_back(rgba);
        file.read(reinterpret_cast<char *>(rgba->chId), 4);
        file.read(reinterpret_cast<char *>(&(rgba->contentSize)), 4);
        file.read(reinterpret_cast<char *>(&(rgba->sizeOfChilds)), 4);

        unsigned int color = 0;
        for (unsigned int i = 0; i < rgba->contentSize / 4; i++)
        {
            file.read((char*)&color, 4);
            SwapBytes(color);
            //std::cout << i + 1 << " " << std::hex << color << std::dec << std::endl;
        }

        std::cout << "Child chunk " << vox.main->childs.at(2)->chId
        << ", content size " << vox.main->childs.at(2)->contentSize << "B"
        << ", size of childs " << vox.main->childs.at(2)->sizeOfChilds << "B"
        << std::endl;

        if (file.eof())
            std::cout << "EOF" << std::endl;

        CHUNK* chnk = new CHUNK;  // todo mborowiec: change type
        //vox.main->childs.push_back(rgba);
        file.read(reinterpret_cast<char *>(chnk->chId), 4);

        std::cout << "Child chunk " << chnk->chId << std::endl;

        if (file.eof())
            std::cout << "EOF" << std::endl;
    }

    file.close();
}

VoxLoader::~VoxLoader() {}

void SwapBytes(unsigned int& x)
{
    uint16_t *a = reinterpret_cast<uint16_t*>(&x);
    *(a+0) ^= *(a+1) ^= *(a+0) ^= *(a+1);
    uint8_t *b = reinterpret_cast<uint8_t*>(a);
    *(b+0) ^= *(b+1) ^= *(b+0) ^= *(b+1);
    b = reinterpret_cast<uint8_t*>(a+1);
    *(b+0) ^= *(b+1) ^= *(b+0) ^= *(b+1);
}

bool CheckHeader(BYTE* header)
{
    if (header[0] == 'V' && header[1] == 'O' && header[2] == 'X' &&
        header[3] == ' ')
    {
        std::cout << "Vox file..." << std::endl;
        return true;
    }

    return false;
}
