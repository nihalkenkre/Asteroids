#include "utils.hpp"

#ifdef WIN32
#include <Shlwapi.h>
#include <strsafe.h>
#endif
#include <cstdlib>

#ifdef WIN32
#pragma comment (lib, "Shlwapi.lib")
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STB_ONLY_PNG
#define STB_ONLY_TGA

#include <stb_image.h>

#ifdef __ANDROID__
#include <android/asset_manager.h>
#include <android/log.h>
#endif

full_file_path::full_file_path (const std::string& partial_file_path)
{
	char path[MAX_PATH];

	wchar_t t_path[MAX_PATH];
	HMODULE module = GetModuleHandle (NULL);
	GetModuleFileName (module, t_path, MAX_PATH);
	PathRemoveFileSpec (t_path);

	char out_path[MAX_PATH];

	wcstombs_s (NULL, path, MAX_PATH, t_path, MAX_PATH);
	strcpy (out_path, path);
	strcat (out_path, "\\");
	strcat (out_path, partial_file_path.c_str ());

	this->path = std::string (out_path);
}


mesh_data::mesh_data ()
{
	printf ("mesh::mesh\n");

	positions = std::vector<float>{ -1.f,-1.f, 1.f,-1.f, 1.f,1.f, -1.f,1.f };
	uvs = std::vector<float>{ 1,1, 0,1, 0,0, 1,0 };

	indices = std::vector<uint32_t>{ 0,1,2, 0,2,3 };

	positions_size = positions.size () * sizeof (positions[0]);
	uvs_size = uvs.size () * sizeof (uvs[0]);

	indices_size = indices.size () * sizeof (indices[0]);
}


image_data::image_data (const std::string& partial_file_path)
{
	printf ("image::image\n");

	full_file_path ffp (partial_file_path);
	uint8_t* p = stbi_load (ffp.path.c_str (), &(int)width, &(int)height, &(int)bpp, 4);

	pixels.resize (width * height * bpp);
	std::memcpy (pixels.data (), p, pixels.size ());

	stbi_image_free (p);
}


#ifdef WIN32
void utils_get_full_texture_path_from_uri (const char* file_path, const char* uri, char* out_full_texture_path)
{
	wchar_t texture_file[MAX_PATH];
	mbstowcs (texture_file, file_path, MAX_PATH);

	PathRemoveFileSpec (texture_file);
	wchar_t t_uri[MAX_PATH];
	mbstowcs (t_uri, uri, MAX_PATH);

	wchar_t uri_path[MAX_PATH];
	StringCchCopy (uri_path, MAX_PATH, L"\\");
	StringCchCat (uri_path, MAX_PATH, t_uri);

	StringCchCat (texture_file, MAX_PATH, uri_path);
	wcstombs (out_full_texture_path, texture_file, MAX_PATH);
}

void utils_get_full_file_path (const char* partial_file_path, char* out_file_path)
{
	char path[MAX_PATH];

	wchar_t t_path[MAX_PATH];
	HMODULE module = GetModuleHandle (nullptr);
	GetModuleFileName (module, t_path, MAX_PATH);
	PathRemoveFileSpec (t_path);

	wcstombs_s (nullptr, path, MAX_PATH, t_path, MAX_PATH);
	strcpy (out_file_path, path);
	strcat (out_file_path, "\\");
	strcat (out_file_path, partial_file_path);
}

void utils_get_files_in_folder (const char* partial_folder_path, file_path** out_file_paths, uint32_t* num_out_files)
{
	char full_folder_path[MAX_PATH];
	utils_get_full_file_path (partial_folder_path, full_folder_path);
	strcat (full_folder_path, "*");

	wchar_t folder_path[MAX_PATH];
	mbstowcs (folder_path, full_folder_path, MAX_PATH);

	WIN32_FIND_DATA ffd;
	HANDLE find_handle = INVALID_HANDLE_VALUE;

	find_handle = FindFirstFile (folder_path, &ffd);
	uint32_t num_files = 0;
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			char file_name[MAX_PATH];
			wcstombs (file_name, ffd.cFileName, MAX_PATH);
			char* base_name = strtok (file_name, ".");
			char* ext = strtok (nullptr, ".");

			if (strcmp (ext, "glb") == 0 || strcmp (ext, "gltf") == 0)
			{
				++num_files;
			}
		}
	} while (FindNextFile (find_handle, &ffd) != 0);

	*num_out_files = num_files;
	*out_file_paths = (file_path*)utils_malloc (sizeof (file_path) * num_files);

	find_handle = FindFirstFile (folder_path, &ffd);
	uint32_t current_file_index = 0;
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			char file_name[MAX_PATH];
			wcstombs (file_name, ffd.cFileName, MAX_PATH);
			char* base_name = strtok (file_name, ".");
			char* ext = strtok (nullptr, ".");

			if (strcmp (ext, "glb") == 0 || strcmp (ext, "gltf") == 0)
			{
				file_path* current_file_path = *out_file_paths + current_file_index;
				strcpy (current_file_path->path, base_name);
				strcat (current_file_path->path, ".");
				strcat (current_file_path->path, ext);
				++current_file_index;
			}
		}
	} while (FindNextFile (find_handle, &ffd) != 0);
}

void utils_import_texture (const char* texture_name, int* width, int* height, int* bpp, uint8_t**
pixels)
{
	char full_path[MAX_PATH];
	utils_get_full_file_path (texture_name, full_path);
	*pixels = stbi_load (full_path, width, height, bpp, 4);
}
#endif

#ifdef __ANDROID__
void utils_import_texture (const char* texture_name, AAssetManager* asset_manager, int* width,
		int* height, int* bpp, uint8_t** pixels)
{
	AAsset* image_asset = AAssetManager_open (asset_manager, texture_name, AASSET_MODE_BUFFER);
	uint64_t length = AAsset_getLength64 (image_asset);
	auto buffer = (uint8_t*) utils_malloc (length);

	if (AAsset_read (image_asset, buffer, length) < 0) {
		__android_log_print(ANDROID_LOG_ERROR, "Asteroids", "Could not read image");
	}

	*pixels = stbi_load_from_memory(buffer, length, width, height, bpp, 4);
}
#endif

void* utils_malloc (const uint32_t size)
{
	return std::malloc (size);
}

void* utils_realloc (void* ptr, uint32_t new_size)
{
	void* new_ptr = std::realloc (ptr, new_size);
	return new_ptr;
}

void utils_free (void* ptr)
{
	if (ptr != nullptr)
	{
		std::free (ptr);
		ptr = nullptr;
	}
}


