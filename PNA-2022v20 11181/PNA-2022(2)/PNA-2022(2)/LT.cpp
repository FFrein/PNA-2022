#pragma once
#include "stdafx.h"

using namespace std;

namespace LT
{
	Entry::Entry(char lexema, int sn, int tn, int idxTI)		// для идентификатор
	{
		this->lexema = lexema;
		this->sign = LT_V_NULL;
		this->sn = sn;
		this->tn = tn;
		this->idxTI = idxTI;
	}
	Entry::Entry(char lexema, char sign, int sn, int tn)		// для операторов
	{
		this->lexema = lexema;
		this->sign = sign;
		this->sn = sn;
		this->tn = tn;
		this->idxTI = LT_TI_NULLXDX;
	}
	Entry::Entry(char lexema, int sn, int tn)					// остальные лексемы
	{
		this->lexema = lexema;
		this->sign = LT_V_NULL;
		this->sn = sn;
		this->tn = tn;
		this->idxTI = LT_TI_NULLXDX;
	}
	Entry::Entry(char lexema)
	{
		this->lexema = lexema;
		this->sign = LT_V_NULL;
		this->sn = LT_TI_NULLXDX;
		this->tn = LT_TI_NULLXDX;
		this->idxTI = LT_TI_NULLXDX;
	}

	LexTable::LexTable()
	{
		maxsize = LT_MAXSIZE;
		current_size = 0;
		table = new Entry * [LT_MAXSIZE];
	}

	LexTable::LexTable(int size)
	{
		if (size > LT_MAXSIZE)
		{
			printf_s(ERROR_TRACE, "LT.cpp", "LexTable", "size lexem table");
			throw ERROR_THROW(122);
		}
		maxsize = LT_MAXSIZE;
		current_size = 0;
		table = new Entry * [size];
	}

	void Add(LexTable& lextable, Entry* entry)
	{
		if (lextable.current_size < lextable.maxsize)
		{
			lextable.table[lextable.current_size] = entry;
			lextable.current_size++;
		}
		else
			throw ERROR_THROW(123);
	}

	Entry GetEntry(LexTable& lextable, int n)
	{
		if(n > lextable.current_size)
			throw ERROR_THROW(124);

		return *lextable.table[n];
	}

	void Delete(LexTable& lextable)
	{
		for (int i = 0; i < lextable.current_size; i++)
		{
			delete lextable.table[i];
		}
		delete[] lextable.table;
	}

	void PrintLexTable(LexTable& lextable, const wchar_t* in)
	{
		wchar_t* lex = new wchar_t[wcslen(in) + wcslen(PARM_LEX_DEFAULT_EXT) + 1]{};
		wcscat(lex, in);
		wcscat(lex, PARM_LEX_DEFAULT_EXT);
		

		ofstream lexStream(lex);
		delete[] lex;
		if (!lexStream.is_open())
		{
			printf_s(ERROR_TRACE, "LT.cpp", "PrintLexTable", "protocol open error");
			throw ERROR_THROW(126);
		}
		int currentString = 1;
		lexStream << currentString << '\t';
		for (int i = 0; i < lextable.current_size;)
		{
			if (currentString == lextable.table[i]->sn)
			{
				lexStream << lextable.table[i]->lexema;
				i++;
			}
			else
			{
				lexStream << '\n' << ++currentString << '\t';
			}
		}
		lexStream.close();
	}
}