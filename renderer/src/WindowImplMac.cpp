#include "WindowImpl.h"

#include "SwapChainImpl.h"

using namespace Engine;
using namespace Renderer;

Window::~Window()
{
}

void Window::Initialize()
{
}

void* Window::GetNativeHandle() const
{
	return mNativeHandle;
}

void Window::Update()
{
}

void Window::GetSize(uint16_t & x, uint16_t & y) const
{

}

void Window::CreateSwapChain()
{
    mSwapChain = std::make_unique<SwapChainVK>();
    mSwapChain->Initialize(1920, 1080);
}

ISwapChain* Window::GetSwapChain() const
{
    return mSwapChain.get();
}

void Window::AcquireSwapChainImage()
{
    
}

void Window::SwapBuffers()
{
    
}

void Window::KeyEvent(const int16_t key, const int16_t wch, const int16_t type)
{
}

void Window::MouseEvent(const int16_t key, const int16_t wch, const int16_t type)
{
}
