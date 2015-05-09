////////////////////////////////////////////////////////////////////////////////
// Some utility functions needed at most places and not easily categorizable
// estd.h - name inspired by stroustrup's book "a tour of c++" 
// Tested in VS 2013 on windows 8.1 64 bit only
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <vector>
#include <set>
#include <random>
#include <chrono>
#include <algorithm>
#include <functional>
#include <locale>
#include <cctype>
#include <memory>
#include <list>
#include <set>

///////////////////////////////////////////////////////////////////////////////
// Anonymous namespace functions
namespace
{

/// struct tm //////////////////////////////////////////////////////////////////
inline bool
operator ==(struct tm const& first, struct tm const& second)
{
	return (0 == memcmp(static_cast<const void *>(&first),
		static_cast<const void *>(&second), sizeof(struct tm)));
}
inline bool
operator !=(struct tm const& first, struct tm const& second)
{
	return (!(first == second));
}
inline bool
operator <(struct tm const& first, struct tm const& second)
{
	struct tm temp_first = first, temp_second = second;
	time_t t_f = mktime(&temp_first), t_s = mktime(&temp_second);
	return (difftime(t_f, t_s) < 0);
}
inline bool
operator >=(struct tm const& first, struct tm const& second)
{
	return (!(first < second));
}

/// vector display /////////////////////////////////////////////////////////////
template <typename T>
std::wostream&
operator<<(std::wostream& os, std::vector<typename T> const& input)
{
	std::wstring output;
	for (size_t i = 0; i < input.size() - 1; i++){
		output += (std::to_wstring(input.at(i)) + L",");
	}
	output += std::to_wstring(input.at(input.size() - 1));
	return (os << output);
}

std::wostream&
operator<<(std::wostream& os, std::vector<wchar_t> const& input)
{
	std::wstring output;
	for (size_t i = 0; i < input.size() - 1; i++){
		output.push_back(input.at(i));
		output += L",";
	}
	output.push_back(input.at(input.size() - 1));
	return (os << output);
}
std::wostream&
operator<<(std::wostream& os, std::vector<std::wstring> const& input)
{
	std::wstring output;
	for (size_t i = 0; i < input.size() - 1; i++){
		output += (input.at(i) + L",");
	}
	output += input.at(input.size() - 1);
	return (os << output);
}

//std::wostream&
//operator<<(std::wostream& os, std::vector<int> const& input)
//{
//	std::wstring output;
//	for (size_t i = 0; i < input.size() - 1; i++){
//		output += (std::to_wstring(input.at(i)) + L",");
//	}
//	output += std::to_wstring(input.at(input.size() - 1));
//	return (os << output);
//}
//std::wostream&
//operator<<(std::wostream& os, std::vector<unsigned int> const& input)
//{
//	std::wstring output;
//	for (size_t i = 0; i < input.size() - 1; i++){
//		output += (std::to_wstring(input.at(i)) + L",");
//	}
//	output += std::to_wstring(input.at(input.size() - 1));
//	return (os << output);
//}
}

///////////////////////////////////////////////////////////////////////////////
namespace estd
{
using WSTR_VEC = std::vector<std::wstring>;
using STR_VEC = std::vector<std::string>;
using WSTR_SET = std::set<std::wstring>;
using STR_SET = std::set<std::string>;

using WCHAR_VEC = std::vector<wchar_t>;
using CHAR_VEC = std::vector<char>;
using WCHAR_SET = std::set<wchar_t>;
using CHAR_SET = std::set<char>;

/// Trimming ///////////////////////////////////////////////////////////////////
//template <class valueType> valueType
//trim(const valueType &s);
//template <class valueType> valueType
//ltrim(valueType &s);
//template <class valueType> valueType
//rtrim(valueType &s);
inline std::wstring
trim(std::wstring s)
{
	using namespace std;
	auto  wsfront = find_if_not(s.begin(), s.end(),
		[](int c){return std::isspace(c); });
	return wstring(wsfront,
		find_if_not(s.rbegin(), wstring::reverse_iterator(wsfront),
		[](int c){return std::isspace(c); }).base());
}
inline std::string
trim(std::string s)
{
	using namespace std;
	auto  wsfront = find_if_not(s.begin(), s.end(),
		[](int c){return std::isspace(c); });
	return string(wsfront,
		find_if_not(s.rbegin(), string::reverse_iterator(wsfront),
		[](int c){return std::isspace(c); }).base());
}

inline std::wstring
ltrim(std::wstring s)
{
	s.erase(s.begin(), std::find_if(
		s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}
inline std::string
ltrim(std::string s)
{
	s.erase(s.begin(), std::find_if(
		s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

inline std::string
rtrim(std::string s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}
inline std::wstring
rtrim(std::wstring &s)
{
	s.erase(std::find_if(
		s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

/// to_string, to_wstring //////////////////////////////////////////////////////
inline std::wstring
to_wstring(bool b){
	return (b ? L"true" : L"false");
}

template <typename T> std::wstring
to_wstring(T const& C, std::wstring const& delim = L",")
{
	std::wstring ret = L"";
	for (auto const &it : C){
		ret += (std::to_wstring(it) + delim);
	}
	if (!ret.empty()){
		ret.erase(ret.end() - delim.size(), ret.end());
	}
	return ret;
}
template <typename T> std::string
to_string(T const& C, std::string const& delim = ",")
{
	std::string ret = "";
	for (auto const &it : C){
		ret += std::to_string(it) + delim;
	}
	if (!ret.empty()){
		ret.erase(ret.end() - delim.size(), ret.end());
	}
	return ret;
}

inline std::wstring
to_wstring(WCHAR_VEC const& C, std::wstring const& delim = L",")
{
	std::wstring ret = L"";
	for (auto const &it : C){
		ret.push_back(it);
		ret += delim;
	}
	if (!ret.empty()){
		ret.erase(ret.end() - delim.size(), ret.end());
	}
	return ret;
}
inline std::string
to_string(CHAR_VEC const& C, std::string const& delim = ",")
{
	std::string ret = "";
	for (auto const &it : C){
		ret.push_back(it);
		ret += delim;
	}
	if (!ret.empty()){
		ret.erase(ret.end() - delim.size(), ret.end());
	}
	return ret;
}
inline std::wstring
to_wstring(WCHAR_SET const& C, std::wstring const& delim = L",")
{
	std::wstring ret = L"";
	for (auto const &it : C){
		ret.push_back(it);
		ret += delim;
	}
	if (!ret.empty()){
		ret.erase(ret.end() - delim.size(), ret.end());
	}
	return ret;
}
inline std::string
to_string(CHAR_SET const& C, std::string const& delim = ",")
{
	std::string ret = "";
	for (auto const &it : C){
		ret.push_back(it);
		ret += delim;
	}
	if (!ret.empty()){
		ret.erase(ret.end() - delim.size(), ret.end());
	}
	return ret;
}

inline std::wstring
to_wstring(WSTR_VEC const& C, std::wstring const& delim = L",")
{
	std::wstring ret = L"";
	for (auto const &it : C){
		ret += it + delim;
	}
	if (!ret.empty()) ret.erase(ret.end() - delim.size(), ret.end());
	return ret;
}
inline std::string
to_string(STR_VEC const& C, std::string const& delim = ",")
{
	std::string ret = "";
	for (auto const &it : C){
		ret += it + delim;
	}
	if (!ret.empty())  ret.erase(ret.end() - delim.size(), ret.end());
	return ret;
}

inline std::wstring
to_wstring(WSTR_SET const& C, std::wstring const& delim = L",")
{
	std::wstring ret = L"";
	for (auto const &it : C){
		ret += it + delim;
	}
	if (!ret.empty()){
		ret.erase(ret.end() - delim.size(), ret.end());
	}
	return ret;
}
inline std::string
to_string(STR_SET const& C, std::string const& delim = ",")
{
	std::string ret = "";
	for (auto const &it : C){
		ret += it + delim;
	}
	if (!ret.empty()){
		ret.erase(ret.end() - delim.size(), ret.end());
	}
	return ret;
}

/// Conversion ////////////////////////////////////////////////////////////
inline std::wstring 
tolower(std::wstring val){
	std::transform(val.begin(), val.end(), val.begin(), ::towlower);
	return val;
}
inline std::wstring
toupper(std::wstring val){
	std::transform(val.begin(), val.end(), val.begin(), ::towupper);
	return val;
}
inline std::string 
tolower(std::string val){
	std::transform(val.begin(), val.end(), val.begin(), ::tolower);
	return val;
}
inline std::string 
toupper(std::string val){
	std::transform(val.begin(), val.end(), val.begin(), ::toupper);
	return val;
}

template<typename T> std::vector<T>
tolower(std::vector<T> val)
{
	for (size_t i = 0; i < val.size(); i++){
		val[i] = tolower(val[i]);
	}
	return val;
}
template<typename T> std::vector<T>
toupper(std::vector<T> val)
{
	for (size_t i = 0; i < val.size(); i++){
		val[i] = toupper(val[i]);
	}
	return val;
}

//inline WSTR_VEC 
//tolower(WSTR_VEC& val)
//{
//	for (auto &it : val)
//		std::transform(it.begin(), it.end(), it.begin(), ::towlower);
//	return val;
//}
//inline WSTR_VEC 
//toupper(WSTR_VEC& val)
//{
//	for (auto &it : val)
//		std::transform(it.begin(), it.end(), it.begin(), ::towupper);
//	return val;
//}

/// numeric conversion ////////////////////////////////////////////////////
template<typename T> 
unsigned int
stou(T const& str, size_t * idx = 0, int base = 10) 
{
	unsigned long result = std::stoul(str, idx, base);
	if (result > std::numeric_limits<unsigned int>::max()) {
		throw std::out_of_range("stou");
	}
	return static_cast<unsigned int>(result);
}
//inline unsigned int 
//stou(std::string const& str, size_t * idx = 0, int base = 10)
//{
//	unsigned long result = std::stoul(str, idx, base);
//	if (result > std::numeric_limits<unsigned>::max()) {
//		throw std::out_of_range("stou");
//	}
//	return static_cast<unsigned int>(result);
//}

/// Add padding //////////////////////////////////////////////////////////
template <typename T> std::wstring
prepend_zero_w(T const& item, size_t total_size)
{
	// This will only work for numeric types and compilation will fail otherwise
	auto max = std::numeric_limits<T>::max();

	std::wstring ret = std::to_wstring(item);
	if (ret.size() < total_size){
		ret.insert(0, total_size - ret.size(), L'0');
	}
	return ret;
}
template <typename T> std::string
prepend_zero(T const& item, size_t total_size)
{
	// This will only work for numeric types and compilation will fail otherwise
	auto max = std::numeric_limits<T>::max();

	std::string ret = std::to_string(item);
	if (ret.size() < total_size){
		ret.insert(0, total_size - ret.size(), '0');
	}
	return ret;
}
inline std::wstring
prepend_space_w(std::wstring item, size_t total_size)
{
	if (item.size() < total_size){
		item.insert(0, total_size - item.size(), L' ');
	}
	return item;
}
inline std::string
prepend_space(std::string item, size_t total_size)
{
	if (item.size() < total_size){
		item.insert(0, total_size - item.size(), L' ');
	}
	return item;
}
inline std::wstring
append_space_w(std::wstring item, size_t total_size)
{
	if (item.size() < total_size){
		item.insert(item.size(), total_size - item.size(), L' ');
	}
	return item;
}
inline std::string
append_space(std::string item, size_t total_size)
{
	if (item.size() < total_size){
		item.insert(item.size(), total_size - item.size(), ' ');
	}
	return item;
}

/// repeat /////////////////////////////////////////////////////////////////////
inline std::wstring
repeat(std::wstring const& item, size_t count)
{
	std::wstring ret{ L"" };
	for (size_t i = 0; i < count; i++){
		ret += item;
	}
	return ret;
}
inline std::string
repeat(std::string const& item, size_t count)
{
	std::string ret{ "" };
	for (size_t i = 0; i < count; i++){
		ret += item;
	}
	return ret;
}

/// Sort ///////////////////////////////////////////////////////////////////////
template<typename C> void
sort(C& c){
	std::sort(c.begin(), c.end());
}
template<typename C, typename Pred> void
sort(C& c, Pred p){
	std::sort(c.begin(), c.end(), p);
}

/// Compare ////////////////////////////////////////////////////////////////////
inline int
compare_ci(std::wstring s1, std::wstring s2)
{
	using namespace std;
	transform(s1.begin(), s1.end(), s1.begin(), ::towlower);
	transform(s2.begin(), s2.end(), s2.begin(), ::towlower);
	return s1.compare(s2);
}
inline int
compare_ci(std::string s1, std::string s2)
{
	using namespace std;
	transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
	transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
	return s1.compare(s2);
}

template<typename T>
inline int
trimandcompare(T const& s1, T const& s2){
	return trim(s1).compare(trim(s2));
}
template<typename T>
inline int
trimandcompare_ci(T const& s1, T const& s2){
	return compare_ci(trim(s1), trim(s2));
}

/// is_power_of ////////////////////////////////////////////////////////////////
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
is_power_of (int value, int base)
{
	if (base == 0 && value == 0)
		return true;
	if (base == 0 || value == 0)
		return false;
	
	auto comp = base;
	while (std::abs(comp) < std::abs(value)){
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
		while (std::abs(comp) < std::abs(value)){
			comp *= base;
		}
	}
	else{
		while (std::abs(comp) > std::abs(value)){
			comp *= base;
		}
	}
	return (comp == value);
}

/// is_available ///////////////////////////////////////////////////////////////
template<typename Val, typename C> bool
is_available(Val const& val, C const& c)
{
	return(c.end() != find(c.begin(), c.end(), val));
}

/// any_of //////////////////////////////////////////////////////////////////
inline bool
any_of(std::wstring const& val, WSTR_VEC const& c)
{
	using namespace std;
	if (0 == c.size() || 0 == val.size()){
		return false;
	}
	return any_of(c.begin(), c.end(),
		[&, val](wstring const& src){return (src == val); });
}
inline bool
any_of(std::string const& val, STR_VEC const& c)
{
	using namespace std;
	if (0 == c.size() || 0 == val.size()){
		return false;
	}
	return any_of(c.begin(), c.end(),
		[&, val](string const& src){return (src == val); });
}
inline bool
any_of_ci(std::wstring val, WSTR_VEC const& c)
{
	using namespace std;
	if (c.size() == 0 || val.size() == 0){
		return false;
	}
	val = tolower(val);
	return any_of(c.begin(), c.end(),
		[&, val](wstring const& src){return (tolower(src) == val); });
}
inline bool
any_of_ci(std::string val, STR_VEC const& c)
{
	using namespace std;
	if (c.size() == 0 || val.size() == 0){
		return false;
	}
	val = tolower(val);
	return any_of(c.begin(), c.end(),
		[&, val](string const& src){return (tolower(src) == val); });
}

/// start_with ////////////////////////////////////////////////////////////
template <typename T> 
bool
start_with(T const& src, T const& search_str)
{
	if ((0 == src.size()) || (0 == search_str.size()) 
		|| (src.size() < search_str.size()))
	{
		return false;
	}
	return (0 == src.compare(0, search_str.size(), search_str));
}
template <typename T> 
bool
start_with_ci(T src, T search_str)
{
	return (start_with(tolower(src), tolower(search_str)));
}


/// Tokenizer /////////////////////////////////////////////////////////////////
enum class TokenizerOP
{
	SIMPLE,
	TRIMMED, NONEMPTY, NONEMPTYTRIMMED,
	UNIQUE, UNIQUETRIMMED,
	UNIQUENONEMPTY, UNIQUENONEMPTYTRIMMED
};
template <typename valueType>
class Tokenizer
{
private:
	TokenizerOP			op_;
	valueType			delims_;

	valueType
		Trim(valueType const &s)
	{
		auto  wsfront = std::find_if_not(s.begin(), s.end(), 
			[](int c){return std::isspace(c); });
		return valueType(wsfront,
			std::find_if_not(s.rbegin(), 
			valueType::const_reverse_iterator(wsfront),
			[](int c){return std::isspace(c); }).base());
	}
	void
		TrimTokens(std::vector<valueType>& tokens)
	{
		for (size_t i = 0; i < tokens.size(); i++)
			tokens[i] = Trim(tokens[i]);
	}
	void
		RemoveEmptyTokens(std::vector<valueType>& tokens)
	{
		using namespace std;
		vector<valueType> nonemptytokens;

		for (auto &it : tokens){
			auto  wsfront = find_if_not(it.begin(), it.end(), 
				[](int c){return std::isspace(c); });
			if (it.end() != wsfront){
				nonemptytokens.push_back(it);
			}
		}
		if (tokens.size() != nonemptytokens.size())
			tokens = nonemptytokens;
	}
	void
		KeepUniqueTokens(std::vector<valueType>& tokens)
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

	void
		GetTokens(valueType const& text, std::vector<valueType>& tokens)
	{
		using namespace std;
		tokens.clear();
		valueType::size_type lastPos = text.find_first_not_of(delims_, 0);
		valueType::size_type pos = text.find_first_of(delims_, lastPos);

		while (valueType::npos != pos || valueType::npos != lastPos)
		{
			tokens.push_back(text.substr(lastPos, pos - lastPos));
			lastPos = text.find_first_not_of(delims_, pos);
			pos = text.find_first_of(delims_, lastPos);
		}

		if (TokenizerOP::TRIMMED == op_){
			TrimTokens(tokens);
		}
		if (TokenizerOP::NONEMPTY == op_){
			RemoveEmptyTokens(tokens);
		}
		if (TokenizerOP::NONEMPTYTRIMMED == op_){
			TrimTokens(tokens);
			RemoveEmptyTokens(tokens);
		}
		if (TokenizerOP::UNIQUE == op_){
			KeepUniqueTokens(tokens);
		}
		if (TokenizerOP::UNIQUETRIMMED == op_){
			TrimTokens(tokens);
			KeepUniqueTokens(tokens);
		}
		if (TokenizerOP::UNIQUENONEMPTY == op_){
			KeepUniqueTokens(tokens);
			RemoveEmptyTokens(tokens);
		}
		if (TokenizerOP::UNIQUENONEMPTYTRIMMED == op_){
			TrimTokens(tokens);
			KeepUniqueTokens(tokens);
			RemoveEmptyTokens(tokens);
		}
		return;
	}
	void
		GetTokens(valueType const& text, std::vector<valueType>& tokens,
		valueType const& delims)
	{
		delims_ = delims;
		GetTokens(text, tokens);
		return;
	}

public:
	Tokenizer() :Tokenizer(TokenizerOP::SIMPLE){}
	Tokenizer(TokenizerOP op) : op_(op){}
	Tokenizer(valueType const& delims, TokenizerOP op = TokenizerOP::SIMPLE)
		:delims_(delims), op_(op){};

	std::vector<valueType>
		Tokens(valueType const& text)
	{
		std::vector<valueType> tokens;
		GetTokens(text, tokens);
		return tokens;
	}
	std::vector<valueType>
		Tokens(valueType const& text, valueType const& delims)
	{
		delims_ = delims;
		return Tokens(text);
	}
	std::vector<valueType>
		Tokens(valueType const& text, TokenizerOP op)
	{
		op_ = op;
		return Tokens(text);
	}
};

////////////////////////////////////////////////////////////////////////////////
class PrintTimer
{
public:
	virtual std::wstring
		to_wstring(std::chrono::hours::rep value, std::chrono::hours dummy) = 0;
	virtual std::wstring
		to_wstring(std::chrono::minutes::rep value, std::chrono::minutes dummy) = 0;
	virtual std::wstring
		to_wstring(std::chrono::seconds::rep value, std::chrono::seconds dummy) = 0;
	virtual std::wstring
		to_wstring(std::chrono::microseconds::rep value, std::chrono::microseconds dummy) = 0;
	virtual std::wstring
		to_wstring(std::chrono::milliseconds::rep value, std::chrono::milliseconds dummy) = 0;
	virtual std::wstring
		to_wstring(std::chrono::nanoseconds::rep value, std::chrono::nanoseconds dummy) = 0;
};

template<typename clock_type, typename dur>
class ExecutionTimer
{
private:
	typename clock_type::time_point m_start;
	typename clock_type::time_point m_end;
	std::shared_ptr<PrintTimer>		m_printer;

public:
	ExecutionTimer() : ExecutionTimer(nullptr) {};
	ExecutionTimer(std::shared_ptr<PrintTimer> const& printer){
		m_start = clock_type::now();;
		m_end = clock_type::now();;
		m_printer = printer;
	};

	void set_start(){
		m_start = clock_type::now();
	}
	void set_start(typename clock_type::time_point start){
		m_start = start;
	}
	void set_end(){
		m_end = clock_type::now();
	}
	void set_end(typename clock_type::time_point end){
		m_end = end;
	}
	void set_printer(std::shared_ptr<PrintTimer> const& printer){
		m_printer = printer;
	}

	typename dur::rep elapsed()
	{
		return std::chrono::duration_cast<dur>(m_end - m_start).count();
	};
	std::wstring to_wstring(){
		return to_wstring(m_printer);
	}
	std::wstring to_wstring(std::shared_ptr<PrintTimer> const& printer)
	{
		if (printer){
			auto ret = printer->to_wstring(elapsed(),
				std::chrono::duration_cast<dur>(std::chrono::seconds(1)));
			return ret;
		}
		else{
			return std::to_wstring(elapsed());
		}
	}
};

////////////////////////////////////////////////////////////////////////////////
// Allocate arrays whose cleaning is to be handled by users
// No allocation in case of zero length vector input
// Null characters will not be added at the end of arrays.
class AllocateArrays
{
public:
	template <typename T>
	T*
	blank(std::vector<T> input)
	{
		T * ret = nullptr;
		if (!input.empty()) 
		{
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
		if (ret) 
		{
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
		if (ret) 
		{
			for (size_t i = 0; i < input.size(); i++) {
				if (input[i].empty())
					continue;
				for (size_t j = 0; j < input[i].size(); j++) {
					ret[i][j] = input[i][j];
				}
			}
		}
		return ret;
	}

private:
	template<typename T>
	void
	free_all(T ** input, size_t length)
	{
		if (!input)
			return;
		if (*input)
		{
			for (size_t i = 0; i < length; i++)
			{
				if (!input[i]) continue;
				delete[] input[i];
				input[i] = nullptr;
			}
			delete[] input;
		}
	}
};

}
