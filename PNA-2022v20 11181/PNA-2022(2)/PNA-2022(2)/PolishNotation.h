#pragma once
#include "stdafx.h"

namespace PN
{
	int findExpression(LT::LexTable& lextable);
	bool polishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable);
	int priority(char v);
}