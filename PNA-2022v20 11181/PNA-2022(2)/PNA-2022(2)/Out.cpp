#pragma once
#include "stdafx.h"


using namespace std;

namespace Out
{
	OUT::OUT()
	{
		memset(outfile, NULL, sizeof(wchar_t) * PARM_MAX_SIZE);
		stream = NULL;
	}
	// ------- —оздание и открытие потокового вывода протокола -----
	OUT getout(wchar_t outfile[])
	{
		OUT out;
		out.stream = new std::ofstream;
		out.stream->open(outfile);

		if (!out.stream->is_open())
		{
			printf_s(ERROR_TRACE, "Out.cpp", "getout", "open file");
			throw ERROR_THROW(107);
		}
		wcscpy(out.outfile, outfile);

		return out;
	}


	// ------- «акрытие потока вывода протокола -------
	void Close(OUT out)
	{
		out.stream->close();
		delete out.stream;
	}
}
