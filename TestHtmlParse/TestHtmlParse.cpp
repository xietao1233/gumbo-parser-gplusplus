// TestHtmlParse.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\\HtmlParser\\gumbo.h"
#include <locale.h>
#include <iostream>
#include <string>
#include <fstream>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#include "HtmlParser.h"


std::string OutputConsole(const char* pUtf8)
{
	std::string result;
	size_t uSize = 0;
	wchar_t* pUnicode = NULL;
	char* pAnsi = NULL;

	uSize = MultiByteToWideChar(CP_UTF8, 0, pUtf8, -1, (wchar_t*)NULL, 0);
	if (uSize)
	{
		pUnicode = new wchar_t[uSize];

		MultiByteToWideChar(CP_UTF8, 0, pUtf8, -1, (wchar_t*)pUnicode, uSize);
		uSize = WideCharToMultiByte(CP_ACP, 0, pUnicode, -1, (char*)NULL, 0, NULL, nullptr);
		if (uSize)
		{
			pAnsi = new char[uSize];

			WideCharToMultiByte(CP_ACP, 0, pUnicode, -1, pAnsi, uSize, NULL, nullptr);
			result = pAnsi;

			delete[] pAnsi;
		}

		delete[] pUnicode;
	}

	return result;
}

int main(int argc, const char** argv) {
	if (argc != 2) {
		printf("Usage: get_title <html filename>.\n");
		exit(EXIT_FAILURE);
	}
	const char* filename = argv[1];

	FILE* fp = fopen(filename, "r");
	if (!fp) {
		printf("File %s not found!\n", filename);
		exit(EXIT_FAILURE);
	}

	std::ostringstream oos;
	HTMLNODE RootInfo = { GUMBO_TAG_DIV,"class","nav" };
	HTMLNODE FindInfo = { GUMBO_TAG_LI ,"","" };
	//HTMLNODE FindInfo = { GUMBO_TAG_SPAN ,"class","cate-sub-title" };
	char* input;
	fseek(fp, 0, SEEK_END);
	int input_length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	input = new char[input_length];
	fread(input, 1, input_length, fp);

	CHtmlParser parser(input, input_length);

	auto find = parser.GetHtmlNavigation(&RootInfo, &FindInfo);
	for each(auto& item in find)
	{
		oos << item.first << " : " << item.second << std::endl;
	}

	delete[] input;

	std::cout << oos.str().c_str() << std::endl;

	getchar();

	return 0;
}
