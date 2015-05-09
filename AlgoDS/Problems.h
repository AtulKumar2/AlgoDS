#pragma once
#include "CommonDefs.h"
#include "Mathematics.h"

////////////////////////////////////////////////////////////////////////////////
enum class CLRS4_Probs
{
	MAXSUBARR_BF_413, MAXSUBARR_R_413,
	MAXSUBARR_MIX_413, MAXSUBARR_CMP_413,
	MAXSUBARR_LT_415,
	MATRIXMULT_BF, MATRIXMULT_STRASSEN, MATRIXMULT_CMP,
	COMPLEXNUMMULT_BF, COMPLEXNUMMULT_KARATSUBA, COMPLEXNUMMULT_CMP,
	EXIT, END
};
std::wstring
to_wstring(CLRS4_Probs probs, bool longform = true);

Matrix
MatrixMultiplyStrassen(Matrix const& first, Matrix const& second);

class CLRS4
{
public:  // Problems
	std::pair < STATUS_GEN, MaxDiffPair >
		MaxSubArray413(CLRS4_Probs prob, PriceSeq const&, size_t basecasehint = 0);
	std::pair < STATUS_GEN, Matrix >
		MatrixMultiply(CLRS4_Probs prob, 
		Matrix const& first, Matrix const& second, 
		size_t strassen_base_case_size = default_strassen_base_case_size());
	ComplexNum
		ComplexNumMult(CLRS4_Probs, ComplexNum const&, ComplexNum const&);

	static size_t 
		default_strassen_base_case_size(){ return DEFAULT_STARSSEN_BASE_CASE_SIZE; }

private:
	static const size_t DEFAULT_STARSSEN_BASE_CASE_SIZE = 256;

	template<typename T> void
		MaxSubArrayBF(T const& Prices,
		typename T::const_iterator & itbuy, typename T::const_iterator& itsell);

	MaxSubArrayInfo
		MaxSubArrayBF(INT_VEC const& variations, 
			size_t low, size_t high);
	MaxSubArrayInfo
		MaxSubArrayR(INT_VEC const& variations, 
			size_t low, size_t high, 
			size_t basecasehint = 0);
	MaxSubArrayInfo
		MaxCrossingSubArray(INT_VEC const& variations, 
			size_t low, size_t mid, 
			size_t high);
	MaxSubArrayInfo
		MaxSubArrayLT(INT_VEC const& variations, size_t low, size_t high);

	Matrix
		MatrixMultBF(Matrix const& A, Matrix const& B);
	Matrix
		MatrixMultStrassen(Matrix const& first, Matrix const& second,
						size_t strassen_base_case_size);
	Matrix
		MatrixMultStrassen_Rect(Matrix const& first, Matrix const& second,
		size_t strassen_base_case_size);

public:
	// Old abandoned ones
	STATUS_GEN
		MaxSubArrayBF413(MaxProfitPoints& maxpoints, DayPriceMap const& Prices);
	STATUS_GEN
		MaxSubArrayBF413(MaxProfitPoints& maxpoints, DatePriceMap const& Prices);
	STATUS_GEN
		MaxSubArrayR413(MaxProfitPoints& maxpoints, DayPriceMap const & Prices);
	STATUS_GEN
		MaxSubArrayR413(MaxProfitPoints& maxpoints, DatePriceMap const& Prices);
	STATUS_GEN
		MaxSubArrayMix413(MaxProfitPoints& maxpoints, DayPriceMap const& Prices, size_t basecase);
};

///////////////////////////////////////////////////////////////////////////////
enum class Palindrome_Probs
{
	IS_PALIN, COUNT_ALL_BF, COUNT_ALL_Q, COUNT_ALL_CMP,
	EXIT, END
};
std::wstring to_wstring(Palindrome_Probs prob);

// http://finding-palindromes.blogspot.in/2012/05/finding-palindromes-efficiently.html
// http://johanjeuring.blogspot.in/2007/08/finding-palindromes.html
// http://algs4.cs.princeton.edu/53substring/Manacher.java.html
// http://en.wikipedia.org/wiki/Longest_palindromic_substring
// http://www.akalin.cx/longest-palindrome-linear-time
// https://zhuhongcheng.wordpress.com/2009/08/02/a-simple-linear-time-algorithm-for-finding-longest-palindrome-sub-string/
// http://stackoverflow.com/questions/3647453/counting-palindromic-substrings-in-on
// http://stackoverflow.com/questions/1115001/write-a-function-that-returns-the-longest-palindrome-in-a-given-string
class Palindrome
{
public:
	std::pair<unsigned int, std::set<std::wstring>>
		count_all_bf(std::wstring const& text, bool getpalindromelist = false);
	std::pair<unsigned int, std::set<std::wstring>>
		count_all_q(std::wstring const& text, bool getpalindromelist = false);
	bool 
		is_palindrome(std::wstring const& text);
};

///////////////////////////////////////////////////////////////////////////////
enum class SORTING_Probs
{
	START,
	QUICK = START, INSERTION, INSERTION_R, MERGE, BUBBLE,
	EXIT, END
};

class Sorting
{
public:
	Sorting(TRACE_STATE = TRACE_STATE::TRACE_NO);

	STATUS_GEN BubbleSort(int * list, int low, int high);
	STATUS_GEN BubbleSort(char * list, int low, int high){ return STATUS_GEN::UNIMPLEMENTED; }
	STATUS_GEN BubbleSort(char ** list, int low, int high){ return STATUS_GEN::UNIMPLEMENTED; }

	STATUS_GEN InsSort(int * list, int low, int high);
	STATUS_GEN InsSort(char * list, int low, int high){ return STATUS_GEN::UNIMPLEMENTED; }
	STATUS_GEN InsSort(char ** list, int low, int high){ return STATUS_GEN::UNIMPLEMENTED; }

	STATUS_GEN InsSortR(int * list, int low, int high);
	STATUS_GEN InsSortR(char * list, int low, int high){ return STATUS_GEN::UNIMPLEMENTED; }
	STATUS_GEN InsSortR(char ** list, int low, int high){ return STATUS_GEN::UNIMPLEMENTED; }

	STATUS_GEN QuickSort(int * list, int low, int high);
	STATUS_GEN QuickSort(char * list, int low, int high){ return STATUS_GEN::UNIMPLEMENTED; }
	STATUS_GEN QuickSort(char ** list, int low, int high){ return STATUS_GEN::UNIMPLEMENTED; }

	STATUS_GEN MergeSort(int * list, int low, int high);
	STATUS_GEN MergeSort(intmax_t * list, int low, int high){ return STATUS_GEN::UNIMPLEMENTED; }
	STATUS_GEN MergeSort(char * list, int low, int high){ return STATUS_GEN::UNIMPLEMENTED; }
	STATUS_GEN MergeSort(char ** list, int low, int high){ return STATUS_GEN::UNIMPLEMENTED; }

private:

	int QuickSort_Partition(int * list, int low, int high);
	int QuickSort_Partition(char * list, int low, int high){ return (int)STATUS_GEN::UNIMPLEMENTED; }

	int QuickSort_PivotIndex(int low, int high);

	STATUS_GEN Merge(int * list, int low, int mid, int high);
	STATUS_GEN Merge(intmax_t * list, int low, int mid, int high){ return STATUS_GEN::UNIMPLEMENTED; }
	STATUS_GEN Merge(char * list, int low, int mid, int high){ return STATUS_GEN::UNIMPLEMENTED; }
	STATUS_GEN Merge(char ** list, int low, int mid, int high){ return STATUS_GEN::UNIMPLEMENTED; }

private:
	TRACE_STATE m_tracestate;
};
///////////////////////////////////////////////////////////////////////////////
enum class SEARCH_Probs
{
	START, LINEAR = START, BINARY, EXIT, END
};
class Search
{
public:
	int LinearSearch(int item, int* list, unsigned int list_length);
	int LinearSearch(intmax_t item, intmax_t *list, unsigned int list_length);
	int LinearSearch(char item, char* list, unsigned int list_length);
	int LinearSearch(char *item, int item_length, char **list, unsigned int list_length)
	{
		return NOT_IMPLEMENTED;
	}

	int BinSearch(int item, int* list, unsigned int low, unsigned int high);
	int BinSearch(char item, char* list, unsigned int low, unsigned int high)
	{
		return NOT_IMPLEMENTED;
	}
	int BinSearch(char *item, unsigned int item_length, char **list, unsigned int low, unsigned int high)
	{
		return NOT_IMPLEMENTED;
	}

	STATUS_GEN getlasterror();

private:
	STATUS_GEN lasterror = STATUS_GEN::SUCCESS;
};

///////////////////////////////////////////////////////////////////////////////
enum class BINOPS_Probs 
{ 
	ADD, EXIT, END 
};
std::wstring to_wstring(BINOPS_Probs, bool longform = true);

class BinOps
{
public:
	UINT_VEC
		AddBinArrays(UINT_VEC num1, UINT_VEC num2);
};