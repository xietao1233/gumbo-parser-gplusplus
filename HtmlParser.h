
#include "./src/gumbo.h"
#include <vector>
#include <string>
#include <map>

class CHtmlParser
{
public:
	CHtmlParser(char* html_content, size_t html_length/*UTF-8*/);
	~CHtmlParser();

	std::vector<GumboNode*> GetNodeByAttribute(GumboNode* root_node, unsigned int tag_type, unsigned int node_type = GUMBO_NODE_ELEMENT, std::string attribute_name = "", std::string attribute_value = "");
	std::map<std::string, std::string> GetHtmlNavigation(unsigned int tag_type, std::string attribute_name, std::string attribute_value/*, unsigned int node_type = GUMBO_NODE_ELEMENT*/);

private:
	GumboOutput* output;

	std::string htmlContent;
};
