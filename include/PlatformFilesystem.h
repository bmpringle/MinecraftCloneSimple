#ifndef PLATFORMFILESYSTEM_H
#define PLATFORMFILESYSTEM_H

    //normally, I never use namespace abbreviations for clarity, but this requires it.
    #ifdef __MINGW32__
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem::v1;
    #else
    #include <filesystem>
    namespace fs = std::filesystem;
    #endif

#endif