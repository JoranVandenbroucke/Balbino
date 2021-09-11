#pragma once
#include <vulkan/vulkan.hpp>

namespace Balbino
{
	class CCamera
	{
	public:
		CCamera() = default;
		~CCamera() = default;
		CCamera( const CCamera& ) = delete;
		CCamera( CCamera&& ) = delete;
		CCamera& operator=( const CCamera& ) = delete;
		CCamera& operator=( CCamera&& ) = delete;

		void* operator new( size_t size );
		void* operator new( size_t size, int, const char*, int );
		void* operator new[]( size_t size ) = delete;

		void operator delete( void* ptr )noexcept;
		void operator delete( void* ptr, int, const char*, int)noexcept;
		void operator delete[]( void* ptr ) = delete;

		void SetDrawIndex(int index);
		int GetDrawIndex() const;

	private:
		int m_index;
	};
}