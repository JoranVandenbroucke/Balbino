#pragma once
namespace Balbino
{
	template <class T>
	class Singleton
	{
	public:
		static T& GetInstance();
		Singleton() = default;
		~Singleton() = default;
		Singleton( const Singleton& ) = delete;
		Singleton( Singleton&& ) = delete;
		Singleton& operator=( const Singleton& ) = delete;
		Singleton& operator=( Singleton&& ) = delete;
	};

	template <class T>
	T& Singleton<T>::GetInstance()
	{
		static T init{};
		return init;
	}
}
