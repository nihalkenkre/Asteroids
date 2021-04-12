#include <Windows.h>
#include <Windowsx.h>
#include <conio.h>

#include <cstdio>
#include <cstdlib>
#include <memory>

#include "error.hpp"
#include "log.hpp"
#include "game.hpp"

#define ID_GAME_TICK 1237

game* game_obj = nullptr;
std::unique_ptr<game_class> game_class_obj = nullptr;

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
        try
        {
            game_class_obj->process_key_down (w_param);
        }
        catch (AGE_RESULT result)
        {
            log_error (result);
        }

        break;

    case WM_KEYUP:
        try
        {
            game_class_obj->process_key_up (w_param);
        }
        catch (AGE_RESULT result)
        {
            log_error (result);
        }

        break;

    case WM_TIMER:
        current_tick_count = GetTickCount ();
        
        try
        {
            game_class_obj->update ((uint32_t)(current_tick_count - last_tick_count));
        }
        catch (AGE_RESULT result)
        {
            log_error (result);
        }

        last_tick_count = current_tick_count;

        break;

    case WM_LBUTTONDOWN:
        try
        {
            game_class_obj->process_left_mouse_click (GET_X_LPARAM (l_param), GET_Y_LPARAM (l_param));
        }
        catch (AGE_RESULT result)
        {
            log_error (result);
        }
        break;

    case WM_RBUTTONDOWN:
        try
        {
            game_class_obj->process_right_mouse_click (GET_X_LPARAM (l_param), GET_Y_LPARAM (l_param));
        }
        catch (AGE_RESULT result)
        {
            log_error (result);
        }
        break;

    case WM_MOUSEMOVE:
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

    try
    {
        game_class_obj = std::make_unique<game_class> (h_instance, h_wnd);

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
                game_class_obj->submit_present ();
            }
        }
    }
    catch (AGE_RESULT result)
    {
        log_error (result);
    }


    /*game_obj = (game*)game_create ();
    if (game_obj == nullptr)
    {
        goto exit;
    }

    AGE_RESULT result = game_init (game_obj, h_instance, h_wnd);
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
            result = game_submit_present (game_obj);
            if (result != AGE_RESULT::SUCCESS)
            {
                log_error (result);
                goto exit;
            }
        }
    }*/

    KillTimer (h_wnd, ID_GAME_TICK);
 
    DestroyWindow (h_wnd);

    _getch ();
    FreeConsole ();

    return EXIT_SUCCESS;
}