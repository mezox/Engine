#pragma once

#include <memory>
#include "Window.h"
#include "SwapChain.h"

namespace Engine
{
	class Window : public IWindow, public IInputHandler
	{
	public:
		virtual ~Window();

        // IWindow
		void Initialize() override;
		void* GetNativeHandle() const override final;

		void Update() override;
		void GetSize(uint16_t& x, uint16_t& y) const override;
        void CreateSwapChain() override;
        
        Renderer::ISwapChain* GetSwapChain() const override;
        
        void AcquireSwapChainImage() override;
        void SwapBuffers() override;

		void KeyEvent(const int16_t key, const int16_t wch, const int16_t type) override;
		void MouseEvent(const int16_t key, const int16_t wch, const int16_t type) override;

	private:
		void* mNativeHandle{ nullptr };
        std::unique_ptr<Renderer::ISwapChain> mSwapChain;
	};
}
