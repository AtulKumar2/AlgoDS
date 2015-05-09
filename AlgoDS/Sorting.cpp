#include "stdafx.h"
#include <cstring>
#include <cassert>
#include <inttypes.h>
#include "CommonDefs.h"
#include "Utilities.h"
#include "TestHarness.h"
#include "estd.h"

namespace
{
#define MAXSIZE_CONSOLE_SORT_LIST	1024
#define MAXSIZE_CONSOLE_SORT_LIST_STRING	MAXSIZE_CONSOLE_SORT_LIST*MAXSIZE_ITEM
}

///////////////////////////////////////////////////////////////////////////////
Sorting::Sorting(TRACE_STATE tracestate)
	: m_tracestate(tracestate)
{}

STATUS_GEN 
Sorting::BubbleSort(int *list, int low, int high)
{
	if (NULL == list || low > high) {
		return STATUS_GEN::BAD_PARAM;
	}

	for (int i = low; i < high - 1; i++)
	{
		for (int j = high; j > i; j--)
		{
			if (list[j] < list[j - 1])
			{
				int temp = list[j];
				list[j] = list[j - 1];
				list[j - 1] = temp;
			}
		}
	}
	return STATUS_GEN::SUCCESS;
}

STATUS_GEN 
Sorting::InsSort(int * list, int low, int high)
{
	if (low > high) {
		return STATUS_GEN::SUCCESS;
	}

	for (int i = low + 1; i <= high; i++)
	{
		int key = list[i];
		int j = i - 1;

		while (j >= low && list[j] > key){
			list[j + 1] = list[j];
			j--;
		}
		list[j + 1] = key;
		if (TRACE_STATE::TRACE_NO != m_tracestate)
			utils::printlist(list, low, high);
	}

	return STATUS_GEN::SUCCESS;
}
STATUS_GEN 
Sorting::InsSortR(int * list, int low, int high)
{
	if (low >= high)
		return STATUS_GEN::SUCCESS;

	InsSortR(list, low, high - 1);

	for (int i = low; i <= high - 1; i++){
		if (list[i] > list[high]){
			int temp = list[high];
			for (int j = high - 1; j >= i; j--){
				list[j + 1] = list[j];
			}
			list[i] = temp;
			break;
		}
	}

	return STATUS_GEN::SUCCESS;
}

STATUS_GEN 
Sorting::QuickSort(int * list, int low, int high)
{
	if (low < high)
	{
		int pivot = QuickSort_Partition(list, low, high);
		QuickSort(list, low, pivot - 1);
		QuickSort(list, pivot + 1, high);
		if (TRACE_STATE::TRACE_NO != m_tracestate)
			utils::printlist(list, low, high);
	}
	return STATUS_GEN::SUCCESS;
}
int 
Sorting::QuickSort_Partition(int * list, int low, int high)
{
	int PivoIndex = QuickSort_PivotIndex(low, high);
	int PivotValue = list[PivoIndex];
	utils::swapvalues(&list[PivoIndex], &list[high]);

	int StoreIndex = low;
	for (int i = low; i < high - 1; i++){
		if (list[i] < PivotValue){
			utils::swapvalues(&list[i], &list[StoreIndex]);
			StoreIndex++;
		}
	}
	utils::swapvalues(&list[StoreIndex], &list[high]);
	return StoreIndex;
}

int 
Sorting::QuickSort_PivotIndex(int low, int high)
{
	return low;
}

STATUS_GEN 
Sorting::MergeSort(int * list, int low, int high)
{
	STATUS_GEN status = STATUS_GEN::SUCCESS;
	if (low < high){
		int mid = (low + high) / 2;

		status = MergeSort(list, low, mid);
		if (STATUS_GEN::SUCCESS == status) status = MergeSort(list, mid + 1, high);

		if (STATUS_GEN::SUCCESS == status) status = Merge(list, low, mid, high);
	}
	return status;
}
STATUS_GEN 
Sorting::Merge(int * list, int low, int middle, int high)
{
	int *left = NULL, *right = NULL;
	int n1 = middle - low + 1;
	int n2 = high - middle;

	if (!(left = (int *)malloc(sizeof(int) * n1))) return STATUS_GEN::MEMORY_ALLOC_FAIL;
	if (!(right = (int *)malloc(sizeof(int) * n2))) {
		FREE(left);
		return STATUS_GEN::MEMORY_ALLOC_FAIL;
	}
	for (int i = 0; i < n1; i++)	left[i] = list[low + i];
	for (int j = 0; j < n2; j++)	right[j] = list[middle + 1 + j];

	int cl = 0, cr = 0;
	for (int a = low; a <= high; a++){
		// Either left or right has exhausted. Nothing left to compare now.
		if (cl == n1){
			for (int t = cr; t < n2; t++)
				list[a + t - cr] = right[t];
			break;
		}
		if (cr == n2){
			for (int t = cl; t < n1; t++)
				list[a + t - cl] = left[t];
			break;
		}

		// Compare top elements and put them on original list in sorted order
		if (left[cl] <= right[cr]){
			list[a] = left[cl];
			cl++;
		}
		else{
			list[a] = right[cr];
			cr++;
		}
	}
	FREE(left);
	FREE(right);

	return STATUS_GEN::SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
/// Test Harness
///////////////////////////////////////////////////////////////////////////////
STATUS_GEN
Sorting_Test::Run()
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
Sorting_Test::Run(ModuleData module_data)
{
	m_module_data = module_data;
	return Run();
}

STATUS_GEN 
Sorting_Test::RunConsole()
{
	using namespace std;

	wcout<< L"SORT OPERATIONS\n---------------" << endl;
	
	SORTING_Probs choice_algo = SORTING_Probs::END;
	OPERAND_TYPE choice_oper = OPERAND_TYPE::END;
	unsigned int menuread;

	for (auto const& it : AlgoMenuEntries){
		wcout << to_wstring(it.Id) << Menu_Res::NumTextSep() 
			<< it.Text << Menu_Res::SS();
	}
	wcout << endl << L"Enter Algo Choice:";
	wcin >> menuread;
	for (auto const& it : AlgoMenuEntries){
		if (it.Id == menuread){
			choice_algo = it.EnumType;
			break;
		}
	}
	if ((SORTING_Probs::EXIT == choice_algo)
		|| (SORTING_Probs::END == choice_algo)) {
		return STATUS_GEN::SUCCESS;
	}

	for (auto const& it : OperMenuEntries){
		wcout << to_wstring(it.Id) << 
			Menu_Res::NumTextSep() << it.Text << Menu_Res::SS();
	}
	wcout << endl << L"Enter Operand Type:";
	wcin >> menuread;
	for (auto &it : OperMenuEntries){
		if (it.Id == menuread){
			choice_oper = it.EnumType;
			break;
		}
	}
	if (OPERAND_TYPE::EXIT == choice_oper ||
		!(OperandType().IsValidType(choice_oper))) {
		return STATUS_GEN::SUCCESS;
	}

	wchar_t dowerepeat = L'y';
	do {
		getSortingData(choice_oper);

		wcout << L"Unsorted list : ";
		PrintInternalList(choice_oper);

		STATUS_GEN status = DoProblems(choice_algo, choice_oper);

		if (STATUS_GEN::SUCCESS != status){
			wcout << L"Sorting operation failed. Error " << to_wstring(status) << endl;
		}
		else{
			if (!m_sort_status){
				wcout << L"Incorrect sorting" << endl;
				wcout << L"Output list : ";
			}else{
				wcout << L"Sorted list : ";
			}
			PrintInternalList(choice_oper);
		}

		FreeInternalLists();
		wcout << L"Repeat sorting sub operation (Y/N) : ";
		wcin >> dowerepeat;
	} while (L'y' == dowerepeat || L'Y' == dowerepeat);

	return STATUS_GEN::SUCCESS;
}

STATUS_GEN 
Sorting_Test::DoProblems(SORTING_Probs probs, OPERAND_TYPE opertype)
{
	STATUS_GEN status = STATUS_GEN::SUCCESS;

	if (OPERAND_TYPE::INT == opertype)
	{
		auto sort_size = m_i_input.size();
		auto sort_list = estd::AllocateArrays().filled(m_i_input);
		if (!sort_list) {
			return STATUS_GEN::MEMORY_ALLOC_FAIL;
		}
		switch (probs)
		{
		case SORTING_Probs::QUICK:
			status = Sorting().BubbleSort(sort_list, 0, sort_size - 1);
			break;
		case SORTING_Probs::INSERTION:
			status = Sorting().QuickSort(sort_list, 0, sort_size - 1);
			break;
		case SORTING_Probs::INSERTION_R:
			status = Sorting().InsSortR(sort_list, 0, sort_size - 1);
			break;
		case SORTING_Probs::MERGE:
			status = Sorting().MergeSort(sort_list, 0, sort_size - 1);
			break;
		case SORTING_Probs::BUBBLE:
			status = Sorting().BubbleSort(sort_list, 0, sort_size - 1);
			break;
		case SORTING_Probs::EXIT:
		case SORTING_Probs::END:
		default:
			return STATUS_GEN::PROB_BAD_IP;
			break;
		}
		if (status != STATUS_GEN::SUCCESS)
			return status;

		std::sort(m_i_input.begin(), m_i_input.end());
		m_sort_status = true;
		for (size_t i = 0; i < m_i_input.size(); i++)
		{
			if (m_i_input[i] != sort_list[i]) {
				m_sort_status = false;
			}
			m_i_input[i] = sort_list[i];
		}
		delete[] sort_list;
	}

	//if (SORTING_Probs::BUBBLE == probs)
	//{
	//	if (OPERAND_TYPE::INT == opertype)
	//		status = Sorting().BubbleSort(i_sortList, 0, SortListSize - 1);
	//	else if (OPERAND_TYPE::CHAR == opertype)
	//		status = Sorting().BubbleSort(c_sortList, 0, SortListSize - 1);
	//	else if (OPERAND_TYPE::STR == opertype)
	//		status = Sorting().BubbleSort(str_sortList, 0, SortListSize - 1);
	//}
	//if (SORTING_Probs::QUICK == probs)
	//{
	//	if (OPERAND_TYPE::INT == opertype)
	//		status = Sorting().QuickSort(i_sortList, 0, SortListSize - 1);
	//	else if (OPERAND_TYPE::CHAR == opertype)
	//		status = Sorting().QuickSort(c_sortList, 0, SortListSize - 1);
	//	else if (OPERAND_TYPE::STR == opertype)
	//		status = Sorting().QuickSort(str_sortList, 0, SortListSize - 1);
	//}
	//if (SORTING_Probs::INSERTION == probs)
	//{
	//	if (OPERAND_TYPE::INT == opertype)
	//		status = Sorting().InsSort(i_sortList, 0, SortListSize - 1);
	//	else if (OPERAND_TYPE::CHAR == opertype)
	//		status = Sorting().InsSort(c_sortList, 0, SortListSize - 1);
	//	else if (OPERAND_TYPE::STR == opertype)
	//		status = Sorting().InsSort(str_sortList, 0, SortListSize - 1);
	//}
	//if (SORTING_Probs::INSERTION_R == probs)
	//{
	//	if (OPERAND_TYPE::INT == opertype)
	//		status = Sorting().InsSortR(i_sortList, 0, SortListSize - 1);
	//	else if (OPERAND_TYPE::CHAR == opertype)
	//		status = Sorting().InsSortR(c_sortList, 0, SortListSize - 1);
	//	else if (OPERAND_TYPE::STR == opertype)
	//		status = Sorting().InsSortR(str_sortList, 0, SortListSize - 1);
	//}
	//if (SORTING_Probs::MERGE == probs)
	//{
	//	if (OPERAND_TYPE::INT == opertype)
	//		status = Sorting().MergeSort(i_sortList, 0, SortListSize - 1);
	//	else if (OPERAND_TYPE::CHAR == opertype)
	//		status = Sorting().MergeSort(c_sortList, 0, SortListSize - 1);
	//	else if (OPERAND_TYPE::STR == opertype)
	//		status = Sorting().MergeSort(str_sortList, 0, SortListSize - 1);
	//}

	return STATUS_GEN::SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//Asks user for search list search item
///////////////////////////////////////////////////////////////////////////////
STATUS_GEN 
Sorting_Test::getSortingData(OPERAND_TYPE opertype)
{
	if (!OperandType().IsValidType(opertype))
		return STATUS_GEN::BAD_PARAM;

	FreeInternalLists();

	std::wstring input{ L"" };
	std::wstring prompt{ L"Enter unsorted list" };
	
	switch (opertype)
	{
	case OPERAND_TYPE::INT:
	case OPERAND_TYPE::UINT:
		prompt += Menu_Res::SS() + L"separated by comma (integral values)";
		break;
	case OPERAND_TYPE::CHAR:
		prompt += Menu_Res::SS() + L"no delimiter (char)";
		break;
	case OPERAND_TYPE::STR:
		prompt += Menu_Res::SS() + L"separated by comma (string)";
		break;
	case OPERAND_TYPE::EXIT:
	case OPERAND_TYPE::END:
	default:
		return STATUS_GEN::BAD_INPUT;
		break;
	}

	// Get user input
	std::wcout << prompt << Menu_Res::SS() << Menu_Res::PE();
	std::wcin >> input;
	if (input.empty()) {
		return STATUS_GEN::BAD_INPUT;
	}

	try
	{
		if (opertype == OPERAND_TYPE::CHAR)
		{
			for (auto const& it : input) {
				m_wc_input.push_back(it);
			}
		}
		else
		{
			estd::Tokenizer<std::wstring> tokenizer{ L",;:", estd::TokenizerOP::TRIMMED };

			if (OPERAND_TYPE::STR == opertype) {
				m_wstr_input = tokenizer.Tokens(input);
			}
			else if (OPERAND_TYPE::INT == opertype) {
				auto temp_list = tokenizer.Tokens(input);
				for (auto const& it : temp_list) {
					m_i_input.push_back(_wtoi(it.c_str()));
				}
			}
			else if (OPERAND_TYPE::UINT == opertype) {
				auto temp_list = tokenizer.Tokens(input);
				for (auto const& it : temp_list) {
					m_i_input.push_back(estd::stou(it));
				}
			}

		}
	}
	catch (...) 
	{
		return STATUS_GEN::BAD_INPUT;
	}

	return STATUS_GEN::SUCCESS;
}

Sorting_Test::Sorting_Test(TRACE_STATE tracestate)
{
	m_tracestate = tracestate;

	// Top level menu
	unsigned int MenuId = 1;
	AlgoMenuEntries.push_back(SortMenuItem(
		SORTING_Probs::BUBBLE, MenuId++, L"L", L"Bubble Sort"));
	AlgoMenuEntries.push_back(SortMenuItem(
		SORTING_Probs::QUICK, MenuId++, L"B", L"Quick Sort"));
	AlgoMenuEntries.push_back(SortMenuItem(
		SORTING_Probs::MERGE, MenuId++, L"L", L"Merge Sort"));
	AlgoMenuEntries.push_back(SortMenuItem(
		SORTING_Probs::INSERTION, MenuId++, L"B", L"Insertion Sort"));
	AlgoMenuEntries.push_back(SortMenuItem(
		SORTING_Probs::INSERTION_R, MenuId++, L"L", L"Insertion sort recursive"));
	AlgoMenuEntries.push_back(SortMenuItem(
		SORTING_Probs::EXIT, MenuId++, L"E", L"Exit"));
	std::sort(AlgoMenuEntries.begin(), AlgoMenuEntries.end());

	// Operand menu
	OperMenuEntries = OperandType().MenuEntries();
}

void 
Sorting_Test::PrintInternalList(OPERAND_TYPE opertype)
{
	std::wstring output{};

	switch (opertype)
	{
	case OPERAND_TYPE::INT:
		output = estd::to_wstring(m_i_input); break;
	case OPERAND_TYPE::UINT:
		output = estd::to_wstring(m_ui_input); break;
	case OPERAND_TYPE::CHAR:
		output = estd::to_wstring(m_wc_input); break;
	case OPERAND_TYPE::STR:
		output = estd::to_wstring(m_wstr_input); break;
	case OPERAND_TYPE::EXIT:
	case OPERAND_TYPE::END:
	default:
		break;
	}
	std::wcout << output << std::endl;
	return;
}

void 
Sorting_Test::FreeInternalLists()
{
	FREE(i_sortList)
	FREE(c_sortList)
	utils::DeallocateCStringArray(str_sortList, SortListSize);
	
	m_i_input.clear();
	m_ui_input.clear();
	m_wc_input.clear();
	m_wstr_input.clear();
}

Sorting_Test::~Sorting_Test()
{
	FreeInternalLists();
}

///////////////////////////////////////////////////////////////////////////////
void TestSorting(WSTRING_VEC args)
{
	wchar_t dowerepeat = L'y';
	do{
		(new Sorting_Test())->Run();
			
		std::wcout << L"Repeat sorting operation? (Y/N) : ";
		std::wcin >> dowerepeat;
	} while (L'y' == dowerepeat || L'Y' == dowerepeat);
}