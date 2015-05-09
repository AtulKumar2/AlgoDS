#include "stdafx.h"
#include <cassert>
#include "CommonDefs.h"
#include "Utilities.h"
#include "TestHarness.h"

using std::wstring;
using std::wcout;
using std::endl;

////////////////////////////////////////////////////////////////////////////////
/*
Without using symbols /, % and * operators, WAP to divide a number by 3. 
itoa() function is available.
http://en.wikipedia.org/wiki/Long_division
*/
void divide_by_3(int num, int& quotient, int& remainder)
{
	wchar_t num_arr[32] = {L'\0'};
	//_itow(num, num_arr, 10);
	_itow_s(num, num_arr, 10);
	wstring quotient_s(L""), dividend(num_arr);
		
	for (wstring::iterator i = dividend.begin(); i != dividend.end(); i++)
	{
		int divisor = 0, curr_dividend = (remainder*10) + (*i - 48);
		remainder = curr_dividend;
		quotient_s.append(L"0"); 
		while((divisor += 3) <= curr_dividend){
			remainder = curr_dividend - divisor;
			quotient_s [i - dividend.begin()] += 1;
		}
	}
	quotient = _wtoi(quotient_s.c_str());
	return; 
}
void test_divide_by_3()
{
	int quotient = 0, remainder = 0;
	divide_by_3(10345, quotient, remainder);
	wcout << 10345 << L" divided by 3 gives quotient " << quotient << L", remainder " << remainder << endl;
	divide_by_3(10344, quotient=0, remainder=0);
	wcout << 10344 << L" divided by 3 gives quotient " << quotient << L", remainder " << remainder << endl;
	divide_by_3(1034456, quotient=0, remainder=0);
	wcout << 1034456 << L" divided by 3 gives quotient " << quotient << L", remainder " << remainder << endl;
	return;
}

////////////////////////////////////////////////////////////////////////////////
STATUS_GEN
Twisters_Test::Run()
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
	case RUNTYPE::FileInput: status = TestBase::RunFI(); break;
	case RUNTYPE::CommandLine: status = TestBase::RunCL(); break;
	default:
		break;
	}
	return status;
}
STATUS_GEN
Twisters_Test::Run(ModuleData module_data)
{
	m_module_data = module_data;
	return Run();
}

STATUS_GEN
Twisters_Test::RunConsole()
{
	test_divide_by_3();
	return STATUS_GEN::SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
void TestTwisters(WSTRING_VEC args)
{
	test_divide_by_3();
	//test_round();
	return;
}