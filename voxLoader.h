/**
 * Vox Loader header file
 *
 * author: Marcin Borowiec
*/
#include <string>
#include <vector>

#ifndef VOX_LOADER_H_
#define VOX_LOADER_H_

typedef unsigned char BYTE;
typedef unsigned int DWORD;

void SwapBytes(unsigned int& x);
bool CheckHeader(BYTE* header);


typedef struct Voxel Voxel;
struct Voxel
{
    DWORD x;
    DWORD y;
    DWORD z;
    DWORD colorIndex;

    Voxel() { x = 0; y = 0; z = 0; colorIndex = 0; }
};

typedef struct CHUNK CHUNK;
struct CHUNK
{
    BYTE chId[5];
    DWORD contentSize;
    DWORD sizeOfChilds;
    std::vector<CHUNK*> childs;
};

typedef struct Size Size;
struct Size : CHUNK
{
    DWORD xSize;
    DWORD ySize;
    DWORD zSize;
};

typedef struct Xyzi Xyzi;
struct Xyzi : CHUNK
{
    DWORD numVoxels;
    std::vector<Voxel*> voxels;
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
    VoxLoader(const std::string& filename);
    ~VoxLoader();

private:
    Vox vox;
};

#endif // VOX_LOADER_H_
