#pragma once
#include "stdafx.h"

using namespace std;

void LexAnalysis(In::IN& in, LT::LexTable& lex, IT::IdTable& id)
{
	IT::adate.params.count = 0;
	IT::Add(id, &IT::adate);
	IT::atime.params.count = 0;
	IT::Add(id, &IT::atime);



	IT::IDDATATYPE	iddatatype = IT::IDDATATYPE::DEF;			// тип данных
	IT::IDTYPE	idtype = IT::IDTYPE::D;							// тип идентикатора
	IT::Entry* IT_ENTRY = NULL;

	char lexema[TI_STR_MAXSIZE];
	char IDname[TI_STR_MAXSIZE];								//переменная для запоминания имени идентификатора на случай записи значения в idtable
	int indexIdTable = 2;
	int currentRow = 1;
	int currentLex = 0;
	bool lexID = true, lexInt = false; int main = 0;
	stack<string> areaOfVisibility;
	areaOfVisibility.push(GLOBAL);


	int line = 1;
	string current_line = "";
	int count_word_in_line = 0;

	while (FindLexema(in, lexema))
	{
		currentLex++;
		lexID = true; lexInt = false;
		current_line = current_line + lexema + " ";
		count_word_in_line += 1;
		if (lexema == "|")
		{
			count_word_in_line = 0;
			current_line = "";
			line += 1;
		}
		switch (*lexema)
		{
		case LEX_RIGHTBRACE:
		case LEX_SEMICOLON:
		case LEX_COMMA:
		case LEX_LEFTBRACE:
		case LEX_LEFTHESIS:
		case LEX_RIGHTHESIS:
		case LEX_EQUAL:
		case LEX_MORE:
		case LEX_LESS:
		{
			LT::Add(lex, new LT::Entry(*lexema, currentRow, currentLex));
			lexID = false;
			if (*lexema == LEX_RIGHTBRACE)
				areaOfVisibility.pop();
			break;
		}
		case IN_CODE_VERTICAL_LINE:
			currentRow++;
			currentLex = 0;
			lexID = false;
			break;
		case 'd':
		{
			FST::FST fst_def = FST_DEF(lexema);
			if (FST::execute(fst_def))
			{
				LT::Add(lex, new LT::Entry(LEX_FUNCTION, currentRow, currentLex));
				idtype = IT::IDTYPE::F;
				lexID = false;
			}
			break;
		}
		case 'i':
		{
			FST::FST fst_int = FST_INT(lexema);
			if (FST::execute(fst_int))
			{
				LT::Add(lex, new LT::Entry(LEX_INT, currentRow, currentLex));
				iddatatype = IT::IDDATATYPE::INT;
				lexID = false;
			}
			if (lexID)
			{
				FST::FST fst_if = FST_IF(lexema);
				if (FST::execute(fst_if))
				{
					LT::Add(lex, new LT::Entry(LEX_IF, currentRow, currentLex));
					lexID = false;
				}
			}
			break;
		}
		case 'm':
		{
			FST::FST fst_main = FST_MAIN(lexema);
			if (FST::execute(fst_main))
			{
				main++;
				LT::Add(lex, new LT::Entry(LEX_MAIN, currentRow, currentLex));
				areaOfVisibility.push(lexema);
				lexID = false;
			}
			break;
		}
		case 'a':
		{
			FST::FST fst_param = FST_ARG(lexema);
			if (FST::execute(fst_param))
			{
				LT::Add(lex, new LT::Entry(LEX_PARAM, currentRow, currentLex));
				idtype = IT::IDTYPE::P;

				if (IT_ENTRY)
				{
					IT_ENTRY->params.count++;
					IT_ENTRY->params.types.push_back(iddatatype);
				}

				lexID = false;
			}
			break;
		}
		case 'r':
		{
			FST::FST fst_return = FST_RETURN(lexema);
			if (FST::execute(fst_return))
			{
				LT::Add(lex, new LT::Entry(LEX_RETURN, currentRow, currentLex));
				lexID = false;
			}
			if (lexID)
			{
				FST::FST fst_var = FST_REPEAT(lexema);
				if (FST::execute(fst_var))
				{
					char str[10];
					static int count = 0;
					sprintf(str, "%d", count++);
					LT::Add(lex, new LT::Entry(LEX_REPEAT, currentRow, currentLex));
					areaOfVisibility.push(strcat(lexema, str));
					lexID = false;
				}
			}
			break;
		}
		case 's':
		{
			if (string(lexema) == "str")
			{
				FST::FST fst_string = FST_STR(lexema);
				if (FST::execute(fst_string))
				{
					LT::Add(lex, new LT::Entry(LEX_STR, currentRow, currentLex));
					iddatatype = IT::IDDATATYPE::STR;
					lexID = false;
				}
			}
			else
			{
				goto default_mark;
			}
			break;
		}
		case 'v':
		{
			FST::FST fst_var = FST_VAR(lexema);
			if (FST::execute(fst_var))
			{
				LT::Add(lex, new LT::Entry(LEX_VAR, currentRow, currentLex));
				idtype = IT::IDTYPE::V;
				lexID = false;
			}
			break;
		}
		//обработка чисел в разных системах исчисления
		case 'h':
		{
			FST::FST fst_literal_i16 = FST_LITERAL_I16(lexema);		// целочисленный литерал в 16-система счисления
			if (FST::execute(fst_literal_i16))
			{
				lexID = false; lexInt = true;
				BaseToDecimal(lexema, 16);
			}
			break;
		}
		case 'o':
		{
			FST::FST fst_literal_i8 = FST_LITERAL_I8(lexema);		// целочисленный литерал в 8-система счисления
			if (FST::execute(fst_literal_i8))
			{
				lexID = false; lexInt = true;
				BaseToDecimal(lexema, 8);
			}
			break;
		}
		case 'b':
		{
			FST::FST fst_literal_i2 = FST_LITERAL_I2(lexema);		// целочисленный литерал в 2-система счисления
			if (FST::execute(fst_literal_i2))
			{
				lexID = false; lexInt = true;
				BaseToDecimal(lexema, 2);
			}
			break;
		}
		case 'p':
		{
			FST::FST fst_write = FST_WRITE(lexema);
			if (FST::execute(fst_write))
			{
				LT::Add(lex, new LT::Entry(LEX_PRINT, currentRow, currentLex));
				lexID = false;
			}
			break;
		}
		case 't':
		{
			FST::FST fst_write = FST_THEN(lexema);
			if (FST::execute(fst_write))
			{
				char str[10];
				static int count = 0;
				sprintf(str, "%d", count++);
				LT::Add(lex, new LT::Entry(LEX_THEN, currentRow, currentLex));
				areaOfVisibility.push(strcat(lexema, str));
				lexID = false;
			}
			break;
		}
		case 'e':
		{
			FST::FST fst_write = FST_ELSE(lexema);
			if (FST::execute(fst_write))
			{
				char str[10];
				static int count = 0;
				sprintf(str, "%d", count++);
				LT::Add(lex, new LT::Entry(LEX_ELSE, currentRow, currentLex));
				areaOfVisibility.push(strcat(lexema, str));
				lexID = false;
			}
			break;
		}
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		{
			FST::FST fst_literal_minus_i = FST_LITERAL_MINUS_I(lexema);		// отрицательный целочисленный литерал
			if (FST::execute(fst_literal_minus_i))
			{
				lexID = false; lexInt = true;
			}
			if (lexID)
			{
				FST::FST fst_operator = FST_OPERATOR(lexema);				// операторы
				if (FST::execute(fst_operator))
				{
					LT::Add(lex, new LT::Entry(LEX_OPERATOR, *lexema, currentRow, currentLex));
					lexID = false;
				}
			}
			break;
		}
		case IN_CODE_QUOTES: // строковые литералы
		{
			FST::FST fst_literal_s = FST_LITERAL_S(lexema);
			if (FST::execute(fst_literal_s))
			{
				lexID = false;
				int IsLiteralString = IT::IsLiteralString(id, lexema);	// возвращает -1 если нет такого литерала, иначе указываем не него
				if (IsLiteralString + 1)
					LT::Add(lex, new LT::Entry(LEX_LITERAL, currentRow, currentLex, IsLiteralString));
				else
				{
					LT::Add(lex, new LT::Entry(LEX_LITERAL, currentRow, currentLex, indexIdTable++));

					LITERAL_STR
						IT::Add(id, new IT::Entry(lex.current_size - 1, iddatatype, idtype, lexema));
					ID_RESET
				}
			}
			break;
		}
		default:
		{
			default_mark:
			FST::FST fst_literal_i = FST_LITERAL_I(lexema);		// целочисленные литералы
			if (FST::execute(fst_literal_i))
			{
				lexID = false; lexInt = true;
			}
			break;
		}
		}

		if (lexInt)
		{
			int IsLiteralInt = IT::IsLiteralInt(id, lexema, IDname);	// возвращает -1 если нет такого целочисленного литерала, иначе указываем не него
			if (IsLiteralInt + 1)
				LT::Add(lex, new LT::Entry(LEX_LITERAL, currentRow, currentLex, IsLiteralInt));
			else
			{
				LT::Add(lex, new LT::Entry(LEX_LITERAL, currentRow, currentLex, indexIdTable++));

				LITERAL_INT
					IT::Add(id, new IT::Entry(lex.current_size - 1, iddatatype, idtype, lexema));
				ID_RESET
			}
		}

		if (lexID)	// идентификатор
		{
			FST::FST fst_id = FST_ID(lexema);
			if (isdigit(lexema[0]))
			{
				printf_s(ERROR_TRACE, "LexAnalysis.cpp", "LexAnalysis", "lexem");
				throw ERROR_THROW_IN(135, currentRow, currentLex);
			}
			if (FST::execute(fst_id))
			{
				for (int i = 0; i < 256 ; i++) {
					IDname[i] = lexema[i];
					if (lexema[i] == '\0')
						break;
				}
					
				int isId = IT::IsId(id, lexema, areaOfVisibility); // возвращает -1 если нет в таблице идентификаторов
				if (isId + 1)
				{
					if (idtype != IT::IDTYPE::D)
					{
						if (!strcmp(id.table[isId]->areaOfVisibility, areaOfVisibility.top().c_str()))
						{
							printf_s(ERROR_TRACE, "LexAnalysis.cpp", "LexAnalysis", "area visible");
							throw ERROR_THROW_IN(131, currentRow, currentLex);
						}
						if (iddatatype == IT::IDDATATYPE::DEF)
						{
							printf_s(ERROR_TRACE, "LexAnalysis.cpp", "LexAnalysis", "type?");
							throw ERROR_THROW_IN(121, currentRow, currentLex);
						}

						LT::Add(lex, new LT::Entry(LEX_ID, currentRow, currentLex, indexIdTable++));
						IT::Add(id, new IT::Entry(lex.current_size - 1, areaOfVisibility.top(), lexema, iddatatype, idtype));

						ID_RESET
					}
					else
						LT::Add(lex, new LT::Entry(LEX_ID, currentRow, currentLex, isId));
				}
				else
				{
					if (iddatatype == IT::IDDATATYPE::DEF)
					{
						printf_s(ERROR_TRACE, "LexAnalysis.cpp", "LexAnalysis", "type?");
						throw ERROR_THROW_IN(121, currentRow, currentLex);
					}
					if (idtype == IT::IDTYPE::D)
					{
						printf_s(ERROR_TRACE, "LexAnalysis.cpp", "LexAnalysis", "D type");
						throw ERROR_THROW_IN(132, currentRow, currentLex);
					}
					LT::Add(lex, new LT::Entry(LEX_ID, currentRow, currentLex, indexIdTable++));
					IT::Add(id, new IT::Entry(lex.current_size - 1, areaOfVisibility.top(), lexema, iddatatype, idtype));
					
					if (idtype == IT::IDTYPE::F)
					{
						areaOfVisibility.push(lexema);
						IT_ENTRY = id.table[id.current_size - 1];
					}

					ID_RESET
				}
			}
			else
			{
				printf_s(ERROR_TRACE, "LexAnalysis.cpp", "LexAnalysis", "lexem");
				stringstream ss(current_line);
				string word;
				int split_counter = 0;
				cout << "line: " + current_line + "-> ";
				while (ss >> word) {
					if (split_counter == count_word_in_line)
					{
						cout << ">>>" + word + "<<<(error lexema)";
					}
					else
					{
						cout << word;
					}
				}
				cout << endl;
				throw ERROR_THROW_IN(120, currentRow, currentLex);
			}
		}
	}
	if (main == 0)
	{
		printf_s(ERROR_TRACE, "LexAnalysis.cpp", "LexAnalysis", "main == 0");
		throw ERROR_THROW(133);
	}
	if (main > 1)
	{
		printf_s(ERROR_TRACE, "LexAnalysis.cpp", "LexAnalysis", "main > 1");
		throw ERROR_THROW(134);
	}
	LT::Add(lex, new LT::Entry('$', currentRow, currentLex));
}

bool FindLexema(In::IN& in, char* lexema)
{
	bool s_literal = false;
	static int i = 0; int indexLexema = 0;
	for (; i < in.size && in.text[i] != IN_CODE_SPACE || s_literal; i++)
	{
		if (indexLexema >= TI_STR_MAXSIZE - 1)
			throw ERROR_THROW(125)

			lexema[indexLexema++] = in.text[i];
		if (in.text[i] == IN_CODE_QUOTES)
			s_literal = !s_literal;
	}
	lexema[indexLexema] = TI_STR_DEFAULT;
	i++;

	return indexLexema;
}

void BaseToDecimal(char* lexema, int base)
{
	int number = 0;
	int k;
	for (int i = 1; lexema[i] != '\0'; i++)
	{
		if (lexema[i] <= '9' && lexema[i] >= '0') k = lexema[i] - '0';
		else if (lexema[i] >= 'A' && lexema[i] <= 'F') k = lexema[i] - 'A' + 10;
		else if (lexema[i] >= 'a' && lexema[i] <= 'f') k = lexema[i] - 'a' + 10;
		else continue;
		number = base * number + k;
	}
	sprintf(lexema, "%d", number);
}
