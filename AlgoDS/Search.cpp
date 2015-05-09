#include "stdafx.h"
#include <cstring>
#include <cassert>
#include <inttypes.h>
#include "CommonDefs.h"
#include "Utilities.h"
#include "TestHarness.h"

using std::wstring;
using std::endl;
using std::wcout;

///////////////////////////////////////////////////////////////////////////////
namespace
{
#define MAXSIZE_SEARCH_LIST				MAXSIZE_LIST
#define MAXSIZE_SEARCH_ITEM				MAXSIZE_ITEM
#define MAXSIZE_CONSOLE_SEARCH_LIST		1024
#define MAXSIZE_CONSOLE_SEARCH_LIST_STRING	MAXSIZE_CONSOLE_SEARCH_LIST*MAXSIZE_ITEM
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Simple Linear search
int Search::LinearSearch(char item, char *list, unsigned int list_length)
{
	int loc = NOT_FOUND;
	if(!list || 0 == list_length)
		return loc;

	for (unsigned int i = 0; i < list_length; i++){
		if (item == list[i]){
			loc = i;
			break;
		}
	}
	return loc;
}

int Search::LinearSearch(intmax_t item, intmax_t *list, unsigned int list_length)
{
	int loc = NOT_FOUND;
	if (NULL == list || 0 == list_length)
		return loc;

	for (unsigned int i = 0; i < list_length; i++){
		if (item == list[i]){
			loc = i;
			break;
		}
	}
	return loc;
}

int Search::LinearSearch(int item, int *list, unsigned int list_length)
{
	int loc = NOT_FOUND;
	if(NULL == list || 0 == list_length)
		return loc;

	for (unsigned int i = 0; i < list_length; i++){
		if (item == list[i]){
			loc = i;
			break;
		}
	}
	return loc;
}

int Search::BinSearch(int item, int* list, unsigned int low, unsigned int high)
{
	if (NULL == list || low > high){
		lasterror = STATUS_GEN::BAD_PARAM;
		return NOT_FOUND;
	}
	if (low == high){
		if (list[low] == item) return low;
		else return NOT_FOUND;
	}

	int mid = (low + high) / 2;
	if (item <= list[mid])
		return BinSearch(item, list, low, mid);
	else
		return BinSearch(item, list, mid + 1, high);
}

///////////////////////////////////////////////////////////////////////////////
Search_Test::Search_Test()
{
	// Top level menu
	int MenuId = 1;
	AlgoMenuEntries.push_back(SearchMenuItem(SEARCH_Probs::LINEAR, MenuId++, L"L", L"Linear Search"));
	AlgoMenuEntries.push_back(SearchMenuItem(SEARCH_Probs::BINARY, MenuId++, L"B", L"Binary Search"));
	AlgoMenuEntries.push_back(SearchMenuItem(SEARCH_Probs::EXIT, MenuId++, L"E", L"Exit"));

	// Operand menu
	OperMenuEntries = OperandType().MenuEntries();

	// Implementation status
	ROW row;
	row.insert({ OPERAND_TYPE::INT, !(NOT_IMPLEMENTED == Search().LinearSearch(0, NULL, 0)) });
	row.insert({ OPERAND_TYPE::CHAR, !(NOT_IMPLEMENTED == Search().LinearSearch('a', NULL, 0)) });
	row.insert({ OPERAND_TYPE::STR, !(NOT_IMPLEMENTED == Search().LinearSearch(NULL, 0, NULL, 0)) });
	ImplementationStatus.insert({ SEARCH_Probs::LINEAR, row });
	row.clear();

	row.insert({ OPERAND_TYPE::INT, !(NOT_IMPLEMENTED == Search().BinSearch(0, NULL, 0, 0)) });
	row.insert({ OPERAND_TYPE::CHAR, !(NOT_IMPLEMENTED == Search().BinSearch('a', NULL, 0, 0)) });
	row.insert({ OPERAND_TYPE::STR, !(NOT_IMPLEMENTED == Search().BinSearch(NULL, 0, NULL, 0, 0)) });
	ImplementationStatus.insert({ SEARCH_Probs::BINARY, row });
	row.clear();
}
Search_Test::~Search_Test()
{
	FreeInputData();
}

STATUS_GEN
Search_Test::Run()
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
Search_Test::Run(ModuleData module_data)
{
	m_module_data = module_data;
	return Run();
}

STATUS_GEN 
Search_Test::RunConsole()
{
	using namespace std;
	wcout << L"SEARCH OPERATIONS\n ---------------\n";
	
	SEARCH_Probs choice_algo		= SEARCH_Probs::END;
	OPERAND_TYPE choice_oper	= OperandType().InvalidType();
	int menuread;

	for (auto const& it : AlgoMenuEntries){ 
		wcout << to_wstring(it.Id) << Menu_Res::NumTextSep() << it.Text << Menu_Res::SS();
	}
	wcout << endl << L"Enter Algo Choice";
	wcin >> menuread;
	for (auto &it : AlgoMenuEntries){
		if (it.Id == menuread){
			choice_algo = it.EnumType;
			break;
		}
	}

	if (SEARCH_Probs::END != choice_algo) 
	{
		for (auto const& it : OperMenuEntries){
			wcout << to_wstring(it.Id) << Menu_Res::NumTextSep() << it.Text << Menu_Res::SS();
		}
		wcout << endl << L"Enter Operand Type";
		wcin >> menuread;
		for (auto &it : OperMenuEntries){
			if (it.Id == menuread){
				choice_oper = it.EnumType;
				break;
			}
		}
	}

	unsigned int findloc = NOT_FOUND;
	wchar_t dowerepeat = L'y';

	do{
		if ((SEARCH_Probs::END == choice_algo) || !(OperandType().IsValidType(choice_oper)))
			break;

		getSearchData(choice_oper);
		findloc = NOT_IMPLEMENTED;
		if (ImplementationStatus.count(choice_algo) > 0 &&
			ImplementationStatus.at(choice_algo).count(choice_oper) > 0)
			findloc = CallSearchFunc(choice_algo, choice_oper);

		switch (findloc){
		case NOT_FOUND:
			wcout << L"Item not found." << endl;
			break;
		case NOT_IMPLEMENTED:
			wcout << L"This is not implemented\n" << endl;
			break;
		default:
			wcout << L"Item found at " << findloc << endl;
			break;
		}

		fseek(stdin, 0, SEEK_END); // Clear STDIN
		wcout << L"Repeat search sub operation? (Y/N) : ";
		dowerepeat = getchar();
	} while (dowerepeat == L'y' || dowerepeat == L'Y');

	return STATUS_GEN::SUCCESS;
}
	
int 
Search_Test::CallSearchFunc(SEARCH_Probs algo, OPERAND_TYPE opertype)
{
	int findloc = NOT_FOUND;

	if (SEARCH_Probs::LINEAR == algo){
		switch (opertype){
		case OPERAND_TYPE::INT:	
			findloc = Search().LinearSearch(i_searchItem, i_searchList, SearchListSize);
			break;
		case OPERAND_TYPE::CHAR:
			findloc = Search().LinearSearch(c_searchItem, c_searchList, SearchListSize);
			break;
		case OPERAND_TYPE::STR:
			findloc = Search().LinearSearch(c_searchItem, c_searchList, SearchListSize);
			break;
		case OPERAND_TYPE::END:
		default:
			break;
		}
	}

	if (SEARCH_Probs::BINARY == algo)
	{
		switch (opertype)
		{
		case OPERAND_TYPE::INT:
			findloc = Search().BinSearch(i_searchItem, i_searchList, 0, SearchListSize - 1);
			break;
		case OPERAND_TYPE::CHAR:
			findloc = Search().BinSearch(c_searchItem, c_searchList, 0, SearchListSize - 1);
			break;
		case OPERAND_TYPE::STR:
			findloc = Search().BinSearch(c_searchItem, c_searchList, 0, SearchListSize - 1);
			break;
		case OPERAND_TYPE::END:
		default:
			break;
		}
	}
		
	return findloc;
}

/*
Asks user for search list search item
*/
STATUS_GEN 
Search_Test::getSearchData(OPERAND_TYPE opertype)
{
	FreeInputData();

	char PromptList[MAX_MENU_ITEM] = "Enter search list";
	char PromptItem[MAX_MENU_ITEM] = "Enter search Item";

	if (opertype == OPERAND_TYPE::INT){
		strcat_s(PromptList, MAX_MENU_ITEM, " separated by comma (inetger)");
		strcat_s(PromptItem, MAX_MENU_ITEM, " (inetger)");
	}
	else if (opertype == OPERAND_TYPE::CHAR){
		strcat_s(PromptList, MAX_MENU_ITEM, " no delimiter (char)");
		strcat_s(PromptItem, MAX_MENU_ITEM, " (char)");
	}
	else if (opertype == OPERAND_TYPE::STR){
		strcat_s(PromptList, MAX_MENU_ITEM, " separated by comma (string)");
		strcat_s(PromptItem, MAX_MENU_ITEM, " (string)");
	}
	else
		return STATUS_GEN::BAD_PARAM;

	// Get the search List
	char *useriplist = NULL;
	if (opertype == OPERAND_TYPE::STR)
		SearchListSize = MAXSIZE_CONSOLE_SEARCH_LIST_STRING;
	else
		SearchListSize = MAXSIZE_CONSOLE_SEARCH_LIST;

	if (useriplist = (char *)malloc(sizeof(char) * SearchListSize))
		memset(useriplist, 0, sizeof(char) * SearchListSize);
	else
		return STATUS_GEN::MEMORY_ALLOC_FAIL;

	printf("%s :", PromptList);
	scanf_s("%s", useriplist, sizeof(char) * SearchListSize);
	if (!useriplist || !(*useriplist)) {
		FREE(useriplist);
		return STATUS_GEN::BAD_INPUT;
	}
		
	if (opertype == OPERAND_TYPE::CHAR){
		if (c_searchList = (char *)malloc(sizeof(char) * SearchListSize))
			memset(c_searchList, 0, sizeof(char) * SearchListSize);
		else{
			FREE(useriplist);
			return STATUS_GEN::MEMORY_ALLOC_FAIL;
		}
		strcpy_s(c_searchList, SearchListSize-1, useriplist);
		SearchListSize = strnlen_s(c_searchList, SearchListSize);
	}else{
		if (OPERAND_TYPE::STR == opertype && !(str_searchList = (char **)malloc(sizeof(char *) * SearchListSize))){
			FREE(useriplist);
			return STATUS_GEN::BAD_INPUT;
		}
		if (OPERAND_TYPE::INT == opertype && !(i_searchList = (int *)malloc(sizeof(int) * SearchListSize))){
			FREE(useriplist);
			return STATUS_GEN::BAD_INPUT;
		}

		char *next_token = NULL, *token = NULL, *seps = " ,;:-\t";
		unsigned int cnt = 0;
		token = strtok_s(useriplist, seps, &next_token);

		while (cnt < SearchListSize && token){
			if (opertype == OPERAND_TYPE::INT){
				i_searchList[cnt++] = atoi(token);
			}
			else
				utils::AllocateAndAssignCString(&str_searchList[cnt++], token, MAXSIZE_SEARCH_ITEM);
				
			token = strtok_s(NULL, seps, &next_token);
		}
		SearchListSize = cnt;
	}
	FREE(useriplist);

	// Get the search term
	char useripitem[MAXSIZE_SEARCH_ITEM] = "\0";
	printf("%s :", PromptItem);
	scanf_s("%s", useripitem, _countof(useripitem));
	if (!useripitem || !*useripitem) return STATUS_GEN::BAD_INPUT;

	if (OPERAND_TYPE::CHAR == opertype){
		c_searchItem = *useripitem;
	}
	else if (OPERAND_TYPE::INT == opertype){
		i_searchItem = atoi(useripitem);
	}
	else if (OPERAND_TYPE::STR == opertype){
		SearchItemSize = MAXSIZE_SEARCH_ITEM;
		if (OPERAND_TYPE::STR == opertype && !(str_searchItem = (char *)malloc(sizeof(char) * SearchItemSize)))
			return STATUS_GEN::MEMORY_ALLOC_FAIL;
		utils::AllocateAndAssignCString(&str_searchItem, useripitem, SearchItemSize);
	}

	return STATUS_GEN::SUCCESS;
}

void Search_Test::FreeInputData()
{
	FREE(i_searchList);
	FREE(im_searchList);
	FREE(c_searchList);
	FREE(str_searchItem);

	utils::DeallocateCStringArray(str_searchList, SearchListSize);
}

///////////////////////////////////////////////////////////////////////////////
void TestSearch(WSTRING_VEC args)
{
	char dowerepeat = 'y';
	do{
		(new Search_Test())->Run();

		fseek(stdin, 0, SEEK_END); // Clear STDIN
		printf_s("Repeat search operation? (Y/N) : ");
		dowerepeat = getchar();
	} while ('y' == dowerepeat || 'Y' == dowerepeat);
	return;
} 