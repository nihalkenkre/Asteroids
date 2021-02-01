#ifndef GAME_HPP
#define GAME_HPP

#include "error.hpp"
#ifdef __ANDROID__
#include <android_native_app_glue.h>
#endif

#include <cstdint>
#ifdef WIN32
#include <Windows.h>
AGE_RESULT game_init (const HINSTANCE h_instance, const HWND h_wnd);
AGE_RESULT game_process_left_mouse_click (const int32_t x, const int32_t y);
AGE_RESULT game_process_right_mouse_click (const int32_t x, const int32_t y);
AGE_RESULT game_process_mouse_move (const int32_t x, const int32_t y);
AGE_RESULT game_process_key_down (const WPARAM w_param);
AGE_RESULT game_process_key_up (const WPARAM w_param);
#elif __ANDROID__
AGE_RESULT game_init_from_app (struct android_app* p_app);
AGE_RESULT game_init_from_native_window (ANativeWindow* window, AAssetManager* asset_manager);
#endif

AGE_RESULT game_update (uint32_t delta_time);
AGE_RESULT game_submit_present ();
void game_shutdown ();

#endif