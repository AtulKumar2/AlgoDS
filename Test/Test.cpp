// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Common.h"
#include "Header.h"

#include <vector>
#include <list>
#include <map>

#include <istream>
#include <ostream>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <iterator>
#include <algorithm>
#include <functional> 

#include <locale>
#include <memory>

#include <ctime>
#include <ratio>
#include <chrono>

#include <random>
#include <limits>

#include <cctype>

///////////////////////////////////////////////////////////////////////////////
inline bool
is_power_of(unsigned int value, unsigned int base)
{
	if (base == 0 && value == 0)
		return true;
	if (base == 0 || value == 0)
		return false;

	auto comp = base;
	while (comp < value) {
		comp *= base;
	}
	return (comp == value);
}
inline bool
is_power_of(int value, int base)
{
	if (base == 0 && value == 0)
		return true;
	if (base == 0 || value == 0)
		return false;

	auto comp = base;
	while (std::abs(comp) < std::abs(value)) {
		comp *= base;
	}
	return (comp == value);
}
inline bool
is_power_of(double value, double base)
{
	if (base == 0 && value == 0)
		return true;
	if (base == 0 || value == 0)
		return false;

	auto comp = base;
	if (std::abs(comp) >= 1)
	{
		while (std::abs(comp) < std::abs(value)) {
			comp *= base;
		}
	}
	else {
		while (std::abs(comp) > std::abs(value)) {
			comp *= base;
		}
	}
	return (comp == value);
}

///////////////////////////////////////////////////////////////////////////////
template <typename valueType>
static std::vector <valueType> Split(valueType text, const valueType& delimiter)
{
	std::vector <valueType> tokens;
	size_t pos = 0;
	valueType token;

	while ((pos = text.find(delimiter)) != valueType::npos)
	{
		token = text.substr(0, pos);
		tokens.push_back(token);
		text.erase(0, pos + delimiter.length());
	}
	tokens.push_back(text);

	return tokens;
}

template <class valueType>
valueType trim(const valueType &s)
{
	auto  wsfront = std::find_if_not(s.begin(), s.end(), [](int c){return std::isspace(c); });
	return valueType(wsfront,
		std::find_if_not(s.rbegin(), valueType::const_reverse_iterator(wsfront),
		[](int c){return std::isspace(c); }).base());
}

void printvector(std::vector<std::wstring>& vec)
{
	for (auto &it : vec)
		std::wcout << it << L"|";
	std::wcout << L"\n";
}

void printdouble()
{
	std::vector<double> test_list{ 1.23456789, -2.34567891, -3.45678912, 4.56789123 };
	for (auto const& it : test_list){
		std::wcout << std::to_wstring(it) << std::endl;
	}
	return;
}

///////////////////////////////////////////////////////////////////////////////
template <typename valueType>
class Tokenizer
{
public:
	enum class OP{ SIMPLE, TRIMMEDTOKENS, NONEMPTYTOKENS, NONEMPTYTRIMEEDTOKENS, UNIQUETOKENS };
private:
	OP			_op;
	valueType	_delims;
	valueType _Trim(const valueType &s)
	{
		auto  wsfront = std::find_if_not(s.begin(), s.end(), [](int c){return std::isspace(c); });
		return valueType(wsfront,
			std::find_if_not(s.rbegin(), valueType::const_reverse_iterator(wsfront),
			[](int c){return std::isspace(c); }).base());
	}
	void _TrimTokens(std::vector<valueType>& tokens)
	{
		for (size_t i = 0; i < tokens.size(); i++)
			tokens[i] = _Trim(tokens[i]);
	}
	void _RemoveEmptyTokens(std::vector<valueType>& tokens)
	{
		using namespace std;
		vector<valueType> nonemptytokens;

		for (auto &it : tokens){
			auto  wsfront = find_if_not(it.begin(), it.end(), [](int c){return std::isspace(c); });
			if (it.end() != wsfront){
				nonemptytokens.push_back(it);
			}
		}
		if (tokens.size() != nonemptytokens.size())
			tokens = nonemptytokens;
	}
	void _KeepUniqueTokens(std::vector<valueType>& tokens)
	{
		using namespace std;
		if (tokens.size() <= 1)
			return;
		
		vector<valueType> UniqueTokens;
		UniqueTokens.push_back(tokens.front());
		for (auto it = tokens.begin() + 1; it != tokens.end(); it++){
			if (it == find(tokens.begin(), it, *it))
				UniqueTokens.push_back(*it);
		}
		if (tokens.size() != UniqueTokens.size())
			tokens = UniqueTokens;
	}
public:
	Tokenizer(OP op = OP::SIMPLE, const valueType& delims = " ") :_op(op), _delims(delims){};
	void GetTokens(const valueType& text, std::vector<valueType>& tokens)
	{
		using namespace std;
		tokens.clear();
		valueType::size_type lastPos = text.find_first_not_of(_delims, 0);
		valueType::size_type pos = text.find_first_of(_delims, lastPos);

		while (valueType::npos != pos || valueType::npos != lastPos)
		{
			tokens.push_back(text.substr(lastPos, pos - lastPos));
			lastPos = text.find_first_not_of(_delims, pos);
			pos = text.find_first_of(_delims, lastPos);
		}

		if (OP::TRIMMEDTOKENS == _op){
			_TrimTokens(tokens);
		}
		if (OP::NONEMPTYTOKENS == _op){
			_RemoveEmptyTokens(tokens);
		}
		if (OP::NONEMPTYTRIMEEDTOKENS == _op){
			_TrimTokens(tokens);
			_RemoveEmptyTokens(tokens);
		}
		if (OP::UNIQUETOKENS == _op){
			_KeepUniqueTokens(tokens);
		}
	}
	void GetTokens(const valueType& text, std::vector<valueType>& tokens, const valueType& delims)
	{
		_delims(delims);
		GetTokens(text, tokens);
	}
};

///////////////////////////////////////////////////////////////////////////////
class Foo
{
private:
	struct Impl;
	std::unique_ptr<Impl> pImpl;
public:
	Foo();
	Foo(const std::wstring& ip);
	Foo(const std::wstring& ip, int k);
	~Foo();
	
	Foo::Foo(const Foo& other);
	Foo& Foo::operator = (const Foo& other);
	
	void printip(std::wstring & classname) const;
};

struct Foo::Impl
{
	std::wstring ip_ = L"";
	int k_ = -INT_MAX;

	Impl(){}
	Impl(std::wstring ip) : ip_(ip){ k_ = 0; }
	Impl(std::wstring ip, int k) : ip_(ip), k_(k) {}
	~Impl(){}
	Impl(const Impl& src){ *this = src; }
	Impl& operator=(const Impl& src) 
	{
		if (this != &src){
			ip_ = src.ip_;
			k_ = src.k_;
		}
		return *this;
	}
	
	void printip(std::wstring & classname) const 
	{ 
		std::wcout << classname << L":" << ip_ << L";" << k_ << L"\n";
	}
};

Foo::Foo() : pImpl(std::make_unique<Impl>()){}
Foo::Foo(const std::wstring& ip) : pImpl(std::make_unique<Impl>(ip)){}
Foo::Foo(const std::wstring& ip, int k) : pImpl(std::make_unique<Impl>(ip, k)){}
Foo::~Foo(){}

Foo::Foo(const Foo& src) : pImpl(std::unique_ptr<Impl>(new Impl(*src.pImpl)))
{}
Foo& Foo::operator = (const Foo& src)
{
	if (this != &src)
		*pImpl = *src.pImpl;
	return *this;
}
void Foo::printip(std::wstring &classname) const
{ 
	pImpl->printip(classname); 
}

Foo TestFoo()
{
	Foo Foo1;
	Foo1.printip(std::wstring(L"TestFoo1"));
	Foo Foo2(L"test", 11);
	Foo2.printip(std::wstring(L"TestFoo2"));
	return (Foo1 = Foo2);
}

///////////////////////////////////////////////////////////////////////////////
std::time_t systemtime_now()
{
	using namespace std;
	chrono::time_point<chrono::system_clock> system_now = chrono::system_clock::now();
	return chrono::system_clock::to_time_t(system_now);
}

std::wstring put_time(const std::tm* date_time, const wchar_t* c_time_format)
{
	std::wostringstream woss;
	woss << std::put_time(const_cast<std::tm*>(date_time), c_time_format);
	return woss.str();
}
void timedifftest()
{
	using namespace std::chrono;
	time_point<high_resolution_clock> t1 = high_resolution_clock::now();

	std::wcout << L"printing out 1000 stars...\n";
	for (int i = 0; i<1000; ++i) std::wcout << L"*";
	std::wcout << std::endl;

	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

	std::wcout << L"It took me " << time_span.count() << L" seconds.";
	std::wcout << std::endl;
}

void timedifftest_tm()
{
	struct tm first{ 0 }, second{ 0 };
	first.tm_year = 2000 - 1900;
	second.tm_year = 2001 - 1900;
	first.tm_mon = second.tm_mon = 1 - 1;
	first.tm_mday = second.tm_mday = 1;

	char       buff[80];
	asctime_s(buff, sizeof(buff), &first);
	std::cout << buff << std::endl;

	asctime_s(buff, sizeof(buff), &second);
	std::cout << buff << std::endl;

	time_t t_f = mktime(&first);
	time_t t_s = mktime(&second);
}

///////////////////////////////////////////////////////////////////////////////
int GenerateRandom(std::vector<int>& list, size_t total,
					int lowest, int highest)
{
	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(lowest, highest);
	
	auto dice = std::bind(distribution,generator);
	for (size_t i = 0; i < total; i++)
	{
		list.push_back(dice());
	}
	return 0;
}
int GenerateRandom(std::vector<wchar_t>& list, size_t total)
{ //'a' 97 to 'z' 122 'A' to 'Z' 65 to 90
	std::vector<int> intlist;
	GenerateRandom(intlist, total, 65, 122);
	
	for (const auto& it:intlist){
		if (it <= 90)
			list.push_back('A' + (it-65));
		else if (it <= 93 && it > 90)
			list.push_back('A' + (it - 90));
		else if (it > 93 && it < 97)
			list.push_back('a' + (it - 93));
		else
			list.push_back('a' + (it - 97));
	}
	return 0;
}

int 
GenerateRandom(
	std::vector<std::wstring> &list, size_t total, int maxlength)
{
	std::vector<int> stringsize;
	std::vector<wchar_t> stringchars;

	//GenerateRandom(stringsize, total, 1, maxlength);
	for (const auto& size : stringsize)
	{
		GenerateRandom(stringchars, size);
		std::wstring temp=L"";
		for (const auto& stringchar : stringchars){
			temp += stringchar;
		}
		stringchars.clear();
		list.push_back(temp);
	}
	return 0;
}


///////////////////////////////////////////////////////////////////////////
/// RandomData Implementation
void 
RandomData::CreateGenerator(
	std::vector<int> seed_init, std::initializer_list<int> seed_init_seq)
{
	if (seed_init.size() != 0){
		std::seed_seq seedseq(seed_init.begin(), seed_init.end());
		generator = std::default_random_engine(seedseq);
	}
	else if (seed_init_seq.size() != 0){
		std::seed_seq seedseq(seed_init_seq);
		generator = std::default_random_engine(seedseq);
	}
	else{
		auto seed = std::chrono::system_clock::now().time_since_epoch().count();
		generator = std::default_random_engine(seed);
	}
}

void 
RandomData::GetRandomValues(
	std::vector<int>& randomvalues, size_t total,
	int lowest = INT_MIN, int highest = INT_MAX)
{
	std::uniform_int_distribution<int> distribution(lowest, highest);
	auto dice = std::bind(distribution, generator);
	for (size_t i = 0; i < total; i++){
		randomvalues.push_back(dice());
	}
}

int 
RandomData::Generate(
	std::vector<int>& randomvalues, size_t total,
	int lowest, int highest)
{
	CreateGenerator();
	GetRandomValues(randomvalues, total, lowest, highest);
	return 0;
}
int 
RandomData::Generate(
	std::vector<wchar_t>& list, size_t total)
{ //'a' 97 to 'z' 122 'A' to 'Z' 65 to 90
	std::vector<int> intlist;
	GetRandomValues(intlist, total, 65, 122);

	for (const auto& it : intlist){
		if (it <= 90)
			list.push_back('A' + (it - 65));
		else if (it <= 93 && it > 90)
			list.push_back('A' + (it - 90));
		else if (it > 93 && it < 97)
			list.push_back('a' + (it - 93));
		else
			list.push_back('a' + (it - 97));
	}
	return 0;
}

int 
RandomData::Generate(
	std::vector<std::wstring> &list, size_t total, int maxlength)
{
	std::vector<int> itemsizes, itemseeders;
	std::vector<wchar_t> itemchars;

	Generate(itemsizes, total, 1, maxlength);
	for (const auto& size : itemsizes)
	{
		GetRandomValues(itemseeders, size);
		CreateGenerator(itemseeders);
		Generate(itemchars, size);
		std::wstring temp = L"";
		for (const auto& itemchar : itemchars){
			temp += itemchar;
		}
		itemchars.clear();
		itemseeders.clear();
		list.push_back(temp);
	}
	return 0;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
auto customlist(std::vector<typename T> list) -> decltype(list){
	std::vector<typename T> retlist = list;
	return retlist;
}

///////////////////////////////////////////////////////////////////////////////
void CheckInput()
{
	wchar_t in;
	std::wstring seq;
	std::vector<int> input;
	std::wcout << L"Enter values : ";
	
	while (std::wcin >> std::noskipws >> in)
	{
		if (L'\n' == in || (L',' == in))
		{
			if (!seq.empty()){
				try{
					input.push_back(std::stoi(seq));
				}catch (std::exception e){ 
					std::wcout << L"Bad input" << std::endl;
				}
				seq.clear();
			}
			if (L'\n' == in) break;
			else continue;
		}
		seq.push_back(in);
	}

	std::wcout << L"Values entered : ";
	std::copy(begin(input), end(input), std::ostream_iterator<int, wchar_t>(std::wcout, L", "));
	std::cout << std::endl;
	getchar();
}

///////////////////////////////////////////////////////////////////////////////
template<class T, char sep = ','>
struct comma_sep { //type used for temporary input
	T t; //where data is temporarily read to
	operator const T&() const { return t; } //acts like an int in most cases
};
template<class T, char sep>
std::istream& operator>>(std::istream& in, comma_sep<T, sep>& t)
{
	if (!(in >> t.t)) //if we failed to read the int
		return in; //return failure state
	if (in.peek() == sep) //if next character is a comma
		in.ignore(); //extract it from the stream and we're done
	else //if the next character is anything else
		in.clear(); //clear the EOF state, read was successful
	return in; //return 
}

int inputcheck() 
{
	typedef std::istream_iterator<comma_sep<int>> istrit;
	typedef std::ostream_iterator<int> ostrit;

	std::istringstream in("1,2,3,4,5");
	std::vector<int> vec = std::vector<int>(istrit(in), istrit());
	std::copy(vec.begin(), vec.end(), ostrit(std::cout, ","));
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
void Testiterator()
{
	typedef std::map<unsigned int, unsigned int> PricesMap;
	PricesMap Prices;
	
	std::vector<unsigned int> m_ui_operands = { 54, 12, 45, 32, 176, 34, 98 };
	for (size_t i = 1; i <= m_ui_operands.size(); i++){
		Prices.insert({ i, m_ui_operands.at(i - 1) });
	}
	for (size_t i = 1; i < m_ui_operands.size(); i++){
		std::wcout << std::to_wstring(m_ui_operands.at(i-1)) << L" " ;
	}
	std::wcout << std::to_wstring(m_ui_operands.at(m_ui_operands.size() - 1)) << std::endl;
	for (auto it = Prices.begin(); it != Prices.end(); it++){
		std::wcout << (*it).first << L" " 
				<< (*it).second << std::endl;
	}
	/*for (int i = 1; i < 8; i++){
		std::wcout << std::to_wstring(i) << L" " << std::to_wstring(Prices[i]) << std::endl;
	}*/
	
	unsigned int diff = std::numeric_limits<unsigned int>::min();
	PricesMap::const_iterator itbuy = Prices.end(), itsell = Prices.end();

	for (auto &buy = Prices.begin(); buy != --(Prices.end()); buy++)
	{
		auto temp = buy;
		for (auto &sell = ++temp; sell != Prices.end(); sell++)
		{
			if (sell->second > buy->second)
			{
				unsigned int tempdiff = sell->second - buy->second;
				if (tempdiff > diff) {
					diff = tempdiff;
					itbuy = buy; 
					itsell = sell;
				}
			}
		}
		//std::wcout << std::endl;
	}
	std::wcout << L"BP\t" << std::to_wstring((*itbuy).first) << L" | " << std::to_wstring((*itbuy).second) << std::endl;
	std::wcout << L"SP\t" << std::to_wstring((*itsell).first) << L" | " << std::to_wstring((*itsell).second) << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
class ArrayAllocator
{
public:
	template<typename T>
	void
		free_all(T ** input, size_t length)
	{
		if (!input)
			return;
		for (size_t i = 0; i < length; i++)
		{
			if (input[i])
			{
				delete[] input[i];
				input[i] = nullptr;
			}
		}
		delete[] input;
	}

	template <typename T>
	T*
		blank(std::vector<T> input)
	{
		T * ret = nullptr;
		if (!input.empty()) {
			ret = new T[input.size()];
			if (ret) {
				memset(ret, 0, input.size()*sizeof(T));
			}
		}
		return ret;
	}

	template <typename T>
	T**
		blank(std::vector < 
			std::basic_string<T, std::char_traits<T>, std::allocator<T>>> input)
	{
		T ** ret = nullptr;
		if (!input.empty())
		{
			ret = new T*[input.size()];
			if (ret)
			{
				memset(ret, 0, input.size()*sizeof(T *));

				for (size_t i = 0; i < input.size(); i++)
				{
					if (input[i].empty())
						continue;
					ret[i] = new T[input[i].size()];
					if (!ret[i])
					{
						free_all(ret, i);
						ret = nullptr;
						break;
					}
					else {
						memset(ret[i], 0, input[i].size()*sizeof(T));
					}
				}
			}
		}
		return ret;
	}

	template <typename T>
	T*
		filled(std::vector<T> input)
	{
		T * ret = blank(input);
		if (ret) {
			for (size_t i = 0; i < input.size(); i++) {
				ret[i] = input[i];
			}
		}
		return ret;
	}

	template <typename T>
	T**
		filled(std::vector < 
			std::basic_string<T, std::char_traits<T>, std::allocator<T>>> input)
	{
		T ** ret = blank(input);
		if (ret) {
			for (size_t i = 0; i < input.size(); i++) {
				for (size_t j = 0; j < input[i].size(); j++) {
					ret[i][j] = input[i][j];
				}
			}
		}
		return ret;
	}

	//int *			
	//	empty(std::vector<int>);
	//unsigned int *	
	//	empty(std::vector<unsigned int>);
	//char *		
	//	empty(std::vector<char>);
	//wchar_t *	
	//	empty(std::vector<wchar_t>);
	//char **
	//	empty(std::vector<std::string>);
	//wchar_t **	
	//	empty(std::vector<std::wstring>);
	//
	//int *			
	//	filled(std::vector<int>);
	//unsigned int *	
	//	filled(std::vector<unsigned int>);
	//char *		
	//	filled(std::vector<char>);
	//wchar_t *
	//	filled(std::vector<wchar_t>);
	//char **		
	//	filled(std::vector<std::string>);
	//wchar_t **	
	//	filled(std::vector<std::wstring>);
};

//int * 
//ArrayAllocator::empty(std::vector<int> input)
//{
//	int * ret = nullptr;
//	if (!input.empty()){
//		ret = new int[input.size()];
//		if (ret) {
//			memset(ret, 0, sizeof(input.size()*sizeof(int)));
//		}
//	}
//	return ret;
//}
//unsigned int * 
//ArrayAllocator::empty(std::vector<unsigned int> input)
//{
//	unsigned int * ret = nullptr;
//	if (!input.empty()) {
//		ret = new unsigned int[input.size()];
//		if (ret) {
//			memset(ret, 0, sizeof(input.size()*sizeof(unsigned int)));
//		}
//	}
//	return ret;
//}
//char * 
//ArrayAllocator::empty(std::vector<char> input)
//{
//	char * ret = nullptr;
//	if (!input.empty()) {
//		ret = new char[input.size()];
//		if (ret) {
//			memset(ret, 0, sizeof(input.size()*sizeof(char)));
//		}
//	}
//	return ret;
//}
//char ** 
//ArrayAllocator::empty(std::vector<std::string> input)
//{
//	char ** ret = nullptr;
//	if (!input.empty()) 
//	{
//		ret = new char*[input.size()];
//		if (ret) 
//		{
//			memset(ret, 0, sizeof(input.size()*sizeof(char *)));
//		
//			for (size_t i = 0; i < input.size(); i++)
//			{
//				if (input[i].empty())
//					continue;
//				ret[i] = new char[input[i].size()];
//				if (!ret[i])
//				{
//					free_all(ret, i);
//					ret = nullptr;
//					break;
//				}
//				else {
//					memset(ret[i], 0, sizeof(input[i].size()*sizeof(char)));
//				}
//			}
//		}
//	}
//	return ret;
//}
//wchar_t * 
//ArrayAllocator::empty(std::vector<wchar_t> input)
//{
//	wchar_t * ret = nullptr;
//	if (!input.empty()) {
//		ret = new wchar_t[input.size()];
//		if (ret) {
//			memset(ret, 0, sizeof(input.size()*sizeof(wchar_t)));
//		}
//	}
//	return ret;
//}
//wchar_t ** 
//ArrayAllocator::empty(std::vector<std::wstring> input)
//{
//	wchar_t ** ret = nullptr;
//	if (!input.empty())
//	{
//		ret = new wchar_t*[input.size()];
//		if (ret)
//		{
//			memset(ret, 0, sizeof(input.size()*sizeof(wchar_t *)));
//
//			for (size_t i = 0; i < input.size(); i++)
//			{
//				if (input[i].empty())
//					continue;
//				ret[i] = new wchar_t[input[i].size()];
//				if (!ret[i])
//				{
//					free_all(ret, i);
//					ret = nullptr;
//					break;
//				}
//				else {
//					memset(ret[i], 0, sizeof(input[i].size()*sizeof(wchar_t)));
//				}
//			}
//		}
//	}
//	return ret;
//}
//
//
//int * 
//ArrayAllocator::filled(std::vector<int> input)
//{
//	int * ret = empty(input);
//	if (ret){
//		for (size_t i = 0; i < input.size(); i++){
//			ret[i] = input[i];
//		}
//	}
//	return ret;
//}
//unsigned int * 
//ArrayAllocator::filled(std::vector<unsigned int> input)
//{
//	unsigned int * ret = empty(input);
//	if (ret) {
//		for (size_t i = 0; i < input.size(); i++) {
//			ret[i] = input[i];
//		}
//	}
//	return ret;
//}
//char * 
//ArrayAllocator::filled(std::vector<char> input)
//{
//	char * ret = empty(input);
//	if (ret) {
//		for (size_t i = 0; i < input.size(); i++) {
//			ret[i] = input[i];
//		}
//	}
//	return ret;
//}
//char ** 
//ArrayAllocator::filled(std::vector<std::string> input)
//{
//	char ** ret = empty(input);
//	if (ret) {
//		for (size_t i = 0; i < input.size(); i++) {
//			for (size_t j = 0; j < input[i].size(); j++) {
//				ret[i][j] = input[i][j];
//			}
//		}
//	}
//	return ret;
//}
//wchar_t * 
//ArrayAllocator::filled(std::vector<wchar_t> input)
//{
//	wchar_t * ret = empty(input);
//	if (ret) {
//		for (size_t i = 0; i < input.size(); i++) {
//			ret[i] = input[i];
//		}
//	}
//	return ret;
//}
//wchar_t ** 
//ArrayAllocator::filled(std::vector<std::wstring> input)
//{
//	wchar_t ** ret = empty(input);
//	if (ret) {
//		for (size_t i = 0; i < input.size(); i++) {
//			for (size_t j = 0; j < input[i].size(); j++) {
//				ret[i][j] = input[i][j];
//			}
//		}
//	}
//	return ret;
//}


///////////////////////////////////////////////////////////////////////////////
void TestGenericEnum();
void TestEnum1();
void TestEnum2();
int TestTime();
void TestContainers();
void templatetest();
void test_mymenu();
void RunOOP();
void outputredirection();
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int 
_tmain(int argc, _TCHAR* argv[])
{
	using namespace std;

	//////////////////////////////////////////////////////
	vector<int> test_i{ 1,2,3,4 };
	auto test_i_ptr = ArrayAllocator().blank(test_i);
	for (size_t i = 0; i < test_i.size(); i++)
	{
		wcout << to_wstring(test_i_ptr[i]) << L" ";
	}
	wcout << endl;

	test_i_ptr = ArrayAllocator().filled(test_i);
	for (size_t i = 0; i < test_i.size(); i++)
	{
		wcout << to_wstring(test_i_ptr[i]) << L" ";
	}
	wcout << endl;

	vector<char> test_c { '1','2','3','4' };
	auto test_c_ptr = ArrayAllocator().blank(test_c);
	for (size_t i = 0; i < test_c.size(); i++)
	{
		wcout << test_c_ptr[i] << L" ";
	}
	wcout << endl;

	test_c_ptr = ArrayAllocator().filled(test_c);
	for (size_t i = 0; i < test_c.size(); i++)
	{
		wcout << test_c_ptr[i] << L" ";
	}
	wcout << endl;


	//////////////////////////////////////////////////////

	//std::wcout << is_power_of(8u, 2u) << std::endl;
	//std::wcout << is_power_of(8, 2) << std::endl;
	//std::wcout << is_power_of(8.0, 2.0) << std::endl;

	//RunOOP();

	//for (auto const&it : vector<RunBase *>{new Run1(), new Run2()})
	//{
	//	it->Run();
	//}

	//templatetest();
	//test_mymenu();
	//wcout << test234().abc << endl << test234::res1() << endl;
	//templatetest();
	//int *array = new int[10];
	//timedifftest();
	//timedifftest_tm();
	//outputredirection();

	/*wcout << vector<int>{1, 2, 3, 4} << std::endl;
	wcout << vector<wchar_t>{L'a', L'b', L'c', L'd'} << std::endl;
	wcout << vector<wstring>{L"a", L"b", L"c", L"d"} << std::endl;*/
	//Testiterator();
	//std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), L'\n');
	//std::wcout << std::numeric_limits<std::streamsize>::max();

	//inputcheck();
	//CheckInput();

	//std::vector<std::wstring> wstrk;
	//for (int j = 0; j < 5; j++)
	//{
	//	//GenerateRandom(wstrk, 20, 10);
	//	RandomData().Generate(wstrk, 20, 10);
	//	for (const auto&it : wstrk)
	//		wcout << it << L"\t";
	//	wcout << endl << L"------------" << endl;
	//	wstrk.clear();
	//}

	/*std::vector<int> k;
	for (int j = 0; j < 3; j++)
	{
		GenerateRandom(k, 40, 0, 1000);
		for (const auto&it : k)
			wcout << it << L"\t";
		wcout << endl << L"------------" << endl ;
		k.clear();
	}*/
	/*std::vector<wchar_t> wk;
	for (int j = 0; j < 5; j++)
	{
		GenerateRandom(wk, 200);
		for (const auto&it : wk)
			wcout << it;
		wcout << endl << L"------------" << endl;
		wk.clear();
	}*/

	/*wcout << k.size() << endl;
	auto retval = customlist<int>(k);
	wcout << retval.size() << endl;*/

	//TestContainers();
	//TestTime();
	//TestEnum2();
	//TestGenericEnum();

	//time_t timer = systemtime_now();
	//std::tm tm_stamp;
	//localtime_s(&tm_stamp, &timer);
	//const wchar_t* format = L"%Y%m%d %H:%M:%S";
	//wcout << put_time(&tm_stamp, format) << std::endl;

	//TestFoo().printip(std::wstring(L"TestFoo"));
	//
	//Foo Foo1(L"main1");
	//Foo1.printip(std::wstring(L"Foo1"));
	//Foo Foo2(Foo1);
	//Foo2.printip(std::wstring(L"Foo2"));
	//Foo Foo3 = Foo1;
	//Foo3.printip(std::wstring(L"Foo3"));
	//Foo3 = Foo2;
	//Foo3.printip(std::wstring(L"Foo3"));
	//
	//Foo Foo11(L"main11", 15);
	//Foo11.printip(std::wstring(L"Foo11"));
	//Foo Foo21(Foo11);
	//Foo21.printip(std::wstring(L"Foo21"));
	//Foo Foo31 = Foo11;
	//Foo31.printip(std::wstring(L"Foo31"));
	//Foo31 = Foo21;
	//Foo31.printip(std::wstring(L"Foo31"));


	//wstring str{ L" , a,b , ; c - d;" }, delim{ L",;-" };
	//vector<wstring> tokens;

	//wcout << L"Original String\n";
	//wcout << str << "\n";

	//Tokenizer<wstring>(Tokenizer<wstring>::OP::SIMPLE, delim).GetTokens(str, tokens);
	//wcout << L"Simple Tokenize\n";
	//printvector(tokens);
	//printdouble();

	//Tokenizer<wstring>(Tokenizer<wstring>::OP::TRIMMEDTOKENS, delim).GetTokens(str, tokens);
	//wcout << L"Trimmed Tokens\n";
	//printvector(tokens);

	//Tokenizer<wstring>(Tokenizer<wstring>::OP::NONEMPTYTRIMEEDTOKENS, delim).GetTokens(str, tokens);
	//wcout << L"Non empty Trimmed Tokens\n";
	//printvector(tokens);

	//Tokenizer<wstring>(Tokenizer<wstring>::OP::UNIQUETOKENS, delim).GetTokens(str, tokens);
	//wcout << L"Unique Tokens\n";
	//printvector(tokens);

	getchar();

	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
template<class T>
int trimandcompare(const T &s1, const T &s2, bool casesensitive)
{
	if (!(std::is_same<T, std::wstring>::value || std::is_same<T, std::string>::value))
		throw std::invalid_argument;
	return compare<T>(trim(s1), trim(s2), casesensitive);
}
template<class T>
int compare(const T &s1, const T &s2, bool casesensitive)
{
	using namespace std;
	if (!(is_same<T, std::wstring>::value || is_same<T, std::string>::value))
		throw invalid_argument("Provide either std:;string or std::wstring");

	T temp_s1(s1), temp_s2(s2);
	if (!casesensitive){
		transform(temp_s1.begin(), temp_s1.end(), temp_s1.begin(), ::tolower);
		transform(temp_s2.begin(), temp_s2.end(), temp_s2.begin(), ::tolower);
	}
	return temp_s1.compare(temp_s2);
}

///////////////////////////////////////////////////////////////////////////////
enum class ARGTYPE{ START, ConsoleOps, FileInput, CommandLine, END };

class ArgData{
private:
	ARGTYPE _op = ARGTYPE::START;
	std::wstring _name = nullptr;
	std::vector<std::wstring> _extranames;
public:
	ArgData(ARGTYPE op, std::wstring name) :_op(op), _name(name) {};

	ARGTYPE gettype(){ return _op; }
	void addextranames(std::vector<std::wstring> extranames) { _extranames = extranames; }
	std::wstring extranames()
	{
		std::wstring temp(L"");
		for (size_t i = 0; i < _extranames.size() - 1; i++){ temp.append(_extranames[i] + L","); }
		temp.append(_extranames[_extranames.size() - 1]);
		return temp;
	}
	void display(){ std::wcout << (int)_op << L": " << _name << L", " << extranames(); }
	bool matchnames(std::wstring candidate)
	{
		if (0 == _name.compare(0, _name.length(), candidate)) return true;
		else{
			for each (std::wstring var in _extranames){
				if (0 == var.compare(0, var.length(), candidate)) return true;
			}
		}
		return false;
	}
};
std::vector<ArgData> argmap;
std::vector<std::wstring> arglist;
#define ARGSTART_CHAR '-'

///////////////////////////////////////////////////////////////////////////////
void initlists()
{
	ArgData *temp = nullptr;

	temp = new ArgData(ARGTYPE::ConsoleOps, L"consoleops");
	temp->addextranames({ L"co" });
	argmap.push_back(*temp);

	temp = new ArgData(ARGTYPE::FileInput, L"fileinput");
	temp->addextranames({ L"fi" });
	argmap.push_back(*temp);

	temp = new ArgData(ARGTYPE::CommandLine, L"commandline");
	temp->addextranames({ L"cl" });
	argmap.push_back(*temp);
}

///////////////////////////////////////////////////////////////////////////////
bool operator ==(const struct tm & first, const struct tm & second){
	return (0 == memcmp((void *)&first, (void *)&second, sizeof(struct tm)));
}
bool operator <(const struct tm & first, const struct tm & second){
	struct tm temp_first = first, temp_second = second;
	return (difftime(mktime(&temp_first), mktime(&temp_first)) < 0);
}

int TestTime()
{
	using std::wcout;
	using std::endl;

	struct tm first, second;
	wcout << L"first == second : " << ((first == second)?L"true":L"false") << endl;
	first.tm_year = 1987;
	second.tm_year = 1986;
	wcout << L"first < second : " << ((first < second) ? L"true" : L"false") << endl;

	return 0;
}

