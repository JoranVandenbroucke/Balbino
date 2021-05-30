#pragma once
namespace Balbino
{
	class EntryPoint
	{
	public:
		EntryPoint() = default;
		~EntryPoint() = delete;
		EntryPoint( const EntryPoint& ) = delete;
		EntryPoint( EntryPoint&& ) = delete;
		EntryPoint& operator=( const EntryPoint& ) = delete;
		EntryPoint& operator=( EntryPoint&& ) = delete;
	};

}