#pragma once
#include "stdafx.h"

using namespace std;

namespace In
{
	// ------- ќбработка информации из файла -------
	IN getin(wchar_t infile[])
	{
		IN in = { 0, 1, 0, new char[IN_MAX_LEN_TEXT] {}, IN_CODE_TABLE };

		ifstream fin(infile);
		if (!fin.is_open())
		{
			throw ERROR_THROW(105);
		}
		char ch;
		int currentCol = 0; // столбец в исходнике (-in)
		bool s_literal = false;

		while (fin.get(ch))
		{
			currentCol++; 
			//	---------- ќбработка символов -----------

			if (ch == IN_CODE_QUOTES)
			{
				s_literal = !s_literal;
			}

			if (s_literal)
			{
				if (ch == IN_CODE_ENDL)
				{
					printf_s(ERROR_TRACE, "In.cpp", "getin","literal");
					throw ERROR_THROW_IN(111, in.lines, currentCol);
				}
				AddText(in, ch);
				continue;
			}

			switch (in.code[unsigned char(ch)])
			{
			case IN::F:
				printf_s(ERROR_TRACE, "In.cpp", "getin", "case F");
				throw ERROR_THROW_IN(110, in.lines, currentCol);
				break;

			case IN::T:
				AddText(in, ch);
				break;

			case IN::I:
				in.ignor++;
				break;

			case IN::S: // пробел и табул€ци€
				if (in.size)
					if (in.code[unsigned char(in.text[in.size - 1])] == IN::T || in.code[unsigned char(in.text[in.size - 1])] == IN::V)
					{
						AddText(in, ch);
					}
					else
						in.ignor++;
				break;

			case IN::V:
				if (in.size)
					if (in.code[unsigned char(in.text[in.size - 1])] != IN::S )
					{
						AddSpace(in);
					}
				AddText(in, ch);
				AddSpace(in);
				break;

			case IN::N:		
				if (in.size > 3)
					if (
						in.text[in.size - 4] == '=' && in.text[in.size - 1] == IN_CODE_SPACE && in.text[in.size - 2] == MINUS ||\
						in.text[in.size - 4] == '(' && in.text[in.size - 1] == IN_CODE_SPACE && in.text[in.size - 2] == MINUS ||\
						in.text[in.size - 4] == ',' && in.text[in.size - 1] == IN_CODE_SPACE && in.text[in.size - 2] == MINUS
						)
						in.size--;
					if (in.text[in.size - 4] == '(')
					{
						if (
							in.text[in.size - 1] == IN_CODE_SPACE && in.text[in.size - 2] == ADD ||\
							in.text[in.size - 1] == IN_CODE_SPACE && in.text[in.size - 2] == MUL ||\
							in.text[in.size - 1] == IN_CODE_SPACE && in.text[in.size - 2] == DIV ||\
							in.text[in.size - 1] == IN_CODE_SPACE && in.text[in.size - 2] == MOD 
							) 
						{
							printf_s(ERROR_TRACE, "In.cpp", "case::N", "не правильно ((?:*|/|+|%)\d)");
							throw ERROR_THROW_IN(602, in.lines, currentCol + 1);
						}
					}

				AddText(in, ch);
				break;

			default:
				if (in.size)
					if (in.size && in.code[unsigned char(in.text[in.size - 1])] != IN::S)
					{
						AddSpace(in);
					}
				in.text[in.size] = in.code[unsigned char(ch)];
				in.size++;
				AddSpace(in);

				currentCol = 0;
				in.lines++;
			}
		}
		if (s_literal)
		{
			printf_s(ERROR_TRACE, "In.cpp", "getin", "literal");
			throw ERROR_THROW_IN(111, in.lines, currentCol + 1);
		}

		fin.close();

		return in;
	}

	void Delete(IN& in)
	{
		delete[] in.text;
	}

	void AddText(IN& in, char ch) 
	{
		in.text[in.size] = ch;
		in.size++;
	}

	void AddSpace(IN& in)
	{
		in.text[in.size] = IN_CODE_SPACE;
		in.size++;
	}
}