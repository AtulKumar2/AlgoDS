#include "stdafx.h"
#include "Resources.h"
#include "Utilities.h"
#include "CommonDefs.h"
#include "TestHarness.h"
#include <tuple>
#include <bitset>

///////////////////////////////////////////////////////////////////////////////
std::wstring to_wstring(ModuleData const& mdata)
{
	std::wstring tag_delim{ L"=" };
	std::wstring tag_sep{ L"; " };

	std::wstring ret{
		std::wstring{ L"InputBlock ID" } +tag_delim + std::to_wstring(mdata.InputBlockId)
		+ tag_sep + L"Run Type" + tag_delim + to_wstring(mdata.runtype)
		+ tag_sep + L"Trace State" + tag_delim + to_wstring(mdata.tracestate)
		+ tag_sep + L"Major Op" + tag_delim + to_wstring(mdata.majorops)
		+ tag_sep + L"Input File" + tag_delim + mdata.ipfilename
		+ tag_sep + L"Run Type" + tag_delim + mdata.opfilename
		+ tag_sep + L"Program arguments" + tag_delim + estd::to_wstring(mdata.args) 
	};

	return ret;
}

///////////////////////////////////////////////////////////////////////////////
/// Implementation of TestBase
///////////////////////////////////////////////////////////////////////////////
TestBase::TestBase(ModuleData module_data) 
	:m_module_data(module_data)
{
	m_tracestate = m_module_data.tracestate;
	m_use_sample = false;
	m_use_sample_default = false;
	//m_sample_hint_datas = {};

	m_output_block = {};

	m_operands = {};
	m_i_operands = {};
	m_ui_operands = {};
	m_c_operands = {};
	m_str_operands = {};
	m_testcase_settings = {};
}

std::wstring 
TestBase::ClassName() {
	return MajorOps().GetName(m_module_data.majorops); 
}
MAJOR_OPS 
TestBase::MajorOP(){
	return m_module_data.majorops; 
}

STATUS_GEN 
TestBase::Run(ModuleData moduleData)
{
	return STATUS_GEN::UNIMPLEMENTED;
}
STATUS_GEN 
TestBase::Run()
{
	return STATUS_GEN::UNIMPLEMENTED;
}
STATUS_GEN 
TestBase::RunConsole(){
	return STATUS_GEN::UNIMPLEMENTED;
}
STATUS_GEN 
TestBase::RunCL(){
	return STATUS_GEN::UNIMPLEMENTED;
}
STATUS_GEN 
TestBase::RunFI(){
	return STATUS_GEN::UNIMPLEMENTED;
}

STATUS_GEN 
TestBase::RetrieveFileData(ModuleData const& module_data)
{
	m_module_data = module_data;
	return RetrieveFileData();
}
STATUS_GEN 
TestBase::RetrieveFileData()
{
	using namespace std;

	unsigned int IpBlockId = m_module_data.InputBlockId;
	wstring SSD = Logger_Res::SubStrDelim();
	wstring LogHint = __FUNCTIONW__ + SSD + L"Block " + to_wstring(IpBlockId);

	// Validity
	InputFileHandler iphandler(m_module_data.ipfilename, InputFileHandler::LOG_LEVEL::BLOCK);
	if (!iphandler.IsAvailable()){
		WriteLog(LogHint + SSD + to_wstring(STATUS_GEN::MISSING_FILE), LogType::ERR);
		return STATUS_GEN::MISSING_FILE;
	}

	// Operands
	m_operand_type = iphandler.GetOperandType(IpBlockId);
	m_operands = iphandler.Operands(IpBlockId);

	// Sample Data
	iphandler.GetSampleDataOptions(IpBlockId, m_use_sample, m_use_sample_default);
	m_sample_hints = iphandler.SampleHints(IpBlockId);

	// Output block
	auto opblockid = iphandler.OutputBlockId(IpBlockId);
	if (STATUS_GEN::SUCCESS == opblockid.second){
		if (iphandler.IsValidBlockId(m_output_block.m_Id = opblockid.first)){
			if (STATUS_GEN::SUCCESS ==
				iphandler.GetOpBlockData(m_output_block.m_Id, m_output_block)){
					m_output_block.m_optype = 
						OutputBlockInfo().Type(m_output_block.m_target);
			}
		}
	}
	
	// Miscellaneous settings
	auto testcase_settings = iphandler.TestCaseSettings(IpBlockId);

	estd::Tokenizer < wstring > 
		tokenizer{ estd::TokenizerOP::NONEMPTYTRIMMED };
	for (auto const& it : tokenizer.Tokens(testcase_settings, Common_Res::TUPLE_SEP()))
	{
		auto setting_tokens = tokenizer.Tokens(it, Common_Res::TUPLE_INNERSEP());
		if (setting_tokens.size() == 2)
			m_testcase_settings.push_back({ setting_tokens[0], setting_tokens[1] });
	}
	
	// Sub operations
	m_subops = tokenizer.Tokens(iphandler.SubOp(IpBlockId), Common_Res::TUPLE_SEP());

	// Error scenarios
	auto message = LogHint + SSD + to_wstring(STATUS_GEN::PROB_BAD_IP);
	if (m_subops.empty()){
		WriteLog(message + SSD + L"No subop given", LogType::ERR);
		return STATUS_GEN::PROB_BAD_IP;
	}
	if (!OperandType().IsValidType(m_operand_type)){
		WriteLog(message + SSD + L"No operand type given", LogType::ERR);
		return STATUS_GEN::PROB_BAD_IP;
	}
	if (0 != m_operands.size() && 0 != m_sample_hints.size()){
		WriteLog(message + SSD + L"Both Operands and sample hints cannot be given", LogType::ERR);
		return STATUS_GEN::PROB_BAD_IP;
	}

	// Conclusion
	if (m_sample_hints.size() != 0){
		m_use_sample = true;
		m_use_sample_default = false;
	}
	if (0 == m_operands.size() && 0 == m_sample_hints.size()){
		m_use_sample = true;
		m_use_sample_default = true;
	}

	return STATUS_GEN::SUCCESS;
}

TestBase::MenuChoices 
TestBase::GetMenuChoices()
{
	MenuChoices menu_choices{ m_top_menu.InvalidId(), m_operand_menu.InvalidId() };
	if (m_top_menu.IsValidId(menu_choices.subop = GetMenuChoices(m_top_menu)))
		menu_choices.operand = GetMenuChoices(m_operand_menu);

	return menu_choices;
}
TestBase::MenuChoices 
TestBase::GetMenuChoices(std::list<ConsoleMenu::Flags> const& flags)
{
	m_top_menu.SetFlags(flags);
	m_operand_menu.SetFlags(flags);
	return GetMenuChoices();
}

MenuId 
TestBase::GetMenuChoices(ConsoleMenu const& menu,
	ConsoleMenu::DisplaySort sort_option, std::list<ConsoleMenu::Flags> const& display_option)
{
	auto validity = menu.IsValid();
	if (!validity.first){
		std::wcout << L"Bad menu : " << validity.second << std::endl;
		return menu.InvalidId();
	}
	return menu.Choice(sort_option, display_option);
}

STATUS_GEN
TestBase::UpdateOperandMenu(std::vector<OPERAND_TYPE> const &skip_entries)
{
	m_operand_menu_data = OperandType().MenuData(skip_entries);
	for (auto const& it : m_operand_menu_data){
		m_operand_menu.Add({ it.idnum }, { it.long_text, it.short_text });
	}
	m_operand_menu.SetHeading(L"Enter Input Type");
	m_operand_menu.SetExitID({ OperandType().ExitItemData(skip_entries).idnum });
	return STATUS_GEN::SUCCESS;
}

OPERAND_TYPE 
TestBase::GetOperandType(MenuId const & menu_id) const
{
	for (auto const &it : m_operand_menu_data){
		if (it.idnum == menu_id.m_val)
			return it.id;
	}
	return OperandType().InvalidType();
}
STATUS_GEN
TestBase::GetOperands(OPERAND_TYPE operand, bool use_sample, bool use_sample_default)
{
	return STATUS_GEN::UNIMPLEMENTED;
}

void TestBase::CleanOperandLists()
{
	m_str_operand.clear();
	m_i_operands.clear();
	m_ui_operands.clear();
	m_c_operands.clear();
	m_str_operands.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// Generate Sample Data
///////////////////////////////////////////////////////////////////////////////
std::wstring 
to_wstring(HintData const& hint_data)
{
	using namespace std;

	wstring ret = hint_data.m_name + SData_Res::HintInnerSep();
	switch (hint_data.m_type)
	{
	case OPERAND_TYPE::INT:  ret += to_wstring(hint_data.m_intval); break;
	case OPERAND_TYPE::UINT: ret += to_wstring(hint_data.m_uintval); break;
	case OPERAND_TYPE::CHAR: ret += to_wstring(hint_data.m_wcval); break;
	case OPERAND_TYPE::STR:	ret += hint_data.m_textval; break;
	default: ret += SData_Res::HintUnk(); break;
	}
	return ret;
}
std::wstring 
to_wstring(HintData_VEC const& C)
{
	std::wstring ret = L"";
	for (auto const &it : C){
		ret += to_wstring(it) + Common_Res::TUPLE_SEP();
	}
	if (!ret.empty())	ret.erase(ret.end() - 1);
	return ret;
}

SampleData::SampleData(std::wstring const& Hints, HintSpecs const& hintspecs)
	:m_hints(Hints), m_hintspecs(hintspecs){
}

HintData_VEC
SampleData::GetHintList(std::wstring const& Hints){
	return GetHintList(Hints, m_hintspecs);
}
HintData_VEC
SampleData::GetHintList(HintSpecs const& hintspecs){
	return GetHintList(m_hints, hintspecs);
}
HintData_VEC
SampleData::GetHintList(std::wstring const& Hints, HintSpecs const& hintspecs)
{
	using namespace std;
	if (Hints.empty() || hintspecs.empty()) return{};

	wstring SSD{ Logger_Res::SubStrDelim() };
	estd::Tokenizer<wstring> tokenizer(
		Common_Res::TUPLE_SEP(), estd::TokenizerOP::NONEMPTYTRIMMED);
	auto HintTuples = tokenizer.Tokens(Hints);
	if (HintTuples.size() == 0) return{};

	HintSpecs hintspecs_lc;
	for (auto const& it : hintspecs){
		if (!OperandType().IsValidType(it.second)){
			WriteLog(Hints + SSD + ErrBadHintType() + SSD + it.first, LogType::WARN);
			continue;
		}
		hintspecs_lc.insert({ estd::tolower(it.first), it.second });
	}

	HintData_VEC HintDatas;
	// Total:<+ve num>,min:<+ve num>,max:<+ve num>
	for (auto &it : tokenizer.Tokens(Hints))
	{
		//max:<+ve num>
		WSTRING_VEC tokens = tokenizer.Tokens(it, HintInnerSep());
		if (tokens.size() != 2){
			WriteLog(Hints + SSD + it + SSD + ErrMalformedHint() + SSD + it, LogType::WARN);
			continue;
		}
		if (hintspecs_lc.count(estd::tolower(tokens[0])) == 0){
			WriteLog(Hints + SSD + it + SSD + ErrHintNotHinted() + SSD + it, LogType::WARN);
			continue;
		}
		HintData hintdata;
		hintdata.m_type = hintspecs_lc[estd::tolower(tokens[0])];
		hintdata.m_name = tokens[0];

		try{
			switch (hintdata.m_type){
			case OPERAND_TYPE::INT: hintdata.m_intval = stoi(tokens[1]); break;
			case OPERAND_TYPE::UINT:hintdata.m_uintval = estd::stou(tokens[1]); break;
			case OPERAND_TYPE::STR:hintdata.m_textval = tokens[1]; break;
			case OPERAND_TYPE::CHAR:hintdata.m_wcval = tokens[1].c_str()[0]; break;
			default:
				break;
			}
		}
		catch (exception e){
			WriteLog(Hints + SSD + it + SSD + ErrBadHintData(), LogType::WARN);
			continue;
		}
		HintDatas.insert(HintDatas.end(), hintdata);
	}
	return HintDatas;
}

HintData_VEC 
SampleData::Generate(HintSpecs const& hintspecs)
{
	using namespace std;
	auto SSD{ Logger_Res::SubStrDelim() };
	HintData_VEC HintDatas;
	if (hintspecs.size() == 0) return HintDatas;

	//for (auto const& it : IncludeHintTypes)
	//{
	//	HintType hintType = it;
	//	unsigned int val;

	//	switch (hintType)
	//	{
	//	case HintType::TOTAL:
	//		RandomData().Generate()
	//	case HintType::MIN_UINT:
	//	case HintType::MAX_UINT:
	//	case HintType::MAXSTRLENGTH:
	//		try{
	//			HintDatas.push_back(HintData{ hintType, { estd::stou(tokens[1]) } });
	//		}
	//		catch (exception e){
	//			WriteLog(Hints + SSD + ErrBadHintData() + SSD + it, LogType::WARN);
	//			continue;
	//		}
	//		break;
	//	case HintType::MIN_INT:
	//	case HintType::MAX_INT:
	//		try{
	//			HintDatas.push_back(HintData{ hintType, { stoi(tokens[1]) } });
	//		}
	//		catch (exception e){
	//			WriteLog(Hints + SSD + ErrBadHintData() + SSD + it, LogType::WARN);
	//			continue;
	//		}
	//		break;
	//	case HintType::END:
	//	default:
	//		break;
	//	}
	//}

	return HintDatas;
}
size_t 
SampleData::Generate(INT_VEC& Samples, size_t Total, int Min, int Max)
{
	utils::RandomData().Generate(Samples, Total, Min, Max);
	return Samples.size();
}
size_t 
SampleData::Generate(WSTRING_VEC& Samples, size_t Total, size_t MaxStrLength)
{
	utils::RandomData().Generate(Samples, Total, MaxStrLength);
	return Samples.size();
}
size_t 
SampleData::Generate(WCHAR_VEC& Samples, size_t Total)
{
	utils::RandomData().Generate(Samples, Total);
	return Samples.size();
}

///////////////////////////////////////////////////////////////////////////////
/// Get User choices on console menu
///////////////////////////////////////////////////////////////////////////////
std::wstring 
UserMenu::MenuMsgStr(MenuMsgOpts option, std::wstring const& additional)
{
	return MenuMsgStr(m_MajorOp, option, additional);
}
std::wstring 
UserMenu::MenuMsgStr(
MAJOR_OPS majorop, MenuMsgOpts option, std::wstring const& additional)
{
	std::wstringstream wss;
	std::wstring MajorOp = MajorOps().GetName(majorop);

	switch (option)
	{
	case MenuMsgOpts::HEADING_MAJOROP: //L"CLRS_Ch4 operations\n---------------"
		wss << MajorOp + SS() + Ops() << L"\n" << UL; break;
	case MenuMsgOpts::REPEAT_MAJOROP: //L"Repeat CLRS_Ch4 operation? (Y/N) : "
		wss << Rpt() + SS() + MajorOp + SS() + Ops() << L"?" << SS() + YN() << SS() + PE(); break;
	case MenuMsgOpts::REPEAT_SUBOP: //L"Repeat sorting sub operation? (Y/N) : "
		wss << Rpt() + SS() + MajorOp + SS() + SOps() << L"?" << SS() + YN() << SS() + PE(); break;
	case MenuMsgOpts::USER_INPUT: // L"Enter choice : "
		wss << UIp() << SS() + PE(); 	break;
	case MenuMsgOpts::SAMPLE_DATA: // L"Sample Data? (Y/N) : "
		wss << SD() << L"?" << SS() + YN() << SS() + PE(); break;
	case MenuMsgOpts::SAMPLE_DATA_DEFAULT: // "Default Samples? (Y/N)  : "
		wss << DEF() + SS() + SAM() << QM() << SS() + YN() << SS() + PE(); break;
	case MenuMsgOpts::SAMPLE_DATA_TOTAL: // "Samples - Total (+ve Int) : "
		wss << SAM() << SS() + HY() << SS() + TOT() << SS() + PI() << SS() + PE(); break;
	case MenuMsgOpts::SAMPLE_DATA_LOWEST: // "Samples - Minimum value (Int) : "
		wss << SAM() << SS() + HY() << SS() + MIN() << SS() + INT() << SS() + PE(); break;
	case MenuMsgOpts::SAMPLE_DATA_HIGHEST: // "Samples - Maximum value (Int) : "
		wss << SAM() << SS() + HY() << SS() + MAX() << SS() + INT() << SS() + PE(); break;
	case MenuMsgOpts::SAMPLE_DATA_MAXSTRLENGTH: // "Samples - Maximum string length (+ve Int) : "
		wss << SAM() << SS() + HY() << SS() + MAX() << SS() + SL() << SS() + PI() << SS() + PE(); break;
	case MenuMsgOpts::OPERAND_INPUT_INT: // "Enter (Int) Separated by (...) : "
		wss << EN() << SS() + INT() << SS() + SEP() << SS() + SBR() << additional << SS() + EBR() << SS() + PE(); break;
	case MenuMsgOpts::OPERAND_INPUT_UINT: // "Enter (+ve Int) Separated by (...) : "
		wss << EN() << SS() + PI() << SS() + SEP() << SS() + SBR() << additional << SS() + EBR() << SS() + PE(); break;
	case MenuMsgOpts::OPERAND_INPUT_CHAR: // "Enter (Char) Separated by (...) : "
		wss << EN() << SS() + CH() << SS() + SEP() << SS() + SBR() << additional << SS() + EBR() << SS() + PE(); break;
	case MenuMsgOpts::OPERAND_INPUT_STR: // "Enter (String) Separated by (...) : "
		wss << EN() << SS() + STR() << SS() + SEP() << SS() + SBR() << additional << SS() + EBR() << SS() + PE(); break;
	case MenuMsgOpts::BAD_INPUT: // "Bad Input "
		wss << BI(); break;
	default:
		wss << UNK() << SS() + PE(); break;
	}
	return wss.str();
}

void 
UserMenu::SampleChoice(bool& OptForSample, bool& DefaultSamples)
{
	OptForSample = DefaultSamples = false;
	wchar_t menuread_c = L'n';
	utils::clear_wcin();
	std::wcout << MenuMsgStr(MenuMsgOpts::SAMPLE_DATA);
	std::wcin >> menuread_c;
	if (OptForSample = (L'Y' == menuread_c || L'y' == menuread_c)){
		menuread_c = L'n';
		utils::clear_wcin();
		std::wcout << MenuMsgStr(MenuMsgOpts::SAMPLE_DATA_DEFAULT);
		std::wcin >> menuread_c;
		DefaultSamples = (L'Y' == menuread_c || L'y' == menuread_c);
	}
}

OPERAND_TYPE 
UserMenu::OperandMenuChoice(std::vector<OperMenuItem> const& OperMenuEntries)
{
	unsigned int menuread_i;
	utils::clear_wcin();
	for (auto &it : OperMenuEntries){
		std::wcout << it.Id << NumTextSep << it.Text << SS;
	}
	std::wcout << std::endl << MenuMsgStr(MenuMsgOpts::USER_INPUT);
	std::wcin >> menuread_i;
	for (auto &it : OperMenuEntries){
		if (it.Id == menuread_i)
			return it.EnumType;
	}
	return OperandType().InvalidType();
}
void 
UserMenu::SampleStyle(
size_t& Total, int& Lowest, int& Highest, OPERAND_TYPE opertype)
{
	utils::clear_wcin();
	std::wcout << MenuMsgStr(MenuMsgOpts::SAMPLE_DATA_TOTAL);
	std::wcin >> Total;
	if (OPERAND_TYPE::INT == opertype)
	{
		utils::clear_wcin();
		std::wcout << MenuMsgStr(MenuMsgOpts::SAMPLE_DATA_LOWEST);
		std::wcin >> Lowest;

		utils::clear_wcin();
		std::wcout << MenuMsgStr(MenuMsgOpts::SAMPLE_DATA_HIGHEST);
		std::wcin >> Highest;
	}
}
void 
UserMenu::SampleStyle(
size_t& Total, unsigned int& Lowest, unsigned int& Highest)
{
	using namespace std;
	utils::clear_wcin();
	wcout << MenuMsgStr(MenuMsgOpts::SAMPLE_DATA_TOTAL);
	wcin >> Total;

	utils::clear_wcin();
	wcout << MenuMsgStr(MenuMsgOpts::SAMPLE_DATA_LOWEST);
	wcin >> Lowest;

	utils::clear_wcin();
	wcout << MenuMsgStr(MenuMsgOpts::SAMPLE_DATA_HIGHEST);
	wcin >> Highest;
}
void 
UserMenu::SampleStyle(size_t& Total, size_t& MaxStrLength)
{
	utils::clear_wcin();
	std::wcout << MenuMsgStr(MenuMsgOpts::SAMPLE_DATA_MAXSTRLENGTH);
	std::wcin >> MaxStrLength;
}

void 
UserMenu::GetOperands(
UINT_VEC& Operands, std::wstring const& Separators)
{
	using std::wstring;

	wchar_t in;
	wstring seq;

	utils::clear_wcin();
	std::wcout << MenuMsgStr(MenuMsgOpts::OPERAND_INPUT_UINT);

	while (std::wcin >> std::noskipws >> in){
		if ((wstring::npos != InputBreak().find(in)) ||
			(wstring::npos != Separators.find(in))){
			if (!seq.empty()){
				try{
					if (std::stoi(seq) < 0)
						throw;
					else
						Operands.push_back(std::stoi(seq));
				}
				catch (std::exception e){
					std::wcout << MenuMsgStr(MenuMsgOpts::BAD_INPUT) << std::endl;
					break;
				}
				seq.clear();
			}
			if (std::wstring::npos != InputBreak().find(in))
				break;
			else
				continue;
		}
		seq.push_back(in);
	}
	return;
}
void 
UserMenu::GetOperands(
INT_VEC& Operands, std::wstring const& Separators)
{
	using std::wstring;

	wchar_t in;
	wstring seq;

	utils::clear_wcin();
	std::wcout << MenuMsgStr(MenuMsgOpts::OPERAND_INPUT_INT);

	while (std::wcin >> std::noskipws >> in){
		if ((wstring::npos != InputBreak().find(in)) ||
			(wstring::npos != Separators.find(in))){
			if (!seq.empty()){
				try{
					Operands.push_back(std::stoi(seq));
				}
				catch (std::exception e){
					std::wcout << MenuMsgStr(MenuMsgOpts::BAD_INPUT) << std::endl;
					break;
				}
				seq.clear();
			}
			if (wstring::npos != InputBreak().find(in))
				break;
			else
				continue;
		}
		seq.push_back(in);
	}
	return;
}

// Keep pushing characters in a sequence till a separator is found.
// Find the first non-whitespace character in sequence and discard everything else.
void 
UserMenu::GetOperands(
WCHAR_VEC& Operands, std::wstring const& Separators)
{
	using std::wstring;

	wchar_t in;
	std::wstring seq;

	utils::clear_wcin();
	std::wcout << MenuMsgStr(MenuMsgOpts::OPERAND_INPUT_CHAR);

	while (std::wcin >> std::noskipws >> in){
		if ((wstring::npos != InputBreak().find(in)) ||
			(wstring::npos != Separators.find(in))){
			if (!seq.empty()){
				std::locale loc;
				for (auto &it : seq){
					if (!std::isspace(it, loc)){
						Operands.push_back(in);
						break; // Ignore remianing characters
					}
				}
				seq.clear();
			}
			if (wstring::npos != InputBreak().find(in))
				break;
			else
				continue;
		}
		seq.push_back(in); // Keep pushing till we find a separator
	}
	return;
}
void 
UserMenu::GetOperands(
WSTRING_VEC& Operands, std::wstring const& Separators)
{
	wchar_t in;
	std::wstring seq;

	utils::clear_wcin();
	std::wcout << MenuMsgStr(MenuMsgOpts::OPERAND_INPUT_STR);

	while (std::wcin >> std::noskipws >> in){
		if ((std::wstring::npos != InputBreak().find(in)) ||
			(std::wstring::npos != Separators.find(in))){
			if (!seq.empty()){
				Operands.push_back(seq);
				seq.clear();
			}
			if (std::wstring::npos != InputBreak().find(in))
				break;
			else
				continue;
		}
		seq.push_back(in);
	}
	return;
}

///////////////////////////////////////////////////////////////////////////////
/// Menu display for user /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool 
any_of(MenuItemInfo const& c, std::wstring const& item){
	return estd::any_of(item, { c.longtext, c.shorttext });
}
bool 
any_of_ci(MenuItemInfo const& c, std::wstring item){
	return estd::any_of_ci(item, { c.longtext, c.shorttext });
}

ConsoleMenu::ConsoleMenu(
	std::wstring const& heading, 
	std::vector<std::pair<MenuId, MenuItemInfo>> const& items, 
	MenuId const& exit_id, 
	std::list<Flags> const& flags)
{
	m_heading = m_heading;
	Add(items);
	m_exit_id = exit_id;
	m_display_options.reset();
	SetFlags({ Flags::Horizontal, Flags::LongText, Flags::ShowHeading });
	SetFlags(flags);
}

ConsoleMenu::~ConsoleMenu(){
}
ConsoleMenu::ConsoleMenu(ConsoleMenu const& src){
	*this = src;
}
ConsoleMenu& 
ConsoleMenu::operator=(ConsoleMenu const& src){
	if (this != &src){
		m_heading = src.m_heading;
		m_items = src.m_items;
		m_exit_id = src.m_exit_id;
		m_display_options = src.m_display_options;
	}
	return *this;
}

MenuId 
ConsoleMenu::Choice(DisplaySort display_sort, std::list<Flags> const& flags)
const
{
	using namespace std;

	FlagMask display_options = m_display_options;
	ModifyFlagMask(display_options, flags);

	bool show_heading = display_options[static_cast<size_t>(Flags::ShowHeading)];
	bool show_largetext = display_options[static_cast<size_t>(Flags::LongText)];
	bool show_vertical = display_options[static_cast<size_t>(Flags::Vertical)];

	if (show_heading && !m_heading.empty())
		wcout << m_heading << endl;

	typedef tuple<unsigned int, wstring, MenuId> display_item;
	vector<display_item> display_items;

	unsigned int cnt = 1;
	for (auto const& it : m_items){
		auto id = it.first;
		display_items.push_back(make_tuple(
			cnt++, (show_largetext ? it.second.longtext : it.second.shorttext), it.first));
	}

	switch (display_sort)
	{
	case DisplaySort::LongText:
	case DisplaySort::ShortText:
		sort(display_items.begin(), display_items.end(),
			[](display_item const & first, display_item const & second) {
			return (get<1>(first) < get<1>(second)); });
		break;
	default:
		break;
	}

	for (auto const& it : display_items){
		wcout << to_wstring(get<0>(it)) << L". " << get<1>(it);
		if (show_vertical)	wcout << endl;
		else wcout << L" ";
	}
	unsigned int choice = 0;
	//if (show_vertical)
	wcout << endl; 
	wcout << L"Enter choice : ";

	wcin >> choice;
	for (auto const& it : display_items){
		if (get<0>(it) == choice){
			choice = get<0>(it); break;
		}
	}
	return{ choice };
}
MenuId
ConsoleMenu::InvalidId() const
{ 
	return m_invalidid;
}

STATUS_GEN 
ConsoleMenu::Add(MenuId const& id, MenuItemInfo const& item){
	return Add({ { id, item } });
}
STATUS_GEN 
ConsoleMenu::Add(std::vector<std::pair<MenuId, MenuItemInfo>> const& items){
	for (auto const& it : items){
		if (it.first != InvalidId()){
			if (m_items.count(it.first) != 0)
				return STATUS_GEN::CONTAINER_ITEM_DUP;
			m_items.insert(std::make_pair(it.first, it.second));
		}
	}
	return STATUS_GEN::SUCCESS;
}

STATUS_GEN 
ConsoleMenu::Delete(std::vector<MenuId> const& item_ids)
{
	for (auto & it : item_ids){
		for (auto iter = m_items.begin(); iter != m_items.end();){
			if ((*iter).first == it){
				iter = m_items.erase(iter);
			}
			else{
				iter++;
			}
		}
	}
	return STATUS_GEN::SUCCESS;
}

void 
ConsoleMenu::SetFlags(std::list<Flags> const& flags)
{
	ModifyFlagMask(m_display_options, flags);
}

void 
ConsoleMenu::ModifyFlagMask(FlagMask & MaskVar, std::list<Flags> const& NewFlags)
const
{
	auto scst = [](Flags a){return static_cast<size_t>(a); };
	for (auto const& it : NewFlags)
	{
		switch (it)
		{
		case Flags::Horizontal:
			MaskVar.set(scst(Flags::Horizontal));
			MaskVar.reset(scst(Flags::Vertical)); break;
		case Flags::Vertical:
			MaskVar.set(scst(Flags::Vertical));
			MaskVar.reset(scst(Flags::Horizontal)); break;
		case Flags::LongText:
			MaskVar.set(scst(Flags::LongText));
			MaskVar.reset(scst(Flags::ShorTText)); break;
		case Flags::ShorTText:
			MaskVar.set(scst(Flags::ShorTText));
			MaskVar.reset(scst(Flags::LongText)); break;
		case Flags::ShowHeading:
			MaskVar.set(scst(Flags::ShowHeading));
			MaskVar.reset(scst(Flags::NoShowHeading)); break;
		case Flags::NoShowHeading:
			MaskVar.set(scst(Flags::NoShowHeading));
			MaskVar.reset(scst(Flags::ShowHeading)); break;
		default:
			break;
		}
	}
}

void 
ConsoleMenu::SetHeading(std::wstring const& heading){
	m_heading = heading;
}

void
ConsoleMenu::SetExitID(MenuId const& menu_id)
{
	m_exit_id = menu_id;
}

std::pair<bool, std::wstring> 
ConsoleMenu::IsValid() const
{
	if (m_display_options[static_cast<unsigned>(Flags::ShowHeading)]
		&& m_heading.empty())
		return { false, L"Wrong Heading" };
	if (m_items.empty())
		return{ false, L"Empty container" };
	if (m_items.count(m_exit_id) == 0)
		return{ false, L"Wrong exit code" };

	return{ true, L"" };
}

bool 
ConsoleMenu::IsExitId(MenuId const& menu_id) const
{
	return (m_exit_id == menu_id);
}
bool 
ConsoleMenu::IsValidId(MenuId const& menu_id) const
{
	if ((m_exit_id == menu_id) || (m_invalidid == menu_id))
		return false;
	if (m_items.count(menu_id) == 0)
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////
std::wstring
PrintExecutionTimer::to_wstring(
std::chrono::hours::rep value, std::chrono::hours dummy)
{
	using namespace std::chrono;
	auto ret = std::to_wstring(value) + SS + L"hours";
	return ret;
}
std::wstring
PrintExecutionTimer::to_wstring(
std::chrono::minutes::rep value, std::chrono::minutes dummy)
{
	using namespace std::chrono;
	std::wstring ret{ L"" };
	auto cmp_unit = minutes(60).count();
	auto dummy_to_pass = duration_cast<hours>(dummy);

	if (value > cmp_unit){
		ret = to_wstring(hours::rep(value / cmp_unit), dummy_to_pass);
		ret += SS + std::to_wstring(value % cmp_unit) + SS + L"minutes";
	}
	else if (value == cmp_unit){
		ret = to_wstring(hours::rep(1), dummy_to_pass);
	}
	else{
		ret = std::to_wstring(value) + SS + L"minutes";
	}
	return ret;
}

std::wstring
PrintExecutionTimer::to_wstring(
std::chrono::seconds::rep value, std::chrono::seconds dummy)
{
	using namespace std::chrono;
	std::wstring ret{ L"" };
	auto cmp_unit = seconds(60).count();
	auto dummy_to_pass = duration_cast<minutes>(dummy);

	if (value > cmp_unit){
		ret = to_wstring(minutes::rep(value / cmp_unit), dummy_to_pass);
		ret += SS + std::to_wstring(value % cmp_unit) + SS + L"seconds";
	}
	else if (value == cmp_unit){
		ret = to_wstring(minutes::rep(1), dummy_to_pass);
	}
	else{
		ret = std::to_wstring(value) + SS + L"seconds";
	}
	return ret;
}

std::wstring
PrintExecutionTimer::to_wstring(
std::chrono::milliseconds::rep value, std::chrono::milliseconds dummy)
{
	using namespace std::chrono;
	std::wstring ret{ L"" };
	auto cmp_unit = milliseconds(1000).count();
	auto dummy_to_pass = duration_cast<seconds>(dummy);

	if (value > cmp_unit){
		ret = to_wstring(seconds::rep(value / cmp_unit), dummy_to_pass);
		ret += SS + std::to_wstring(value % cmp_unit) + SS + L"milliseconds";
	}
	else if (value == cmp_unit){
		ret = to_wstring(seconds::rep(1), dummy_to_pass);
	}
	else{
		ret = std::to_wstring(value) + SS + L"milliseconds";
	}
	return ret;
}

std::wstring
PrintExecutionTimer::to_wstring(
std::chrono::microseconds::rep value, std::chrono::microseconds dummy)
{
	using namespace std::chrono;
	std::wstring ret{ L"" };
	auto cmp_unit = microseconds(1000).count();
	auto dummy_to_pass = duration_cast<milliseconds>(dummy);

	if (value > cmp_unit){
		ret = to_wstring(milliseconds::rep(value / cmp_unit), dummy_to_pass);
		ret += SS + std::to_wstring(value % cmp_unit) + SS + L"microseconds";
	}
	else if (value == cmp_unit){
		ret = to_wstring(milliseconds::rep(1), dummy_to_pass);
	}
	else{
		ret = std::to_wstring(value) + SS + L"microseconds";
	}
	return ret;
}

std::wstring
PrintExecutionTimer::to_wstring(
	std::chrono::nanoseconds::rep value, std::chrono::nanoseconds dummy)
{
	using namespace std::chrono;
	std::wstring ret{ L"" };
	auto cmp_unit = nanoseconds(1000).count();
	auto dummy_to_pass = duration_cast<microseconds>(dummy);

	if (value > cmp_unit) {
		ret = to_wstring(microseconds::rep(value / cmp_unit), dummy_to_pass);
		ret += SS + std::to_wstring(value % cmp_unit) + SS + L"nanoseconds";
	}
	else if (value == cmp_unit) {
		ret = to_wstring(microseconds::rep(1), dummy_to_pass);
	}
	else {
		ret = std::to_wstring(value) + SS + L"nanoseconds";
	}
	return ret;
}