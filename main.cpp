#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <codecvt>
#include <locale>
#include "HtmlParser.h"

#include "./src/gumbo.h"


std::string utf8_to_gb2312(std::string const &strUtf8)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
	std::wstring wTemp = cutf8.from_bytes(strUtf8);
#ifdef _MSC_VER
	std::locale loc("zh-CN");
#else
	std::locale loc("zh_CN.GB18030");
#endif
	const wchar_t* pwszNext = nullptr;
	char* pszNext = nullptr;
	mbstate_t state = {};

	std::vector<char> buff(wTemp.size() * 2);
	int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t> >
		(loc).out(state,
			wTemp.data(), wTemp.data() + wTemp.size(), pwszNext,
			buff.data(), buff.data() + buff.size(), pszNext);

	if (std::codecvt_base::ok == res)
	{
		return std::string(buff.data(), pszNext);
	}
	return "";
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

	GumboNode* title_child = nullptr;
	GumboAttribute* attri = nullptr;
	char* input;
	fseek(fp,0,SEEK_END);
	int input_length = ftell(fp);
	fseek(fp,0,SEEK_SET);
	input = new char[input_length];
	fread(input,1,input_length,fp);

	CHtmlParser parser(input,input_length);
	std::vector<GumboNode*> title = parser.GetNodeByAttribute(nullptr, GUMBO_TAG_DIV/*, GUMBO_NODE_ELEMENT, std::string("class"), std::string("right zxgx")*/);

	for (unsigned int i=0;i<title.size();i++)
	{
		title_child = title[i];
		std::cout<<std::endl<<"[ Html node ] : "<<i<<std::endl;
		for (unsigned int i = 0; i < title_child->v.element.attributes.length; i++)
		{
			attri = (GumboAttribute*)title_child->v.element.attributes.data[i];
			std::cout << utf8_to_gb2312(attri->name) << ": " << utf8_to_gb2312(attri->value) << std::endl;
		}
	}

	delete[] input;

	return 0;
}
