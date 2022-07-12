#include "WindowManager.h"

#if defined(_WIN32) || defined(WIN32)
#include <Windows.h>
#endif

void WindowManager::HideConsole()
{
#ifdef __unix__
#elif defined(_WIN32) || defined(WIN32)
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
#endif
}
void WindowManager::ShowConsole()
{
#ifdef __unix__
#elif defined(_WIN32) || defined(WIN32)
    ::ShowWindow(::GetConsoleWindow(), SW_SHOW);
#endif
}
bool WindowManager::IsConsoleVisible()
{
#ifdef __unix__
#elif defined(_WIN32) || defined(WIN32)
    return ::IsWindowVisible(::GetConsoleWindow()) != FALSE;
#endif
}
void WindowManager::OnToggleConsole(bool active)
{
    if (active)
    {
        ShowConsole();
    }
    else
    {
        HideConsole();
    }
}