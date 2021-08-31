#pragma once
namespace Balbino
{
	template <class T>
	class CSingleton
	{
	public:
		static T& GetInstance();
		CSingleton() = default;
		virtual ~CSingleton() = default;
		CSingleton( const CSingleton& ) = delete;
		CSingleton( CSingleton&& ) = delete;
		CSingleton& operator=( const CSingleton& ) = delete;
		CSingleton& operator=( CSingleton&& ) = delete;
	};

	template <class T>
	T& CSingleton<T>::GetInstance()
	{
		static T init{};
		return init;
	}
}
