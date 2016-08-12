#include "HtmlParser.h"
#include "src/gumbo.h"
#include "string.h"
#include <string>
#include <vector>
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

std::map<std::string, std::string> CHtmlParser::GetHtmlNavigation(unsigned int tag_type, std::string attribute_name, std::string attribute_value/*, unsigned int node_type = GUMBO_NODE_ELEMENT*/)
{
	std::map<std::string, std::string> result;
	std::vector<GumboNode*> findNavi;
	GumboNode* childNode=0;
	std::string url,name;

	try
	{
		findNavi = GetNodeByAttribute(0, tag_type, GUMBO_NODE_ELEMENT, attribute_name, attribute_value);
		for (unsigned int i=0; i<findNavi.size(); i++)
		{
			if (findNavi[i] && findNavi[i]->v.element.children.length)
			{
				childNode=(GumboNode*)findNavi[i]->v.element.children.data[0];
				if (childNode)
				{

				}
			}
			//result.insert(std::make_pair())
		}
	}
	catch(std::exception &e)
	{
		throw e;
	}

	return result;
}
