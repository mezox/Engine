#pragma once

#include <memory>
#include "RendererBase.h"

namespace Renderer
{
	class RENDERER_API IEngine
	{
	public:
		virtual ~IEngine() = default;

		virtual void StartFrame() {}
		virtual void Update() {}
		virtual void EndFrame() {}
	};

	RENDERER_API std::shared_ptr<IEngine> CreateEngineService();

	class RENDERER_API EngineServiceLocator
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

	class Engine : public IEngine
	{
	public:
		Engine();

		virtual void StartFrame() override;
		virtual void Update() override;
		virtual void EndFrame() override;
	};
}