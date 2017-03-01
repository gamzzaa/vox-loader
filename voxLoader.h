/**
 * Vox Loader header file
 *
 * author: Marcin Borowiec
*/
#include <string>
#include <vector>

#ifndef VOX_LOADER_H_
#define VOX_LOADER_H_

void swap_bytes(unsigned int& x);


typedef unsigned char BYTE;
typedef unsigned int DWORD;

typedef struct VOXEL VOXEL;
struct VOXEL
{
    DWORD x;
    DWORD y;
    DWORD z;
    DWORD colorIndex;

    VOXEL() { x = 0; y = 0; z = 0; colorIndex = 0; }
};

typedef struct CHUNK CHUNK;
struct CHUNK
{
    BYTE chId[5];
    DWORD contentSize;
    DWORD sizeOfChilds;
    std::vector<CHUNK*> childs;
};

typedef struct SIZE_CHUNK SIZE_CHUNK;
struct SIZE_CHUNK : CHUNK
{
    DWORD xSize;
    DWORD ySize;
    DWORD zSize;
};

typedef struct XYZI_CHUNK XYZI_CHUNK;
struct XYZI_CHUNK : CHUNK
{
    DWORD numVoxels;
    std::vector<VOXEL*> voxels;
};

typedef struct RGBA_CHUNK RGBA_CHUNK;
struct RGBA_CHUNK : CHUNK
{
    std::vector<DWORD> palette;
};

typedef struct
{
    BYTE header[4];
    DWORD version;
    CHUNK* main;
} Vox;

class VoxLoader
{
public:
    VoxLoader(const std::string& voxFile);
    ~VoxLoader();

private:
    Vox vox;
};

#endif // VOX_LOADER_H_
