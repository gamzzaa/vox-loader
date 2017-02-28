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
    // content structure
    DWORD x;
    DWORD y;
    DWORD z;
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
