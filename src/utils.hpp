#ifndef UTILS_HPP
#define UTILS_HPP

#ifdef WIN32
#include <Windows.h>
#elif __ANDROID__
#include <android/asset_manager.h>
#endif
#include <cstdbool>
#include <cstdint>

#include <vector>
#include <string>

#include <vulkan/vulkan.h>

#define CHECK_AGAINST_RESULT(func, result) result = func; if (result != 0) return result;

class full_file_path
{
public:
    full_file_path (const std::string& partial_file_path);

    std::string path;
};


class mesh_data
{
public:
    mesh_data ();

    std::vector<float> positions;
    std::vector<float> uvs;

    std::vector<uint32_t> indices;

    uint32_t positions_size;
    uint32_t uvs_size;
    uint32_t indices_size;
};


class buffer_data
{
public:
    buffer_data () {}
    buffer_data (
        const VkDeviceSize& size,
        const VkSharingMode& sharing_mode,
        const VkBufferUsageFlags& usage_flags,
        const uint32_t& queue_family_index
    ) : size (size), sharing_mode (sharing_mode), usage_flags (usage_flags), queue_family_index (queue_family_index) {}

    VkDeviceSize size;
    VkSharingMode sharing_mode;
    VkBufferUsageFlags usage_flags;
    uint32_t queue_family_index;
};


class image_data
{
public:
    image_data () {}
    image_data (const std::string& partial_file_path);

    uint32_t width;
    uint32_t height;
    uint32_t bpp;

    std::vector<uint8_t> pixels;
};


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