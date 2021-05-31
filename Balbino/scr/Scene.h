#pragma once
namespace Balbino
{
	class Scene
	{
	public:
		Scene() = default;
		~Scene() = delete;
		Scene( const Scene& ) = delete;
		Scene( Scene&& ) = delete;
		Scene& operator=( const Scene& ) = delete;
		Scene& operator=( Scene&& ) = delete;
	};
}
