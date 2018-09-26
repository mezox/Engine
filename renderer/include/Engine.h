#pragma once

#include <memory>
#include "RendererBase.h"

namespace Engine
{
    class IWindow;
    
	class RENDERER_API IEngine
	{
	public:
		virtual ~IEngine() = default;

        virtual void Initialize() = 0;
        virtual std::unique_ptr<IWindow> CreateWindow() = 0;
        virtual void StartFrame() = 0;
		virtual void Update() = 0;
		virtual void EndFrame() = 0;
        virtual void DeInitialize() = 0;
	};

    std::shared_ptr<IEngine> CreateEngineService();

	class EngineServiceLocator
	{
	public:
		static void Provide(std::shared_ptr<IEngine> service)
		{
			mService = std::move(service);
		}

		static std::shared_ptr<IEngine> Service()
		{
			return mService;
		}

		static bool Available()
		{
			return !!mService;
		}

	private:
		static std::shared_ptr<IEngine> mService;
	};

	class ImmersiveEngine : public IEngine
	{
	public:
		ImmersiveEngine();

        virtual void Initialize() override;
        virtual std::unique_ptr<IWindow> CreateWindow() override;
		virtual void StartFrame() override;
		virtual void Update() override;
		virtual void EndFrame() override;
        virtual void DeInitialize() override;
	};
}
