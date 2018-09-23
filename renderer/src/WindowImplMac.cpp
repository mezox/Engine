#include "WindowImpl.h"

using namespace Engine;

Window::~Window()
{
}

void Window::Initialize()
{
}

void* Window::GetNativeHandle() const
{
	return hWnd;
}

void Window::Update()
{
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
