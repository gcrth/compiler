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
	GramCat(bool isTeriminal_, int type_)
	{
		isTeriminal = isTeriminal_;
		type = type_;
	}
	bool isTeriminal;
	bool isToken;
	int type;
	int tokenIdex;
	vector<GramCat>data;
};

int loadXml(string xmlName, vector<Token>&inputBuffer, vector< TiXmlElement>&tokenList)
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
	//root = root->NextSiblingElement();
	root = root->FirstChildElement();
	for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		tokenList.push_back(*elem);
		string buf;

		int number;
		string value;
		int type;
		int line;
		bool valid;
		TiXmlElement* attri = elem->FirstChildElement();
		TiXmlNode* attriValue = attri->FirstChild();
		sscanf(attriValue->ToText()->Value(), "%d", &number);

		attri = attri->NextSiblingElement();
		attriValue = attri->FirstChild();
		value = attriValue->ToText()->Value();

		attri = attri->NextSiblingElement();
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

		attri = attri->NextSiblingElement();
		attriValue = attri->FirstChild();
		sscanf(attriValue->ToText()->Value(), "%d", &line);

		attri = attri->NextSiblingElement();
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

int conTreeNorm(GramCat &gramRoot, vector< TiXmlElement>&xmlTokenList, TiXmlElement *xmlRoot)
{
	

	if (gramRoot.isTeriminal == true)
	{
		xmlRoot->LinkEndChild(new TiXmlElement(xmlTokenList[gramRoot.tokenIdex]));
	}
	else
	{
		TiXmlElement *xmlGram = new TiXmlElement("nonterminal");
		string typeName;
		switch (gramRoot.type)
		{
		case  NonTerType::EXT_DEC_LIST: {typeName = "EXT_DEC_LIST"; break; }
		case  NonTerType::EXT_DEC: {typeName = "EXT_DEC"; break; }
		case  NonTerType::FUNC_DEF: {typeName = "FUNC_DEF"; break; }
		case  NonTerType::TYPE_SPEC: {typeName = "TYPE_SPEC"; break; }
		case  NonTerType::ARGU_LIST: {typeName = "ARGU_LIST"; break; }
		case  NonTerType::ARGUMENT: {typeName = "ARGUMENT"; break; }
		case  NonTerType::CODE_BLOCK: {typeName = "CODE_BLOCK"; break; }
		case  NonTerType::SENT_LIST: {typeName = "SENT_LIST"; break; }
		case  NonTerType::SENT: {typeName = "SENT"; break; }
		case  NonTerType::RTN_SENT: {typeName = "RTN_SENT"; break; }
		case  NonTerType::EXPR_SENT: {typeName = "EXPR_SENT"; break; }
		case  NonTerType::DECLARE: {typeName = "DECLARE"; break; }
		case  NonTerType::SELECTION: {typeName = "SELECTION"; break; }
		case  NonTerType::ITERATION: {typeName = "ITERATION"; break; }
		case  NonTerType::EXPR: {typeName = "EXPR"; break; }
		case  NonTerType::ASSI_EXP: {typeName = "ASSI_EXP"; break; }
		case  NonTerType::ADD_EXP: {typeName = "ADD_EXP"; break; }
		case  NonTerType::MULT_EXP: {typeName = "MULT_EXP"; break; }
		case  NonTerType::FACTOR: {typeName = "FACTOR"; break; }
		case  NonTerType::EXP_LIST: {typeName = "EXP_LIST"; break; }
		}
		xmlGram->SetAttribute("type", typeName.c_str());
		xmlRoot->LinkEndChild(xmlGram);
		for (size_t i = 0; i < gramRoot.data.size(); i++)
		{
			conTreeNorm(gramRoot.data[i], xmlTokenList, xmlGram);
		}
	}
	return 0;
}

int conTreeAb(GramCat &gramRoot, vector< TiXmlElement>&xmlTokenList, vector<Token>&inputBuffer,TiXmlElement *xmlRoot)
{


	if (gramRoot.isTeriminal == true)
	{
		xmlRoot->LinkEndChild(new TiXmlElement(xmlTokenList[gramRoot.tokenIdex]));
	}
	else
	{
		TiXmlElement *xmlGram = new TiXmlElement("nonterminal");
		string typeName;
		switch (gramRoot.type)
		{
		case  NonTerType::EXT_DEC_LIST: {typeName = "EXT_DEC_LIST"; break; }
		case  NonTerType::EXT_DEC: {typeName = "EXT_DEC"; break; }
		case  NonTerType::FUNC_DEF: {typeName = "FUNC_DEF"; break; }
		case  NonTerType::TYPE_SPEC: {typeName = "TYPE_SPEC"; break; }
		case  NonTerType::ARGU_LIST: {typeName = "ARGU_LIST"; break; }
		case  NonTerType::ARGUMENT: {typeName = "ARGUMENT"; break; }
		case  NonTerType::CODE_BLOCK: {typeName = "CODE_BLOCK"; break; }
		case  NonTerType::SENT_LIST: {typeName = "SENT_LIST"; break; }
		case  NonTerType::SENT: {typeName = "SENT"; break; }
		case  NonTerType::RTN_SENT: {typeName = "RTN_SENT"; break; }
		case  NonTerType::EXPR_SENT: {typeName = "EXPR_SENT"; break; }
		case  NonTerType::DECLARE: {typeName = "DECLARE"; break; }
		case  NonTerType::SELECTION: {typeName = "SELECTION"; break; }
		case  NonTerType::ITERATION: {typeName = "ITERATION"; break; }
		case  NonTerType::EXPR: {typeName = "EXPR"; break; }
		case  NonTerType::ASSI_EXP: {typeName = "ASSI_EXP"; break; }
		case  NonTerType::ADD_EXP: {typeName = "ADD_EXP"; break; }
		case  NonTerType::MULT_EXP: {typeName = "MULT_EXP"; break; }
		case  NonTerType::FACTOR: {typeName = "FACTOR"; break; }
		case  NonTerType::EXP_LIST: {typeName = "EXP_LIST"; break; }
		}
		xmlGram->SetAttribute("type", typeName.c_str());
		if (gramRoot.tokenIdex != -1)
		{
			char buf[1000];
			xmlGram->SetAttribute("value", inputBuffer[gramRoot.tokenIdex].value.c_str());
			sprintf(buf, "%d", inputBuffer[gramRoot.tokenIdex].number);
			xmlGram->SetAttribute("number", buf);
			sprintf(buf, "%d", inputBuffer[gramRoot.tokenIdex].line);
			xmlGram->SetAttribute("line", buf);
		}
		xmlRoot->LinkEndChild(xmlGram);
		for (size_t i = 0; i < gramRoot.data.size(); i++)
		{
			conTreeAb(gramRoot.data[i], xmlTokenList, inputBuffer, xmlGram);
		}
	}
	return 0;
}

int saveToXmlNorm(string fileName, GramCat &gramRoot, vector<TiXmlElement>&xmlTokenList, string inXmlName)
{
	TiXmlDocument doc;
	TiXmlDeclaration *dec = new TiXmlDeclaration("1.0", "UTF-8", "");
	doc.LinkEndChild(dec);
	TiXmlElement *xmlRoot = new TiXmlElement(inXmlName.c_str());
	conTreeNorm(gramRoot, xmlTokenList, xmlRoot);
	doc.LinkEndChild(xmlRoot);
	doc.SaveFile(fileName.c_str());
	return 0;
}

int saveToXmlAb(string fileName, GramCat &gramRoot, vector<TiXmlElement>&xmlTokenList, vector<Token>&inputBuffer, string inXmlName)
{
	TiXmlDocument doc;
	TiXmlDeclaration *dec = new TiXmlDeclaration("1.0", "UTF-8", "");
	doc.LinkEndChild(dec);
	TiXmlElement *xmlRoot = new TiXmlElement(inXmlName.c_str());
	conTreeAb(gramRoot, xmlTokenList, inputBuffer, xmlRoot);
	doc.LinkEndChild(xmlRoot);
	doc.SaveFile(fileName.c_str());
	return 0;
}


int lr(string inXmlName, string outXmlName, string outXmlNameAbas)
{
	vector<Token>inputBuffer;
	vector< TiXmlElement>xmlTokenList;
	vector<GramCat>anaStack,anaStackAb;
	vector<int>statStack;
	GramCat gramTemp;

	loadXml(inXmlName, inputBuffer, xmlTokenList);
	inputBuffer.emplace_back(0xffff, string("#"), TokenType::punctuator, 0xffff, true);
	gramTemp.isTeriminal = true;
	gramTemp.tokenIdex = 0xffff;
	gramTemp.type = TokenType::punctuator;
	anaStack.push_back(gramTemp);
	anaStackAb.push_back(gramTemp);
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
					Listj = i + 3;
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
			gramTemp.isTeriminal = true;
			gramTemp.tokenIdex = tokenIndex;
			gramTemp.type = token.type;
			anaStack.push_back(gramTemp);
			anaStackAb.push_back(gramTemp);
			tokenIndex++;
		}
		else if (actionValue / 10000 == 1)//reduce
		{
			int reduceNumber = actionValue % 10000;
			int gramType, popNum;
			switch (reduceNumber)
			{
			case  0: {gramType = NonTerType::EXT_DEC_LIST; popNum = 1; break; }
			case  1: {gramType = NonTerType::EXT_DEC_LIST; popNum = 2; break; }
			case  2: {gramType = NonTerType::EXT_DEC; popNum = 1; break; }
			case  3: {gramType = NonTerType::EXT_DEC; popNum = 1; break; }
			case  4: {gramType = NonTerType::FUNC_DEF; popNum = 6; break; }
			case  5: {gramType = NonTerType::FUNC_DEF; popNum = 5; break; }
			case  6: {gramType = NonTerType::TYPE_SPEC; popNum = 1; break; }
			case  7: {gramType = NonTerType::TYPE_SPEC; popNum = 1; break; }
			case  8: {gramType = NonTerType::ARGU_LIST; popNum = 1; break; }
			case  9: {gramType = NonTerType::ARGU_LIST; popNum = 3; break; }
			case  10: {gramType = NonTerType::ARGUMENT; popNum = 2; break; }
			case  11: {gramType = NonTerType::CODE_BLOCK; popNum = 3; break; }
			case  12: {gramType = NonTerType::CODE_BLOCK; popNum = 2; break; }
			case  13: {gramType = NonTerType::SENT_LIST; popNum = 2; break; }
			case  14: {gramType = NonTerType::SENT_LIST; popNum = 1; break; }
			case  20: {gramType = NonTerType::SENT; popNum = 1; break; }
			case  21: {gramType = NonTerType::SENT; popNum = 1; break; }
			case  22: {gramType = NonTerType::SENT; popNum = 1; break; }
			case  23: {gramType = NonTerType::SENT; popNum = 1; break; }
			case  24: {gramType = NonTerType::SENT; popNum = 1; break; }
			case  25: {gramType = NonTerType::SENT; popNum = 1; break; }
			case  26: {gramType = NonTerType::RTN_SENT; popNum = 3; break; }
			case  27: {gramType = NonTerType::EXPR_SENT; popNum = 2; break; }
			case  28: {gramType = NonTerType::DECLARE; popNum = 3; break; }
			case  29: {gramType = NonTerType::SELECTION; popNum = 5; break; }
			case  30: {gramType = NonTerType::SELECTION; popNum = 7; break; }
			case  31: {gramType = NonTerType::ITERATION; popNum = 5; break; }
			case  32: {gramType = NonTerType::EXPR; popNum = 1; break; }
			case  33: {gramType = NonTerType::ASSI_EXP; popNum = 1; break; }
			case  34: {gramType = NonTerType::ASSI_EXP; popNum = 3; break; }
			case  35: {gramType = NonTerType::ADD_EXP; popNum = 1; break; }
			case  36: {gramType = NonTerType::ADD_EXP; popNum = 3; break; }
			case  37: {gramType = NonTerType::ADD_EXP; popNum = 3; break; }
			case  38: {gramType = NonTerType::MULT_EXP; popNum = 1; break; }
			case  39: {gramType = NonTerType::MULT_EXP; popNum = 3; break; }
			case  40: {gramType = NonTerType::FACTOR; popNum = 1; break; }
			case  41: {gramType = NonTerType::FACTOR; popNum = 1; break; }
			case  42: {gramType = NonTerType::FACTOR; popNum = 3; break; }
			case  43: {gramType = NonTerType::FACTOR; popNum = 1; break; }
			case  44: {gramType = NonTerType::FACTOR; popNum = 4; break; }
			case  45: {gramType = NonTerType::FACTOR; popNum = 3; break; }
			case  46: {gramType = NonTerType::EXP_LIST; popNum = 1; break; }
			case  47: {gramType = NonTerType::EXP_LIST; popNum = 3; break; }
			case  48: {gramType = NonTerType::MULT_EXP; popNum = 3; break; }
			default:
			{
				cerr << "invalid token\n"
					<< "index" << tokenIndex << endl
					<< "line" << inputBuffer[tokenIndex].line << endl;
				exit(1); 
			}
			}
			vector<GramCat>reduceList(anaStack.end()-popNum, anaStack.end());
			vector<GramCat>reduceListAb(anaStackAb.end() - popNum, anaStackAb.end());
			for (size_t i = 0; i < (size_t)popNum; i++)
			{
				statStack.pop_back();
				anaStack.pop_back();
				anaStackAb.pop_back();
			}
			GramCat gram(false,gramType);
			gram.tokenIdex = -1;
			gram.data = reduceList;
			anaStack.push_back(gram);

			if (popNum == 1)
			{
				anaStackAb.push_back(reduceListAb.back());
			}
			else if (reduceNumber == 34 || reduceNumber == 36 || reduceNumber == 37 || reduceNumber == 48 || reduceNumber == 39)
			{
				GramCat gram(false, gramType);
				gram.tokenIdex = reduceListAb[1].tokenIdex;
				gram.data.push_back(reduceListAb[0]);
				gram.data.push_back(reduceListAb[2]);
				anaStackAb.push_back(gram);
			}
			else if (gramType == 42)
			{
				GramCat gram(false, gramType);
				gram.tokenIdex = -1;
				gram.data.push_back(reduceListAb[1]);
				anaStackAb.push_back(gram);
			}
			else
			{
				GramCat gramAb(false, gramType);
				gramAb.tokenIdex = -1;
				gramAb.data = reduceListAb;
				anaStackAb.push_back(gramAb);
			}

			statStack.push_back(gotoList[statStack.back()][gramType]%10000);
		}
		else if (actionValue / 10000 == 3)//acc
		{

			break;
		}
		else if (actionValue / 10000 == 4)//error
		{
			cerr << "invalid token\n"
				<< "index" << tokenIndex << endl
				<< "line" << inputBuffer[tokenIndex].line << endl;
			exit(1);
		}
	}

	saveToXmlNorm(outXmlName, anaStack.back(), xmlTokenList, inXmlName);
	saveToXmlAb(outXmlNameAbas, anaStackAb.back(), xmlTokenList, inputBuffer, inXmlName);

	return 0;
}


int main()
{
	string in("out.xml"), out("tree.xml"),outab("treeab.xml");
	lr(in, out, outab);
	return 0;
}
