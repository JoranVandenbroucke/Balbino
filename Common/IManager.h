#pragma once
#include "Core.h"
struct IScene;
struct IResourceManager;

struct BALBINO_API ISystem
{
	virtual ~ISystem(){};

	virtual bool Update( bool isPause = false ) = 0;

	[[nodiscard]] virtual IResourceManager* GetResourceManager() const = 0;
	[[nodiscard]] virtual IScene* GetCurrentActiveScene() const = 0;
};
