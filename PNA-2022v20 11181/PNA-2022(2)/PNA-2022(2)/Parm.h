#pragma once
#include "stdafx.h"

#define PARM_IN		_TEXT("-in:")
#define PARM_OUT	_TEXT("-out:")
#define PARM_LOG	_TEXT("-log:")
#define PARM_MAX_SIZE 300
#define PARM_OUT_DEFAULT_EXT _TEXT(".asm")
#define PARM_LOG_DEFAULT_EXT _TEXT(".log")

namespace Parm
{
	struct PARM
	{
		wchar_t in[PARM_MAX_SIZE];
		wchar_t out[PARM_MAX_SIZE];
		wchar_t log[PARM_MAX_SIZE];
	};

	PARM getparm(int argc, wchar_t* argv[]);	// Обработка входных параметров
};