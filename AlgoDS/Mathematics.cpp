#include "stdafx.h"
#include "Mathematics.h"
#include "Utilities.h"
#include "estd.h"
#include <iterator>

////////////////////////////////////////////////////////////////////////////////
ComplexNum::ComplexNum() : ComplexNum(0, 0){}
ComplexNum::ComplexNum(int p_real, int p_imaginary)
	: real(p_real), imaginary(p_imaginary)
{}
ComplexNum::ComplexNum(ComplexNum& rhs){
		*this  = rhs;
}
void
ComplexNum::set(int p_real, int p_imaginary)
{
	real = p_real;
	imaginary = p_imaginary;
}
std::wstring 
ComplexNum::to_wstring(){
	std::wstring SS{ L" " };
	auto temp = std::to_wstring(real);
	if (imaginary < 0){
		temp += SS + L"-" + SS + std::to_wstring(std::abs(imaginary)) + L"i";
	}
	else{
		temp += SS + L"+" + SS + std::to_wstring(imaginary) + L"i";
	}
	return temp;
}

ComplexNum&
ComplexNum::operator=(ComplexNum const& rhs){
	if (this != &rhs){
		this->real = rhs.real;
		this->imaginary = rhs.imaginary;
	}
	return *this;
}

ComplexNum
ComplexNum::operator+(ComplexNum const& rhs){
	ComplexNum temp{ this->real + rhs.real, this->imaginary + rhs.imaginary };
	return temp;
}
ComplexNum
ComplexNum::operator-(ComplexNum const& rhs){
	return ComplexNum{ this->real - rhs.real, this->imaginary - rhs.imaginary };
}
ComplexNum&
ComplexNum::operator+=(ComplexNum const& rhs){
	this->real += rhs.real;
	this->imaginary += rhs.imaginary;
	return *this;
}
ComplexNum&
ComplexNum::operator-=(ComplexNum const& rhs){
	this->real -= rhs.real;
	this->imaginary -= rhs.imaginary;
	return *this;
}

bool
ComplexNum::operator==(ComplexNum const& rhs){
	return (this->real == rhs.real && this->imaginary == rhs.imaginary);
}
bool
ComplexNum::operator!=(ComplexNum const& rhs){
	return !(*this == rhs);
}

///////////////////////////////////////////////////////////////////////////////
bool
operator==(MatrixPoint const& A, MatrixPoint const& B){
	return ((A.rowid == B.rowid) && (A.colid == B.colid));
}
bool
operator!=(MatrixPoint const& A, MatrixPoint const& B){
	return (!(A == B));
}

///////////////////////////////////////////////////////////////////////////////
bool
operator==(MatrixSpecs const& A, MatrixSpecs const& B){
	return ((A.m_rows == B.m_rows) && (A.m_columns == B.m_columns));
}
bool
operator!=(MatrixSpecs const& A, MatrixSpecs const& B){
	return (!(A == B));
}


////////////////////////////////////////////////////////////////////////////////

Matrix::Matrix() :Matrix(0, 0){}
Matrix::Matrix(MatrixSpecs const& specs, int common_value)
	: Matrix(specs.m_rows, specs.m_columns, common_value){}
Matrix::Matrix(size_t rows, size_t cols, int common_value)
	: m_rows(rows), m_columns(cols)
{
	if (cols != 0){
		for (size_t i = 0; i < rows; i++){
			m_store.push_back(std::vector<int>(cols, common_value));
		}
	}
}
Matrix::Matrix(std::vector<INT_VEC> const& source)
	:Matrix(0, 0)
{
	m_rows = source.size();
	if (m_rows != 0){
		m_columns = source[0].size();
		for (size_t i = 1; i < m_rows; i++){
			if (m_columns != source[i].size())
				throw std::invalid_argument(e_invalid_state());
		}
	}
	m_store = source;
}

//Matrix::Matrix(std::vector<INT_VEC> const& source,
//	MatrixDIM row, MatrixDIM col)
//{
//	rowlim = row;
//	collim = col;
//	auto rs = rowlim.first, re = rowlim.second;
//	auto cs = collim.first, ce = collim.second;
//
//	if (rs > re || cs > ce)
//		throw std::invalid_argument(e_invalid_state());
//	if (rs == 0 && re == 0 && cs == 0 && ce == 0)
//		return;
//
//	auto tempr = source.size();
//	if ((tempr < (re+1)) || (tempr < (re - rs) + 1)){
//		throw std::invalid_argument(e_invalid_state());
//	}
//
//	auto tempc = source[0].size();
//	for (size_t i = 1; i < tempr; i++){
//		if (tempc != source[i].size())
//			throw std::invalid_argument(e_invalid_state());
//	}
//	if ((tempc < (ce+1)) || (tempc < (ce - cs) + 1))
//		throw std::invalid_argument(e_invalid_state());
//
//	m_store = source;
//}

Matrix::Matrix(Matrix const& other)
{
	m_rows = other.m_rows;
	m_columns = other.m_columns;
	m_store = other.m_store;
}
Matrix::Matrix(Matrix && other)
	: Matrix()
{
	swap(*this, other);
}
Matrix::~Matrix(){}
Matrix&
Matrix::operator=(Matrix other)
{
	swap(*this, other);
	return *this;
}
//Matrix&
//Matrix::operator=(Matrix && other)
//{
//	swap(*this, other);
//	return *this;
//}

void
swap(Matrix &A, Matrix &B)
{
	using std::swap;
	swap(A.m_rows, B.m_rows);
	swap(A.m_columns, B.m_columns);
	swap(A.m_store, B.m_store);
}

int
Matrix::element(size_t row, size_t col, bool secure) const
{
	if (secure){
		if (!is_valid_param(row, col))
			throw std::out_of_range(e_dim());
		if (m_store.empty() ||
			m_store.size() < row || m_store[row].size() < col)
			throw std::runtime_error(e_invalid_state());
	}
	return m_store[row][col];
}
Matrix::INT_VEC
Matrix::row(size_t row) const
{
	if (m_rows <= row)
		throw std::out_of_range(e_oor_r());
	return{ m_store[row].begin(), m_store[row].end() };

	/*for (size_t i = 0; i < m_columns; i++){
	ret.push_back(m_store[row][i]);
	}
	return ret;*/
}
Matrix::INT_VEC
Matrix::column(size_t col) const
{
	if (m_columns < col)
		throw std::out_of_range(e_oor_c());
	INT_VEC ret(m_rows);
	for (size_t row = 0; row < m_rows; row++){
		ret[row] = m_store[row][col];
	}
	return ret;
}

/*
Matrix
Matrix::sub_matrix(MatrixDIM rowinfo, MatrixDIM colinfo, bool secure) const
{
if (secure){
if (!is_valid_param(rowinfo, colinfo))
throw std::range_error(e_invalid_ip());
}
Matrix ret{ rowinfo.second - rowinfo.first + 1, colinfo.second - colinfo.first + 1 };
for (size_t i = 0; i <= ret.rowlim.second; i++)
{
std::copy(m_store[i].begin() + rowinfo.first, m_store[i].end() + rowinfo.second, ret.m_store[i]);
}
return ret;
}
*/
Matrix
Matrix::square_quadrant(MatrixPoint const& quadpt, bool secure) const
{
	if (secure){
		if (0 != m_rows % 2 || 0 != m_columns % 2)
			throw std::logic_error(e_unsuitable_state());
		if (!is_valid_quadpoint(quadpt))
			throw std::out_of_range(e_invalid_ip());
	}

	/*auto QDim = m_rows / 2;
	Matrix result;
	result.set_dim(QDim, QDim);
	result.m_store.assign(QDim, {});
	for (size_t r = 0; r < QDim; r++){
	auto start = m_store[quadpt.rowid + r].begin() + quadpt.colid;
	result.m_store[r].insert(result.m_store[r].begin(), start, start + QDim);
	}*/
	return square_sub_matrix(quadpt, m_rows / 2);
}

Matrix
Matrix::square_sub_matrix(MatrixPoint const& startpt, size_t dim) const
{
	if (startpt.rowid > (m_rows - dim) || startpt.colid > (m_columns - dim))
		throw std::logic_error(e_invalid_ip());

	Matrix result;
	result.set_dim(dim, dim);
	result.m_store.assign(dim, {});
	for (size_t r = 0; r < dim; r++){
		auto start = m_store[startpt.rowid + r].begin() + startpt.colid;
		result.m_store[r].insert(result.m_store[r].begin(), start, start + dim);
	}
	return result;
}

void
Matrix::set_dim(size_t row, size_t col)
{
	m_rows = row;
	m_columns = col;
}

void
Matrix::set_element(int value, size_t row, size_t col)
{
	if (!is_valid_param(row, col))
		throw std::out_of_range(e_dim());
	if (m_store.empty() || m_store.at(row).empty())
		throw std::out_of_range(e_dim());
	m_store[row][col] = value;
}
void
Matrix::set_row(std::vector<int> const& value,
MatrixPoint const& pt, bool secure)
{
	if (secure)
	{
		if (!is_valid_point(pt))
			throw std::invalid_argument(e_invalid_ip());
		/*if ((pt.colid + value.size()) > m_columns )
		throw std::invalid_argument(e_nm_c());*/
	}
	//if (0 == pt.colid)
	//{
	//	m_store.at(pt.rowid).assign(value.begin(), value.end());
	//}
	//else
	//{
	if (m_store.at(pt.rowid).empty()){
		m_store.at(pt.rowid).assign(m_columns, 0);
	}
	if (m_columns != m_store.at(pt.rowid).size()){
		throw std::logic_error(e_invalid_state());
	}
	std::copy(value.begin(), value.end(), (m_store.at(pt.rowid).begin()));
	//std::copy(value.begin(), value.end(), (m_store.at(pt.rowid).begin() + pt.colid));
	//}
	return;
}
void
Matrix::set_col(std::vector<int> const& value,
MatrixPoint const& pt, bool secure)
{
	if (secure){
		if (!is_valid_point(pt))
			throw std::invalid_argument(e_invalid_ip());
		if ((pt.colid + value.size()) > m_rows)
			throw std::invalid_argument(e_nm_c());
	}
	for (size_t r = pt.rowid; r < m_rows; r++){
		m_store[r][pt.colid] = value[r];
	}
}
void
Matrix::set_elements(std::vector<INT_VEC> const& data, bool secure)
{
	if (secure){
		if (data.size() != m_rows)
			throw std::out_of_range(e_dim());
	}
	for (size_t r = 0; r < data.size(); r++){
		set_row(data[r], { r, 0 }, secure);
	}
}
void
Matrix::set_elements(Matrix const& data,
MatrixPoint const& startpt, bool secure)
{
	if (secure){
		if (startpt.rowid + data.m_rows > m_rows ||
			startpt.colid + data.m_columns > m_columns){
			throw std::logic_error(e_invalid_ip());
		}
		if (m_store[0].size() < data.m_columns + startpt.colid)
			throw std::logic_error(e_invalid_state());
	}
	for (size_t r = 0; r < data.m_rows; r++)
	{
		auto copy_val = data.row(r);
		std::copy(copy_val.begin(), copy_val.end(),
			m_store[startpt.rowid + r].begin() + startpt.colid);
	}
	return;
}

void
Matrix::set_quad(MatrixPoint const& quadpt, Matrix const& data, bool secure)
{
	if (secure)
	{
		if (0 != m_rows % 2 || 0 != m_columns % 2)
			throw std::logic_error(e_invalid_state());
		if (0 != data.m_rows % 2 || 0 != data.m_columns % 2)
			throw std::logic_error(e_invalid_ip());
		if (!is_valid_quadpoint(quadpt))
			throw std::out_of_range(e_invalid_ip());
	}

	auto QDim = m_rows / 2;
	for (size_t r = 0; r < QDim; r++){
		std::copy(data.m_store[r].begin(), data.m_store[r].end(),
			m_store[quadpt.rowid + r].begin() + quadpt.colid);
	}
	return;
}
Matrix
Matrix::add_quads(MatrixPoint const& quadpt_a,
MatrixPoint const& quadpt_b, bool secure) const
{
	if (secure)
	{
		if (0 != m_rows % 2 || 0 != m_columns % 2)
			throw std::logic_error(e_unsuitable_state());
		if (!is_valid_quadpoint(quadpt_a) || !is_valid_quadpoint(quadpt_b))
			throw std::out_of_range(e_invalid_ip());
	}

	auto QDim = m_rows / 2;
	Matrix result;
	result.set_dim(QDim, QDim);
	result.m_store.assign(QDim, {});
	for (size_t r = 0; r < QDim; r++)
	{
		result.m_store[r].reserve(QDim);
		auto begin_a = m_store[quadpt_a.rowid + r].begin() + quadpt_a.colid;
		auto begin_b = m_store[quadpt_b.rowid + r].begin() + quadpt_b.colid;
		//auto begin_r = result.m_store[r].begin();
		std::transform(begin_a, begin_a + QDim, begin_b,
			std::back_inserter(result.m_store[r]), std::plus<int>());
		/*
		for (size_t c = 0; c < QDim; c++){
		*(begin_r + c) = *(begin_a + c) + *(begin_b + c);
		}
		for (size_t c = 0; c < QDim; c++){
		result.m_store[r][c] = m_store[ quad_point_a.rowid + r][ quad_point_a.colid + c]
		+ m_store[ quad_point_b.rowid + r][ quad_point_b.colid + c];
		}*/
	}
	return result;
}
Matrix
Matrix::sub_quads(MatrixPoint const& quadpt_a,
MatrixPoint const& quadpt_b, bool secure) const
{
	if (secure)
	{
		if (0 != m_rows % 2 || 0 != m_columns % 2)
			throw std::logic_error(e_unsuitable_state());
		if (!is_valid_quadpoint(quadpt_a) || !is_valid_quadpoint(quadpt_b))
			throw std::out_of_range(e_invalid_ip());
	}

	auto QDim = m_rows / 2;
	Matrix result;
	result.set_dim(QDim, QDim);
	result.m_store.assign(QDim, {});
	for (size_t r = 0; r < QDim; r++)
	{
		result.m_store[r].reserve(QDim);
		auto begin_a = m_store[quadpt_a.rowid + r].begin() + quadpt_a.colid;
		auto begin_b = m_store[quadpt_b.rowid + r].begin() + quadpt_b.colid;
		//auto begin_r = result.m_store[r].begin();
		std::transform(begin_a, begin_a + QDim, begin_b,
			std::back_inserter(result.m_store[r]), std::minus<int>());
		/*
		for (size_t c = 0; c < QDim; c++){
		*(begin_r + c) = *(begin_a + c) - *(begin_b + c);
		}*/
		/*for (size_t c = 0; c < QDim; c++){
		result.m_store[r][c] = m_store[ quad_point_a.rowid + r][ quad_point_a.colid + c]
		- m_store[ quad_point_b.rowid + r][ quad_point_b.colid + c];
		}*/
	}
	return result;
}

void
Matrix::fill_random(int minval, int maxval)
{
	for (size_t r = 0; r <= m_rows; r++){
		auto rowvalues = utils::RandomData().Generate(m_columns, minval, maxval);
		set_row(rowvalues, { r, 0 }, false);
	}
}

//Matrix
//Matrix::add_sub_matrices(
//std::pair<MatrixDIM, MatrixDIM> A,
//std::pair<MatrixDIM, MatrixDIM> B,
//bool secure)
//{
//	if (secure)
//	{
//		if (!rowlim.is_proper_sub_dim(A.first) || !collim.is_proper_sub_dim(A.second) ||
//			!rowlim.is_proper_sub_dim(B.first) || !collim.is_proper_sub_dim(B.second))
//			return{};
//		if ((A.first.span() != B.first.span()) || (A.second.span() != B.second.span()))
//			return{};
//	}
//	Matrix result{ A.first.span(), A.second.span() };
//	for (size_t i = 0; i < A.first.span(); i++){
//		for (size_t j = 0; j < A.second.span(); j++){
//			result.m_store[i][j] = m_store[A.first.first + i][A.second.first + j] +
//				m_store[B.first.first + i][B.second.first + j];
//		}
//	}
//	return result;
//}
//
//Matrix
//Matrix::subtract_sub_matrices(
//std::pair<MatrixDIM, MatrixDIM> A,
//std::pair<MatrixDIM, MatrixDIM> B,
//bool secure)
//{
//	if (secure)
//	{
//		if (!rowlim.is_proper_sub_dim(A.first) || !collim.is_proper_sub_dim(A.second) ||
//			!rowlim.is_proper_sub_dim(B.first) || !collim.is_proper_sub_dim(B.second))
//			return{};
//		if ((A.first.span() != B.first.span()) || (A.second.span() != B.second.span()))
//			return{};
//	}
//	Matrix result{ A.first.span(), A.second.span() };
//	for (size_t i = 0; i < A.first.span(); i++){
//		for (size_t j = 0; j < A.second.span(); j++){
//			result.m_store[i][j] = m_store[A.first.first + i][A.second.first + j] -
//				m_store[B.first.first + i][B.second.first + j];
//		}
//	}
//	return result;
//}
//
//Matrix
//add_sub_matrices(Matrix const& A, MatrixDIM ra, MatrixDIM ca,
//Matrix const& B, MatrixDIM rb, MatrixDIM cb, bool secure)
//{
//	if (secure)
//	{
//		if (!A.rowlim.is_proper_sub_dim(ra) || !A.collim.is_proper_sub_dim(ca) ||
//			!B.rowlim.is_proper_sub_dim(rb) || !B.collim.is_proper_sub_dim(cb))
//			return{};
//		if ((ra.span() != ca.span()) || (rb.span() != cb.span()))
//			return{};
//	}
//	Matrix result{ra.span(), rb.span()};
//	for (size_t i = 0; i < ra.span(); i++){
//		for (size_t j = 0; j < rb.span(); j++){
//			result.m_store[i][j] = A.m_store[ra.first + i][ca.first + j] +
//				B.m_store[rb.first + i][cb.first + j];
//		}
//	}
//	return result;
//}
//Matrix
//subtract_sub_matrices(Matrix const& A, MatrixDIM ra, MatrixDIM ca,
//Matrix const& B, MatrixDIM rb, MatrixDIM cb, bool secure)
//{
//	if (secure)
//	{
//		if (!A.rowlim.is_proper_sub_dim(ra) || !A.collim.is_proper_sub_dim(ca) ||
//			!B.rowlim.is_proper_sub_dim(rb) || !B.collim.is_proper_sub_dim(cb))
//			return{};
//		if ((ra.span() != ca.span()) || (rb.span() != cb.span()))
//			return{};
//	}
//	Matrix result{ ra.span(), rb.span() };
//	for (size_t i = 0; i < ra.span(); i++){
//		for (size_t j = 0; j < rb.span(); j++){
//			result.m_store[i][j] = A.m_store[ra.first + i][ca.first + j] -
//				B.m_store[rb.first + i][cb.first + j];
//		}
//	}
//	return result;
//}

bool
Matrix::is_square() const{
	return (m_columns == m_rows);
}
bool
Matrix::is_legal() const
{
	if (m_store.size() < m_rows)
		return false;
	auto colsize = m_store.at(0).size();
	if (colsize < m_columns)
		return false;
	for (size_t i = 1; i < m_store.size(); i++){
		if (m_store.at(i).size() != colsize)
			return false;
	}
	return true;
}
bool
Matrix::is_valid_quadpoint(MatrixPoint const& mp) const
{
	if (0 == mp.rowid && (0 == mp.colid || (m_columns / 2) == mp.colid))
		return true;
	if ((m_rows / 2) == mp.rowid && (0 == mp.colid || (m_columns / 2) == mp.colid))
		return true;
	return false;
}
bool
Matrix::is_valid_param(size_t row, size_t col) const{
	return (row < m_rows || col < m_columns);
}
bool
Matrix::is_valid_point(MatrixPoint const& point) const{
	return (point.rowid < m_rows && point.colid < m_columns);
}

std::wstring
Matrix::to_wstring(bool pretty_print) const
{
	size_t max_padding = 0;
	if (pretty_print)
	{
		int max_pos = 0;
		int min_neg = 0;
		for (auto const &it_r : m_store)
		{
			for (auto const &it_el : it_r)
			{
				if (it_el < 0){
					min_neg = (min_neg < it_el) ? min_neg : it_el;
				}
				else{
					max_pos = (max_pos > it_el) ? max_pos : it_el;
				}
			}
		}
		max_padding = std::to_wstring(min_neg).size();
		if (max_padding < std::to_wstring(max_pos).size())
			max_padding = std::to_wstring(max_pos).size();
	}
	std::wstring ret{ L"row:col " +
		std::to_wstring(m_rows) + L":" + std::to_wstring(m_columns) + L"\n"
	};

	for (auto const &rows : m_store)
	{
		for (auto const &elm : rows)
		{
			if (max_padding == 0)
			{
				ret += std::to_wstring(elm) + L" ";
			}
			else
			{
				if (elm < 0){
					ret += L"-";
					ret += estd::prepend_zero_w(
						std::abs(elm), (max_padding - 1u)) + L" ";
				}
				else{
					ret += estd::prepend_zero_w(elm, max_padding) + L" ";
				}
			}
		}
		ret += L"\n";
	}
	ret.erase(ret.size() - 1); // Remove last newline
	return ret;
}

bool
operator== (Matrix const& first, Matrix const& second)
{
	if (first.m_rows != second.m_rows || first.m_columns != second.m_columns)
		throw std::logic_error(first.e_invalid_ip());
	for (size_t r = 0; r < first.m_rows; r++){
		for (size_t c = 0; c < first.m_columns; c++){
			if (first.m_store[r][c] != second.m_store[r][c])
				return false;
		}
	}
	return true;
}
bool
operator!= (Matrix const& first, Matrix const& second){
	return (!(first == second));
}

Matrix
operator+ (Matrix const& first, Matrix const& second)
{
	if (first.m_rows != second.m_rows || first.m_columns != second.m_columns)
		throw std::range_error(first.e_dim());

	Matrix result{ first };
	for (size_t r = 0; r < first.m_rows; r++){
		for (size_t c = 0; c < first.m_columns; c++){
			result.m_store[r][c] += second.m_store[r][c];
		}
	}
	return result;
}
Matrix
operator- (Matrix const& first, Matrix const& second)
{
	if (first.m_rows != second.m_rows || first.m_columns != second.m_columns)
		throw std::range_error(first.e_dim());

	Matrix result{ first };
	for (size_t r = 0; r < first.m_rows; r++){
		for (size_t c = 0; c < first.m_columns; c++){
			result.m_store[r][c] -= second.m_store[r][c];
		}
	}
	return result;
}
