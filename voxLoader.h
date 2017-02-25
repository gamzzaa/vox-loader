/**
 * Vox Loader header file
 *
 * author: Marcin Borowiec
*/
#include <string>

#ifndef VOX_LOADER_H_
#define VOX_LOADER_H_

typedef struct
{
    char header[4];
    int version;
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
