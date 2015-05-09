#include "stdafx.h"
#include <inttypes.h>
#include "Utilities.h"
#include "estd.h"
#include "Commondefs.h"
#include "TestHarness.h"
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////
std::wstring to_wstring(BINOPS_Probs ops, bool longform)
{
	switch (ops)
	{
	case BINOPS_Probs::ADD:
		return (longform ? L"Add binary numbers": L"ADD");
		break;
	case BINOPS_Probs::EXIT:
		return (longform ? Common_Res::EXIT_LONG() : Common_Res::EXIT_SHORT());
		break;
	case BINOPS_Probs::END:
	default:
		return Common_Res::UNKNONWN();
	}
}
//std::wstring to_wstring(BINIP)
//{
//
//}

///////////////////////////////////////////////////////////////////////////////
BinOps_Test::BinOps_Test(TRACE_STATE tracestate)
{
	m_tracestate = tracestate;
	auto menu_str = [](BINOPS_Probs prob){
		return MenuItemInfo{ to_wstring(prob), to_wstring(prob, false) };
	};
	unsigned int menu_id = 1;
	m_topmenu_data = std::map<BINOPS_Probs, MenuData>
	{
		{ BINOPS_Probs::ADD, { { menu_id++ }, MenuItemInfo{ menu_str(BINOPS_Probs::ADD) } } },
		{ BINOPS_Probs::EXIT, { { menu_id++ }, MenuItemInfo{ menu_str(BINOPS_Probs::EXIT) } } },
	};
	for (auto const& it : m_topmenu_data){
		if (it.first == BINOPS_Probs::EXIT)
			m_topmenu_exitid = it.second.menu_id;
	}
}

BinOps_Test::~BinOps_Test()
{
}

STATUS_GEN
BinOps_Test::Run()
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
BinOps_Test::Run(ModuleData module_data)
{
	m_module_data = module_data;
	return Run();
}

/////////////////////////////
// Test Harness
/////////////////////////////
STATUS_GEN
BinOps_Test::RunConsole(BINOPS_Probs ops)
{
	using namespace std;
	using Problems = BINOPS_Probs;

	for (auto const &it : m_topmenu_data){
		m_top_menu.Add(it.second.menu_id, it.second.item_info); 
	}
	m_top_menu.SetHeading(ClassName());
	m_top_menu.SetExitID(m_topmenu_exitid);
	UpdateOperandMenu({ OPERAND_TYPE::CHAR, OPERAND_TYPE::STR });
	auto menu_choices = GetMenuChoices();
	
	if (!m_top_menu.IsValidId(menu_choices.subop) ||
		!m_operand_menu.IsValidId(menu_choices.subop))
		return STATUS_GEN::SUCCESS;

	BINOPS_Probs problem = BINOPS_Probs::END;
	for (auto const& it : m_topmenu_data){
		if (it.second.menu_id == menu_choices.subop)
			problem = it.first;
	}
	OPERAND_TYPE oper = GetOperandType(menu_choices.operand);

	char dowerepeat = 'y';
	do{
		STATUS_GEN status = STATUS_GEN::SUCCESS;
		if (BINOPS_Probs::ADD == problem)
		{
			vector<UINT_VEC> input;
			UINT_VEC user_data_ui;
			wstring user_data;
			for (size_t i = 1; i <= 2; i++)
			{
				wcout << L"Enter binary number " << to_wstring(i) << Menu_Res::PE();
				wcin >> user_data;
				for (auto const& it : user_data)
				{
					if (it != L'0' && it != L'1'){
						user_data_ui.clear();
						break;
					}
					try{
						user_data_ui.push_back(estd::stou(wstring{ it }));
					}
					catch (...){
						user_data_ui.clear();
						break;
					}
				}

				if (user_data_ui.empty()){
					input.clear();
					break;
				}
				else{
					input.push_back(user_data_ui);
					user_data_ui.clear();
					user_data.clear();
				}
			}
			if (input.empty()){
				wcout << SGen_Res::PROB_BAD_IP() << endl;
			}
			else{
				auto sum = BinOps().AddBinArrays(input[0], input[1]);
				wcout << L"Sum is" << Menu_Res::PE() 
					<< estd::to_wstring(sum, L"") << endl;
			}
		}

		fseek(stdin, 0, SEEK_END); // Clear STDIN
		wcout << L"More sub operation (y/n) : ";
		dowerepeat = getchar();

	} while (('y' == dowerepeat) || ('Y' == dowerepeat));

	return STATUS_GEN::SUCCESS;
}

//////////////////////////////////////////////////////////////

// Add two arrays with binary digits and return another array
// Highest array index is the least significant number
// Error will be returned if array in sum cannot contain result
// No allocation will be done inside this call. 
// Caller must provide sufficient memory
//////////////////////////////////////////////////////////////
UINT_VEC
BinOps::AddBinArrays(UINT_VEC num1, UINT_VEC num2)
{
	size_t length1 = num1.size(), length2 = num2.size();
	if (0 == length1 || 0 == length2) 
		return{};

	size_t maxlength = ((length1 > length2) ? length1 : length2) + 1; // Keep space for overflow
	size_t smalllistlength = (length1 < length2) ? length1 : length2;

	unsigned int carry = 0, localsum = 0;
	UINT_VEC rev_sum;

	for (size_t i = 0; i < smalllistlength; i++)
	{
		localsum = *(num1.rbegin() + i) + *(num2.rbegin() + i) + carry;
		rev_sum.push_back((localsum <= 1) ? localsum : ((localsum == 2) ? 0 : 1));
		carry = (localsum > 1) ? 1 : 0;
	}
	for (size_t i = smalllistlength; i < maxlength - 1; i++)
	{
		localsum = ((length1 > length2) ? 
					*(num1.rbegin() + i) : *(num2.rbegin() + i)) + carry;
		rev_sum.push_back((localsum <= 1) ? localsum : ((localsum == 2) ? 0 : 1));
		carry = (localsum > 1) ? 1 : 0;
	}

	// Overflow handling
	if (1 == carry){
		rev_sum.push_back(carry);
	}
	UINT_VEC sum;
	sum.assign(rev_sum.size(), 0);
	std::reverse_copy(rev_sum.begin(), rev_sum.end(), sum.begin());
		
	return sum;
}
