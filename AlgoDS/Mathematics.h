#pragma once

#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
struct ComplexNum
{
	int real;
	int imaginary;

	ComplexNum();
	ComplexNum(int p_real, int p_imaginary);
	ComplexNum(ComplexNum&);

	void
		set(int p_real, int p_imaginary);
	std::wstring 
		to_wstring();

	ComplexNum& 
		operator=(ComplexNum const&);

	ComplexNum
		operator+(ComplexNum const&);
	ComplexNum
		operator-(ComplexNum const&);
	ComplexNum& 
		operator+=(ComplexNum const&);
	ComplexNum& 
		operator-=(ComplexNum const&);

	bool 
		operator==(ComplexNum const&);
	bool
		operator!=(ComplexNum const&);
};

///////////////////////////////////////////////////////////////////////////////
struct MatrixPoint
{
	size_t rowid;
	size_t colid;

	friend bool
		operator==(MatrixPoint const& A, MatrixPoint const& B);
	friend bool
		operator!=(MatrixPoint const& A, MatrixPoint const& B);
};
bool
operator==(MatrixPoint const& A, MatrixPoint const& B);
bool
operator!=(MatrixPoint const& A, MatrixPoint const& B);

struct MatrixSpecs
{
	size_t					m_rows;
	size_t					m_columns;

	friend bool
		operator==(MatrixSpecs const& A, MatrixSpecs const& B);
	friend bool
		operator!=(MatrixSpecs const& A, MatrixSpecs const& B);
};
bool
operator==(MatrixSpecs const& A, MatrixSpecs const& B);
bool
operator!=(MatrixSpecs const& A, MatrixSpecs const& B);

class Matrix
{
protected:
	using INT_VEC = std::vector<int>;

public:
	std::vector<INT_VEC>	m_store;
	size_t					m_rows;
	size_t					m_columns;

public:
	Matrix();
	Matrix(MatrixSpecs const&, int common_value = 0);
	Matrix(size_t row, size_t col, int common_value = 0);
	Matrix(std::vector<INT_VEC> const& data);
	Matrix(Matrix const&);
	Matrix(Matrix &&);
	~Matrix();
	Matrix&	operator=(Matrix);

	int
		element(size_t row, size_t col, bool secure = false) const;
	INT_VEC
		row(size_t) const;
	INT_VEC
		column(size_t) const;
	Matrix
		square_quadrant(MatrixPoint const& quadpt, bool secure = false) const;
	Matrix
		square_sub_matrix(MatrixPoint const& startpt, size_t size) const;

	void
		set_dim(size_t rows, size_t cols);
	void
		set_row(INT_VEC const&, MatrixPoint const&, bool secure = true);
	void
		set_col(INT_VEC const&, MatrixPoint const&, bool secure = true);
	void
		set_element(int value, size_t row, size_t col);
	void
		set_elements(std::vector<INT_VEC> const&, bool secure = true);
	void
		set_elements(Matrix const&, MatrixPoint const&, bool secure = true);

	void
		set_quad(MatrixPoint const& quadpt, Matrix const& data, bool secure = false);
	Matrix
		add_quads(MatrixPoint const&, MatrixPoint const&, bool secure = false) const;
	Matrix
		sub_quads(MatrixPoint const&, MatrixPoint const&, bool secure = false) const;

	void
		fill_random(int minval = std::numeric_limits<int>::min() / 8,
		int maxval = std::numeric_limits<int>::max() / 8);
	bool
		is_square() const;
	bool
		is_legal() const;
	std::wstring
		to_wstring(bool pretty_print = false) const;

	friend void
		swap(Matrix &, Matrix &);
	friend bool
		operator==(Matrix const&, Matrix const&);
	friend bool
		operator!=(Matrix const&, Matrix const&);
	friend Matrix
		operator+(Matrix const&, Matrix const&);
	friend Matrix
		operator-(Matrix const&, Matrix const&);

private:
	bool
		is_valid_param(size_t row, size_t col) const;
	bool
		is_valid_point(MatrixPoint const&) const;
	bool
		is_valid_quadpoint(MatrixPoint const&) const;

	std::string e_dim() const { return "Dimensions do not match"; }
	std::string e_oor_r() const { return "Row out of range"; }
	std::string e_oor_c() const { return "Column out of range"; }
	std::string e_nm_r() const { return "Row length not matching"; }
	std::string e_nm_c() const { return "Column length not matching"; }
	std::string e_invalid_ip() const { return "Invalid input"; }
	std::string e_invalid_state() const{ return "Object state inconsistent"; }
	std::string e_unsuitable_state() const{ return "Object state not suitable for operation"; }
};
