#ifdef WIN32
#include <Windows.h>
#include <Windowsx.h>
#include <conio.h>

#include <cstdio>
#include <cstdlib>

#include "error.hpp"
#include "log.hpp"
#include "game.hpp"

#define ID_GAME_TICK 1237

unsigned long long last_tick_count = 0;

LRESULT CALLBACK WindowProc (HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    unsigned long long current_tick_count = 0;

    switch (msg)
    {
    case WM_QUIT:
        PostQuitMessage (0);
        break;

    case WM_DESTROY:
        PostQuitMessage (0);
        break;

    case WM_CLOSE:
        PostQuitMessage (0);
        break;

    case WM_KEYDOWN:
        age_result = game_process_key_down (w_param);
        if (age_result != AGE_RESULT::SUCCESS)
        {
            log_error (age_result);
            PostQuitMessage (0);
        }
        break;

    case WM_KEYUP:
        age_result = game_process_key_up (w_param);
        if (age_result != AGE_RESULT::SUCCESS)
        {
            log_error (age_result);
            PostQuitMessage (0);
        }
        break;

    case WM_TIMER:
        current_tick_count = GetTickCount ();

        age_result = game_update ((uint32_t)(current_tick_count - last_tick_count));

        if (age_result != AGE_RESULT::SUCCESS)
        {
            log_error (age_result);
            PostQuitMessage (0);
        }

        last_tick_count = current_tick_count;

        break;

    case WM_LBUTTONDOWN:
        age_result = game_process_left_mouse_click (GET_X_LPARAM (l_param), GET_Y_LPARAM (l_param));
        if (age_result != AGE_RESULT::SUCCESS)
        {
            log_error (age_result);
            PostQuitMessage (0);
        }
        break;

    case WM_RBUTTONDOWN:
        age_result = game_process_right_mouse_click (GET_X_LPARAM (l_param), GET_Y_LPARAM (l_param));
        if (age_result != AGE_RESULT::SUCCESS)
        {
            log_error (age_result);
            PostQuitMessage (0);
        }
        break;

    case WM_MOUSEMOVE:
        age_result = game_process_mouse_move (GET_X_LPARAM (l_param), GET_Y_LPARAM (l_param));
        if (age_result != AGE_RESULT::SUCCESS)
        {
            log_error (age_result);
            PostQuitMessage (0);
        }
        break;

    default:
        break;
    }

    return DefWindowProc (h_wnd, msg, w_param, l_param);
}


int WINAPI wWinMain (_In_ HINSTANCE h_instance, _In_opt_ HINSTANCE previous_instance, _In_ PWSTR cmd_line, _In_ int cmd_show)
{
    AllocConsole ();
    freopen ("CONOUT$", "w", stdout);
    printf ("Hello Console\n");

    HANDLE con_hnd = GetStdHandle (STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX font = { sizeof (font) };
    GetCurrentConsoleFontEx (con_hnd, FALSE, &font);
    font.dwFontSize.X = 0;
    font.dwFontSize.Y = 14;
    SetCurrentConsoleFontEx (con_hnd, FALSE, &font);

    WNDCLASS wc = {0};
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = h_instance;
    wc.lpszClassName = L"DOD";
    wc.hCursor = LoadCursor (h_instance, IDC_ARROW);

    if (!RegisterClass (&wc))
    {
        return EXIT_FAILURE;
    }

    HWND h_wnd = CreateWindow (
        L"DOD", 
        L"DOD",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1280,
        720, 
        nullptr,
        nullptr,
        h_instance,
        nullptr
    );

    if (!h_wnd)
    {
        return EXIT_FAILURE;
    }

    ShowWindow (h_wnd, cmd_show);
    UpdateWindow (h_wnd);

    AGE_RESULT result = game_init (h_instance, h_wnd);
    if (result != AGE_RESULT::SUCCESS)
    {
        log_error (result);
        goto exit;
    }

    last_tick_count = GetTickCount64 ();

    SetTimer (h_wnd, ID_GAME_TICK, 15, nullptr);

    MSG msg;
    ZeroMemory (&msg, sizeof (msg));

    while (
        msg.message != WM_QUIT && 
        msg.message != WM_CLOSE && 
        msg.message != WM_DESTROY
    )
    {
        if (PeekMessage (&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage (&msg);
            DispatchMessage (&msg);
        }
        else
        {
            result = game_submit_present ();
            if (result != AGE_RESULT::SUCCESS)
            {
                log_error (result);
                goto exit;
            }
        }
    }

    KillTimer (h_wnd, ID_GAME_TICK);

 exit:
    game_shutdown ();

    DestroyWindow (h_wnd);

    _getch ();
    FreeConsole ();

    return EXIT_SUCCESS;
}
#elif __ANDROID__
#include <android_native_app_glue.h>
#include <jni.h>
#include <android/log.h>
#include <memory>
#include <android/input.h>
#include <android/native_window.h>
#include <chrono>

#include "game.hpp"
#include "log.hpp"

const char* TAG = "Asteroids";
uint32_t tick_rate_msecs = 15;

bool is_game_inited = false;

static void dummy_draw (struct android_app* p_app) {
    __android_log_write(ANDROID_LOG_VERBOSE, TAG, "draw_something");
    ANativeWindow_Buffer window_buffer;
    ANativeWindow *p_window = p_app->window;
    ANativeWindow_setBuffersGeometry(p_window, 0, 0, WINDOW_FORMAT_RGBA_8888);
    if (ANativeWindow_lock(p_window, &window_buffer, nullptr) < 0) {
        return;
    }
    memset(window_buffer.bits, 0, window_buffer.stride * window_buffer.height * sizeof(uint32_t));

    ANativeWindow_unlockAndPost(p_window);
}

static void handle_cmd (struct android_app* p_app, int cmd) {
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "handling cmd");

    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            age_result = game_init(p_app);
            if (age_result != AGE_RESULT::SUCCESS) {
                log_error(age_result);
                break;
            }

            is_game_inited = true;

            break;

        case APP_CMD_TERM_WINDOW:
            game_shutdown();
            break;

        default:
            break;
    }
}

static int32_t handle_input_event (struct android_app* p_app, AInputEvent* event) {
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "handling input");

    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    float window_width = static_cast<float> (ANativeWindow_getWidth(p_app->window));
    float window_height = static_cast<float> (ANativeWindow_getHeight(p_app->window));

    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        int32_t pointer_count = AMotionEvent_getPointerCount(event);
        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "pointer count: %d", pointer_count);

        for (int32_t p = 0; p < pointer_count; ++p) {
            int32_t pointer_id = AMotionEvent_getPointerId(event, p);
            float x = AMotionEvent_getX(event, pointer_id);
            float y = AMotionEvent_getY(event, pointer_id);

            //__android_log_print(ANDROID_LOG_VERBOSE, TAG,
//                                "index: %d id: %d\n x_index: %f x_id: %f\n y_index: %f y_id: %f\n",
//                                p, pointer_id, x_index, x_id, y_index, y_id);

            float viewport_mapped_x = (x / window_width) * 2 - 1;
            float viewport_mapped_y = (y / window_height) * 2 - 1;

            __android_log_print(ANDROID_LOG_VERBOSE, TAG, "x :%f y: %f", viewport_mapped_x, viewport_mapped_y);
        }

        return 1;
    }

    return 0;
}

void android_main(struct android_app *p_app) {
    p_app->onAppCmd = handle_cmd;
    p_app->onInputEvent = handle_input_event;

    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    auto start = std::chrono::steady_clock::now();

    while (true) {
        int id;
        int events;
        struct android_poll_source *p_source;

        while ((id = ALooper_pollAll(0, nullptr, &events,
                                     reinterpret_cast<void **>(&p_source))) >= 0) {
            if (p_source != nullptr) {
                p_source->process(p_app, p_source);
            }

            if (p_app->destroyRequested == 1) {
                return;
            }
        }

        if (is_game_inited) {
            auto now = std::chrono::steady_clock::now();
            auto delta_time_msecs = std::chrono::duration_cast<std::chrono::milliseconds>(
                    now - start).count();

            if (delta_time_msecs >= tick_rate_msecs) {
                age_result = game_update(delta_time_msecs);
                if (age_result != AGE_RESULT::SUCCESS) {
                    log_error(age_result);
                }

                start = now;
                //__android_log_print(ANDROID_LOG_VERBOSE, TAG, "delta_time_msecs: %lld",
                                    //delta_time_msecs);
            }

            age_result = game_submit_present();
            if (age_result != AGE_RESULT::SUCCESS) {
                log_error(age_result);
            }
        }
    }
}
#endif