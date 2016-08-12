#include "stdafx.h"
#include "HtmlParser.h"
#include "../HtmlParser/gumbo.h"
#include "string.h"
#include <string>
#include <vector>
#include <codecvt>
#include <algorithm>
#include <iterator>
#include <map>


CHtmlParser::CHtmlParser(char* html_content, size_t html_length/*UTF-8*/)
: output(NULL)
, htmlContent(html_content)
{
	if (!htmlContent.empty())
	{
		output = gumbo_parse_with_options(&kGumboDefaultOptions, htmlContent.c_str(), htmlContent.length());
	}
}

CHtmlParser::~CHtmlParser()
{
	if (output != NULL)
	{
		gumbo_destroy_output(&kGumboDefaultOptions, output);
	}
}

std::vector<GumboNode*> CHtmlParser::GetNodeByAttribute(GumboNode* root_node, unsigned int tag_type, unsigned int node_type/* = GUMBO_NODE_ELEMENT*/, std::string attribute_name/* = ""*/, std::string attribute_value/* = ""*/)
{
	std::vector<GumboNode*> result, child_result;
	unsigned int index = 0;
	GumboNode* child_node = 0;
	GumboAttribute* current_attribute = 0;

	try
	{
		if (!root_node)
			root_node = output->root;

		//判断当前节点
		if (root_node->type == node_type && root_node->v.element.tag == tag_type)
		{
			if (!attribute_name.empty())		//判断属性名是否为null
			{
				for (unsigned int i = 0; i < root_node->v.element.attributes.length; i++)
				{
					current_attribute = (GumboAttribute*)root_node->v.element.attributes.data[i];
					if (!_stricmp(attribute_name.c_str(), current_attribute->name) && !_stricmp(attribute_value.c_str(), current_attribute->value))
						result.push_back(root_node);
				}
			}
			else
				result.push_back(root_node);
		}
		if (root_node->type == GUMBO_NODE_ELEMENT)
		{
			//递归循环调用子节点
			while (index < root_node->v.element.children.length)
			{
				child_node = (GumboNode*)root_node->v.element.children.data[index];
				child_result = GetNodeByAttribute(child_node, tag_type, node_type, attribute_name, attribute_value);
				if (child_result.size())
				{
					std::copy(child_result.begin(), child_result.end(), std::back_inserter(result));
				}
				index++;
			}
		}
	}
	catch (std::exception &e)
	{
		throw e;
	}

	return result;
}

std::map<std::string, std::string> CHtmlParser::GetHtmlNavigation(HTMLNODE* pRootInfo, HTMLNODE* pFindInfo)
{
	std::map<std::string, std::string> result, mapTemp;
	std::vector<GumboNode*> vecRootNode;

	try
	{
		if (pRootInfo)
		{
			vecRootNode = GetNodeByAttribute(0, pRootInfo->type, GUMBO_NODE_ELEMENT, pRootInfo->name, pRootInfo->value);
			for each(auto var in vecRootNode)
			{
				mapTemp = GetHtmlNavigationItem(var, pFindInfo);
				if (mapTemp.size())
				{
					std::for_each(mapTemp.cbegin(), mapTemp.cend(), [&result](std::map<std::string, std::string>::value_type val) {
						result.insert(val);
					});
				}
			}
		}
		else
		{
			result = GetHtmlNavigationItem(0, pFindInfo);
		}
	}
	catch(std::exception &e)
	{
		throw e;
	}

	return result;
}

std::map<std::string, std::string> CHtmlParser::GetHtmlNavigationItem(GumboNode* pRoot, HTMLNODE* pFindInfo)
{
	std::map<std::string, std::string> result;
	std::vector<GumboNode*> vecFindNavi;
	GumboNode *childNode = 0, *childText = 0;
	GumboAttribute* childAttri = 0;
	std::string url, name;

	try
	{
		vecFindNavi = GetNodeByAttribute(pRoot, pFindInfo->type, GUMBO_NODE_ELEMENT, pFindInfo->name, pFindInfo->value);
		for (unsigned int i = 0; i < vecFindNavi.size(); i++)
		{
			if (vecFindNavi[i] && vecFindNavi[i]->v.element.children.length)
			{
				for (unsigned int j = 0; j < vecFindNavi[i]->v.element.children.length; j++)
				{
					childNode = (GumboNode*)vecFindNavi[i]->v.element.children.data[j];
					if (childNode && childNode->type == GUMBO_NODE_ELEMENT && childNode->v.element.tag == GUMBO_TAG_A)
					{
						childText = (GumboNode*)childNode->v.element.children.data[0];
						name = utf8_to_gb2312(childText->v.text.text);
						for (unsigned int k = 0; k < childNode->v.element.attributes.length; k++)
						{
							childAttri = (GumboAttribute*)childNode->v.element.attributes.data[k];
							if (!_stricmp((const char*)childAttri->name, "href"))
							{
								url = utf8_to_gb2312(childAttri->value);
								result.insert(std::make_pair(name, url));
								break;
							}
						}
						break;
					}
				}
			}
		}
	}
	catch (std::exception &e)
	{
		throw e;
	}

	return result;
}

std::string CHtmlParser::utf8_to_gb2312(std::string const &strUtf8)
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