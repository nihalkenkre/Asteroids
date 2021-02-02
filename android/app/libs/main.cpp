//
// Created by nihal on 01-02-2021.
//

#include <android_native_app_glue.h>
#include <jni.h>
#include <android/log.h>
#include <memory>
#include <android/input.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/asset_manager_jni.h>

#include <chrono>

#include "game.hpp"
#include "log.hpp"
#include "types.hpp"

uint32_t tick_rate_msecs = 15;

bool is_game_inited = false;

static void dummy_draw (struct android_app* p_app)
{
    __android_log_write (ANDROID_LOG_VERBOSE, TAG, "draw_something");
    ANativeWindow_Buffer window_buffer;
    ANativeWindow *p_window = p_app->window;
    ANativeWindow_setBuffersGeometry (p_window, 0, 0, WINDOW_FORMAT_RGBA_8888);
    if (ANativeWindow_lock (p_window, &window_buffer, nullptr) < 0)
    {
        return;
    }
    memset (window_buffer.bits, 0, window_buffer.stride * window_buffer.height * sizeof (uint32_t));

    ANativeWindow_unlockAndPost (p_window);
}

static void handle_cmd (struct android_app* p_app, int cmd)
{
    __android_log_print (ANDROID_LOG_VERBOSE, TAG, "handling cmd");

    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    switch (cmd)
    {
        case APP_CMD_INIT_WINDOW:
            age_result = game_init_from_app (p_app);
            if (age_result != AGE_RESULT::SUCCESS)
            {
                log_error (age_result);
                break;
            }

            is_game_inited = true;

            break;

        case APP_CMD_TERM_WINDOW:
            game_shutdown ();
            break;

        default:
            break;
    }
}

static int32_t handle_input_event (struct android_app* p_app, AInputEvent* event)
{
    __android_log_print (ANDROID_LOG_VERBOSE, TAG, "handling input");

    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    float window_width = static_cast<float> (ANativeWindow_getWidth (p_app->window));
    float window_height = static_cast<float> (ANativeWindow_getHeight (p_app->window));

    if (AInputEvent_getType (event) == AINPUT_EVENT_TYPE_MOTION)
    {
        int32_t pointer_count = AMotionEvent_getPointerCount (event);
        __android_log_print (ANDROID_LOG_VERBOSE, TAG, "pointer count: %d", pointer_count);

        for (int32_t p = 0; p < pointer_count; ++p)
        {
            int32_t pointer_id = AMotionEvent_getPointerId (event, p);
            float x = AMotionEvent_getX (event, pointer_id);
            float y = AMotionEvent_getY (event, pointer_id);

            //__android_log_print(ANDROID_LOG_VERBOSE, TAG,
//                                "index: %d id: %d\n x_index: %f x_id: %f\n y_index: %f y_id: %f\n",
//                                p, pointer_id, x_index, x_id, y_index, y_id);

            float viewport_mapped_x = (x / window_width) * 2 - 1;
            float viewport_mapped_y = (y / window_height) * 2 - 1;

            __android_log_print (ANDROID_LOG_VERBOSE, TAG, "x :%f y: %f", viewport_mapped_x, viewport_mapped_y);
        }

        return 1;
    }

    return 0;
}

void android_main(struct android_app *p_app)
{
    p_app->onAppCmd = handle_cmd;
    p_app->onInputEvent = handle_input_event;

    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    auto start = std::chrono::steady_clock::now ();

    while (true)
    {
        int id;
        int events;
        struct android_poll_source *p_source;

        while ((id = ALooper_pollAll (0, nullptr, &events,
                                      reinterpret_cast<void **>(&p_source))) >= 0)
        {
            if (p_source != nullptr)
            {
                p_source->process (p_app, p_source);
            }

            if (p_app->destroyRequested == 1)
            {
                return;
            }
        }

        if (is_game_inited)
        {
            auto now = std::chrono::steady_clock::now ();
            auto delta_time_msecs = std::chrono::duration_cast<std::chrono::milliseconds> (
                    now - start).count ();

            if (delta_time_msecs >= tick_rate_msecs)
            {
                age_result = game_update (delta_time_msecs);
                if (age_result != AGE_RESULT::SUCCESS)
                {
                    log_error (age_result);
                }

                start = now;
                //__android_log_print(ANDROID_LOG_VERBOSE, TAG, "delta_time_msecs: %lld",
                //delta_time_msecs);
            }

            age_result = game_submit_present ();
            if (age_result != AGE_RESULT::SUCCESS)
            {
                log_error (age_result);
            }
        }
    }
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_ntkinteractive_asteroids_MainActivity_HelloFromNDK (JNIEnv *env, jobject thiz)
{
    return (*env).NewStringUTF("Hello From NDK");
}

ANativeWindow* window = nullptr;
AAssetManager* asset_manager = nullptr;

extern "C"
JNIEXPORT void JNICALL
Java_com_ntkinteractive_asteroids_VkSurfaceView_GameInit (JNIEnv *env, jobject thiz, jobject surface, jobject
        assetManager)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    window = ANativeWindow_fromSurface (env, surface);
    asset_manager = AAssetManager_fromJava (env, assetManager);

    age_result = game_init_from_native_window (window, asset_manager);

    if (age_result != AGE_RESULT::SUCCESS)
    {
        log_error (age_result);
    }

    /*while (true)
    {
        age_result = game_update (33);

        if (age_result != AGE_RESULT::SUCCESS)
        {
            log_error (age_result);
            break;
        }

        age_result = game_submit_present ();

        if (age_result != AGE_RESULT::SUCCESS)
        {
            log_error (age_result);
            break;
        }
    }*/
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ntkinteractive_asteroids_VkSurfaceView_GameUpdate (JNIEnv *env, jobject thiz)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    age_result = game_update (33);
    if (age_result != AGE_RESULT::SUCCESS)
    {
        log_error (age_result);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ntkinteractive_asteroids_VkSurfaceView_GameSubmitPresent (JNIEnv *env, jobject thiz)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    age_result = game_submit_present ();
    if (age_result != AGE_RESULT::SUCCESS)
    {
        log_error (age_result);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ntkinteractive_asteroids_VkSurfaceView_GameShutdown (JNIEnv *env, jobject thiz)
{
    game_shutdown ();

    ANativeWindow_release (window);
}
