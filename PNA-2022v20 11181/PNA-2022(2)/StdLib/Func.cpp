#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#pragma warning(disable: 4996)

extern "C"
{
	int strlens(char* str)
	{
		return strlen(str);
	}

	int atois(char* str)
	{
		int n = 0;
		while (*str >= '0' && *str <= '9') {
			n *= 10;
			n += *str++;
			n -= '0';
		}
		return n;
	}

	int print_int(int p)
	{
		std::cout << p << std::endl;
		return 0;
	}

	int print_str(char* str)
	{
		setlocale(LC_ALL, "rus");
		std::cout << str << std::endl;
		return 0;
	}

	int adate()
	{
		time_t now = time(NULL);
		tm* ltm = localtime(&now);
		return (1900 + ltm->tm_year) + ((1 + ltm->tm_mon) * 10000) + (ltm->tm_mday * 1000000);
	}

	int atime()
	{
		time_t now = time(NULL);
		tm* ltm = localtime(&now);
		return (ltm->tm_hour) * 100 + 1 + ltm->tm_min;
	}

}