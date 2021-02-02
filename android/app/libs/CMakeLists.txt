cmake_minimum_required (VERSION 3.4.1)

set (SRC_PATH ../../../src)

add_definitions (-DVK_USE_PLATFORM_ANDROID_KHR)
add_library (native_app_glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

add_library (game SHARED ${SRC_PATH}/game.cpp vulkan_wrapper.cpp main.cpp ${SRC_PATH}/vulkan_interface.cpp ${SRC_PATH}/graphics.cpp ${SRC_PATH}/log.cpp ${SRC_PATH}/utils.cpp ${SRC_PATH}/types.cpp ${SRC_PATH}/vk_utils.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall")

target_include_directories (game PRIVATE ${CMAKE_SOURCE_DIR} ${ANDROID_NDK}/sources/android/native_app_glue ${SRC_PATH}/external/stb ${SRC_PATH} ${SRC_PATH}/android)

set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

target_link_libraries (game native_app_glue android log)