#pragma once
#include "stdafx.h"

using namespace std;

namespace GN
{
	void GenerationASM(std::ostream* stream, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		ostream* file = stream;
		*file << BEGIN;
		*file << EXTERN;
		*file << STACK(4096);
		GenConstAndData(idtable, file);
		*file << CODE;
		GenCode(lextable, idtable, file);
		*file << END;
	}

	void GenConstAndData(IT::IdTable& idtable, ostream* file)
	{
		vector <string> result;

		vector <string> con;  con.push_back(CONST);
		vector <string> data;  data.push_back(DATA);

		for (int i = 0; i < idtable.current_size; i++)// const, data
		{
			string str = "\t" + idtable.table[i]->FullName;

			if (idtable.table[i]->idtype == IT::IDTYPE::L)	// литерал - в .const
			{
				switch (idtable.table[i]->iddatatype)
				{
				case IT::IDDATATYPE::INT:  str += " sdword " + itoS(idtable.table[i]->value.vint);  break;
				case IT::IDDATATYPE::STR:  str += " byte " + string(idtable.table[i]->value.vstr.str) + ", 0";  break;
				}
				con.push_back(str);
			}
			else if (idtable.table[i]->idtype == IT::IDTYPE::V)	// переменна€ - в .data
			{
				switch (idtable.table[i]->iddatatype)
				{
				case IT::IDDATATYPE::INT: str += " sdword 0";  break;
				case IT::IDDATATYPE::STR: str += " dword ?";  break;
				}
				data.push_back(str);
			}
		}
		result.insert(result.end(), con.begin(), con.end());  //result.push_back("\n");
		result.insert(result.end(), data.begin(), data.end()); //result.push_back("\n");
		for (auto r : result)
			*file << r << endl;
	}

	void GenCode(LT::LexTable& lextable, IT::IdTable& idtable, ostream* file)
	{
		string str;
		string funcName;	// им€ текущей функции
		int branchingnNum = -1, open = 0;
		stack<A> kol;

		for (int i = 0; i < lextable.current_size; i++)
		{
			switch (LT_ENTRY(i)->lexema)
			{
			case LEX_MAIN:
			{
				str = SEPSTR("MAIN") + "main PROC\n";
				break;
			}
			case LEX_FUNCTION:
			{
				funcName = IT_ENTRY(i + 1)->FullName;
				str = GenFunctionCode(lextable, idtable, i);
				break;
			}
			case LEX_RETURN:
			{
				str = GenExitCode(lextable, idtable, i, funcName);
				break;
			}
			case LEX_ID: // вызов функции
			{
				if (LT_ENTRY(i + 1)->lexema == LEX_LEFTHESIS && LT_ENTRY(i + 1)->lexema != LEX_FUNCTION) // не объ€вление, а вызов
					str = GenCallFuncCode(lextable, idtable, i);
				break;
			}
			case LEX_IF: // условие
			{
				branchingnNum++;
				str = GenBranchingCode(lextable, idtable, i, branchingnNum);
				break;
			}
			case LEX_LEFTBRACE:
			{
				open++;
				break;
			}
			case LEX_RIGHTBRACE:	// переход на метку в конце кондишна
			{
				open--;
				if (LT_ENTRY(i + 1)->lexema == LEX_ELSE)
				{
					kol.pop();
					kol.push(A(open, branchingnNum, IfEnum::thenOrElse));
					str += "jmp next" + itoS(kol.top().branchingnNum) + '\n';
				}
				else 
					if (!kol.empty())
						if (kol.top().openRightbrace == open)
						{
							if (kol.top().ifEnum == IfEnum::repeat)
							{
								str += "jmp cyclenext" + itoS(kol.top().branchingnNum) + '\n';
								str += "cycle" + itoS(kol.top().branchingnNum) + ":\n";
								kol.pop();
							}
							else
							{
								str += "next" + itoS(kol.top().branchingnNum) + ":\n";
								kol.pop();
							}
						}
				break;
			}
			case LEX_THEN: // условие верно(метка)
			{
				kol.push(A(open, branchingnNum, IfEnum::thenOrElse));
				str += "true" + itoS(branchingnNum) + ":";
				break;
			}
			case LEX_ELSE: // условие неверно(метка)
			{
				str += "false" + itoS(branchingnNum) + ":";
				break;
			}
			case LEX_REPEAT: // цикл с условием (метка)
			{
				branchingnNum++;
				kol.push(A(open, branchingnNum, IfEnum::repeat));
				str += "cyclenext" + itoS(kol.top().branchingnNum) + ":\n";
				str += GenBranchingCode(lextable, idtable, i, kol.top().branchingnNum);
				
				break;
			}
			case LEX_EQUAL: // присваивание (вычисление выражений)
			{
				PN::polishNotation(i, lextable, idtable);
				str = GenEqualCode(lextable, idtable, i);
				break;
			}
			case LEX_PRINT: // вывод
			{
				IT::Entry* e;
				if (lextable.table[i + 1]->lexema == '(' && lextable.table[i + 2]->lexema =='#')// print(#l)
				{
					e = IT_ENTRY(i + 3);
				}
				else if(lextable.table[i + 1]->lexema == '(') // print(i)
				{
					e = IT_ENTRY(i + 2);
				}
				else// print i
				{
					e = IT_ENTRY(i + 1);
				}

				switch (e->iddatatype)
				{
				case IT::IDDATATYPE::INT:
					str += "push " + string(e->FullName) + "\ncall print_int\n";
					break;
				case IT::IDDATATYPE::STR:
					if (e->idtype == IT::IDTYPE::L)  str += "\npush offset " + string(e->FullName) + "\ncall print_str\n";
					else  str += "push " + string(e->FullName) + "\ncall print_str\n";
					break;
				}
				break;
			}

			}

			if (!str.empty())
			{
				*file << str << endl;
				str.clear();
			}
		}
	}

	string GenEqualCode(LT::LexTable& lextable, IT::IdTable& idtable, int& i)
	{
		string str;
		IT::Entry* e1 = IT_ENTRY(i - 1); // левый операнд
		i++;
		switch (e1->iddatatype)
		{
		case IT::IDDATATYPE::INT:
		{
			for (; LT_ENTRY(i)->lexema != LEX_SEMICOLON; i++)
			{
				switch (LT_ENTRY(i)->lexema)
				{
				case LEX_LITERAL:
				case LEX_ID:
				{
					if (IT_ENTRY(i)->idtype == IT::IDTYPE::F) // если в выражении вызов функции
					{
						str = str + GenCallFuncCode(lextable, idtable, i); // функци€ возвращает результат в eax
						str = str + "push eax\n";				// результат выражени€ в стек дл€ дальнейшего вычислени€ выражени€
						break;
					}
					else  
					str = str + "push " + IT_ENTRY(i)->FullName + "\n";
					break;
				}
				case LEX_OPERATOR:
					switch (LT_ENTRY(i)->sign)
					{
					case '+':
						str += "pop ebx\npop eax\nadd eax, ebx\npush eax\n"; break;
					case '-':
						str += "pop ebx\npop eax\nsub eax, ebx\npush eax\n"; break;
					case '*':
						str += "pop ebx\npop eax\nimul eax, ebx\npush eax\n"; break;
					case '/':
						str += "pop ebx\npop eax\ncdq\nidiv ebx\npush eax\n"; break;
					case '%':
						str += "pop ebx\npop eax\ncdq\nidiv ebx\npush edx\n"; break;
					}
				}
			}

			str += "pop " + string(e1->FullName) + '\n';
			break;
		}
		case IT::IDDATATYPE::STR:// разрешить присваивать строкам только строки, литералы и вызовы функций
		{
			char lex = LT_ENTRY(i)->lexema;
			IT::Entry* e2 = IT_ENTRY(i);
			if (lex == LEX_ID && (e2->idtype == IT::IDTYPE::F)) // вызов функции
			{
				str += GenCallFuncCode(lextable, idtable, i);
				str += "mov " + string(e1->FullName) + ", eax";
			}
			else if (lex == LEX_LITERAL) // литерал
			{
				str =+ "mov " + string(e1->FullName) + ", offset " + string(e2->FullName);
			}
			else // ид(переменна€) - через регистр
			{
				str += "mov ecx, " + string(e2->FullName) + "\nmov " + string(e1->FullName) + ", ecx";
			}
		}
		}
		return str;
	}

	string GenFunctionCode(LT::LexTable& lextable, IT::IdTable& idtable, int& i)
	{
		string str;

		str += SEPSTR(IT_ENTRY(i + 1)->FullName) + string(IT_ENTRY(i + 1)->FullName) + string(" PROC,\t");
		//дальше параметры
		i += 3; // начало - то что сразу после открывающей скобки
		while (LT_ENTRY(i)->lexema != LEX_RIGHTHESIS) // пока параметры не кончатс€
		{
			if (LT_ENTRY(i)->lexema == LEX_ID) // параметр
				str += string(IT_ENTRY(i)->FullName) + (IT_ENTRY(i)->iddatatype == IT::IDDATATYPE::INT ? " : sdword, " : " : dword, ");
			i++;
		}
		int f = str.rfind(',');
		if (f > 0)
			str[f] = ' ';

		str += "\n; --- сохранить регистры ---\npush ebx\npush edx\n; ----------------------";

		return str;
	}

	string GenExitCode(LT::LexTable& lextable, IT::IdTable& idtable, int& i, string funcname)
	{
		string str = "; --- восстановить регистры ---\npop edx\npop ebx\n; -------------------------\n";
		if (LT_ENTRY(i + 1)->lexema != LEX_SEMICOLON)	// выход из функции (вернуть значение)
		{
			str += "mov eax, " + string(IT_ENTRY(i + 1)->FullName) + "\n";
		}
		str += "ret\n";
		str += funcname + " ENDP" + SEPSTREMP;
		return str;
	}

	string GenCallFuncCode(LT::LexTable& lextable, IT::IdTable& idtable, int& i)
	{
		string str;

		IT::Entry* e = IT_ENTRY(i); // идентификатор вызываемой функции
		stack <IT::Entry*> temp;

		for (i++; LT_ENTRY(i)->lexema != '@'; i++)
		{
			if (LT_ENTRY(i)->lexema == LEX_ID || LT_ENTRY(i)->lexema == LEX_LITERAL)
				temp.push(IT_ENTRY(i)); // // заполн€ем стек в пр€мом пор€дке	
		}

		// раскручиваем стек
		while (!temp.empty())
		{
			if (temp.top()->idtype == IT::IDTYPE::L && temp.top()->iddatatype == IT::IDDATATYPE::STR)
				str += "push offset " + string(temp.top()->FullName) + "\n";
			else   str += "push " + string(temp.top()->FullName) + "\n";
			temp.pop();
		}

		str += "call " + string(e->FullName) + '\n';
		i++;

		return str;
	}

	string GenBranchingCode(LT::LexTable& lextable, IT::IdTable& idtable, int& i, int branchingnNum)
	{
		string str;
		IT::Entry* lft = IT_ENTRY(i + 2); // левый операнд
		IT::Entry* rgt = IT_ENTRY(i + 4); // правый операнд
		bool f = false, t = false;
		string fstr, tstr;

		str += "mov edx, " + lft->FullName + "\ncmp edx, " + rgt->FullName + "\n";
		switch (LT_ENTRY(i + 3)->lexema)
		{
		case LEX_MORE:  tstr = "jg";  fstr = "jl";  break;
		case LEX_LESS:   tstr = "jl";  fstr = "jg";  break;
		}

		if (LT_ENTRY(i)->lexema != LEX_REPEAT)
		{
			for (int j = i + 6; LT_ENTRY(j - 2)->lexema != LEX_RIGHTBRACE; j++) // пропустили условие
			{
				if (LT_ENTRY(j)->lexema == LEX_THEN)
					t = true;
				if (LT_ENTRY(j)->lexema == LEX_ELSE)
					f = true;
			}
			if (t) str += "\n" + tstr + " true" + itoS(branchingnNum);
			if (f) str += "\n" + fstr + " false" + itoS(branchingnNum);
			if (!t || !f)  str = str + "\njmp next" + itoS(branchingnNum);
		}
		else
		{
			str += fstr + " cycle" + itoS(branchingnNum);
		}
		
		return str;
	}

	string itoS(int x)
	{
		stringstream r;  r << x;  return r.str();
	}
}


