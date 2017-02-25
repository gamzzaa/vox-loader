/**
 * Vox Loader header file
 *
 * author: Marcin Borowiec
*/
#include <string>

#ifndef VOX_LOADER_H_
#define VOX_LOADER_H_

typedef unsigned char BYTE;
typedef unsigned int DWORD;

typedef struct
{
    BYTE chId[4];

} Chunk;

typedef struct
{
    BYTE header[4];
    DWORD version;
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
