
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "Common.h"

class cout_redirect
{
public:
	cout_redirect(std::streambuf *new_buffer)
		: old(std::cout.rdbuf(new_buffer))
	{ }

	~cout_redirect() {
		std::cout.rdbuf(old);
	}

private:
	std::streambuf *old;
};

class wcout_redirect
{
public:
	wcout_redirect(std::wstreambuf *new_buffer)
		: old(std::wcout.rdbuf(new_buffer))
	{ }

	~wcout_redirect() {
		std::wcout.rdbuf(old);
	}

private:
	std::wstreambuf *old;
};

void outputredirection()
{
	using namespace std;

	wstringstream buffer;
	//wstreambuf *old = std::wcout.rdbuf(buffer.rdbuf());
	wcout_redirect redirect_dummy(buffer.rdbuf());

	wstring expected{ L"1,2,3,4" };
	wcout << vector < unsigned int > { 1, 2, 3, 4 };
	wstring actual = buffer.str();
	
	if (actual == expected)
		wcout << L"true";
	else
		wcout << L"false";

	return;
}