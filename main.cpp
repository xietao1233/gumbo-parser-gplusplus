// Copyright 2013 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Author: jdtang@google.com (Jonathan Tang)
//
// Retrieves the title of a page.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <codecvt>

#include "./src/gumbo.h"

void OutputConsole(const char* pUtf8)
{
	size_t uSize = 0;
	WCHAR* pUnicode = NULL; 
	char* pAnsi = NULL;

	uSize = MultiByteToWideChar(CP_UTF8, NULL, pUtf8, -1, NULL, NULL);
	if (uSize)
	{
		pUnicode = new WCHAR[uSize];

		MultiByteToWideChar(CP_UTF8, NULL, pUtf8, -1, pUnicode, uSize);
		uSize = WideCharToMultiByte(CP_ACP, NULL, pUnicode, -1, NULL, NULL, NULL, NULL);
		if (uSize)
		{
			pAnsi = new char[uSize];

			WideCharToMultiByte(CP_ACP, NULL, pUnicode, -1, pAnsi, uSize, NULL, NULL);
			printf("%s\n", pAnsi);

			delete[] pAnsi;
		}

		delete[] pUnicode;
	}
}

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

	FILE* fp;
	fopen_s(&fp, filename, "r");
	if (!fp) {
		printf("File %s not found!\n", filename);
		exit(EXIT_FAILURE);
	}

	char* input;
	int input_length;
	read_file(fp, &input, &input_length);
	GumboOutput* output = gumbo_parse_with_options(
		&kGumboDefaultOptions, input, input_length);

	std::vector<GumboNode*> title = CHtmlParser::GetNodeByAttribute(output->root, GUMBO_TAG_DIV/*, GUMBO_NODE_ELEMENT, std::string("class"), std::string("right zxgx")*/);

	for each (auto var in title)
	{
		GumboNode* title_child = 0;

		for (unsigned int i = 0; i < var->v.element.attributes.length; i++)
		{
			GumboAttribute* attri = (GumboAttribute*)var->v.element.attributes.data[i];
			std::cout << utf8_to_gb2312(attri->name) << ": " << utf8_to_gb2312(attri->value) << std::endl;
		}
	}

	gumbo_destroy_output(&kGumboDefaultOptions, output);
	free(input);

	return 0;
}
