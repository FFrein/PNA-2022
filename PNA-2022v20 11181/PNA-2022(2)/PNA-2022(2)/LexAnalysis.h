#pragma once
#include "stdafx.h"

#define ID_RESET		idtype = IT::IDTYPE::D; \
						iddatatype = IT::IDDATATYPE::DEF;

#define LITERAL_INT		iddatatype = IT::IDDATATYPE::INT; \
						idtype = IT::IDTYPE::L;

#define LITERAL_STR		iddatatype = IT::IDDATATYPE::STR; \
						idtype = IT::IDTYPE::L;


void LexAnalysis(In::IN& in, LT::LexTable& lex, IT::IdTable& id);
bool FindLexema(In::IN&, char*);
void BaseToDecimal(char*, int);