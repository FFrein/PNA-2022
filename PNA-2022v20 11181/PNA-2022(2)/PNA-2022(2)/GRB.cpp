#pragma once
#include "stdafx.h"

#define GRB_ERROR_SERIES 600

namespace GRB
{
	#define NS(n) Rule::Chain::N(n)
	#define TS(n) Rule::Chain::T(n)

	Greibach greibach(NS('S'), TS('$'), 11,
		Rule(NS('S'), GRB_ERROR_SERIES + 0, //Неверная структура программы
			3,	// m{N}S | tdi(F){NrU;}S | пустой переход
			Rule::Chain(5, TS('m'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(13, TS('t'), TS('d'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('U'), TS(';'), TS('}'), NS('S')),
			Rule::Chain()
		),
		Rule(NS('N'), GRB_ERROR_SERIES + 1, 	//ошибочный оператор
			6,	// tY;N |  i=E;N | p(W);N | ~K{N}N | ?KJN | пустой переход
			Rule::Chain(4, TS('t'), NS('Y'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(6, TS('p'), TS('('), NS('W'),  TS(')'), TS(';'), NS('N')),
			Rule::Chain(6, TS('~'), NS('K'), TS('{'), NS('N'), TS('}'), NS('N')),
			Rule::Chain(4, TS('?'), NS('K'), NS('J'), NS('N')),
			Rule::Chain()
		),
		Rule(NS('U'), GRB_ERROR_SERIES + 5, 	// Только литерал или идентификатор
			2,	// l | i
			Rule::Chain(1, TS('l')),
			Rule::Chain(1, TS('i'))
		),
		Rule(NS('J'), GRB_ERROR_SERIES + 6, 	// Ошибка при условном переходе
			2,	// :{N} | :{N}!{N}
			Rule::Chain(4, TS(':'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(8, TS(':'), TS('{'), NS('N'), TS('}'), TS('!'), TS('{'), NS('N'), TS('}'))
		),
		Rule(NS('Y'), GRB_ERROR_SERIES + 7, 	// Ошибка при определении переменной
			5,	// vi | vi=U | vi=i(W) | vi=U#U
			Rule::Chain(2, TS('v'), TS('i')),
			Rule::Chain(4, TS('v'), TS('i'), TS('='), NS('U')), // идентификатор равен U
			Rule::Chain(7, TS('v'), TS('i'), TS('='), TS('i'), TS('('), NS('W'), TS(')')), // идентификатор равен функция с параметрами
			Rule::Chain(6, TS('v'), TS('i'), TS('='), TS('i'), TS('('), TS(')')), // идентификатор равен функция без параметров
			Rule::Chain(6, TS('v'), TS('i'), TS('='), NS('U'), TS('#'), NS('U')) // идентификатор равен выражение
		),
		Rule(NS('K'), GRB_ERROR_SERIES + 8, 	// Ошибка при определении условия перехода
			2,	// (U<U) | (U>U)
			Rule::Chain(5, TS('('), NS('U'), TS('<'), NS('U'), TS(')')),
			Rule::Chain(5, TS('('), NS('U'), TS('>'), NS('U'), TS(')'))
		),
		Rule(NS('E'), GRB_ERROR_SERIES + 2,		// Ошибка в выражении
			4, // iM | lM | (E)M | i(W)M
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M'))
		),
		Rule(NS('M'), GRB_ERROR_SERIES + 2,		//	Ошибка в выражении
			2,	// #E | пустой переход
			Rule::Chain(2, TS('#'), NS('E')),
			Rule::Chain()
		),
		Rule(NS('F'), GRB_ERROR_SERIES + 3,		// Ошибка в параметрах при определении функции
			2, // tP | tP,F
			Rule::Chain(2, TS('t'), NS('P')),
			Rule::Chain(4, TS('t'), NS('P'), TS(','), NS('F'))
		),
		Rule(NS('P'), GRB_ERROR_SERIES + 3,		// Ошибка в параметрах при определении функции
			1, // ai
			Rule::Chain(2, TS('a'), TS('i'))
		),
		Rule(NS('W'), GRB_ERROR_SERIES + 4,		// Ошибка в параметрах при вызове функции
			4, // i | l | i,W | l,W | #l,W
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W')),
			Rule::Chain(2, TS('#'), TS('l'), TS(','), NS('W'))
		)
	);


	Rule::Chain::Chain(short size, GRBALPHABET s, ...)
	{
		nt = new GRBALPHABET[this->size = size];

		int* ptr = (int*)& s;
		for (short i = 0; i < size; i++)
			nt[i] = (GRBALPHABET)ptr[i];
	}

	Rule::Rule(GRBALPHABET nn, int iderror, short size, Chain c, ...)
	{
		this->nn = nn;
		this->iderror = iderror;
		this->chains = new Chain[this->size = size];

		Chain* ptr = &c;
		for (int i = 0; i < size; i++)
				this->chains[i] = ptr[i];
	}

	Greibach::Greibach(GRBALPHABET startN, GRBALPHABET stbottomT, short size, Rule r, ...)
	{
		this->startN = startN;
		this->stbottomT = stbottomT;
		this->rules = new Rule[this->size = size];
		Rule* ptr = &r;
		for (int i = 0; i < size; i++)
			rules[i] = ptr[i];
	}

	short Greibach::getRule(GRBALPHABET nn, Rule& rule)
	{
		short rc = -1, k = 0;

		while (k < size && rules[k].nn != nn)
			k++;

		if (k < size)
			rule = rules[rc = k];

		return rc;
	}

	Rule Greibach::getRule(short n)
	{
		Rule rc;
		if (n < size)
			rc = rules[n];
		return rc;
	}

	char* Rule::getCRule(char* b, short nchain)
	{
		char buf[200];
		b[0] = Chain::alphabet_to_char(nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0;
		chains[nchain].getCChain(buf);
		strcat_s(b, sizeof(buf) + 5, buf);

		return b;
	}

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)
	{
		short rc = -1;
			
		while (j < size && chains[j].nt[0] != t && chains[j].nt[0] != 0)
			j++;

		rc = (j < size ? j : -1);
		if (rc >= 0)
			pchain = chains[rc];
		return rc;
	}

	char* Rule::Chain::getCChain(char* b)
	{
		for (int i = 0; i < size; i++)
			b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	}

	Greibach getGreibach() { return greibach; }
}