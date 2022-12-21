#pragma once
#include "stdafx.h"

using namespace std;

namespace IT
{
	IdTable::IdTable(int size)
	{
		if (size > TI_MAXSIZE)
		{
			printf_s(ERROR_TRACE, "IT.cpp", "IdTable", "size id");
			throw ERROR_THROW(127);
		}
		maxsize = TI_MAXSIZE;
		current_size = 0;
		table = new Entry * [size];
	}

	//функция переменная, параметр
	Entry::Entry(int idxfirstLE, string areaOfVisibility, const char* id, IDDATATYPE iddatatype, IDTYPE idtype)
	{
		this->idxfirstLE = idxfirstLE;
		strncpy(this->areaOfVisibility, areaOfVisibility.c_str(), ID_MAXSIZE - 1);
		strncpy(this->id, id, ID_MAXSIZE - 1);
		this->iddatatype = iddatatype;
		this->idtype = idtype;
		if (iddatatype == IT::IDDATATYPE::INT)
			this->value.vint = TI_INT_DEFAULT;
		else
		{
			this->value.vstr.len = 0;
			this->value.vstr.str[0] = TI_STR_DEFAULT;
		}
		this->params.count = 0;
		if(this->idtype == IT::IDTYPE::V || this->idtype == IT::IDTYPE::P)
			FullName = string(this->id) + '_' + string(this->areaOfVisibility);
		else
			FullName = string(this->id);
	}

	Entry::Entry(int idxfirstLE, IDDATATYPE iddatatype, IDTYPE idtype, char* value)				//литерал
	{
		char str[10];
		static int count = 0;
		sprintf(str, "%d", count++);
		strcpy(this->id, "L");
		strcat(this->id, str);
		strcpy(this->areaOfVisibility, GLOBAL);
		this->idxfirstLE = idxfirstLE;
		this->iddatatype = iddatatype;
		this->idtype = idtype;
		if (iddatatype == IT::IDDATATYPE::INT)
			this->value.vint = atoi(value);
		else
		{
			this->value.vstr.len = strlen(value);
			strncpy(this->value.vstr.str, value, TI_STR_MAXSIZE - 1);
			this->value.vstr.str[TI_STR_MAXSIZE - 1] = TI_STR_DEFAULT;
		}
		FullName = string(this->id);
	}

	void Add(IdTable& idtable, Entry* entry)
	{
		if (idtable.current_size < idtable.maxsize)
		{
			idtable.table[idtable.current_size] = entry;
			idtable.current_size++;
		}
		else
		{
			printf_s(ERROR_TRACE, "IT.cpp", "Add", "no id");
			throw ERROR_THROW(128);
		}
	}

	Entry GetEntry(IdTable& idtable, int n)
	{
		if (n > idtable.current_size)
		{
			printf_s(ERROR_TRACE, "IT.cpp", "GetEntry", "no id");
			throw ERROR_THROW(129);
		}

		return *idtable.table[n];
	}

	int IsId(IdTable& idtable, char id[ID_MAXSIZE], stack<string> areaOfVisibility)
	{
		int size = areaOfVisibility.size();
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < idtable.current_size; j++)
			{
				if (!strcmp(idtable.table[j]->areaOfVisibility, areaOfVisibility.top().c_str()))
					if (!strcmp(idtable.table[j]->id, id))
						return j;
			}
			areaOfVisibility.pop();
		}
		return -1;
	}

	int IsLiteralInt(IdTable& idtable, char* lexema, char* IDname)
	{
		int number = atoi(lexema);
		for (int i = 0; i < idtable.current_size; i++)
		{
			if (strcmp(idtable.table[i]->id, IDname) == 0 && idtable.table[i]->value.vint == 0)
				idtable.table[i]->SetVint(number);
			if (idtable.table[i]->idtype == IT::IDTYPE::L && idtable.table[i]->iddatatype == IT::IDDATATYPE::INT) {
				if (idtable.table[i]->value.vint == number)
					return i;
			}
		}
		return -1;
	}

	int IsLiteralString(IdTable& idtable, char* lexema)
	{
		for (int i = 0; i < idtable.current_size; i++)
		{
			if (idtable.table[i]->idtype == IT::IDTYPE::L && idtable.table[i]->iddatatype == IT::IDDATATYPE::STR)
				if (!strcmp(idtable.table[i]->value.vstr.str, lexema))
					return i;
		}
		return -1;
	}

	void Delete(IdTable& idtable)
	{
		for (int i = 2; i < idtable.current_size; i++)
		{
			delete idtable.table[i];
		}
		delete[] idtable.table;
	}

	void PrintIdTable(IdTable& idtable, const wchar_t* in)
	{
		wchar_t* id = new wchar_t[wcslen(in) + wcslen(PARM_ID_DEFAULT_EXT) + 1]{};
		wcscat(id, in);
		wcscat(id, PARM_ID_DEFAULT_EXT);

		ofstream idStream(id);
		delete[] id;
		if (!idStream.is_open())
		{
			printf_s(ERROR_TRACE, "IT.cpp", "PrintIdTable", "size lexem");
			throw ERROR_THROW(125);
		}

		idStream << "ПЕРЕМЕННЫЕ" << endl;
		idStream << "Индекс LE\t" << "Область видимости\t" << "Идентификатор\t" << "Тип Идентификатора\t" << "Тип данных\t" << "Значение\t" << "Длина строки\n";
		for (int i = 0; i < idtable.current_size; i++)
		{
			if (idtable.table[i]->idtype == IT::IDTYPE::V)
				if (idtable.table[i]->iddatatype == IT::IDDATATYPE::INT)
					idStream << setw(9) << idtable.table[i]->idxfirstLE << '\t' << setw(17) << idtable.table[i]->areaOfVisibility<< '\t' << setw(13) << idtable.table[i]->id << '\t' << setw(18) << "V" << '\t' << setw(10) << "INT" << '\t' << setw(8) << idtable.table[i]->value.vint << '\t' << setw(12) << "-" << endl;
				else
					idStream << setw(9) << idtable.table[i]->idxfirstLE << '\t' << setw(17) << idtable.table[i]->areaOfVisibility << '\t' << setw(13) << idtable.table[i]->id << '\t' << setw(18) << "V" << '\t' << setw(10) << "STR" << '\t' << setw(8) << idtable.table[i]->value.vstr.str << '\t' << setw(12) << (int)idtable.table[i]->value.vstr.len << endl;

			if (idtable.table[i]->idtype == IT::IDTYPE::P)
				if (idtable.table[i]->iddatatype == IT::IDDATATYPE::INT)
					idStream << setw(9) << idtable.table[i]->idxfirstLE << '\t' << setw(17) << idtable.table[i]->areaOfVisibility << '\t' << setw(13) << idtable.table[i]->id << '\t' << setw(18) << "P" << '\t' << setw(10) << "INT" << '\t' << setw(8) << idtable.table[i]->value.vint << '\t' << setw(12) << "-" << endl;
				else
					idStream << setw(9) << idtable.table[i]->idxfirstLE << '\t' << setw(17) << idtable.table[i]->areaOfVisibility << '\t' << setw(13) << idtable.table[i]->id << '\t' << setw(18) << "P" << '\t' << setw(10) << "STR" << '\t' << setw(8) << idtable.table[i]->value.vstr.str << '\t' << setw(12) << (int)idtable.table[i]->value.vstr.len << endl;

		}

		idStream << "\nФУНКЦИИ" << endl;
		idStream << "Индекс LE\t" << "Область видимости\t" << "Идентификатор\t" << "Тип данных возврата\n";
		for (int i = 0; i < idtable.current_size; i++)
		{
			if (idtable.table[i]->idtype == IT::IDTYPE::F)
				if (idtable.table[i]->iddatatype == IT::IDDATATYPE::INT)
				{
					idStream << setw(9) << idtable.table[i]->idxfirstLE << '\t' << setw(17) << idtable.table[i]->areaOfVisibility << '\t' << setw(13) << idtable.table[i]->id << '\t' << setw(19) << "INT" << endl;
				}
				else
				{
					idStream << setw(9) << idtable.table[i]->idxfirstLE << '\t' << setw(17) << idtable.table[i]->areaOfVisibility << '\t' << setw(13) << idtable.table[i]->id << '\t' << setw(19) << "STR" << endl;

				}
		}

		idStream << "\nЛИТЕРАЛЫ" << endl;
		idStream << "Индекс LE\t" << "Идентификатор\t" << "Тип данных\t" << setw(41) << "Значение\t" << "Длина строки\n";
		for (int i = 0; i < idtable.current_size; i++)
		{
			if (idtable.table[i]->idtype == IT::IDTYPE::L)
				if (idtable.table[i]->iddatatype == IT::IDDATATYPE::INT)
					idStream << setw(9) << idtable.table[i]->idxfirstLE << '\t' << setw(13) << idtable.table[i]->id << setw(11) << "INT" << '\t' << setw(40) << idtable.table[i]->value.vint << '\t' << setw(12) << "-" << endl;
				else
					idStream << setw(9) << idtable.table[i]->idxfirstLE << '\t' << setw(13) << idtable.table[i]->id << setw(11) << "STR" << '\t' << setw(40) << idtable.table[i]->value.vstr.str << '\t' << setw(12) << (int)idtable.table[i]->value.vstr.len << endl;
		}
	}
}