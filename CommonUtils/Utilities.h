#ifdef COMMONUTILS_EXPORTS
#define COMMONUTILS_API __declspec(dllexport) 
#else
#define COMMONUTILS_API __declspec(dllimport) 
#endif
#pragma once

////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <random>
#include <algorithm>
#include <limits>

////////////////////////////////////////////////////////////////////////////////
namespace utils
{

enum class STATUS_UTIL
{
	SUCCESS, FAILURE, NOT_IMPLEMENTED,
	BAD_PARAM, MEMORY_ALLOC_FAIL,
	BAD_INPUT, UNKNOWN_FAILURE,
	END
};

// Compare functions for use in qsort
COMMONUTILS_API int 
	CmpFuncInt(const void *a, const void *b);
COMMONUTILS_API int 
	CmpFuncChar(const void *a, const void *b);
COMMONUTILS_API int 
	CmpFuncStr(const void *a, const void *b);

// Compare two equal length lists
COMMONUTILS_API bool 
	CmpEqLenList(int *List1, int* List2, size_t ListLength);
COMMONUTILS_API bool 
	CmpEqLenList(char **List1, char** List2, size_t ListLength);

// Print list
COMMONUTILS_API void 
	printlist(int *list, unsigned int low, unsigned int high);
COMMONUTILS_API void 
	printlist(char *list, unsigned int low, unsigned int high);
COMMONUTILS_API void 
	printlist(char **list, unsigned int low, unsigned int high);

// Swapping two values
COMMONUTILS_API void 
	swapvalues(char * first, char * second);
COMMONUTILS_API void 
	swapvalues(int * first, int * second);

///////////////////////////////////////////////////////////////////////////
//Allocate space equal to the length of value and assign it to target.
//Deallocation should be handled by caller.
COMMONUTILS_API STATUS_UTIL
	AllocateAndAssignCString(char **target, char *value, unsigned int maxsize = SIZE_MAX);
COMMONUTILS_API STATUS_UTIL
	DeallocateCStringArray(char *target[], unsigned int length = 0);

///////////////////////////////////////////////////////////////////////////
COMMONUTILS_API int
	StringToWString(std::string const& s, std::wstring & ws);
COMMONUTILS_API int
	WStringToString(std::wstring const& ws, std::string & s);

///////////////////////////////////////////////////////////////////////////
COMMONUTILS_API std::time_t 
	systemtime_now(void);
COMMONUTILS_API std::wstring
	put_time(const std::tm* date_time, const wchar_t* c_time_format);
COMMONUTILS_API std::wstring
	date_text(time_t timer);

////////////////////////////////////////////////////////////////////////////
COMMONUTILS_API void 
	clear_wcin();
 
///////////////////////////////////////////////////////////////////////////////
/// Generates series of random data
class COMMONUTILS_API RandomData
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

}