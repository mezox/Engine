#include "WindowImpl.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

using namespace Engine;

namespace
{
	constexpr LPCWSTR wndClassName = L"EngineMainWindow";
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	default:
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

Window::~Window()
{
	::UnregisterClassW(wndClassName, GetModuleHandleW(NULL));
}

void Window::Initialize()
{
	WNDCLASSEXW wcex;

	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.hInstance = GetModuleHandleW(NULL);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName = wndClassName;

	RegisterClassExW(&wcex);

	CREATESTRUCTW cs;
	ZeroMemory(&cs, sizeof(cs));

	cs.x = 0;
	cs.y = 0;
	cs.cx = 1920;
	cs.cy = 1080;	// Window height
	cs.hInstance = wcex.hInstance; // Window instance.
	cs.lpszClass = wcex.lpszClassName;		// Window class name
	cs.lpszName = L"MyFirstWindow";	// Window title
	cs.style = WS_OVERLAPPEDWINDOW;		// Window style

	hWnd = ::CreateWindowExW(cs.dwExStyle, cs.lpszClass, cs.lpszName, cs.style, cs.x, cs.y, cs.cx, cs.cy, cs.hwndParent, cs.hMenu, cs.hInstance, cs.lpCreateParams);

	::ShowWindow((HWND)hWnd, SW_SHOWDEFAULT);
	::UpdateWindow((HWND)hWnd);
}

void* Window::GetNativeHandle() const
{
	return hWnd;
}

void Window::Update()
{
	MSG msg;

	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			// NOTE: While GLFW does not itself post WM_QUIT, other processes
			//       may post it to this one, for example Task Manager
			// HACK: Treat WM_QUIT as a close on all windows
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
}

void Window::GetSize(uint16_t & x, uint16_t & y) const
{

}

void Window::KeyEvent(const int16_t key, const int16_t wch, const int16_t type)
{
}

void Window::MouseEvent(const int16_t key, const int16_t wch, const int16_t type)
{
}
