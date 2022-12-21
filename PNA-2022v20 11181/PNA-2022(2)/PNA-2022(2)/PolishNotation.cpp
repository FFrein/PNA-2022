#pragma once
#include "stdafx.h"

using namespace std;

namespace PN
{
	bool polishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		stack<LT::Entry*> stk;    //создаем стек для хранения временных операций
		queue<LT::Entry*> result;
		bool function = false; 
		int quantityParm = 0;
		int i = ++lextable_pos;

		for (; lextable.table[i]->lexema != LEX_SEMICOLON && (lextable.table[i]->lexema != LEX_RIGHTHESIS || !stk.empty()); i++)
		{
			switch (lextable.table[i]->lexema)
			{
			case LEX_ID:            //операнды
			case LEX_LITERAL:
				if (idtable.table[lextable.table[i]->idxTI]->idtype == IT::IDTYPE::F)
				{
					quantityParm = 0;
					function = true;
					result.push(lextable.table[i]);
					break;
				}
				if (function && !quantityParm)
					quantityParm++;

				result.push(lextable.table[i]);
				break;

			case LEX_OPERATOR:
				if (stk.empty() || stk.top()->lexema == LEX_LEFTHESIS)
					stk.push(lextable.table[i]);
				else
				{
					int prioritet = priority(lextable.table[i]->sign);
					if (priority(stk.top()->sign) >= prioritet)
					{
						result.push(stk.top());

						stk.pop();
					}
					stk.push(lextable.table[i]);
				}
				break;

			case LEX_LEFTHESIS:
				stk.push(lextable.table[i]);
				break;

			case LEX_RIGHTHESIS:
				while (stk.top()->lexema != LEX_LEFTHESIS)
				{
					result.push(stk.top());

					stk.pop();
				}
				stk.pop();

				if (function)
				{
					result.push(new LT::Entry('@'));
					result.push(new LT::Entry('0' + quantityParm));
					function = false;
				}
				break;

			case LEX_COMMA:
				if (function)
					quantityParm++;
				while (stk.top()->lexema != LEX_LEFTHESIS)
				{
					result.push(stk.top());

					stk.pop();
				}
				break;
			case LEX_MORE:
			case LEX_LESS:
				result.push(lextable.table[i]);
				break;
			}
			
		}
		while (!stk.empty())
		{
			result.push(stk.top());

			stk.pop();
		}
		for (int j = lextable_pos; j < i; j++)
		{
			if (!result.empty())
			{
				lextable.table[j] = result.front();
				lextable.table[j]->sn = lextable.table[j - 1]->sn;
				lextable.table[j]->tn = lextable.table[j - 1]->tn + 1;
				result.pop();
			}
			else
			{
				lextable.table[j] = new LT::Entry('@', lextable.table[j]->sn = lextable.table[j - 1]->sn, lextable.table[j]->tn = lextable.table[j - 1]->tn + 1);
			}
		}

		return true;
	}

	int priority(char v)
	{
		switch (v)
		{
		case '(':
		case ')': return 1;
		case ',': return 2;
		case '+':
		case '-': return 3;
		case '*':
		case '/': 
		case '%': return 4;
		}
	}

	int findExpression(LT::LexTable& lextable)
	{
		for (static int i = 0; i < lextable.current_size; i++)
		{
			if (lextable.table[i]->lexema == LEX_EQUAL)
				return ++i;
		}
		return 0;
	}
}