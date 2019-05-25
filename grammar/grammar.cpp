#include "pch.h"
#include "dataList.h"

enum TokenType
{
	keyword,
	identifier,
	constant,
	string_literal,
	punctuator
};

class Token
{
public:
	Token()
	{
		valid = true;
	}
	Token(int number_, string value_, int type_, int line_, bool valid_ = true)
	{
		number = number_;
		value = value_;
		type = type_;
		line = line_;
		valid = valid_;
	}

	int number;
	string value;
	int type;
	int line;
	bool valid;

};

int loadXml(string xmlName, vector<Token>&inputBuffer,vector< TiXmlElement*>tokenList)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(xmlName.c_str()))
	{
		cerr << doc.ErrorDesc() << endl;
		return -1;
	}
	TiXmlElement* root = doc.FirstChildElement();
	if (root == NULL)
	{
		cerr << "Failed to load file: No root element." << endl;
		doc.Clear();
		return -1;
	}
	root = root->NextSiblingElement()->FirstChildElement();
	for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		tokenList.push_back(elem);
		string buf;

		int number;
		string value;
		int type;
		int line;
		bool valid;
		TiXmlElement* attri = elem->FirstChildElement();
		TiXmlNode* attriValue = attri->FirstChild();
		sscanf(attriValue->ToText()->Value(), "%d", &number);

		attri = elem->NextSiblingElement();
		attriValue = attri->FirstChild();
		value = attriValue->ToText()->Value();

		attri = elem->NextSiblingElement();
		attriValue = attri->FirstChild();
		buf = attriValue->ToText()->Value();
		if (buf == string("keyword"))
		{
			type = TokenType::keyword;
		}
		else if (buf == string("identifier"))
		{
			type = TokenType::identifier;
		}
		else if (buf == string("constant"))
		{
			type = TokenType::constant;
		}
		else if (buf == string("string_literal"))
		{
			type = TokenType::string_literal;
		}
		else if (buf == string("punctuator"))
		{
			type = TokenType::punctuator;
		}

		attri = elem->NextSiblingElement();
		attriValue = attri->FirstChild();
		sscanf(attriValue->ToText()->Value(), "%d", &line);

		attri = elem->NextSiblingElement();
		attriValue = attri->FirstChild();
		buf = attriValue->ToText()->Value();
		if (buf == string("true"))
		{
			valid = true;
		}
		else if (buf == string("false"))
		{
			valid = false;
		}

		inputBuffer.emplace_back(number, value, type, line, valid);
	}

	return 0;
}

int lr(string inXmlName, string outXmlName, string outXmlNameAbas)
{
	vector<Token>inputBuffer;
	vector< TiXmlElement*>xmlTokenList;
	vector<Token>anaStack;
	vector<int>statStack;

	loadXml(inXmlName, inputBuffer, xmlTokenList);
	inputBuffer.emplace_back(0xffff, string("#"), TokenType::punctuator, 0xffff, true);
	anaStack.emplace_back(0xffff, string("#"), TokenType::punctuator, 0xffff, true);
	statStack.push_back(0);
	int tokenIndex=0;
	while (true)
	{
		Token token;
		token = inputBuffer[tokenIndex];
		int i, j;
		i = statStack.back();
		if (token.type == TokenType::identifier)
		{
			j = 0;
		}
		else if (token.type == TokenType::constant)
		{
			j = 1;
		}
		else if (token.type == TokenType::string_literal)
		{
			j = 2;
		}
		else
		{

		}
	}

}


int main()
{
	std::cout << "Hello World!\n";
	cout << gotoList[1][2];
}
