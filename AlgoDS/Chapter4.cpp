#include "stdafx.h"
#include <limits>
#include <array>
#include <utility>
#include <cmath>
#include "Utilities.h"
#include "estd.h"
#include "CommonDefs.h"
#include "TestHarness.h"
#include "Mathematics.h"
#include "Problems.h"

///////////////////////////////////////////////////////////////////////////////
PriceItem::PriceItem() 
		: PriceItem(0, 0){}
PriceItem::PriceItem(unsigned int a_seq, unsigned int a_price) 
		: seq(a_seq), price(a_price){}
PriceItem::PriceItem(PriceItem const& src){ *this = src; }
PriceItem& 
PriceItem::operator=(PriceItem const& src)
{
	if (this != &src){
		seq = src.seq;
		price = src.price;
	}
	return *this;
}
bool 
PriceItem::operator<(PriceItem const& second)
{
	return (this->seq < second.seq);
}
bool 
PriceItem::operator==(PriceItem const& second)
{
	return ((this->seq == second.seq) && 
			(this->price == second.price));
}

std::wstring 
to_wstring(PriceItem const& price_item)
{
	return (std::to_wstring(price_item.seq) + L" | " 
			+ std::to_wstring(price_item.price));
}

///////////////////////////////////////////////////////////////////////////////
std::wstring to_wstring(MaxDiffPair const& diff_pair)
{
	return (L"First " + to_wstring(diff_pair.first) + L"\n" + 
			L"Second " + to_wstring(diff_pair.last));
}

///////////////////////////////////////////////////////////////////////////////
std::wstring
to_wstring(CLRS4_Probs probs, bool longform)
{
	switch (probs)
	{
	case CLRS4_Probs::MAXSUBARR_BF_413: 
		return{ (longform ? L"MAXSUBARR_BF_413" : L"413BF") };
	case CLRS4_Probs::MAXSUBARR_R_413: 
		return{ (longform ? L"MAXSUBARR_R_413" : L"413R") };
	case CLRS4_Probs::MAXSUBARR_MIX_413: 
		return{ (longform ? L"MAXSUBARR_MIX_413" : L"413MIX") };
	case CLRS4_Probs::MAXSUBARR_CMP_413: 
		return{ (longform ? L"MAXSUBARR_CMP_413" : L"413CMP") };
	case CLRS4_Probs::MAXSUBARR_LT_415: 
		return{ (longform ? L"MAXSUBARR_LT_415" : L"415LT") };

	case CLRS4_Probs::MATRIXMULT_BF: 
		return{ (longform ? L"MATRIXMULT_BF" : L"MATMULTBF") };
	case CLRS4_Probs::MATRIXMULT_STRASSEN: 
		return{ (longform ? L"MATRIXMULT_STRA" : L"MATMULTSTRA") };
	case CLRS4_Probs::MATRIXMULT_CMP: 
		return{ (longform ? L"MATRIXMULT_CMP" : L"MATMULTCMP") };

	case CLRS4_Probs::COMPLEXNUMMULT_BF: 
		return{ (longform ? L"COMPLEXNUMMULT_BF" : L"CNUMMULTBF") };
	case CLRS4_Probs::COMPLEXNUMMULT_KARATSUBA: 
		return{ (longform ? L"COMPLEXNUMMULT_KARATSUBA" : L"CNUMMULTKARATSUBA") };
	case CLRS4_Probs::COMPLEXNUMMULT_CMP: 
		return{ (longform ? L"COMPLEXNUMMULT_CMP" : L"CNUMMULTCMP") };

	case CLRS4_Probs::EXIT:
		return{ (longform ? L"Exit" : L"Ex") };
	case CLRS4_Probs::END:
	default:
		return{ L"" }; break;
	}
}
bool
IsMaxSubArrayProblem(CLRS4_Probs prob)
{
	return (
		CLRS4_Probs::MAXSUBARR_BF_413 == prob ||
		CLRS4_Probs::MAXSUBARR_R_413 == prob ||
		CLRS4_Probs::MAXSUBARR_MIX_413 == prob ||
		CLRS4_Probs::MAXSUBARR_CMP_413 == prob ||
		CLRS4_Probs::MAXSUBARR_LT_415 == prob
		);
}
bool
IsMatrixMultProblem(CLRS4_Probs prob)
{
	return (
		CLRS4_Probs::MATRIXMULT_BF == prob ||
		CLRS4_Probs::MATRIXMULT_STRASSEN == prob ||
		CLRS4_Probs::MATRIXMULT_CMP == prob
		);
}
bool
IsComplexNumMultProblem(CLRS4_Probs prob)
{
	return (
		CLRS4_Probs::COMPLEXNUMMULT_BF == prob ||
		CLRS4_Probs::COMPLEXNUMMULT_KARATSUBA == prob ||
		CLRS4_Probs::COMPLEXNUMMULT_CMP == prob
		);
}

INT_VEC GetVariations(PriceSeq const& prices)
{
	std::vector<int> variations;

	if (prices.size() >= 2){
		auto it = prices.begin();
		++it;
		while (it != prices.end()){
			auto prev = it;
			variations.push_back((it++)->price - (--prev)->price);
		}
	}
	return variations;
}

///////////////////////////////////////////////////////////////////////////////
/// CLRS_Ch4 Problems / Algorithms
///////////////////////////////////////////////////////////////////////////////
std::pair < STATUS_GEN, Matrix >
CLRS4::MatrixMultiply(CLRS4_Probs prob, 
Matrix const& first, Matrix const& second, 
unsigned int strassen_base_case_size)
{
	if (!first.is_legal() || !second.is_legal())
		return{ STATUS_GEN::PROB_BAD_IP, {} };
	if (first.m_columns != second.m_rows)
		return{ STATUS_GEN::PROB_BAD_IP, {} };

	Matrix result;

	switch (prob)
	{
	case CLRS4_Probs::MATRIXMULT_BF:
		result = MatrixMultBF(first.m_store, second.m_store);
		break;
	case CLRS4_Probs::MATRIXMULT_STRASSEN:
	{
		// (N,N) multiplied by (N,N)
		if (first.is_square() && second.is_square() && 
			(first.m_rows == second.m_rows))
		{
			if (!estd::is_power_of(first.m_rows, 2u)){
				return{ STATUS_GEN::PROB_BAD_IP, {} };
			}
			result = MatrixMultStrassen(first, second, strassen_base_case_size);
		} 
		// (kN,N) multiplied by (N,kN)
		else if ((first.m_rows == second.m_columns) && 
			(first.m_rows > first.m_columns) &&
			((first.m_rows % first.m_columns) == 0))
		{
			if (!estd::is_power_of(first.m_columns, 2u)){
				return{ STATUS_GEN::PROB_BAD_IP, {} };
			}
			result = MatrixMultStrassen_Rect(first, second, strassen_base_case_size);
		}
		// (N,kN) multiplied by (kN,N)
		else if ((first.m_rows == second.m_columns) &&
			(first.m_rows < first.m_columns) &&
			((first.m_columns % first.m_rows) == 0))
		{
			if (!estd::is_power_of(first.m_rows, 2u)){
				return{ STATUS_GEN::PROB_BAD_IP, {} };
			}
			result = MatrixMultStrassen_Rect(first, second, strassen_base_case_size);
		}
		else
		{
			return{ STATUS_GEN::PROB_BAD_IP, {} };
		}
		
	}
	break;
	case CLRS4_Probs::EXIT:
	case CLRS4_Probs::END:
	default:
		result = {};
		break;
	}
	if (result.m_store.size() != 0){
		return{ STATUS_GEN::SUCCESS, result };
	}
	else
		return{ STATUS_GEN::PROB_BAD_IP, {} };
}
Matrix
CLRS4::MatrixMultBF(Matrix const& A, Matrix const& B)
{
	size_t rows = A.m_rows, cols = B.m_columns;
	Matrix result{ rows, cols };

	for (size_t r = 0; r < rows; r++)
	{
		for (size_t c = 0; c < cols; c++)
		{
			auto value = 0;
			for (size_t row2 = 0; row2 < B.m_store.size(); row2++)
			{
				value += A.m_store[r][row2] * B.m_store[row2][c];
			}
			result.m_store[r][c] = value;
		}
	}
	return result;
}

Matrix
CLRS4::MatrixMultStrassen(
Matrix const& A, Matrix const& B, 
size_t strassen_base_case_size)
{
	//http://martin-thoma.com/strassen-algorithm-in-python-java-cpp/#tocAnchor-1-5
	/*if (1 == A.m_rows){
		return{ 1, 1, A.store[0][0] * B.store[0][0] };
	}*/

	if (strassen_base_case_size >= A.m_rows){
		return MatrixMultBF(A, B);
	}
	size_t mid = A.m_rows / 2;
	//std::vector<std::vector<MatrixPoint>> quadpts
	//{
	//	{ {}, {}, {} },						// Dummy
	//	{ {}, { 0, 0 }, { 0, mid } },		// 1, 1 // 1, 2
	//	{ {}, { mid, 0 }, { mid, mid } },	// 2, 1 // 2, 2	
	//};
	//S1 = B[1, 2] - B[2, 2]; 
	//S2 = A[1, 1] + A[1, 2]; 
	//S3 = A[2, 1] + A[2, 2]; 
	//S4 = B[2, 1] - B[1, 1]; 
	//S5 = A[1, 1] + A[2, 2]; 
	//S6 = B[1, 1] + B[2, 2]; 
	//S7 = A[1, 2] - A[2, 2]; 
	//S8 = B[2, 1] + B[2, 2]; 
	//S9 = A[1, 1] - A[2, 1]; 
	//S10 = B[1, 1] + B[1, 2];
	//auto S1 = B.sub_quads({0, mid}, {mid, mid});
	//auto S2 = A.add_quads({0, 0},	{0, mid});
	//auto S3 = A.add_quads({mid, 0}, {mid, mid});
	//auto S4 = B.sub_quads({mid, 0}, {0, 0});
	//auto S5 = A.add_quads({0, 0},	{mid, mid});
	//auto S6 = B.add_quads({0, 0},	{mid, mid});
	//auto S7 = A.sub_quads({0, mid}, {mid, mid});
	//auto S8 = B.add_quads({mid, 0}, {mid, mid});
	//auto S9 = A.sub_quads({0, 0},	{mid, 0});
	//auto S10 = B.add_quads({0, 0},	{0, mid});

	/*
	P1 = A[1, 1] * S1		= A[1, 1] * B[1, 2] - A[1, 1] * B[2, 2];
	P2 = S2 	 * B[2, 2]	= A[1, 1] * B[2, 2] + A[1, 2] * B[2, 2];
	P3 = S3 	 * B[1, 1]	= A[2, 1] * B[1, 1] + A[2, 2] * B[1, 1];
	P4 = A[2, 2] * S4		= A[2, 2] * B[2, 1] - A[2, 2] * B[1, 1];
	P5 = S5 	 * S6		= A[1, 1] * B[1, 1] + A[1, 1] * B[2, 2] + A[2, 2] * B[1, 1] + A[2, 2] * B[2, 2];
	P6 = S7 	 * S8		= A[1, 2] * B[2, 1] + A[1, 2] * B[2, 2] - A[2, 2] * B[2, 1] - A[2, 2] * B[2, 2];
	P7 = S9 	 * S10		= A[1, 1] * B[1, 1] + A[1, 1] * B[1, 2] - A[2, 1] * B[1, 1] - A[2, 1] * B[1, 2];*/
	Matrix P1 = MatrixMultStrassen( A.square_quadrant(MatrixPoint{ 0, 0 }), 
		B.sub_quads(MatrixPoint{ 0, mid }, MatrixPoint{ mid, mid }), 
		strassen_base_case_size);
	Matrix P2 = MatrixMultStrassen( A.add_quads(MatrixPoint{ 0, 0 }, MatrixPoint{ 0, mid }), 
		B.square_quadrant(MatrixPoint{ mid, mid }), strassen_base_case_size);
	Matrix P3 = MatrixMultStrassen(	A.add_quads(MatrixPoint{ mid, 0 }, 
		MatrixPoint{ mid, mid }), B.square_quadrant(MatrixPoint{ 0, 0 }), strassen_base_case_size);
	Matrix P4 = MatrixMultStrassen(A.square_quadrant(MatrixPoint{ mid, mid }),
		B.sub_quads(MatrixPoint{ mid, 0 }, MatrixPoint{ 0, 0 }), strassen_base_case_size);
	Matrix P5 = MatrixMultStrassen( A.add_quads(MatrixPoint{ 0, 0 }, MatrixPoint{ mid, mid }), 
		B.add_quads(MatrixPoint{ 0, 0 }, MatrixPoint{ mid, mid }), strassen_base_case_size);
	Matrix P6 = MatrixMultStrassen(	A.sub_quads(MatrixPoint{ 0, mid }, MatrixPoint{ mid, mid }), 
		B.add_quads(MatrixPoint{ mid, 0 }, MatrixPoint{ mid, mid }), 
		strassen_base_case_size);
	Matrix P7 = MatrixMultStrassen( A.sub_quads(MatrixPoint{ 0, 0 }, MatrixPoint{ mid, 0 }), 
		B.add_quads(MatrixPoint{ 0, 0 }, MatrixPoint{ 0, mid }), strassen_base_case_size);

	/*
	C[1, 1] = P5 + P4 - P2 + P6
	C[1, 2] = P1 + P2
	C[2, 1] = P3 + P4
	C[2, 2] = P5 + P1 - P3 - P7
	*/
	Matrix result{A.m_rows, A.m_rows};
	/*result.set_quad(MatrixPoint{ 0, 0 }, Matrix{ P5 + P4 - P2 + P6 });
	result.set_quad(MatrixPoint{ 0, mid }, Matrix{ P1 + P2 });
	result.set_quad(MatrixPoint{ mid, 0 }, Matrix{ P3 + P4 });
	result.set_quad(MatrixPoint{ mid, mid }, Matrix{ P5 + P1 - P3 - P7 });*/
	Matrix C11 = P5 + P4 - P2 + P6;
	Matrix C12 = P1 + P2;
	Matrix C21 = P3 + P4;
	Matrix C22 = P5 + P1 - P3 - P7;
	
	for (size_t r = 0; r < mid; r++)
	{
		std::copy(C11.m_store[r].begin(), C11.m_store[r].end(), result.m_store[r].begin());
		std::copy(C12.m_store[r].begin(), C12.m_store[r].end(), result.m_store[r].begin() + mid);
		std::copy(C21.m_store[r].begin(), C21.m_store[r].end(), result.m_store[mid + r].begin());
		std::copy(C22.m_store[r].begin(), C22.m_store[r].end(), result.m_store[mid + r].begin() + mid);
	}
	return result;
}

Matrix
CLRS4::MatrixMultStrassen_Rect(
Matrix const& first, Matrix const& second, 
size_t strassen_base_case_size)
{
	using std::vector;
	
	size_t dim_parts, tot_parts;
	vector<Matrix> parts_of_first, parts_of_second;

	// (kN,N) multiplied by (N,kN)
	if ((first.m_rows == second.m_columns) &&
		(first.m_rows > first.m_columns) &&
		((first.m_rows % first.m_columns) == 0))
	{
		dim_parts = first.m_columns;
		tot_parts = first.m_rows / first.m_columns;
		
		for (size_t i = 0; i < tot_parts; i++)
		{
			parts_of_first.push_back(
				first.square_sub_matrix(MatrixPoint{ i*dim_parts, 0 }, dim_parts));
		}
		for (size_t i = 0; i < tot_parts; i++)
		{
			parts_of_second.push_back(
				second.square_sub_matrix(MatrixPoint{ 0, i*dim_parts }, dim_parts));
		}

		Matrix result{ first.m_rows, second.m_columns };
		for (size_t i = 0; i < tot_parts; i++)
		{
			for (size_t j = 0; j < tot_parts; j++)
			{
				/*std::wcout << parts_of_first[i].to_wstring() << std::endl;
				std::wcout << parts_of_second[j].to_wstring() << std::endl;*/
				auto temp = MatrixMultStrassen(
					parts_of_first[i], parts_of_second[j], strassen_base_case_size);
				//std::wcout << temp.to_wstring() << std::endl;
				result.set_elements(temp, MatrixPoint{ i*dim_parts, j*dim_parts }, true);
				/*std::wcout << result.to_wstring() << std::endl;*/
			}
		}
		return result;
	}
	
	// (N,kN) multiplied by (kN,N)
	if ((first.m_columns == second.m_rows) &&
		(first.m_rows < first.m_columns) &&
		((first.m_columns % first.m_rows) == 0))
	{	
		size_t dim_parts = first.m_rows;
		size_t tot_parts = first.m_columns / first.m_rows;
		vector<Matrix> parts_of_first;
		for (size_t i = 0; i < tot_parts; i++)
		{
			parts_of_first.push_back(
				first.square_sub_matrix(MatrixPoint{ 0, i*dim_parts }, dim_parts));
		}
		vector<Matrix> parts_of_second;
		for (size_t i = 0; i < tot_parts; i++)
		{
			parts_of_second.push_back(
				second.square_sub_matrix(MatrixPoint{ i*dim_parts, 0 }, dim_parts));
		}

		Matrix result{ first.m_rows, second.m_columns };
		for (size_t i = 0; i < tot_parts; i++)
		{
			auto temp = MatrixMultStrassen(
				parts_of_first[i], parts_of_second[i], strassen_base_case_size);
			result = result + temp;
		}
		return result;
	}
	throw std::invalid_argument(estd::to_string(Problem_Res::E_PROB_INVALID_IP()));
}

std::pair<STATUS_GEN, MaxDiffPair>
CLRS4::MaxSubArray413(CLRS4_Probs prob, PriceSeq const& prices, size_t basecasehint)
{
	if (prices.size() < 2)
		return{ STATUS_GEN::BAD_PARAM, {} };
	
	auto variations = GetVariations(prices);

	MaxSubArrayInfo maxSubArray;
	switch (prob)
	{
	case CLRS4_Probs::MAXSUBARR_BF_413:
		maxSubArray = MaxSubArrayBF(variations, 0, variations.size() - 1); break;
	case CLRS4_Probs::MAXSUBARR_R_413:
		maxSubArray = MaxSubArrayR(variations, 0, variations.size() - 1, 0U); break;
	case CLRS4_Probs::MAXSUBARR_MIX_413:
		maxSubArray = MaxSubArrayR(variations, 0, variations.size() - 1, basecasehint); break;
	case CLRS4_Probs::MAXSUBARR_LT_415:
		maxSubArray = MaxSubArrayLT(variations, 0, variations.size() - 1); break;
	case CLRS4_Probs::MAXSUBARR_CMP_413:
	case CLRS4_Probs::EXIT:
	case CLRS4_Probs::END:
	default:
		return{ STATUS_GEN::BAD_PARAM, {} };
	}
	
	if (maxSubArray.low == 0 && maxSubArray.high == 0)
		return { STATUS_GEN::PROB_NO_SOLN, {} };

	MaxDiffPair result;
	auto it = prices.begin();
	for (size_t k = 0; k < maxSubArray.low; k++, ++it);
	result.first = *it;
	for (size_t k = maxSubArray.low; k <= maxSubArray.high; k++, ++it);
	result.last = *it;

	return{ STATUS_GEN::SUCCESS, result };
}

MaxSubArrayInfo
CLRS4::MaxSubArrayBF(INT_VEC const& variations, size_t low, size_t high)
{
	if (high == low)
		return (MaxSubArrayInfo{ low, high, variations[low] });

	MaxSubArrayInfo result;
	result.sum = 0;
	for (size_t i = low; i <= high; i++)
	{
		int t_sum = 0;
		for (size_t j = i; j <= high; j++)
		{
			t_sum += variations[j];
			if (t_sum > result.sum){
				result.low = i;
				result.high = j;
				result.sum = t_sum;
			}
		}
	}
	return result;
}

MaxSubArrayInfo
CLRS4::MaxSubArrayR(INT_VEC const& A, size_t low, size_t high, size_t basecasesize)
{
	if ((basecasesize == 0) && (high == low))
		return (MaxSubArrayInfo{ low, high, A[low] }); 	// base case: only one element
	if ((basecasesize != 0) && (high == (low + basecasesize - 1))){ // modified base case
		return MaxSubArrayBF(A, low, high);
	}

	size_t mid = (low + high) / 2;
	MaxSubArrayInfo left = MaxSubArrayR(A, low, mid);
	MaxSubArrayInfo right = MaxSubArrayR(A, mid + 1, high);
	MaxSubArrayInfo cross = MaxCrossingSubArray(A, low, mid, high);

	if ((left.sum >= right.sum) && (left.sum >= cross.sum))
		return left;
	else if ((right.sum >= left.sum) && (right.sum >= cross.sum))
		return right;
	else
		return cross;
}

MaxSubArrayInfo
CLRS4::MaxCrossingSubArray(INT_VEC const& A, size_t low, size_t mid, size_t high)
{
	size_t max_left = low, max_right = high;

	int sum = 0;
	int left_sum = std::numeric_limits<int>::min();
	for (size_t i = mid;
		(i != std::numeric_limits<size_t>::max()) && (i >= low);
		i--){
		sum += A[i];
		if (sum > left_sum){
			left_sum = sum;
			max_left = i;
		}
	}

	sum = 0;
	int right_sum = std::numeric_limits<int>::min();
	for (size_t j = mid + 1; j <= high; j++){
		sum += A[j];
		if (sum > right_sum){
			right_sum = sum;
			max_right = j;
		}
	}

	return (MaxSubArrayInfo{ max_left, max_right, left_sum + right_sum });
}

MaxSubArrayInfo
CLRS4::MaxSubArrayLT(INT_VEC const& A, size_t low, size_t high)
{
	if (high >= A.size())
		return{};
	
	size_t start_seq = low;
	while (start_seq <= high && A[start_seq] < 0){
		start_seq++;
	}
	if (start_seq > high)
		return{};

	int max_sum = A[start_seq];
	size_t start = start_seq;
	size_t end = start_seq;

	for (size_t i = start_seq + 1; i <= high; i++)
	{
		int sum_all = 0, sum_positive_seq = 0;
		size_t start_positive_seq = end + 1;

		for (size_t j = end + 1; j <= i; j++)
		{
			sum_all += A[j];
			sum_positive_seq += A[j];
			if (sum_positive_seq < 0){
				sum_positive_seq = 0;
				start_positive_seq = j + 1;
			}
		}
		if (sum_all > 0){
			max_sum += sum_all;
			end = i;
		}
		if (sum_positive_seq > max_sum){
			max_sum = sum_positive_seq;
			start = start_positive_seq;
			end = i;
		}
	}

	return{ start, end, max_sum };
}

STATUS_GEN
CLRS4::MaxSubArrayBF413(
MaxProfitPoints & MaxPoints, DayPriceMap const& Prices)
{ // Brute force approach to maximum subarray problem
	MaxPoints.Buy.dayordate = MaxPoints.Sell.dayordate = DayOrDate::END;
	if (Prices.size() < 2)
		return STATUS_GEN::PROB_BAD_IP;

	DayPriceMap::const_iterator itbuy = Prices.end(), itsell = Prices.end();

	MaxSubArrayBF<DayPriceMap>(Prices, itbuy, itsell);
	if ((Prices.end() == itbuy) || (Prices.end() == itsell))
		return STATUS_GEN::PROB_NO_SOLN;

	MaxPoints.Buy.dayordate = MaxPoints.Sell.dayordate = DayOrDate::DAY;
	MaxPoints.Buy.Day = itbuy->first;
	MaxPoints.Buy.Price = itbuy->second;

	MaxPoints.Sell.Day = itsell->first;
	MaxPoints.Sell.Price = itsell->second;

	return STATUS_GEN::SUCCESS;
}

STATUS_GEN
CLRS4::MaxSubArrayBF413(
MaxProfitPoints& MaxPoints, DatePriceMap const& Prices)
{ // Brute force approach to maximum subarray problem
	MaxPoints.Buy.dayordate = MaxPoints.Sell.dayordate = DayOrDate::END;
	if (Prices.size() < 2)
		return STATUS_GEN::PROB_BAD_IP;

	DatePriceMap::const_iterator itbuy = Prices.end(), itsell = Prices.end();
	MaxSubArrayBF<DatePriceMap>(Prices, itbuy, itsell);
	if ((Prices.end() == itbuy) || (Prices.end() == itsell))
		return STATUS_GEN::PROB_NO_SOLN;

	MaxPoints.Buy.dayordate = MaxPoints.Sell.dayordate = DayOrDate::DATE;
	MaxPoints.Buy.Date = itbuy->first;
	MaxPoints.Buy.Price = itbuy->second;

	MaxPoints.Sell.Date = itsell->first;
	MaxPoints.Sell.Price = itsell->second;

	return STATUS_GEN::SUCCESS;
}

STATUS_GEN
CLRS4::MaxSubArrayR413(
MaxProfitPoints& maxpoints, DayPriceMap const& Prices)
{
	if (Prices.size() < 2)
		return STATUS_GEN::PROB_BAD_IP;

	// {{1,1},{2,2},{3,4},{4,7}} => {1,2,3} 
	// Length one smaller than input
	auto Variations = GetDailyVariations(Prices);
	auto maxSubArray = MaxSubArrayR(Variations, 0, Variations.size() - 1);

	auto it = Prices.begin();

	for (size_t k = 0; k < maxSubArray.low; k++, ++it);
	maxpoints.Buy.Day = it->first;
	maxpoints.Buy.Price = it->second;

	for (size_t k = maxSubArray.low; k <= maxSubArray.high; k++, ++it);
	maxpoints.Sell.Day = it->first;
	maxpoints.Sell.Price = it->second;

	maxpoints.Buy.dayordate = maxpoints.Sell.dayordate = DayOrDate::DAY;

	return STATUS_GEN::SUCCESS;
}

STATUS_GEN
CLRS4::MaxSubArrayR413(
MaxProfitPoints& maxpoints, DatePriceMap const& Prices)
{
	if (Prices.size() < 2)
		return STATUS_GEN::PROB_BAD_IP;

	auto Variations = GetDailyVariations(Prices);
	auto maxSubArray = MaxSubArrayR(Variations, 0, Variations.size() - 1);

	auto it = Prices.begin();

	for (size_t k = 0; k < maxSubArray.low; k++, ++it);
	maxpoints.Buy.Date = it->first;
	maxpoints.Buy.Price = it->second;

	for (size_t k = maxSubArray.low; k <= maxSubArray.high; k++, ++it);
	maxpoints.Sell.Date = it->first;
	maxpoints.Sell.Price = it->second;
	
	maxpoints.Buy.dayordate = maxpoints.Sell.dayordate = DayOrDate::DATE;

	return STATUS_GEN::SUCCESS;
}

template<typename T>
void CLRS4::MaxSubArrayBF
(const T& Prices, 
typename T::const_iterator& itbuy, typename T::const_iterator& itsell)
{
	unsigned int diff = std::numeric_limits<unsigned int>::min();
	for (auto &buy = Prices.begin(); buy != --(Prices.end()); buy++){
		auto temp = buy;
		for (auto &sell = ++temp; sell != Prices.end(); sell++){
			if ((sell->second >= buy->second)
				&& ((sell->second - buy->second) >= diff))
			{
				diff = sell->second - buy->second;
				itbuy = buy;
				itsell = sell;
			}
		}
	}
	return;
}

#pragma optimize("", off)
ComplexNum
CLRS4::ComplexNumMult(CLRS4_Probs prob, 
ComplexNum const& A, ComplexNum const& B)
{
	// [Ar+i*Ai] * [Br+i*Bi] = [Ar*Br-Ai*Bi] + i*[Ar.Bi+Ai*Br]i
	switch (prob)
	{
	case CLRS4_Probs::COMPLEXNUMMULT_BF:
		return{ ((A.real*B.real) - (A.imaginary*B.imaginary)),
			((A.real*B.imaginary) + (A.imaginary*B.real)) };
	case CLRS4_Probs::COMPLEXNUMMULT_KARATSUBA:
	{
		int P1 = A.real*B.real;
		int P2 = A.imaginary*B.imaginary;
		int P3 = (A.real + A.imaginary)*(B.real + B.imaginary);

		return{ P1 - P2, P3 - P2 - P1 };
	}
	default:
		break;
	}
	
	throw std::invalid_argument(
		estd::to_string(Problem_Res::E_PROB_UNRECOGNIZED()));
}
#pragma optimize("", on)

///////////////////////////////////////////////////////////////////////////////
/// Test Harness for CLRS_Ch4
///////////////////////////////////////////////////////////////////////////////
CLRS4_Test::CLRS4_Test(ModuleData module_data)
	:TestBase(module_data)
{
	unsigned int MenuId = 1;
	m_TopMenuEntries.push_back(TopMenuItem(
		CLRS4_Probs::MAXSUBARR_BF_413, MenuId++, L"413BF", L"Max subarray Brute force"));
	m_TopMenuEntries.push_back(TopMenuItem(
		CLRS4_Probs::MAXSUBARR_R_413, MenuId++, L"413R", L"Max subarray Recurs"));
	m_TopMenuEntries.push_back(TopMenuItem(
		CLRS4_Probs::MAXSUBARR_CMP_413, MenuId++, L"413CMP", L"Max subarray Compare"));
	m_OperMenuEntries = OperandType().MenuEntries({OPERAND_TYPE::INT});

	// New type of menu
	auto menu_str = [](CLRS4_Probs prob){
		return MenuItemInfo{ to_wstring(prob), to_wstring(prob, false) };
	};
	m_subop_data = std::map<CLRS4_Probs, MenuItemInfo>{
		{ CLRS4_Probs::MAXSUBARR_BF_413, { menu_str(CLRS4_Probs::MAXSUBARR_BF_413) } },
		{ CLRS4_Probs::MAXSUBARR_R_413, { menu_str(CLRS4_Probs::MAXSUBARR_R_413) } },
		{ CLRS4_Probs::MAXSUBARR_MIX_413, { menu_str(CLRS4_Probs::MAXSUBARR_MIX_413) } },
		{ CLRS4_Probs::MAXSUBARR_CMP_413, { menu_str(CLRS4_Probs::MAXSUBARR_CMP_413) } },
		{ CLRS4_Probs::MAXSUBARR_LT_415, { menu_str(CLRS4_Probs::MAXSUBARR_LT_415) } },
		{ CLRS4_Probs::MATRIXMULT_BF, { menu_str(CLRS4_Probs::MATRIXMULT_BF) } },
		{ CLRS4_Probs::MATRIXMULT_STRASSEN, { menu_str(CLRS4_Probs::MATRIXMULT_STRASSEN) } },
		{ CLRS4_Probs::MATRIXMULT_CMP, { menu_str(CLRS4_Probs::MATRIXMULT_CMP) } },
		{ CLRS4_Probs::COMPLEXNUMMULT_BF, { menu_str(CLRS4_Probs::COMPLEXNUMMULT_BF) } },
		{ CLRS4_Probs::COMPLEXNUMMULT_KARATSUBA, { menu_str(CLRS4_Probs::COMPLEXNUMMULT_KARATSUBA) } },
		{ CLRS4_Probs::COMPLEXNUMMULT_CMP, { menu_str(CLRS4_Probs::COMPLEXNUMMULT_CMP) } },
		{ CLRS4_Probs::EXIT,				{L"Exit", L"Ex"} }
	};

	for (auto const &it : m_subop_data){
		m_top_menu.Add({ static_cast<unsigned int>(it.first) }, 
						{ it.second.longtext, it.second.shorttext });
	}
	UpdateOperandMenu({ OPERAND_TYPE::INT });
	
	// Test case settings
	m_verbose_output = false;
	m_summary_output = false;
	m_basecase_size = 0;
	m_op_repeatcount = 1;
}

STATUS_GEN
CLRS4_Test::Run()
{
	STATUS_GEN status = STATUS_GEN::BAD_INPUT;
	switch (m_module_data.runtype)
	{
	case RUNTYPE::ConsoleOps:
		{
		wchar_t dowerepeat = L'y';
		do
		{
			status = RunConsole();
			utils::clear_wcin();
			std::wcout << MenuMsgStr(m_module_data.majorops, MenuMsgOpts::REPEAT_MAJOROP);
			std::wcin >> dowerepeat;
		} while (L'y' == dowerepeat || L'Y' == dowerepeat);
		}
		break;
	case RUNTYPE::FileInput: status = RunFI(); break;
	case RUNTYPE::CommandLine: status = RunCL(); break;
	default:
		break;
	}
	return status;
}
STATUS_GEN
CLRS4_Test::Run(ModuleData module_data)
{
	m_module_data = module_data;
	return Run();
}

STATUS_GEN 
CLRS4_Test::RunFI()
{
	using namespace std;

	wstring 
		LogHint{ __FUNCTIONW__ },
		SSD{ Logger_Res::SubStrDelim() },
		OPCS{ Common_Res::COMMENT_START() },
		OPTD{ Common_Res::TAG_SEP() },
		OPTS{ Common_Res::TUPLE_SEP() };
	
	auto ipblockid = m_module_data.InputBlockId;
	LogHint += SSD + to_wstring(ipblockid);

	auto status = RetrieveFileData();
	if (STATUS_GEN::SUCCESS != status){
		WriteLog(LogHint + SSD + to_wstring(status), LogType::ERR);
		return status;
	}

	for (auto const& it: m_testcase_settings)
	{
		if (0 == estd::compare_ci(it.first, TestCaseSettings_Res::VERBOSE())){
			m_verbose_output = estd::any_of_ci(it.second, Common_Res::TRUE_MARKERS());
		}
		else if (0 == estd::compare_ci(it.first, TestCaseSettings_Res::SUMMARY_OUTPUT())){
			m_summary_output = estd::any_of_ci(it.second, Common_Res::TRUE_MARKERS());
		}
		else if (0 == estd::compare_ci(it.first, TestCaseSettings_Res::BESTCASE_SIZE())){
			m_basecase_size = estd::stou(it.second);
		}
		else if (0 == estd::compare_ci(it.first, TestCaseSettings_Res::REPEATOP_COUNT())){
			m_op_repeatcount = estd::stou(it.second);
		}
	}

	InputFileHandler ipFileHandler{ m_module_data.ipfilename };
	OutputHandler op_handler{ m_module_data.opfilename, OutputType::File };
	op_handler.Write(estd::repeat(OPCS, 80u) + L"\n");
	op_handler.Write(OPCS + to_wstring(m_module_data) + L"\n");

	// All problems must be of same type.
	std::vector<CLRS4_Probs> problems;
	bool MaxSubArrayProblems = false;
	bool MatrixMultProblems = false;
	bool ComplexNumMultProblems = false;

	for (auto const& it_subop : m_subops){
		for (auto const& it : m_subop_data){
			if (any_of_ci(it.second, it_subop)){
				problems.insert(problems.end(), it.first);
				MaxSubArrayProblems = IsMaxSubArrayProblem(it.first);
				MatrixMultProblems = IsMatrixMultProblem(it.first);
				ComplexNumMultProblems = IsComplexNumMultProblem(it.first);
				break;
			}
		}
	}

	if (problems.empty() || 
		!(MaxSubArrayProblems || MatrixMultProblems || ComplexNumMultProblems)){
		WriteLog(LogHint + SSD + to_wstring(STATUS_GEN::PROB_BAD_IP) + SSD +
			L"Bad Problem data", LogType::ERR);
		return STATUS_GEN::PROB_BAD_IP;
		// All problems must be of same type.
	}
	
	op_handler.Write(OPCS + 
		L"Problems" + OPTD + estd::to_wstring(m_subops) + OPTS + 
		L"Sample" + OPTD + estd::to_wstring(m_use_sample) + OPTS +
		L"Sample Default" + OPTD + estd::to_wstring(m_use_sample_default) + OPTS + L"\n");

	if (m_use_sample && m_use_sample_default)
	{
		CreateSample(problems[0]);
		for (auto const& problem : problems){
			op_handler.Write(OPCS +	SSD + to_wstring(problem) + L"\n");
			status = DoProblems(problem);
		}
	}
	if (!m_sample_hints.empty())
	{
		SampleData sampledata(GetHintSpecs(problems[0])); // All problems are of same type.
		for (auto const& it : m_sample_hints)
		{
			auto hintdata = sampledata.GetHintList(it);
			op_handler.WriteLine();
			op_handler.WriteLine(OPCS + L"Sample Criteria" + SSD + to_wstring(hintdata));

			CreateSample(problems[0], hintdata);
			for (auto const& problem : problems){
				op_handler.WriteLine(OPCS + SSD + to_wstring(problem));
				DoProblems(problem, hintdata);
			}
		}
	}
	if (m_operands.size() != 0)
	{
		if (ComplexNumMultProblems)
		{
			if (m_operands.size() != 2){
				WriteLog(LogHint + SSD + IpFile_Err::OPERAND_WRONG_NUMBERS(), 
					LogType::ERR);
				return STATUS_GEN::PROB_BAD_IP;
			}
			CleanOperandLists();
			for (size_t i = 0; i < 2; i++)
			{
				std::wstring LogHint_Ext = 
					LogHint + SSD + m_operands[i].second.Input();
				INT_VEC operand_tokens;
				status = m_operands[i].second.GetList(operand_tokens);
				if (STATUS_GEN::SUCCESS != status){
					WriteLog(LogHint_Ext + SSD + IpFile_Err::OPERAND_BAD_FORMAT()
						+ SSD + to_wstring(status), LogType::ERR);
					return status;
				}
				if (operand_tokens.size() != 2){
					WriteLog(LogHint_Ext + SSD + IpFile_Err::OPERAND_BAD_FORMAT(),
						LogType::ERR);
					return status;
				}
				m_complex_numbers.push_back({ operand_tokens[0], operand_tokens[1] });
			}
			DoProblems(problems[0]);
		}
	}

	return status;
}

STATUS_GEN 
CLRS4_Test::RunConsole()
{
	using namespace std;
	using namespace std::chrono;
	using Problems = CLRS4_Probs;

	UserMenu userMenu(m_module_data.majorops);
	auto problem = Problems::END;
	auto opertype = OperandType().InvalidType();
	STATUS_GEN status = STATUS_GEN::END;

	if ((Problems::EXIT == (problem = userMenu.TopMenuChoice(m_TopMenuEntries))) 
		|| (Problems::END == problem))
		return STATUS_GEN::SUCCESS;

	bool MaxSubArrayProblems = IsMaxSubArrayProblem(problem);

	if (MaxSubArrayProblems)
		opertype = OPERAND_TYPE::UINT;
	else if ((OPERAND_TYPE::EXIT == (opertype = userMenu.OperandMenuChoice(m_OperMenuEntries))) 
		|| !(OperandType().IsValidType(opertype)))
		return STATUS_GEN::SUCCESS;
	
	wchar_t dowerepeat = L'y';
	do{
		m_use_sample = m_use_sample_default = false;
		userMenu.SampleChoice(m_use_sample, m_use_sample_default);
		if (m_use_sample)
			GetSampleOperands(problem);
		else
			GetOperands(opertype);

		if (MaxSubArrayProblems)
			DoProblems(problem);

		utils::clear_wcin();
		wcout << MenuMsgStr(m_module_data.majorops, MenuMsgOpts::REPEAT_SUBOP);
		wcin >> dowerepeat;
	} while (L'y' == dowerepeat || L'Y' == dowerepeat);

	return STATUS_GEN::SUCCESS;
}

#pragma optimize("", off)
STATUS_GEN
CLRS4_Test::DoProblems(CLRS4_Probs problem, HintData_VEC const& hintdata)
{
	using namespace std::chrono;
	using namespace std;

	wstring LogHint{ __FUNCTIONW__ };
	auto SSD = Logger_Res::SubStrDelim();

	if ((IsMaxSubArrayProblem(problem) && m_ui_operands.empty()) ||
		(IsMatrixMultProblem(problem) && m_matrix.size() != 2) ||
		(IsComplexNumMultProblem(problem) && m_complex_numbers.size() != 2)
		)
	{
		return STATUS_GEN::PROB_BAD_IP;
	}

	OutputHandler op_handler;
	if (m_module_data.runtype == RUNTYPE::FileInput)
		op_handler = OutputHandler{ m_module_data.opfilename, OutputType::File };

	const std::wstring SS{ L" " };
	shared_ptr<estd::PrintTimer> time_printer{ new PrintExecutionTimer() };

	if (IsMaxSubArrayProblem(problem))
	{
		PriceSeq prices;
		for (size_t i = 1; i <= m_ui_operands.size(); i++)
			prices.insert(prices.end(), { i, m_ui_operands.at(i - 1) });

		if (m_verbose_output)
			op_handler.Write(L"Price sequence" + SSD + estd::to_wstring(m_ui_operands) + L"\n");

		if (CLRS4_Probs::MAXSUBARR_CMP_413 != problem)
		{
			estd::ExecutionTimer<high_resolution_clock, milliseconds>execution_timer(time_printer);
			execution_timer.set_start();
			auto max_diff = (problem == CLRS4_Probs::MAXSUBARR_MIX_413) ?
				CLRS4().MaxSubArray413(problem, prices, m_basecase_size) : CLRS4().MaxSubArray413(problem, prices);
			execution_timer.set_end();

			op_handler.Write(L"Max Profit Pair" + SSD + to_wstring(problem)
				+ SSD + to_wstring(max_diff.first)
				+ SSD + execution_timer.to_wstring() + L"\n");
			op_handler.Write(to_wstring(max_diff.second) + L"\n");
		}

		if (CLRS4_Probs::MAXSUBARR_CMP_413 == problem)
		{
			auto message = to_wstring(CLRS4_Probs::MAXSUBARR_CMP_413);
			for (auto prob : { CLRS4_Probs::MAXSUBARR_BF_413,
				CLRS4_Probs::MAXSUBARR_R_413,
				CLRS4_Probs::MAXSUBARR_MIX_413 })
			{
				estd::ExecutionTimer<high_resolution_clock, microseconds> 
					execution_timer(time_printer);
				execution_timer.set_start();
				auto max_diff = (prob == CLRS4_Probs::MAXSUBARR_MIX_413) ?
					CLRS4().MaxSubArray413(prob, prices, m_basecase_size) : CLRS4().MaxSubArray413(prob, prices);
				execution_timer.set_end();

				message += L"\t\n" + to_wstring(prob) + SSD + to_wstring(max_diff.first) + 
					SSD + execution_timer.to_wstring();
				message += L"\t\n" + to_wstring(max_diff.second);
			}
			op_handler.Write(message + L"\n");
		}
	}

	if (IsMatrixMultProblem(problem))
	{
		auto strassen_base_case_size = CLRS4::default_strassen_base_case_size();		
		for (auto const& it: hintdata){
			if (0 == estd::compare_ci(
				estd::trim(it.m_name), SData_Res::HintStrassenBaseSize())){
				strassen_base_case_size = it.m_uintval;
			}
		}
		if (m_verbose_output){
			op_handler.WriteLine(L"Matrix 1\n" + m_matrix[0].to_wstring(true));
			op_handler.WriteLine(L"Matrix 2\n" + m_matrix[1].to_wstring(true));
			op_handler.WriteLine(L"Strassen Base case size" + SSD + 
				std::to_wstring(strassen_base_case_size));
		}
	
		if (CLRS4_Probs::MATRIXMULT_BF == problem ||
			CLRS4_Probs::MATRIXMULT_STRASSEN == problem)
		{
			estd::ExecutionTimer<high_resolution_clock, microseconds> 
				execution_timer(time_printer);
			execution_timer.set_start();
			auto result = CLRS4().MatrixMultiply(problem, 
					m_matrix[0], m_matrix[1], strassen_base_case_size);
			execution_timer.set_end();

			op_handler.Write(L"Matrix multiplied" + SSD + to_wstring(problem)
				+ SSD + to_wstring(result.first)
				+ SSD + execution_timer.to_wstring() + L"\n");
			if (!m_summary_output)
				op_handler.Write(result.second.to_wstring(true) + L"\n");
			op_handler.Write(L"\n");
		}

		if (CLRS4_Probs::MATRIXMULT_CMP == problem)
		{
			vector<CLRS4_Probs> problems
			{ CLRS4_Probs::MATRIXMULT_BF, CLRS4_Probs::MATRIXMULT_STRASSEN };
			vector<pair<STATUS_GEN, Matrix>> results(problems.size());
			vector<estd::ExecutionTimer<high_resolution_clock, microseconds>>
				execution_timer(problems.size());

			for (size_t i = 0; i < problems.size(); i++)
			{
				execution_timer[i].set_start();
				results[i] = CLRS4().MatrixMultiply(problems[i], 
						m_matrix[0], m_matrix[1], strassen_base_case_size);
				execution_timer[i].set_end();
			}
			bool all_same_output = true;
			for (size_t i = 1; i < problems.size(); i++){
				if (results[i].second != results[i - 1].second)
					all_same_output = false;
			}
			wstring message{ all_same_output ? Common_Res::SAME_OP() : Common_Res::DIFF_OP() };

			for (size_t i = 0; i < problems.size(); i++)
			{
				message += L"\t\n" + to_wstring(problems[i]) + SSD + to_wstring(results[i].first) + SSD +
					execution_timer[i].to_wstring(time_printer);
				if (!all_same_output && !m_summary_output)
					message += L"\t\n" + results[i].second.to_wstring(true);
			}
			if (all_same_output && !m_summary_output)
				message += L"\t\n" + results[0].second.to_wstring(true);

			op_handler.WriteLine(message);
		}
	}

	if (IsComplexNumMultProblem(problem))
	{
		if (m_verbose_output){
			op_handler.WriteLine(L"Complex Number 1" + 
				SSD + m_complex_numbers[0].to_wstring());
			op_handler.WriteLine(L"Complex Number 2" + 
				SSD + m_complex_numbers[0].to_wstring());
		}

		if (CLRS4_Probs::COMPLEXNUMMULT_BF == problem ||
			CLRS4_Probs::COMPLEXNUMMULT_KARATSUBA == problem)
		{
			estd::ExecutionTimer<high_resolution_clock, milliseconds>
				execution_timer(time_printer);
			ComplexNum result, old_result;
			size_t cnt = 0;

			execution_timer.set_start();
			old_result = CLRS4().ComplexNumMult(
				problem, m_complex_numbers[0], m_complex_numbers[1]);
			for (; cnt < m_op_repeatcount; cnt++)
			{
				result = CLRS4().ComplexNumMult(
					problem, m_complex_numbers[0], m_complex_numbers[1]);
				if (result != old_result)
					break;
				else
					old_result = result;
			}
			execution_timer.set_end();

			op_handler.WriteLine(
				L"Complex Numbers multiplied" 
				+ SSD + to_wstring(problem)
				+ SSD + std::to_wstring(cnt) + SS + L"times"
				+ SSD + result.to_wstring()
				+ SSD + execution_timer.to_wstring()
			);
		}

		if (CLRS4_Probs::COMPLEXNUMMULT_CMP == problem)
		{
			vector<CLRS4_Probs> problems
			{ CLRS4_Probs::COMPLEXNUMMULT_BF, CLRS4_Probs::COMPLEXNUMMULT_KARATSUBA };
			vector<pair<size_t,ComplexNum>> results(problems.size());
			vector<estd::ExecutionTimer<high_resolution_clock, milliseconds>>
				execution_timer(problems.size());

			for (size_t i = 0; i < problems.size(); i++)
			{
				size_t cnt = 0;
				ComplexNum result, old_result;
				
				execution_timer[i].set_start();
				old_result = CLRS4().ComplexNumMult(
					problems[i], m_complex_numbers[0], m_complex_numbers[1]);
				for (; cnt < m_op_repeatcount; cnt++)
				{
					result = CLRS4().ComplexNumMult(
						problems[i], m_complex_numbers[0], m_complex_numbers[1]);
					if (result != old_result)
						break;
					else
						old_result = result;
				}
				results[i] = make_pair(cnt, result);
				execution_timer[i].set_end();
			}
			bool all_same_output = true;
			for (size_t i = 1; i < problems.size(); i++){
				if (results[i].second != results[i - 1].second)
					all_same_output = false;
			}
			wstring message{ all_same_output ? Common_Res::SAME_OP() : Common_Res::DIFF_OP() };

			for (size_t i = 0; i < problems.size(); i++)
			{
				message += L"\t\n" + to_wstring(problems[i]) + SSD + to_wstring(results[i].first) + SSD +
					execution_timer[i].to_wstring(time_printer);
				if (!all_same_output && !m_summary_output)
					message += L"\t\n" + results[i].second.to_wstring();
			}
			if (all_same_output && !m_summary_output)
				message += L"\t\n" + results[0].second.to_wstring();

			op_handler.WriteLine(message);
		}
	}

	return STATUS_GEN::SUCCESS;
}
#pragma optimize("", on)

STATUS_GEN 
CLRS4_Test::GetOperands(OPERAND_TYPE opertype)
{
	using namespace std;
	CleanOperandLists();

	if (RUNTYPE::ConsoleOps == m_module_data.runtype){
		UserMenu um(m_module_data.majorops);
		switch (opertype){
			case OPERAND_TYPE::UINT: um.GetOperands(m_ui_operands); break;
			case OPERAND_TYPE::INT:  um.GetOperands(m_ui_operands); break;
			case OPERAND_TYPE::CHAR: um.GetOperands(m_c_operands); break;
			case OPERAND_TYPE::STR:  um.GetOperands(m_str_operands); break;
			default: break;
		}
	}
	return STATUS_GEN::SUCCESS;
}

STATUS_GEN 
CLRS4_Test::GetSampleOperands(CLRS4_Probs problem, HintData_VEC const& hintdata)
{
	return STATUS_GEN::UNIMPLEMENTED;
}

STATUS_GEN
CLRS4_Test::CreateSample(CLRS4_Probs prob, HintData_VEC const& hintdata)
{
	using namespace std;

	if (!m_use_sample) // How the heck did we came here?
		return STATUS_GEN::PROB_NO_SOLN;
	CleanOperandLists();

	STATUS_GEN status = STATUS_GEN::SUCCESS;
	if (IsMaxSubArrayProblem(prob))
	{
		size_t s_tot = SampleData::Limits::maxsize_set();
		unsigned int
			s_low = std::numeric_limits<unsigned int>::min(),
			s_high = std::numeric_limits<unsigned int>::max() / 4; // To avoid overflow

		if (!hintdata.empty())
		{
			for (auto const & it : hintdata)
			{
				if(0 == estd::compare_ci(estd::trim(it.m_name), SData_Res::HintTotal()))
					s_tot = it.m_uintval;
				if (0 == estd::compare_ci(estd::trim(it.m_name), SData_Res::HintIntLow()))
					s_low = it.m_uintval;
				if (0 == estd::compare_ci(estd::trim(it.m_name), SData_Res::HintIntHigh()))
					s_high = it.m_uintval;
			}
		}
		utils::RandomData().Generate(m_ui_operands, s_tot, s_low, s_high);
	}

	if (IsMatrixMultProblem(prob))
	{
		MatrixSpecs matrix_specs[] = {
			{ MAT_MAXR, MAT_MAXC },
			{ MAT_MAXR, MAT_MAXC },
		};
		int		s_maxval = MAT_MAXVAL,
				s_minval = MAT_MINVAL; 

		for (auto const& it : hintdata)
		{
			if (0 == estd::compare_ci(estd::trim(it.m_name), SData_Res::HintMatrixRows())){
				matrix_specs[0].m_rows = matrix_specs[1].m_rows = it.m_uintval;
			}
			else if (0 == estd::compare_ci(estd::trim(it.m_name), SData_Res::HintMatrixCols())){
				matrix_specs[0].m_columns = matrix_specs[1].m_columns = it.m_uintval;
			}
			else if (0 == estd::compare_ci(estd::trim(it.m_name), SData_Res::HintMatrix1())){
				auto dims = estd::Tokenizer<wstring>(Common_Res::DIM_SEP(),
					estd::TokenizerOP::NONEMPTYTRIMMED).Tokens(it.m_textval);
				if (2 == dims.size()){
					try{
						matrix_specs[0].m_rows = estd::stou(dims[0]);
						matrix_specs[0].m_columns = estd::stou(dims[1]);
					}
					catch (...){
						matrix_specs[0].m_rows = matrix_specs[0].m_columns = 0;
						status = STATUS_GEN::BAD_SAMPLE_HINT;
						break;
					}
				}
				else{
					matrix_specs[0].m_rows = matrix_specs[0].m_columns = 0;
					status = STATUS_GEN::BAD_SAMPLE_HINT;
					break;
				}
			}
			else if (0 == estd::compare_ci(estd::trim(it.m_name), SData_Res::HintMatrix2())){
				auto dims = estd::Tokenizer<wstring>(Common_Res::DIM_SEP(),
					estd::TokenizerOP::NONEMPTYTRIMMED).Tokens(it.m_textval);
				if (2 == dims.size()){
					try{
						matrix_specs[1].m_rows = estd::stou(dims[0]);
						matrix_specs[1].m_columns = estd::stou(dims[1]);
					}
					catch (...){
						matrix_specs[1].m_rows = matrix_specs[1].m_columns = 0;
						status = STATUS_GEN::BAD_SAMPLE_HINT;
						break;
					}
				}
				else{
					matrix_specs[1].m_rows = matrix_specs[1].m_columns = 0;
					status = STATUS_GEN::BAD_SAMPLE_HINT;
					break;
				}
			}
			else if (0 == estd::compare_ci(estd::trim(it.m_name), 
				SData_Res::HintMaxValue())){
				s_maxval = it.m_uintval;
			}
			else if (0 == estd::compare_ci(estd::trim(it.m_name), 
				SData_Res::HintMinValue())){
				s_minval = it.m_uintval;
			}
		}
		
		if (STATUS_GEN::SUCCESS == status)
		{
			for (size_t i = 0; i < 2; i++)
			{
				Matrix sample_matrix(matrix_specs[i]);
				for (size_t r = 0; r < sample_matrix.m_rows; r++)
				{
					auto rowvalues = utils::RandomData().Generate(
						sample_matrix.m_columns, s_minval, s_maxval);
					sample_matrix.set_row(rowvalues, { r, 0 }, true);
				}
				m_matrix.push_back(sample_matrix);
			}
		}
	}

	return status;
}

HintSpecs 
CLRS4_Test::GetHintSpecs(CLRS4_Probs prob)
{
	if (IsMaxSubArrayProblem(prob))
	{
		return 
		{
			{ SData_Res::HintTotal(),		OPERAND_TYPE::UINT },
			{ SData_Res::HintUIntHigh(),	OPERAND_TYPE::UINT },
			{ SData_Res::HintUIntLow(),		OPERAND_TYPE::UINT },
		};
	}
	if (IsMatrixMultProblem(prob))
	{
		return 
		{
			{ SData_Res::HintMatrixRows(),	OPERAND_TYPE::UINT },
			{ SData_Res::HintMatrixCols(),	OPERAND_TYPE::UINT },
			{ SData_Res::HintMaxValue(),	OPERAND_TYPE::INT },
			{ SData_Res::HintMinValue(),	OPERAND_TYPE::INT },
			{ SData_Res::HintMatrix1(),		OPERAND_TYPE::STR },
			{ SData_Res::HintMatrix2(),		OPERAND_TYPE::STR },
			{ SData_Res::HintStrassenBaseSize(), OPERAND_TYPE::UINT },
		};
	}
	return{};
}

void
CLRS4_Test::CleanOperandLists()
{
	TestBase::CleanOperandLists();
	m_matrix.clear();
	m_complex_numbers.clear();
}

///////////////////////////////////////////////////////////////////////////////
void Run_CLRS4(ModuleData module_data)
{
	wchar_t dowerepeat = L'y';
	do{
		(new CLRS4_Test(module_data))->Run();
		if (RUNTYPE::ConsoleOps == module_data.runtype){
			utils::clear_wcin();
			std::wcout << MenuMsgStr(module_data.majorops, MenuMsgOpts::REPEAT_MAJOROP);
			std::wcin >> dowerepeat;
		}
		else{
			dowerepeat = L'N';
		}
	} while (L'y' == dowerepeat || L'Y' == dowerepeat);
}

void TestCLRS_Ch4(WSTRING_VEC args)
{
	wchar_t dowerepeat = L'y';
	do{
		(new CLRS4_Test())->Run();
		utils::clear_wcin();
		std::wcout << MenuMsgStr(CLRS4_Test().MajorOP(), MenuMsgOpts::REPEAT_MAJOROP);
		std::wcin >> dowerepeat;
	} while (L'y' == dowerepeat || L'Y' == dowerepeat);
}
