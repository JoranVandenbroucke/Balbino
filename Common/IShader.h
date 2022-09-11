#pragma once
#include <cstdint>

struct IShader
{
	virtual ~IShader()
	{
	}

	virtual int GetID() = 0;
	virtual int AddRef() = 0;
	virtual int Release() = 0;
	virtual int ReleaseForce() = 0;

	[[nodiscard]] virtual int GetFlags() const = 0;
	virtual void SetFlags( int Flags ) = 0;
	virtual void ClearFlags( int Flags ) = 0;
	virtual bool Reload( int nFlags, const char* szShaderName ) = 0;
	virtual int Size( int Flags ) = 0;

	virtual void GetMemoryUsage( uint64_t* pSizer ) const = 0;
};
