/**
 * In this file we re-implement the plugin module by using dynamic library.
 */
#include "ObsCloseHelper.h"

#include <QMainWindow>
#include <windows.h>

void ObsCloseHelper::close(QMainWindow* window)
{
	PostMessage(reinterpret_cast<HWND>(window->winId()), WM_CLOSE, 0, 0);
}