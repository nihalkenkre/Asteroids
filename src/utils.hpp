#ifndef UTILS_HPP
#define UTILS_HPP

#ifdef WIN32
#include <Windows.h>
#elif __ANDROID__
#include <android/asset_manager.h>
#endif
#include <cstdbool>
#include <cstdint>

#define CHECK_AGAINST_RESULT(func, result) result = func; if (result != 0) return result;

#ifdef WIN32
 struct file_path
{
    char path[MAX_PATH];
};
#endif

void utils_get_full_file_path (const char* partial_file_path, char* out_file_path);

#ifdef WIN32
void utils_get_files_in_folder (const char* partial_folder_path, file_path** out_file_paths, uint32_t* num_out_files);
void utils_import_texture (const char* file_path, int* width, int* height, int* bpp, uint8_t** pixels);
#elif __ANDROID__
void utils_import_texture (const char* texture_name, AAssetManager* asset_manager, int* width,
        int* height, int* bpp, uint8_t** pixels);
#endif


void* utils_malloc (const uint32_t size);
void* utils_realloc (void* ptr, uint32_t new_size);

void utils_free (void* ptr);

#endif