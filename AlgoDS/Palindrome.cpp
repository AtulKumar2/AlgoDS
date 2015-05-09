#include "stdafx.h"
#include "Resources.h"
#include "Utilities.h"
#include "CommonDefs.h"
#include "TestHarness.h"

///////////////////////////////////////////////////////////////////////////////
std::wstring 
to_wstring(Palindrome_Probs prob)
{
	switch (prob)
	{
	case Palindrome_Probs::IS_PALIN:	return{ L"IsPalin" }; break;
	case Palindrome_Probs::COUNT_ALL_BF: return{ L"CNTA_BF" }; break;
	case Palindrome_Probs::COUNT_ALL_Q: return{ L"CNTA_Q" }; break;
	case Palindrome_Probs::COUNT_ALL_CMP: return{ L"CNTA_CMP" }; break;
	case Palindrome_Probs::EXIT:		return{ L"Exit" }; break;
	case Palindrome_Probs::END:
	default:return{ L"" }; break;
	};
}

///////////////////////////////////////////////////////////////////////////////
bool 
Palindrome::is_palindrome(std::wstring const& text)
{
	if (text.empty()) return false;
	for (size_t i = 0, j = text.size()-1; i < (text.size() / 2); i++, j--){
		if (text[i] != text[j])
			return false;
	}
	return true;
}

// COUNT ALL UNIQUE PALINDROMES IN A GIVEN TEXT STRING
// Brute force method
// Basic Brute force technique. Works good with input string smaller than 2000.
// Dramatically bad performance beyond string bigger than 2000.
std::pair<unsigned int, std::set<std::wstring>>
Palindrome::count_all_bf(std::wstring const& text, bool getpalindromelist)
{
	if (text.empty())
		return{ 0, {} };
	std::set<std::wstring> valid;
	
	for (size_t i = 0; i < text.size(); i++){
		std::wstring testtext{ text[i] };
		if (valid.count(testtext) == 0)
			valid.insert(testtext);

		for (size_t j = i + 1; j < text.size(); j++){
			testtext += text[j];
			if (valid.count(testtext) == 0 && is_palindrome(testtext)){
				valid.insert(testtext);
			}
		}
	}
	if (getpalindromelist)
		return{ valid.size(), valid };
	else
		return{ valid.size(), {} };
}

// COUNT ALL UNIQUE PALINDROMES IN A GIVEN TEXT STRING
// Much faster than the brute force method. 
// Even with 100000 long string, it finishes in 8 seconds
// Insert a fake character like # between two consecutive characters
// Check all palindromes around each of the characters in the new string
// Length of new string will always be 2n + 1 for original string of length n.
std::pair<unsigned int, std::set<std::wstring>>
Palindrome::count_all_q(std::wstring const& input, bool getpalindromelist)
{
	using namespace std;
	const wchar_t falseval{ L'#' };

	if (input.empty())
		return{ 0, {} };

	// Add false character between consecutive positions
	// Mark each single character as palindrome
	wstring stext{L""};
	set<wstring> valid;
	for (auto const& it : input){
		auto it_ws = wstring{ it };
		stext += it_ws +wstring{ falseval };
		if (valid.count(it_ws) == 0)
			valid.insert(it_ws);
	}
	stext.erase(stext.end() - 1); // Remove last #

	auto len = stext.size();
	auto mid = len / 2;

	// First half of the 2n + 1 length string
	for (size_t pivot = 1; pivot <= mid; pivot++)
	{
		wstring pitem{L""};
		if (stext[pivot] != falseval)
			pitem += stext[pivot];

		for (size_t diff = 1; diff <= pivot; diff++){
			if ((stext[pivot + diff] == falseval)) continue;
			else if (stext[pivot - diff] != stext[pivot + diff])
				break;
			else{
				pitem = stext[pivot - diff] + pitem + stext[pivot + diff];
				if (valid.count(pitem) == 0)
					valid.insert(pitem);
			}
		}
	}
	// Second half of the 2n + 1 length string
	for (size_t pivot = mid + 1; pivot <= len - 2; pivot++)
	{
		wstring pitem{ L"" };
		if (stext[pivot] != falseval)
			pitem += stext[pivot];

		for (size_t diff = 1; diff <= (len - 1 - pivot); diff++){
			if ((stext[pivot + diff] == falseval)) continue;
			else if (stext[pivot + diff] != stext[pivot - diff])
				break;
			else{
				pitem = stext[pivot - diff] + pitem + stext[pivot + diff];
				if (valid.count(pitem) == 0)
					valid.insert(pitem);
			}
		}
	}

	if (getpalindromelist)
		return{ valid.size(), valid };
	else
		return{ valid.size(), {} };
}

///////////////////////////////////////////////////////////////////////////////
Palindrome_Test::Palindrome_Test(ModuleData module_data)
	:TestBase(module_data)
{
	unsigned int MenuId = 1;

	// New tyoe of menu
	m_subop_data = std::map<Palindrome_Probs, MenuItemInfo>{
		{ Palindrome_Probs::IS_PALIN, { L"Is it palindrome", L"IsPalin" } },
		{ Palindrome_Probs::COUNT_ALL_BF, { L"Count all BF", L"CNTA_BF" } },
		{ Palindrome_Probs::COUNT_ALL_Q, { L"Count All Quick", L"CNTA_Q" } },
		{ Palindrome_Probs::COUNT_ALL_CMP, { L"Count All Compare", L"CNTA_CMP" } },
		{ Palindrome_Probs::EXIT, { L"Exit", L"Ex" } }
	};

	for (auto const &it : m_subop_data){
		m_top_menu.Add({ static_cast<unsigned int>(it.first) }, { it.second.longtext, it.second.shorttext });
	}
	UpdateOperandMenu({ OPERAND_TYPE::INT, OPERAND_TYPE::UINT });
	m_verbose_outout = false;
	m_print_palindrome = false;
}

STATUS_GEN 
Palindrome_Test::Run()
{
	switch (m_module_data.runtype)
	{
	case RUNTYPE::ConsoleOps: return RunConsole(); break;
	case RUNTYPE::FileInput: return RunFI(); break;
	case RUNTYPE::CommandLine: return RunCL(); break;
	default:
		break;
	}
	return STATUS_GEN::BAD_INPUT;
}
STATUS_GEN 
Palindrome_Test::Run(ModuleData module_data)
{
	m_module_data = module_data;
	return Run();
}

STATUS_GEN
Palindrome_Test::RunFI()
{
	using namespace std;

	wstring
		LogHint{ __FUNCTIONW__ },
		SSD{ Logger_Res::SubStrDelim() },
		OPCS{ Common_Res::COMMENT_START() },
		OPTD{ Common_Res::TAG_SEP() },
		OPTS{ Common_Res::TUPLE_SEP() };

	auto ipblockid = m_module_data.InputBlockId;
	auto status = RetrieveFileData();
	if (STATUS_GEN::SUCCESS != status){
		WriteLog(LogHint + SSD + to_wstring(ipblockid) + SSD +
			StatusGen().str(status), LogType::ERR);
		return status;
	}

	for (auto const& it : m_testcase_settings)
	{
		if (0 == estd::compare_ci(it.first, TestCaseSettings_Res::VERBOSE())){
			m_verbose_outout = estd::any_of_ci(it.second, Common_Res::TRUE_MARKERS());
		}
		if (0 == estd::compare_ci(it.first, TestCaseSettings_Res::PRINT_PALINDROME())){
			m_print_palindrome = estd::any_of_ci(it.second, Common_Res::TRUE_MARKERS());
		}
	}

	InputFileHandler ipFileHandler{ m_module_data.ipfilename };
	OutputHandler op_handler{ m_module_data.opfilename, OutputType::File };
	op_handler.Write(estd::repeat(OPCS, 80) + L"\n");
	op_handler.Write(OPCS + to_wstring(m_module_data) + L"\n");

	auto subop = ipFileHandler.SubOp(ipblockid);
	auto problem = Palindrome_Probs::EXIT;
	for (auto const& it : m_subop_data){
		if (any_of_ci(it.second, subop)){
			problem = it.first;
			break;
		}
	}
	if (Palindrome_Probs::EXIT == problem)
		return STATUS_GEN::SUCCESS;

	HintSpecs hintspecs
	{
		{ SData_Res::HintMaxStrLength(), OPERAND_TYPE::UINT },
		{ SData_Res::HintCharLow(), OPERAND_TYPE::CHAR },
		{ SData_Res::HintCharHigh(), OPERAND_TYPE::CHAR }
	};
	
	if (0 == m_operands.size() && 0 == m_sample_hints.size())
		m_use_sample = m_use_sample_default = true;

	op_handler.Write(OPCS +
		L"Problem" + OPTD + to_wstring(problem) + OPTS +
		L"Sample" + OPTD + estd::to_wstring(m_use_sample) + OPTS +
		L"Sample Default" + OPTD + estd::to_wstring(m_use_sample_default) + OPTS + L"\n");

	if (m_use_sample)
	{
		if (m_sample_hints.empty()){
			GetSampleOperands(problem);
			DoProblems(problem);
		}
		else
		{
			SampleData sampledata(hintspecs);
			for (auto const& it : m_sample_hints){
				auto hintdata = sampledata.GetHintList(it); 
				op_handler.Write(OPCS + to_wstring(hintdata) + SSD +
						utils::date_text(utils::systemtime_now()) + L"\n");
				GetSampleOperands(problem, hintdata);
				DoProblems(problem);
			}
		}
	}

	return status;
}

STATUS_GEN
Palindrome_Test::DoProblems(Palindrome_Probs problem)
{
	using namespace std::chrono;
	using namespace std;

	if (m_str_operand.empty()){
		return STATUS_GEN::BAD_DATA;
	}

	wstring LogHint{ __FUNCTIONW__ };
	const wstring SSD{ Logger_Res::SubStrDelim() };
	const wstring SS{ L" " };

	OutputHandler op_handler;
	if (m_module_data.runtype == RUNTYPE::FileInput)
		op_handler = OutputHandler{ m_module_data.opfilename, OutputType::File };

	const wstring units{ L"seconds" };
	auto mytime_diff = [](high_resolution_clock::time_point start, high_resolution_clock::time_point end)
					{return duration_cast<seconds>(end - start).count(); };

	if (m_verbose_outout)
		op_handler.Write(L"Input string" + SSD + m_str_operand + L"\n");

	pair<unsigned int, set<wstring>> result;
	wstring message{ L"" };

	if (Palindrome_Probs::COUNT_ALL_CMP != problem)
	{
		auto start = high_resolution_clock::now();
		switch (problem) 
		{
		case Palindrome_Probs::IS_PALIN:{
			auto result = Palindrome().is_palindrome(m_str_operand);
			message = L"Palindrome Check" + SSD + estd::to_wstring(result); }
			break;
		case Palindrome_Probs::COUNT_ALL_BF:
			result = Palindrome().count_all_bf(m_str_operand, m_print_palindrome);
			message = L"Palindrome Count" + SSD + to_wstring(result.first);
			if (m_print_palindrome) message += SSD + L"Palindromes" + SSD + estd::to_wstring(result.second);
			break;
		case Palindrome_Probs::COUNT_ALL_Q:
			result = Palindrome().count_all_q(m_str_operand, m_print_palindrome);
			message = L"Palindrome Count : " + to_wstring(result.first);
			if (m_print_palindrome) message += SSD + L"Palindromes" + SSD + estd::to_wstring(result.second);
			break;
		case Palindrome_Probs::COUNT_ALL_CMP:
			break;
		default:
			return STATUS_GEN::BAD_INPUT;
		}
		auto end = high_resolution_clock::now();
		op_handler.Write(message + L"\n");
		op_handler.Write(L"Time taken" + SSD +
			to_wstring(mytime_diff(start, end)) + SS + units + L"\n");
	}

	if (Palindrome_Probs::COUNT_ALL_CMP == problem)
	{
		auto start = high_resolution_clock::now();
		auto resultbf = Palindrome().count_all_bf(m_str_operand, m_print_palindrome);
		auto time_span_BF = mytime_diff(start, high_resolution_clock::now());

		start = high_resolution_clock::now();
		auto resultq = Palindrome().count_all_q(m_str_operand, m_print_palindrome);
		auto time_span_q = mytime_diff(start, high_resolution_clock::now());

		message = L"Palindrome Count\n" +
			to_wstring(Palindrome_Probs::COUNT_ALL_BF) + SSD + to_wstring(resultbf.first) + SSD +
			L"Time taken" + SSD + to_wstring(time_span_BF) + SS + units;
		if (m_print_palindrome) 
			message += SSD + L"Palindromes" + SSD + estd::to_wstring(resultbf.second);
		message += L"\n";
		message+=
			to_wstring(Palindrome_Probs::COUNT_ALL_Q) + SSD + to_wstring(resultq.first) + SSD +
			L"Time taken" + SSD + to_wstring(time_span_q) + SS + units + L"\n";
		if (m_print_palindrome) 
			message += SSD + L"Palindromes" + SSD + estd::to_wstring(resultq.second);
		message += L"\n";
		
		op_handler.Write(message);
	}

	return STATUS_GEN::SUCCESS;
}

STATUS_GEN
Palindrome_Test::GetSampleOperands(
Palindrome_Probs problem, HintData_VEC const& hintdata)
{
	using namespace std;

	if (!m_use_sample) // How the heck we came here?
		return STATUS_GEN::PROB_NO_SOLN;

	size_t
		s_mstrlen = SampleData::Limits::maxlen_str();
	wchar_t
		s_low = L'a' ,
		s_high = L'z'; 

	CleanOperandLists();

	if (!hintdata.empty()){
		for (auto const & it : hintdata){
			if (0 == estd::compare_ci(estd::trim(it.m_name), SData_Res::HintMaxStrLength()))
				s_mstrlen = it.m_uintval;
			if (0 == estd::compare_ci(estd::trim(it.m_name), SData_Res::HintCharLow()))
				s_low = it.m_wcval;
			if (0 == estd::compare_ci(estd::trim(it.m_name), SData_Res::HintCharHigh()))
				s_high = it.m_wcval;
		}
	}

	utils::RandomData rand;
	m_str_operand = rand.Generate(s_mstrlen, s_low, s_high);

	return STATUS_GEN::SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
void Run_Palindrome(ModuleData module_data)
{
	wchar_t dowerepeat = L'y';
	do{
		(new Palindrome_Test(module_data))->Run();
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