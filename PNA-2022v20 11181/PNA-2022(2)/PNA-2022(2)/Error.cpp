#pragma once
#include "stdafx.h"

namespace Error
{
	// серии ошибок:
	//					0 -  99 - системные ошибки
	//				  100 - 104 - ошибки входных параметров
	//				  105 - 109 - ошибки при открытии файла
	//				  110 - 119 - ошибки при чтении файла
	//				  120 - 140 - ошибки на этапе лексического анализатора
	//				  600 - 610 - ошибки на этапе синтаксического анализатора
	//				  700 - 720 - ошибки на этапе семантического анализатора
	ERROR errors[ERROR_MAX_ENTRY] = // таблица ошибок
	{
		ERROR_ENTRY(0, "Недопустимый код ошибки"),
		ERROR_ENTRY(1, "Системный сбой"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100, "Параметр -in должен быть задан"),
		ERROR_ENTRY(101, "Превышена длина входного параметра"),
		ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103), ERROR_ENTRY_NODEF(104),
		ERROR_ENTRY(105, "Ошибка при открытии файла с исходным кодом(-in)"),
		ERROR_ENTRY(106, "Ошибка при создании файла протокола (-log)"),
		ERROR_ENTRY(107, "Ошибка при создании файла протокола (-out)"),
		ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY(110, "Недопустимый символ в исходном файле (-in)"),
		ERROR_ENTRY(111, "Нет закрывающей кавычки (-in)"),
		ERROR_ENTRY_NODEF(112), ERROR_ENTRY_NODEF(113), ERROR_ENTRY_NODEF(114), ERROR_ENTRY_NODEF(115),
		ERROR_ENTRY_NODEF(116), ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),
		ERROR_ENTRY(120, "Нераспознанная лексема"),
		ERROR_ENTRY(121, "Тип данных идентификатора не определен"),
		ERROR_ENTRY(122, "Превышен размер таблицы лексем"),
		ERROR_ENTRY(123, "Таблица лексем переполнена"),
		ERROR_ENTRY(124, "Попытка обращения к незаполненной строке таблицы лексем"),
		ERROR_ENTRY(125, "Превышен размер лексемы"),
		ERROR_ENTRY(126, "Ошибка при открытии файла протокола (таблица лексем)"),
		ERROR_ENTRY(127, "Превышен размер таблицы идентификаторов"),
		ERROR_ENTRY(128, "Таблица идентификаторов переполнена"),
		ERROR_ENTRY(129, "Попытка обращения к незаполненной строке таблица идентификаторов"),
		ERROR_ENTRY(130, "Выход за пределы области видимости"),
		ERROR_ENTRY(131, "Переопределение"),
		ERROR_ENTRY(132, "Тип идентификатора не определен"),
		ERROR_ENTRY(133, "Отсутствует точка входа main"),
		ERROR_ENTRY(134, "Обнаружено несколько точек входа main"),
		ERROR_ENTRY(135, "Название переменной начинается с цифры"),
		ERROR_ENTRY_NODEF(136), ERROR_ENTRY_NODEF(137), ERROR_ENTRY_NODEF(138), ERROR_ENTRY_NODEF(139),
		ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY_NODEF100(200), ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600, "Неверная структура программы"),
		ERROR_ENTRY(601, "Ошибочный оператор"),
		ERROR_ENTRY(602, "Ошибка в выражении"),
		ERROR_ENTRY(603, "Ошибка в параметрах при определении функции"),
		ERROR_ENTRY(604, "Ошибка в параметрах при вызове функции"),
		ERROR_ENTRY(605, "Ошибка: только литерал или идентификатор"),
		ERROR_ENTRY(606, "Ошибка при условном переходе"),
		ERROR_ENTRY(607, "Ошибка при определении переменной"),
		ERROR_ENTRY(608, "Ошибка при определении условия перехода"),
		ERROR_ENTRY_NODEF(609),
		ERROR_ENTRY_NODEF10(610), ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630), ERROR_ENTRY_NODEF10(640), ERROR_ENTRY_NODEF10(650),
		ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670), ERROR_ENTRY_NODEF10(680), ERROR_ENTRY_NODEF10(690),
		ERROR_ENTRY(700, "Деление на 0"),
		ERROR_ENTRY(701, "Типы данных в выражении не совпадают"),
		ERROR_ENTRY(702, "Недопустимое строковое выражение справа от знака \'=\'"),
		ERROR_ENTRY(703, "Тип функции и возвращаемое значение не совпадают"),
		ERROR_ENTRY(704, "Несовпадение типов передаваемых параметров"),
		ERROR_ENTRY(705, "Кол-во ожидаемых функцией и передаваемых параметров не совпадают"),
		ERROR_ENTRY(706, "Неверное условное выражение"),
		ERROR_ENTRY_NODEF(707), ERROR_ENTRY_NODEF(708), ERROR_ENTRY_NODEF(709),
		ERROR_ENTRY_NODEF10(710), ERROR_ENTRY_NODEF10(720), ERROR_ENTRY_NODEF10(730), ERROR_ENTRY_NODEF10(740), ERROR_ENTRY_NODEF10(750),
		ERROR_ENTRY_NODEF10(760), ERROR_ENTRY_NODEF10(770), ERROR_ENTRY_NODEF10(780), ERROR_ENTRY_NODEF10(790),
		ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};

	ERROR geterror(int id)
	{
		if (id < 0 || id > ERROR_MAX_ENTRY)
			return errors[0];
		return errors[id];
	}

	ERROR geterrorin(int id, int line = -1, int col = -1)
	{
		if (id < 0 || id > ERROR_MAX_ENTRY)
		{
			return errors[0];
		}
		errors[id].inext.line = line;
		errors[id].inext.col = col;
		return errors[id];
	}
}