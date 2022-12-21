#pragma once
#include "stdafx.h"

namespace SM
{
	void semAnaliz(LT::LexTable& lextable, IT::IdTable& idtable);
	void CheckFuncType(LT::LexTable& lextable, IT::IdTable& idtable);
	void CheckDivByZero(LT::LexTable& lextable, IT::IdTable& idtable);
	void CheckCompareINT(LT::LexTable& lextable, IT::IdTable& idtable);
	void CheckEqual(LT::LexTable& lextable, IT::IdTable& idtable);
};