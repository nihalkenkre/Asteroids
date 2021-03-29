#ifndef GAME_HPP
#define GAME_HPP

#include "error.hpp"

#include <cstdint>
#include <Windows.h>

typedef struct game_ game;

void* game_create ();
AGE_RESULT game_init (game* game_obj, const HINSTANCE h_instance, const HWND h_wnd);



//AGE_RESULT game_init (const HINSTANCE h_instance, const HWND h_wnd);
AGE_RESULT game_process_left_mouse_click (game* game_obj, const int32_t x, const int32_t y);
AGE_RESULT game_process_right_mouse_click (game* game_obj, const int32_t x, const int32_t y);
AGE_RESULT game_process_mouse_move (game* game_obj, const int32_t x, const int32_t y);
AGE_RESULT game_process_key_down (game* game_obj, const WPARAM w_param);
AGE_RESULT game_process_key_up (game* game_obj, const WPARAM w_param);

AGE_RESULT game_update (game* game_obj, uint32_t delta_time);
AGE_RESULT game_submit_present (game* game_obj);
void game_shutdown (game* game_obj);
void game_destroy (game* game_obj);
#endif