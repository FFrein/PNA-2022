#pragma once
#include "stdafx.h"

using namespace std;

int wmain(int argc, wchar_t* argv[])
{
	setlocale(LC_ALL, "rus");

	Log::LOG log;
	Out::OUT out;
	try
	{

		Parm::PARM parm = Parm::getparm(argc, argv);				// обработка входных параметорв
		log = Log::getlog(parm.log);								// создание потокового вывода в протокол
		Log::WriteLog(log.stream);									// вывод заголовка протокола
		Log::WriteParm(log.stream, parm);							// вывод в протокол информации о входных параметрах


		In::IN in = In::getin(parm.in);								// обработка информации из файла (удаление лишних пробелов и т.д.)
		Log::WriteIn(log.stream, in);								// вывод в протокол информации о входном IN файле 


		LT::LexTable lex(LT_MAXSIZE);								// выделение памяти под таблицу лексем
		IT::IdTable id(TI_MAXSIZE);									// выделение памяти под талицу идентификаторов

		//Лексический анализ
		LexAnalysis(in, lex, id);
		Log::WriteLine(&std::cout, "Лексический анализ завершен без ошибок", "");
		Delete(in);

		//Синтаксический анализ
		MFST_TRACE_START(log.stream)
			MFST::MFST sintaxAnaliz(lex, GRB::getGreibach());
		bool syntax_ok = sintaxAnaliz.start(log.stream);
		if (!syntax_ok)
		{
			Log::WriteLine(&std::cout, "Синтаксический анализ завершен с ошибками\n", "Выполнение программы остановлено", "");
			//throw ERROR_THROW(600);
			return 0;

		}
		Log::WriteLine(&std::cout, "Синтаксический анализ завершен без ошибок", "");
		sintaxAnaliz.printRules(log.stream);


		//Семантический анализ 
		SM::semAnaliz(lex, id);
		Log::WriteLine(&std::cout, "Семантический анализ завершен без ошибок", "");


		//Генерация в ассемблер
		out = Out::getout(parm.out);										// 	создание потокового вывода в выходной файл OUT
		GN::GenerationASM(out.stream, lex, id);
		Log::WriteLine(&std::cout, "\nПрограмма успешно завершена!", "");

		PrintIdTable(id, L"Table");
		PrintLexTable(lex, L"Table");

		Delete(lex);
		Delete(id);
		Out::Close(out);
		Log::Close(log);
	}

	catch (Error::ERROR e)
	{
		cout << "Ошибка " << e.id << ": " << e.message << endl << endl;
		return e.id;
	}

	return 0;
}
