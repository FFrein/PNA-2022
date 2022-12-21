#pragma once
#include "stdafx.h"

namespace Parm
{
	wchar_t* ptrIN = NULL;
	wchar_t* ptrOUT = NULL;
	wchar_t* ptrLOG = NULL;
	PARM getparm(int argc, wchar_t* argv[])
	{
		if (argc == 1)
		{
			printf_s(ERROR_TRACE, "Parm.cpp", "getparm", "argc");
			throw ERROR_THROW(100);
		}
		for (int i = 1; i < argc; i++)
		{
			if (wcslen(argv[i]) > PARM_MAX_SIZE)
			{
				printf_s(ERROR_TRACE, "Parm.cpp", "getparm", "size");
				throw ERROR_THROW(101);
			}

			if (!ptrIN && wcsstr(argv[i], PARM_IN))
				ptrIN = wcsstr(argv[i], PARM_IN) + wcslen(PARM_IN);
			if (!ptrOUT && wcsstr(argv[i], PARM_OUT))
				ptrOUT = wcsstr(argv[i], PARM_OUT) + wcslen(PARM_OUT);
			if (!ptrLOG && wcsstr(argv[i], PARM_LOG))
				ptrLOG = wcsstr(argv[i], PARM_LOG) + wcslen(PARM_LOG);				
		}
		PARM parm;
		if (ptrIN)
			wcscpy(parm.in, ptrIN);
		else 
		{
			printf_s(ERROR_TRACE, "Parm.cpp", "getparm", "in");
			throw ERROR_THROW(100);
		}
		if (ptrOUT)
			wcscpy(parm.out, ptrOUT);
		else
		{
			wcscpy(parm.out, ptrIN);
			wcscat(parm.out, PARM_OUT_DEFAULT_EXT);
		}
		if (ptrLOG)
			wcscpy(parm.log, ptrLOG);
		else
		{
			wcscpy(parm.log, ptrIN);
			wcscat(parm.log, PARM_LOG_DEFAULT_EXT);
		}
		return parm;
	}
}

