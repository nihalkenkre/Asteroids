#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "error.hpp"
#include "types.hpp"

#include <cstdint>

#ifdef __ANDROID__
#include <android/asset_manager.h>
#endif

#ifdef WIN32
AGE_RESULT graphics_init (
    const uint32_t game_large_asteroids_current_max_count,
	const uint32_t game_large_asteroids_live_count,
	const uint32_t game_small_asteroids_current_max_count,
	const uint32_t game_small_asteroids_live_count,
	const uint32_t game_bullets_current_max_count,
	const uint32_t game_bullet_live_count,
	const float screen_aspect_ratio
);
#elif __ANDROID__
AGE_RESULT graphics_init (
		AAssetManager* asset_manager,
		const uint32_t game_large_asteroids_current_max_count,
		const uint32_t game_large_asteroids_live_count,
		const uint32_t game_small_asteroids_current_max_count,
		const uint32_t game_small_asteroids_live_count,
		const uint32_t game_bullets_current_max_count,
		const uint32_t game_bullet_live_count,
		const float screen_aspect_ratio
);
#endif

AGE_RESULT graphics_update_command_buffers (
	const uint32_t game_large_asteroids_live_count,
	const uint32_t game_small_asteroids_live_count,
	const uint32_t game_bullet_live_count, 
	const float screen_aspect_ratio
);

AGE_RESULT graphics_create_transforms_buffer (
	const uint32_t game_large_asteroids_current_max_count, 
	const uint32_t game_small_asteroids_current_max_count, 
	const uint32_t game_bullet_current_max_count
);

AGE_RESULT graphics_update_transforms_buffer_data (
    const float3* game_player_output_position, const float* game_player_output_rotation, const float2* game_player_output_scale, 
	const float3* game_large_asteroids_outputs_positions, const float* game_large_asteroids_outputs_rotations, const float2* game_large_asteroids_outputs_scales,
	const uint32_t game_large_asteroids_live_count, const uint32_t game_large_asteroids_current_max_count, 
	const float3* game_small_asteroids_outputs_positions, const float* game_small_asteroids_outputs_rotations, const float2* game_small_asteroids_outputs_scales,
	const uint32_t game_small_asteroids_live_count, const uint32_t game_small_asteroids_current_max_count, 
	const float3* game_bullets_outputs_positions, const float* game_bullets_outputs_rotations, const float2* game_bullets_outputs_scales,     
	const uint32_t game_bullet_live_count, const uint32_t game_bullets_current_max_count,
	const float background_scale
);

AGE_RESULT graphics_submit_present ();
void graphics_shutdown ();

#endif