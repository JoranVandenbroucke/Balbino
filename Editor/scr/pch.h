#pragma once
#ifndef DBG_NEW
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new 
#endif // _DEBUG
#endif // DBG_NEW
