#pragma once

#define SINGLETON(x) CSingleton<x>::GetInstance()

template<class Singleton>
class CSingleton
{
public:
	static Singleton& GetInstance()
	{
		static Singleton Instance;
		return Instance;
	}
};

