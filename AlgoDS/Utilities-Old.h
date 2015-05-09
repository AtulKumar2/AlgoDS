#pragma once

//#include "stdafx.h"
#include <random>
#include <limits>
#include <set>
#include <memory>

#define FREE(ptr) if (ptr) {free(ptr); ptr=NULL;}

///////////////////////////////////////////////////////////////////////////////
// Anonymous namespace functions
namespace
{
	///////////////////////////////////////////////////////////////////////////
	// Operator overloading
	bool 
		operator ==(struct tm const & first, struct tm const & second)
	{
		return (0 == memcmp(static_cast<const void *>(&first),
			static_cast<const void *>(&second), sizeof(struct tm)));
	}
	bool 
		operator <(struct tm const & first, struct tm const & second)
	{
		struct tm temp_first = first, temp_second = second;
		return (difftime(mktime(&temp_first), mktime(&temp_first)) < 0);
	}
}

////////////////////////////////////////////////////////////////////////////////
/// Left Shift overload
template <typename T>
std::wostream&  operator<<(std::wostream& os, const std::vector<typename T>& input);

////////////////////////////////////////////////////////////////////////////////
/// Trimming
template <class valueType> valueType 
	trim(const valueType &s);
template <class valueType> valueType 
	ltrim(valueType &s);
template <class valueType> valueType 
	rtrim(valueType &s);

///////////////////////////////////////////////////////////////////////////////
/// String comparison
template<typename T> 
inline int compare(const T &s1, const T &s2, bool casesensitive);
template<typename T> 
inline int trimandcompare(const T &s1, const T &s2, bool casesensitive);

///////////////////////////////////////////////////////////////////////////////
void ClearWcin();

///////////////////////////////////////////////////////////////////////////////
// Tokenizer
enum class TokenizerOP
{
	SIMPLE,
	TRIMMEDTOKENS, NONEMPTYTOKENS, NONEMPTYTRIMMEDTOKENS,
	UNIQUETOKENS, UNIQUETRIMMEDTOKENS,
	UNIQUENONEMPTYTOKENS, UNIQUENONEMPTYTRIMMEDTOKENS
};
template <typename valueType>
class Tokenizer
{
private:
	TokenizerOP			op_;
	valueType			delims_;

	valueType
		Trim(valueType const &s){
		auto  wsfront = std::find_if_not(s.begin(), s.end(), [](int c){return std::isspace(c); });
		return valueType(wsfront,
			std::find_if_not(s.rbegin(), valueType::const_reverse_iterator(wsfront),
			[](int c){return std::isspace(c); }).base());
	}

	void
		TrimTokens(std::vector<valueType>& tokens){
		for (size_t i = 0; i < tokens.size(); i++)
			tokens[i] = Trim(tokens[i]);
	}

	void
		RemoveEmptyTokens(std::vector<valueType>& tokens){
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
	void
		KeepUniqueTokens(std::vector<valueType>& tokens){
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
		Tokens(valueType const& text, valueType const& delims){
		delims_ = delims;
		return Tokens(text);
	}
	std::vector<valueType>
		Tokens(valueType const& text, TokenizerOP op)
	{
		op_ = op;
		return Tokens(text);
	}
	void
		GetTokens(valueType const& text, std::vector<valueType>& tokens)
	{
		using namespace std;
		tokens.clear();
		valueType::size_type lastPos = text.find_first_not_of(delims_, 0);
		valueType::size_type pos = text.find_first_of(delims_, lastPos);

		while (valueType::npos != pos || valueType::npos != lastPos){
			tokens.push_back(text.substr(lastPos, pos - lastPos));
			lastPos = text.find_first_not_of(delims_, pos);
			pos = text.find_first_of(delims_, lastPos);
		}

		if (TokenizerOP::TRIMMEDTOKENS == op_){
			TrimTokens(tokens);
		}
		if (TokenizerOP::NONEMPTYTOKENS == op_){
			RemoveEmptyTokens(tokens);
		}
		if (TokenizerOP::NONEMPTYTRIMMEDTOKENS == op_){
			TrimTokens(tokens);
			RemoveEmptyTokens(tokens);
		}
		if (TokenizerOP::UNIQUETOKENS == op_){
			KeepUniqueTokens(tokens);
		}
		if (TokenizerOP::UNIQUETRIMMEDTOKENS == op_){
			TrimTokens(tokens);
			KeepUniqueTokens(tokens);
		}
		if (TokenizerOP::UNIQUENONEMPTYTOKENS == op_){
			KeepUniqueTokens(tokens);
			RemoveEmptyTokens(tokens);
		}
		if (TokenizerOP::UNIQUENONEMPTYTRIMMEDTOKENS == op_){
			TrimTokens(tokens);
			KeepUniqueTokens(tokens);
			RemoveEmptyTokens(tokens);
		}
		return;
	}
	void
		GetTokens(valueType const& text, std::vector<valueType>& tokens,
		valueType const& delims){
		delims_ = delims;
		GetTokens(text, tokens);
		return;
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
			return L"Printing facilities not available";
		}
	}
};

///////////////////////////////////////////////////////////////////////////////
namespace estd
{
	using WSTR_VEC = std::vector<std::wstring>;
	using STR_VEC = std::vector<std::string>;
	using WSTR_SET = std::set<std::wstring>;
	using STR_SET = std::set<std::string>;

	/// Sort //////////////////////////////////////////////////////////////////
	template<typename C> void 
		sort(C& c){
		std::sort(c.begin(), c.end());
	}
	template<typename C, typename Pred> void 
		sort(C& c, Pred p){
		std::sort(c.begin(), c.end(), p);
	}
	template<typename C, typename Val> bool 
		is_available(C const& c, Val const& val){
		return(c.end() != find(c.begin(), c.end(), val));
	}

	/// Conversion ////////////////////////////////////////////////////////////
	std::wstring 
		tolower(std::wstring val);
	std::wstring 
		toupper(std::wstring val);
	std::string 
		tolower(std::string val);
	std::string 
		toupper(std::string val);
	template<typename T> std::vector<T> 
		tolower(std::vector<T>& val){
		for (auto &it : val)
			tolower(it);
		return val;
	}

	template<typename T> std::vector<T> 
		toupper(std::vector<T>& val){
		for (auto &it : val)
			toupper(it);
		return val;
	}
	/*std::vector<std::wstring> tolower(std::vector<std::wstring>& val);
	std::vector<std::wstring> toupper(std::vector<std::wstring>& val);*/

	/// start_with ////////////////////////////////////////////////////////////
	template <typename T> bool 
		start_with(T const& src, T const& search_str){
		if ((0 == src.size()) || (0 == search_str.size()) || (src.size() < search_str.size()))
			return false;
		return (0 == src.compare(0, search_str.size(), search_str));
	}
	template <typename T> bool 
		start_with_ci(T src, T search_str){
		return (start_with(tolower(src), tolower(search_str)));
	}
	//bool start_with(std::wstring const& src, std::wstring const& search_str);
	//bool start_with_ci(std::wstring src, std::wstring search_str);
	//bool start_with(std::string const& src, std::string const& search_str);
	//bool start_with_ci(std::string src, std::string search_str);

	/// any_of //////////////////////////////////////////////////////////////////
	bool 
		any_of_ci(WSTR_VEC const& c, std::wstring val);
	bool 
		any_of(WSTR_VEC const& c, std::wstring const& val);

	/// numeric conversion ////////////////////////////////////////////////////
	unsigned int
		stou(std::wstring const &str, size_t * idx = 0, int base = 10);
	unsigned int
		stou(std::string const &str, size_t * idx = 0, int base = 10);

	/// to_string, to_wstring /////////////////////////////////////////////////
	std::wstring 
		to_wstring(bool b);
	std::wstring
		to_wstring(std::string const&);
	std::string
		to_string(std::wstring const&);

	template <typename T> std::wstring 
		to_wstring(T const& C, std::wstring const& delim = L",")
	{
		std::wstring ret = L"";
		for (auto const &it : C){
			ret += (std::to_wstring(it) + delim);
		}
		if (!ret.empty())  ret.erase(ret.end() - delim.size(), ret.end());
		return ret;
	}
	
	template <typename T> std::string 
		to_string(T const& C, std::string const& delim = ",")
	{
		std::string ret = "";
		for (auto const &it : C){
			ret += std::to_string(it) + delim;
		}
		if (!ret.empty())  ret.erase(ret.end() - delim.size(), ret.end());
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
		if (!ret.empty())  ret.erase(ret.end() - delim.size(), ret.end());
		return ret;
	}
	inline std::string
		to_string(STR_SET const& C, std::string const& delim = ",")
	{
		std::string ret = "";
		for (auto const &it : C){
			ret += it + delim;
		}
		if (!ret.empty())  ret.erase(ret.end() - delim.size(), ret.end());
		return ret;
	}

	/// Add padding //////////////////////////////////////////////////////////
	template <typename T> std::wstring 
		to_zeropadded_w(T const& item, size_t length){
		// This will only work for numeric types and compilation will fail otherwise
		auto max = std::numeric_limits<T>::max();
		
		std::wstring ret = std::to_wstring(item);
		if (ret.size() < length)
			ret.insert(0, length - ret.size(), L'0');
		return ret;
	}
	template <typename T> std::string 
		to_zeropadded(T const& item, size_t length){
		// This will only work for numeric types and compilation will fail otherwise
		auto max = std::numeric_limits<T>::max();

		std::string ret = std::to_wstring(item);
		if (ret.size() < length)
			ret.insert(0, length - ret.size(), '0');
		return ret;
	}

	/// repeat ////////////////////////////////////////////////////////////////
	template <typename T> std::wstring 
		repeat(T const& item, size_t count)
	{
		std::wstring ret{ L"" };
		for (size_t i = 0; i < count; i++){
			ret += to_wstring(item);
		}
		return ret;
	}
	inline std::wstring 
		repeat(std::wstring const& item, size_t count)
	{
		std::wstring ret{ L"" };
		for (size_t i = 0; i < count; i++){
			ret += item;
		}
		return ret;
	}
	template <typename T> std::string 
		repeat(T const& item, size_t count)
	{
		std::string ret{ L"" };
		for (size_t i = 0; i < count; i++){
			ret += to_string(item);
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

	/// Is_power_of ///////////////////////////////////////////////////////////
	template <typename T> bool
		is_power_of(T value, T base)
	{
		auto comp = base;
		while (comp < value){
			comp *= base;
		}
		return (comp == value);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// Generates series of random data
class RandomData
{
private:
	std::default_random_engine generator;

public:
	int
		GetARandomNum(
		int lowest = std::numeric_limits<int>::min(),
		int highest = std::numeric_limits<int>::max());

	std::vector<int>
		Generate(
		size_t total,
		int lowest = std::numeric_limits<int>::min(),
		int highest = std::numeric_limits<int>::max());
	std::vector<unsigned int>
		Generate(
		size_t total,
		unsigned int lowest = std::numeric_limits<unsigned int>::min(),
		unsigned int highest = std::numeric_limits<unsigned int>::max());
	size_t
		Generate(
		std::vector<int>& list,
		size_t total,
		int lowest = std::numeric_limits<int>::min(),
		int highest = std::numeric_limits<int>::max());
	size_t
		Generate(
		std::vector<unsigned int>& list,
		size_t total,
		unsigned int lowest = std::numeric_limits<unsigned int>::min(),
		unsigned int highest = std::numeric_limits<unsigned int>::max());
	size_t
		Generate(
		std::vector<wchar_t>& list,
		size_t total);
	size_t
		Generate(
		std::vector<std::wstring> &list,
		size_t total,
		int maxlength);
	std::wstring
		Generate(
		size_t length,
		wchar_t low,
		wchar_t high);

private:
	void
		CreateGenerator(
		std::vector<int> seed_init = {},
		std::initializer_list<int> seed_init_seq = {});
	void
		GetRandomValues(
		std::vector<int>& randomvalues,
		size_t total,
		int lowest = std::numeric_limits<int>::min(),
		int highest = std::numeric_limits<int>::max());
	void
		GetRandomValues(
		std::vector<unsigned int>& randomvalues,
		size_t total,
		unsigned int lowest = std::numeric_limits<unsigned int>::min(),
		unsigned int highest = std::numeric_limits<unsigned int>::max());
};

///////////////////////////////////////////////////////////////////////////////
namespace Utils
{
	enum STATUS_UTILITY 
	{
		SUTIL_START = -1,
		SUTIL_SUCCESS, SUTIL_FAILURE, SUTIL_NOT_IMPLEMENTED,
		SUTIL_BAD_PARAM, SUTIL_MEMORY_ALLOC_FAIL,
		SUTIL_BAD_INPUT, SUTIL_UNKNOWN_FAILURE,
		SUTIL_END
	};

	// Compare functions for use in qsort
	int CmpFuncInt(const void *a, const void *b);
	int CmpFuncChar(const void *a, const void *b);
	int CmpFuncStr(const void *a, const void *b);

	// Compare two equal length lists
	bool CmpEqLenList(int *List1, int* List2, size_t ListLength);
	bool CmpEqLenList(char **List1, char** List2, size_t ListLength);

	// Print list
	void printlist(int *list, unsigned int low, unsigned int high);
	void printlist(char *list, unsigned int low, unsigned int high);
	void printlist(char **list, unsigned int low, unsigned int high);

	// Swapping two values
	void swapvalues(char * first, char * second);
	void swapvalues(int * first, int * second);

	//Allocate space equal to the length of value and assign it to target.
	//Deallocation should be handled by caller.
	STATUS_UTILITY 
		AllocateAndAssignCString(char **target, char *value, unsigned int maxsize = SIZE_MAX);
	STATUS_UTILITY 
		DeallocateCStringArray(char *target[], unsigned int length = 0);

	///////////////////////////////////////////////////////////////////////////
	int 
		StringToWString(const std::string &s, std::wstring &ws);
	int 
		WStringToString(const std::wstring &ws, std::string &s);

	///////////////////////////////////////////////////////////////////////////
	std::time_t 
		systemtime_now(void);
	std::wstring 
		put_time(const std::tm* date_time, const wchar_t* c_time_format);
	std::wstring
		date_text(time_t timer);
}