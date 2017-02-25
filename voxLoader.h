/**
 * Vox Loader header file
 *
 * author: Marcin Borowiec
*/
#include <string>

#ifndef VOX_LOADER_H_
#define VOX_LOADER_H_

class VoxLoader
{
public:
    VoxLoader(const std::string& voxFile);
    ~VoxLoader();
};

#endif // VOX_LOADER_H_
