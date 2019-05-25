﻿#include "pch.h"
#include "dataList.h"

enum TokenType
{
	keyword,
	identifier,
	constant,
	string_literal,
	punctuator
};

enum NonTerType
{
	EXT_DEC_LIST,
	EXT_DEC,
	FUNC_DEF,
	TYPE_SPEC,
	ARGU_LIST,
	ARGUMENT,
	CODE_BLOCK,
	SENT_LIST,
	SENT,
	RTN_SENT,
	EXPR_SENT,
	DECLARE,
	SELECTION,
	ITERATION,
	EXPR,
	ASSI_EXP,
	ADD_EXP,
	MULT_EXP,
	FACTOR,
	EXP_LIST
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

class GramCat
{
public:
	GramCat()
	{

	}
	GramCat(bool isTeriminal_, int type_, int tokenIdex_)
	{
		isTeriminal = isTeriminal_;
		type = type_;
		tokenIdex = tokenIdex_;
	}
	bool isTeriminal;
	int type;
	int tokenIdex;
	vector<GramCat>data;
};

int loadXml(string xmlName, vector<Token>&inputBuffer, vector< TiXmlElement*>tokenList)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(xmlName.c_str()))
	{
		cerr << doc.ErrorDesc() << endl;
		exit(-1);
	}
	TiXmlElement* root = doc.FirstChildElement();
	if (root == NULL)
	{
		cerr << "Failed to load file: No root element." << endl;
		doc.Clear();
		exit(-1);
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
	vector<GramCat>anaStack;
	vector<int>statStack;

	loadXml(inXmlName, inputBuffer, xmlTokenList);
	inputBuffer.emplace_back(0xffff, string("#"), TokenType::punctuator, 0xffff, true);
	anaStack.emplace_back(true, TokenType::punctuator, inputBuffer.size() - 1);
	statStack.push_back(0);
	int tokenIndex = 0;

	//>>>>>>>>>>
	vector<string>compareList = { "int","void",",","{","}","(",")","+","-","*","/","if","else","while","return",";","=","#" };
	//>>>>>>>>>>
	while (true)
	{
		Token token;
		token = inputBuffer[tokenIndex];
		int Listi, Listj = -1;
		Listi = statStack.back();
		if (token.type == TokenType::identifier)
		{
			Listj = 0;
		}
		else if (token.type == TokenType::constant)
		{
			Listj = 1;
		}
		else if (token.type == TokenType::string_literal)
		{
			Listj = 2;
		}
		else
		{
			for (size_t i = 0; i < compareList.size(); i++)
			{
				if (token.value == compareList[i])
				{
					Listj = i + 2;
					break;
				}
			}
			if (Listj == -1)
			{
				cerr << "invalid token\n"
					<< "index" << tokenIndex << endl
					<< "line" << inputBuffer[tokenIndex].line << endl;
				exit(1);
			}
		}
		int actionValue = actionList[Listi][Listj];
		if (actionValue / 10000 == 2)//shift
		{
			statStack.push_back(actionValue % 10000);
			anaStack.emplace_back(true, token.type, tokenIndex);
		}
		else if (actionValue / 10000 == 1)//reduce
		{
			int reduceNumber = actionValue % 10000;
			int gramType, popNum;
			switch (reduceNumber)
			{
			case 0:{ }
			}
		}
		else if (actionValue / 10000 == 3)//acc
		{

		}
		else if (actionValue / 10000 == 4)//error
		{
			cerr << "invalid token\n"
				<< "index" << tokenIndex << endl
				<< "line" << inputBuffer[tokenIndex].line << endl;
		}
	}

}


int main()
{
	std::cout << "Hello World!\n";
	cout << gotoList[1][2];
}
