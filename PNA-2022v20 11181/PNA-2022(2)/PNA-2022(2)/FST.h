#pragma once
#include "stdafx.h"

#define MAX_LEN_CHAIN 100

namespace FST
{
	struct RELATION							// ребро :символ->вершина графа переходов КА
	{
		char symbol;						// символ перехода
		short nnode;						// номер смежной вершины
		RELATION(
			char с,							// символ перехода
			short ns						// новое состояние
		);
	};
	struct NODE								// вершина графа переходов
	{
		short n_relation;					// количество инциндентных ребер
		RELATION* relations;				// инцидентные ребра
		NODE();
		NODE(
			short n,						// количество инциндентных ребер
			RELATION rel, ...				// список ребер
		);
	};
	struct FST								// недетерминированный конечный автомат
	{
		const char* string;					//цепочка (строка, завершатся 0x00 ) 
		char lexema;						//лексема
		short position;						//текущая позиция в цепочке 
		short nstates;						//количество состояний автомата
		NODE* nodes;						//граф переходов: [0] - начальное состояние, [nstate-1] - конечное
		short* rstates;						//возможные состояния автомата на данной позиции.
		FST(
			const char* s,					//цепочка (строка, завершатся 0x00 ) 
			const char lex,					//лексема
			short ns,						//количество состояний автомата 
			NODE n, ...						//список состояний (граф переходов)
		);
	};

	bool step(FST& fst, short*& rstates);	// один шаг автомата

	bool execute(							// выполнить распознавание цепочки 
		FST& fst							//недетерминировнный конечный автомат
	);

};
