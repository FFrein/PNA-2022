#pragma once
#include "In.h"
#include "Parm.h"
#include "Error.h"

#define MAX_LEN_MESSAGE 300

namespace Log
{
	struct LOG
	{
		wchar_t logfile[PARM_MAX_SIZE];
		std::ofstream* stream;
		LOG();
	};

	LOG getlog(wchar_t logfile[]);									// Создание и открытие потокового вывода протокола

	void WriteLine(std::ostream* stream, const char* c, ...);		// Вывода одной строки в протокол char
	void WriteLine(std::ostream* stream, const wchar_t* c, ...);	// Вывода одной строки в протокол wchar_t
	void WriteLog(std::ostream* stream);							// Вывод заголовка протокола
	void WriteParm(std::ostream* stream, Parm::PARM parm);			// Вывод в протокол информации о входных параметрах
	void WriteIn(std::ostream* stream, In::IN in);					// Вывод в протокол информации о файле

	void Close(LOG log);											// Закрытие потока вывода протокола
}
