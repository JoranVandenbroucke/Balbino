#pragma once

namespace Balbino
{
	class Application
	{
	public:
		Application() = default;
		~Application() = delete;
		Application( const Application& ) = delete;
		Application( Application&& ) = delete;
		Application& operator=( const Application& ) = delete;
		Application& operator=( Application&& ) = delete;
	};
}