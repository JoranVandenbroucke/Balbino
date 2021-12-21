#pragma once

namespace Balbino
{
	class CRenderer;

	class CCameraManager final
	{
	public:
		CCameraManager()=default;
		virtual ~CCameraManager() = default;
		CCameraManager(const CCameraManager&) = delete;
		CCameraManager(CCameraManager&&) = delete;
		CCameraManager& operator=(const CCameraManager&) = delete;
		CCameraManager& operator=(CCameraManager&&) = delete;
	private:
	};
}
