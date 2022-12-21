#pragma once
#include "stdafx.h"

#define LEXEMA_FIXSIZE	1			// фиксированный размер лексемы
#define	LT_MAXSIZE		4096		// максимальное количество строк в таблице лексем	
#define	LT_TI_NULLXDX	-1			// нет элемента таблицы идентификаторов	
#define	LT_V_NULL		'?'			// 


#define	LEX_INT			't'			// лексема для int
#define	LEX_STR			't'			// лексема для str
#define	LEX_ID			'i' 		// лексема для идентификатора
#define	LEX_LITERAL		'l'			// лексема для литерала
#define	LEX_FUNCTION	'd'			// лексема для def
#define	LEX_PARAM		'a'			// лексема для arg
#define	LEX_VAR			'v'			// лексема для var
#define	LEX_REPEAT		'~'			// лексема для repeat
#define	LEX_RETURN		'r'			// лексема для return
#define	LEX_PRINT		'p'			// лексема для print
#define	LEX_MAIN		'm'			// лексема для main
#define	LEX_SEMICOLON	';'			// лексема для ;
#define	LEX_COMMA		','			// лексема для ,
#define	LEX_LEFTBRACE	'{'			// лексема для {
#define	LEX_RIGHTBRACE	'}'			// лексема для }
#define	LEX_LEFTHESIS	'('			// лексема для (
#define	LEX_RIGHTHESIS	')'			// лексема для )
#define	LEX_OPERATOR	'#'			// лексема для операторов (+, -, *, /, %)
#define	LEX_EQUAL		'='			// лексема для =
#define	LEX_MORE		'>'			// лексема для >
#define	LEX_LESS		'<'			// лексема для <
#define	LEX_IF			'?' 		// лексема для if
#define LEX_THEN		':'			// лексема для then
#define LEX_ELSE		'!'			// лексема для else

#define PARM_LEX_DEFAULT_LOCATION	L"Files\\"	
#define PARM_LEX_DEFAULT_EXT		L".lex.txt" //для файла с итогом лексического анализa (таблица лексем)

namespace LT							// таблица лексем
{
	struct Entry						// строка таблицы лексем
	{
		char lexema;					// лексема
		char sign;						// знак лексемы v или LT_V_NULL
		int sn;							// номер строки в исходном тексте
		int tn;							// номер токена в строке
		int idxTI;						// индекс в таблице идентификаторов или LT_TI_NULLIDX

		Entry(char lexema, int sn, int tn, int idxTI);		// для идентификатор
		Entry(char lexema, char sign, int sn, int tn);		// для операторов
		Entry(char lexema, int sn, int tn);					// остальные лексемы
		Entry(char lexema);
	};

	struct LexTable						// экземпляр таблицы лексем
	{
		int maxsize;					// емкость таблицы лексем < LT_MAXSIZE
		int current_size;				// текущий размер таблицы лексем < maxsize
		Entry** table;					// массив указателей на строки таблицы лексем

		LexTable();
		LexTable(int size);			// создать таблицу лексем
	};
	void Add(LexTable& lextable, Entry* entry);						// добавление лексем
	Entry GetEntry(LexTable& lextable, int n);						// получить строку таблицы лексем
	void PrintLexTable(LexTable& lextable, const wchar_t* in);		// вывод лексем в файл
	void Delete(LexTable& lextable);								// удалить таблицу лексем (освободить память)
}