#pragma once

class WindowManager
{
public:
	void HideConsole();
	void ShowConsole();
	void OnToggleConsole(bool active);

	bool IsConsoleVisible();
};