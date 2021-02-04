//
// Created by nihal on 04-02-2021.
//

#ifndef ASTEROIDS_GAME_HPP
#define ASTEROIDS_GAME_HPP

#include "error.hpp"
#include "input.hpp"
#include <android_native_app_glue.h>

AGE_RESULT game_init_from_app (struct android_app* p_app);
AGE_RESULT game_init_from_native_window (ANativeWindow* window, AAssetManager* asset_manager);

AGE_RESULT game_process_player_movement_input (joystick_input* j_input);
AGE_RESULT game_process_player_trigger_input (trigger_input* t_input);

AGE_RESULT game_update (uint32_t delta_time);
AGE_RESULT game_submit_present ();

void game_shutdown ();

#endif //ASTEROIDS_GAME_HPP
