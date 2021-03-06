#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

extern "C"
{
#include "./lua/lua.h"
#include "./lua/lualib.h"
#include "./lua/lauxlib.h"
};
#include "lua_tinker.h"
#ifndef WIN32
#ifdef USE_GOOGLE_POFILER
#include "gperftools/profiler.h"
#endif
#endif

int cfunc1(int a, int b)
{
	return a - b;
}

class class1
{
public:
	class1()
	{
		b = 10;
	}
	int memfunc1(int a)
	{
		return a - b;
	}
private:
	int b;
};

int main(int argc, const char *argv[])
{
	if (argc < 2)
	{
		return 0;
	}

	lua_State * L = luaL_newstate();
	if (!L)
	{
		printf("lua_open error\n");
		return false;
	}
	luaL_openlibs(L);

	lua_tinker::def(L, "cfunc1", cfunc1); 

	lua_tinker::dofile(L, argv[1]);

	int ret = 0;

	unsigned int begin = time(0);

#ifndef WIN32
#ifdef USE_GOOGLE_POFILER
	ProfilerStart("luatest.prof");
#endif
#endif

#ifndef _DEBUG
	for (int i = 0; i < 1/*9000000*/; i++)
#else
	for (int i = 0; i < 1; i++)
#endif
	{
		ret = lua_tinker::call<int>(L, "myfunc1", 1, 2);
	}

#ifndef WIN32
#ifdef USE_GOOGLE_POFILER
	ProfilerStop();
#endif
#endif
	unsigned int end = time(0);

	printf("call ret %d %d\n", ret, end - begin);

	return 0;
}

