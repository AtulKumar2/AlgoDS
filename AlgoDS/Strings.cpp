#include "stdafx.h"
#include <cstring>
#include <cassert>
#include "CommonDefs.h"
#include "Utilities.h"
#include "TestHarness.h"

using std::wstring;
using std::endl;
using std::wcout;
	
///////////////////////////////////////////////////////////////////////////////
enum ALGO_TYPE
{
	WORST,
	SIMPLE,
	SPEEDIER_AND_MEM_INTENSIVE,
	SLOWER_AND_MEM_SAVING,
	BEST_AVERAGE,
	BEST_LARGE_DATA_SET,
	BEST,
};
///////////////////////////////////////////////////////////////////////////////
class String_Probs
{
//To test various string related programs asked in interviews
// wherever possible, links will be shared.
public:
	void strindex(char* str,  char * substr, enum ALGO_TYPE type);
	void testStrIndex();
};
	
/*
Finds right most index of t in c. Assuming str is well-behaving string with 
\0 at end.
*/
void String_Probs::strindex(char* str, char *substr, enum ALGO_TYPE type)
{
	assert(NULL != str && NULL != substr);
	size_t str_len = strnlen_s(str, 10000000), substr_len = strnlen_s(substr, 10000000);
	assert(0 != str_len && 0 != substr_len && str_len >= substr_len);
	char *str_t = _strdup(str);
	char *substr_t = _strdup(substr);
	assert(NULL != str_t && NULL != substr_t);

	int loc = -1;
		
	// Simplest one with complexity equivalent to string & substring length
	if (SIMPLE == type){
		char *temp = str_t + (str_len - substr_len) + 1;
		do{
			temp--;
			if (0 == strncmp(temp, substr_t, substr_len)){
				loc = temp - str_t;
				break;
			}
		}while (temp != str_t);
	}

	// http://www.youtube.com/watch?v=Zj_er99KMb8
	// Lecture - 17 Case Study: Searching for Patterns
	// http://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm
	// Knuth–Morris–Pratt algorithm
	if (BEST_LARGE_DATA_SET == type){
	}
	if (BEST == type){
	}
	wcout << L"Rightmost index of '" << substr_t << L"' in '" << str_t << L"' is ";
	( -1 != loc)? wcout << loc << endl : wcout << L"undefined." << endl;

	free(str_t);
	free(substr_t);
	return;
}
void String_Probs::testStrIndex()
{
	strindex("abcdefabcfgrabcsed", "abc", SIMPLE); 
	strindex("abcdefabcfgrabcsed", "abcd", SIMPLE); 
}

///////////////////////////////////////////////////////////////////////////////
STATUS_GEN
Strings_Test::Run()
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
Strings_Test::Run(ModuleData module_data)
{
	m_module_data = module_data;
	return Run();
}

STATUS_GEN
Strings_Test::RunConsole()
{
	(new String_Probs())->testStrIndex();
	return STATUS_GEN::SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
void TestStrings(WSTRING_VEC args)
{
	(new String_Probs())->testStrIndex();
	return;
}