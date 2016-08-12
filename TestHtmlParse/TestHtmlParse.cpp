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

	// GumboNode* title_child = nullptr;
	// GumboAttribute* attri = nullptr;
	char* input;
	fseek(fp, 0, SEEK_END);
	int input_length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	input = new char[input_length];
	fread(input, 1, input_length, fp);

	CHtmlParser parser(input, input_length);
	// std::vector<GumboNode*> title = parser.GetNodeByAttribute(nullptr, GUMBO_TAG_DIV/*, GUMBO_NODE_ELEMENT, std::string("class"), std::string("right zxgx")*/);

	// for (unsigned int i=0;i<title.size();i++)
	// {
	// 	title_child = title[i];
	// 	std::cout<<std::endl<<"[ Html node ] : "<<i<<std::endl;
	// 	for (unsigned int i = 0; i < title_child->v.element.attributes.length; i++)
	// 	{
	// 		attri = (GumboAttribute*)title_child->v.element.attributes.data[i];
	// 		std::cout << OutputConsole(attri->name) << ": " << OutputConsole(attri->value) << std::endl;
	// 	}
	// }
	auto find = parser.GetHtmlNavigation(GUMBO_TAG_SPAN, std::string("class"), std::string("cate-sub-title"));

	delete[] input;

	return 0;
}
