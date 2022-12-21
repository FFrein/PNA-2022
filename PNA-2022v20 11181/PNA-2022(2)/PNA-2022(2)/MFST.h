#pragma once
#include "stdafx.h"

#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3
typedef std::stack<short>	MFSTSTSTACK;	// стек автомата

#define NS(n) GRB::Rule::Chain::N(n)
#define TS(n) GRB::Rule::Chain::T(n)
#define ISNS(n) GRB::Rule::Chain::isN(n)

#define MFST_TRACE_START(stream) *stream << std::setw( 4)<<std::left<<"Шаг"<<":" \
	<< std::setw(20) << std::left << "Правило"  \
	<< std::setw(30) << std::left << "Входная лента" \
	<< std::setw(20) << std::left << "Стек" \
	<< std::endl;

#define MFST_TRACE1(stream) *stream <<std::setw( 4)<<std::left<<++FST_TRACE_n<<":" \
	<< std::setw(20) << std::left << rule.getCRule(rbuf, currentRuleChain)  \
	<< std::setw(30) << std::left << getCLenta(lbuf, currentPosInLent) \
	<< std::setw(20) << std::left << getCSt(sbuf) \
	<< std::endl;

#define MFST_TRACE2(stream)    *stream <<std::setw( 4)<<std::left<<FST_TRACE_n<<":" \
	<< std::setw(20) << std::left << " "  \
	<< std::setw(30) << std::left << getCLenta(lbuf, currentPosInLent) \
	<< std::setw(20) << std::left << getCSt(sbuf) \
	<< std::endl;

#define MFST_TRACE3(stream)     *stream<<std::setw( 4)<<std::left<<++FST_TRACE_n<<":" \
	<< std::setw(20) << std::left << " "  \
	<< std::setw(30) << std::left << getCLenta(lbuf, currentPosInLent) \
	<< std::setw(20) << std::left << getCSt(sbuf) \
	<< std::endl;

#define MFST_TRACE4(stream, c) *stream<<std::setw(4)<<std::left<<++FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<std::endl;
#define MFST_TRACE5(stream, c) *stream<<std::setw(4)<<std::left<<  FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<std::endl;

#define MFST_TRACE6(stream,c,k) *stream<<std::setw(4)<<std::left<<++FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<k<<std::endl;

#define MFST_TRACE7(stream)  *stream<<std::setw(4)<<std::left<<state.posInLent<<": "\
	<< std::setw(20) << std::left << rule.getCRule(rbuf, state.nRuleChain) \
	<< std::endl;


namespace MFST
{
	struct MFSTState	// состояние автомата (для сохранения)
	{
		short posInLent;		// позиция на ленте
		short nRule;			// номер текущего правила
		short nRuleChain;		// номер текущей цепочки, текущего правила
		MFSTSTSTACK st;	// стек автомата

		MFSTState();
		MFSTState(short posInLent,		// позиция на ленте
			MFSTSTSTACK a_steck,		// стек автомата
			short currentChain);		// номер текущей цепочки, текущего правила

		MFSTState(short posInLent,		// позиция на ленте
			MFSTSTSTACK a_stack, 		// стек автомата
			short currentRule, 			// номер текущего правила
			short currentChain);		// номер текущей цепочки, текущего правила

	};

	struct MFST // магазинный автомат
	{
		enum RC_STEP //код возврата функции step
		{
			NS_OK,			// найдено правило и цепочка, цепочка записана в стек 
			NS_NORULE,		// не найдено правило грамматики (ошибка в грамматике)
			NS_NORULECHAIN,	// не найдена походящая цепочка правила (ошибка в исходном коде)
			NS_ERROR,		// неизвесный нетерминальный символ грамматики
			TS_OK,			// тек. символ ленты == вершине стека, продвинулась лента, pop стека
			TS_NOK,			// тек. символ ленты != вершине стека, восстановленно состояние 
			LENTA_END,		// теущая позиция ленты >= lenta_size 
			SURPRISE		// неожиданный код возврата (ошибка в step)
		};

		struct MFST_Diagnosis	// диагностика
		{
			short	posInLent;		// позиция на ленте 
			RC_STEP	rc_step;			// код завершения шага 
			short	ruleNum;			// номер правила 
			short	nrule_chain;		// номер цепочки правила
			MFST_Diagnosis();
			MFST_Diagnosis(short posInLent, RC_STEP rc_step, short ruleNum, short ruleChainNum);
		} diagnosis[MFST_DIAGN_NUMBER]; // последние самые глубокие сообщения

		GRBALPHABET* lenta;					// перекодированная (TS/NS) лента (из LEX)
		short currentPosInLent;				// текущая позиция на ленте
		short currentRule;					// номер текущего правила
		short currentRuleChain;				// номер текущей цепочки, текущего правила
		short lenta_size;					// размер ленты
		GRB::Greibach grebach;				// грамматика Грейбах
		LT::LexTable lexTable;						
		MFSTSTSTACK st;						// стек автомата
		std::stack<MFSTState> storestate;	// стек для сохранения состояний

		MFST();
		MFST(const LT::LexTable& lexTable,GRB::Greibach grebach);

		char* getCSt(char* buf);								//получить содержиое стека
		char* getCLenta(char* buf, short pos, short n = 25);	//лента: n символов, начиная с pos
		char* getDiagnosis(short n, char* buf, int& error_id_GRB);					//получить n-ую строку диагностики или '\0'
		
		bool savestate(std::ostream* stream);	//сохранить состояние автомата
		bool resetstate(std::ostream* stream);	//восстановить состояние автомата
		bool push_chain(GRB::Rule::Chain chain);

		RC_STEP step(std::ostream* stream);		//выполнить шаг автомата
		bool start(std::ostream* stream);		//запустить автомат
		bool savedDiagnosis(RC_STEP prc_step);

		void printRules(std::ostream* stream);	//вывести последовательность правил

		struct Deducation		// вывод
		{
			short stepsCount;			// количество шагов в выводе
			short* nRules;				// номер правила грамматики
			short* nChainsOfRules;		// номер цепочек правил грамматики

			Deducation()
			{
				this->stepsCount = 0;
				this->nRules = 0;
				this->nChainsOfRules = 0;
			}
		}deducation;

		bool saveoutputTree();			// сохранить дерево вывода
	};
}
			