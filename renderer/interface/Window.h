#pragma once

#include <cstdint>
#include "RendererBase.h"

namespace Renderer
{
    class ISwapChain;
}

namespace Engine
{
	class RENDERER_API IWindow
	{
	public:
		virtual ~IWindow() {}

		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void GetSize(uint16_t& x, uint16_t& y) const = 0;
		virtual void* GetNativeHandle() const = 0;
        virtual void CreateSwapChain() = 0;
        virtual Renderer::ISwapChain* GetSwapChain() const = 0;
        
        virtual void AcquireSwapChainImage() = 0;
        virtual void SwapBuffers() = 0;
	};

	class RENDERER_API IInputHandler
	{
	public:
		virtual ~IInputHandler() {}

		virtual void KeyEvent(const int16_t key, const int16_t wch, const int16_t type) = 0;
		virtual void MouseEvent(const int16_t key, const int16_t wch, const int16_t type) = 0;
	};
}
