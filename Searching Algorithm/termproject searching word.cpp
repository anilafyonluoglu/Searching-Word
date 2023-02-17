#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <memory>
#include <vector>
#include <cmath>
#include <list>
#include <stdio.h>
#include <ctype.h>



using namespace std;
class Word
{
private:
	std::string key;
	int count;
public:
	Word();
	Word(std::string w, int c);
	std::string getWord() const;
	void printWord();
	int getCount() const;
	void increment(int count);
};





Word::Word() : key(""), count(0)
{}



Word::Word(string w, int c) : key(w), count(c)
{}



string Word::getWord() const
{
	return key;
}
void Word::printWord() {
	std::cout << key << " " << count << std::endl;
}



int Word::getCount() const
{
	return count;
}



void Word::increment(int count)
{
	this->count += count;
}
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
class HashTable {
	int cell;
	list<Word>* table;
	std::vector<Word> topTenFrequentWords;
public:
	HashTable(int W);

	bool isEmpty() const;

	void insertWord(string x, int count);



	unsigned long long hashFunction(string& x);

	bool searchTable(string key);



	void displayHash();





};



HashTable::HashTable(int W)
{
	table = new list <Word>[W];
	cell = W;
	return;
}



bool HashTable::isEmpty() const
{
	for (int i = 0; i < cell; i++)
	{
		if (!table[i].empty()) {
			return false;
		}
			
	}
	return true;
}
bool HashTable::searchTable(string x)
{
	int cell_ID;
	cell_ID = hashFunction(x);
	for (list < Word >::iterator i = table[cell_ID].begin();
		i != table[cell_ID].end(); i++)
	{
		if (i->getWord() == x)
		{
			return true;
		}
	}
	return false;
}


void HashTable::insertWord(string x, int count)
{
	Word presentWord(x, count);
	int cell_ID;
	cell_ID = hashFunction(x);
	if (searchTable(x))
	{
		for (list < Word >::iterator i = table[cell_ID].begin();
			i != table[cell_ID].end(); i++)
			if (i->getWord() == x)
			{
				i->increment(count);
			}
	}
	else
	{
		table[cell_ID].push_back(presentWord);
	}
}
unsigned long long HashTable::hashFunction(std::string& word) {
	const int p = 31;
	const int m = 1e9 + 9;
	long long hashValue = 0;
	long long p_pow = 1;
	for (char c : word) {
		if (c >= 'a' && c <= 'z') {
			hashValue = (hashValue + (c - 'a' + 1) * p_pow) % m;
		}
		else if (c >= 'A' && c <= 'Z') {
			hashValue = (hashValue + (c - 'A' + 1) * p_pow) % m;
		}



		p_pow = (p_pow * p) % m;
	}
	return hashValue % cell;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


void HashTable::displayHash()
{


	for (int i = 0; i < 10; i++)
	{
		list<Word>::iterator itr;
		Word mostFrequentWord;
		int mostFrequent = 0;
		int FirstIndexOfTheBiggest;

		for (int j = 0; j < cell; j++)
		{
			if (!table[j].empty())
			{
				for (list < Word >::iterator i = table[j].begin();
					i != table[j].end(); i++)
				{

					if (i->getCount() > mostFrequent)
					{
						mostFrequent = i->getCount();
						mostFrequentWord = *i;
						FirstIndexOfTheBiggest = j;
						itr = i;
					}

				}
			}



		}
		topTenFrequentWords.push_back(mostFrequentWord);
		table[FirstIndexOfTheBiggest].erase(itr);

	}
	for (int i = 0; i < topTenFrequentWords.size(); i++)
	{
		std::cout << i+1 << "-"; topTenFrequentWords[i].printWord();
	}
}

bool isStopWord(std::string& word, HashTable& stopWordsTable) {

	if (stopWordsTable.searchTable(word)) {
		return true;
	}
	else {
		return false;
	}

}
int convertStringToInteger(std::string s) {

	int result = 0;
	for (int i = 0; i < s.size(); ++i) {
		result += (int)((s[i] - '0') * (pow(10, (s.size() - (i + 1)))));
	}
	return result;



}
std::string convertUppercaseToLowercase(std::string str) {
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			str[i] += 32;
		}
	}
	return str;
}
void seperatewords(std::string& str, HashTable& dataStructure, HashTable& stopWordsTable)
{
	std::string word;
	int integerCount;
	std::string count;
	bool theCountFound = false;
	bool UnigramCountFound = false;
	bool dataBetweenQuotationFound = false;
	bool theWordFound = false;
	for (auto x : str)
	{

		if (UnigramCountFound)
		{

			if (!dataBetweenQuotationFound && x == '"') {

				dataBetweenQuotationFound = true;
			}
			else if (dataBetweenQuotationFound && ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z')))
			{

				theWordFound = true;

				word += x;

			}
			else if (dataBetweenQuotationFound && x == '"')
			{

				dataBetweenQuotationFound = false;
			}
			else if (theWordFound && !dataBetweenQuotationFound && (x == ':')) {

				theWordFound = false;
				theCountFound = true;
			}
			else if (theCountFound && (x >= 48 && x <= 57)) {

				count += x;
			}
			else if (theCountFound && (x < 48 || x > 57)) {

				theCountFound = false;
				integerCount = convertStringToInteger(count);


				word = convertUppercaseToLowercase(word);
				if (!isStopWord(word, stopWordsTable))
				{



					dataStructure.insertWord(word, integerCount);



				}

				word = "";
				count = "";
			}
		}
		else if (x == '"')
		{
			if (word == "unigramCount") {
				UnigramCountFound = true;
			}
			word = "";
		}
		else {
			word = word + x;
		}
	}
}
///////////////////////////////////////////////////////////////////////////
int main() {
	std::ifstream stopWordsFile;
	std::ifstream ourData;
	std::string document;
	HashTable dataStructure(350000);
	HashTable stopWords(1000);




	stopWordsFile.open("stopwords.txt");
	std::string presentWord;
	stopWordsFile >> presentWord;
	while (!stopWordsFile.eof()) {



		stopWords.insertWord(presentWord, 1);
		stopWordsFile >> presentWord;
	}

	stopWordsFile.close();



	ourData.open("PublicationsDataSet.txt");



	while (getline(ourData, document)) {
		seperatewords(document, dataStructure, stopWords);
	}

	dataStructure.displayHash();


	std::cout << (double(clock()) / 1000) << " Seconds" << std::endl;
	ourData.close();



	return 0;
}