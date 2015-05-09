// ProgramInput.cpp : Various classes related to program Input.
//

#include "stdafx.h"

#include "Resources.h"
#include "CommonDefs.h"
#include "Utilities.h"
#include "TestHarness.h"
#include "estd.h"

#include <fstream>
#include <sstream>
#include <limits>
#include <cstdio>
#include <memory>

///////////////////////////////////////////////////////////////////////////////
INT_VEC 
GetDailyVariations(DatePriceMap const& DatePrices)
{
	std::vector<int> variations;
	if (DatePrices.size() >= 2){
		auto it = DatePrices.begin();
		++it;
		while (it != DatePrices.end()){
			auto prev = it;
			variations.push_back((it++)->second - (--prev)->second);
		}
	}
	return variations;
}
INT_VEC
GetDailyVariations(DayPriceMap const& DayPrices)
{
	std::vector<int> variations;
	if (DayPrices.size() >= 2){
		auto it = DayPrices.begin();
		++it;
		while (it != DayPrices.end()){
			auto prev = it;
			variations.push_back((it++)->second - (--prev)->second);
		}
	}
	return variations;
}

PricePoint::PricePoint() 
	:dayordate(DayOrDate::END), Date({}), Price(INT_MAX)
{}
PricePoint::PricePoint(PricePoint const& src)
{
	*this = src;
}
PricePoint& 
PricePoint::operator=(PricePoint const& src)
{
	if (this != &src){
		dayordate = src.dayordate;
		Date = src.Date;
		Price = Price;
	}
	return *this;
}
PricePoint::~PricePoint(){}
bool 
PricePoint::operator<(PricePoint const& second)
{
	return(Date < second.Date);
}
bool 
PricePoint::operator==(PricePoint const& second)
{
	return((dayordate == second.dayordate) && 
		(Date == second.Date) && (Price == second.Price));
}

std::wstring 
to_wstring(MaxProfitPoints const& input)
{
	using std::to_wstring;
	using std::wstring;

	wstring output{L""};
	const wstring BP{L"Buy Price  : "};
	const wstring SP{L"Sell Price : " };
	const wstring sep{ L" | " };

	if (input.Buy.dayordate == DayOrDate::DAY){
		auto bd = to_wstring(input.Buy.Day);
		auto sd = to_wstring(input.Sell.Day);
		auto mdlen = (bd.size() > sd.size()) ? bd.size() : sd.size();
		
		output += BP + estd::prepend_zero_w(input.Buy.Day, mdlen) + sep +
							to_wstring(input.Buy.Price) + L"\n";
		output += SP + estd::prepend_zero_w(input.Sell.Day, mdlen) + sep + 
							to_wstring(input.Sell.Price) + L"\n";
	}
	if (input.Buy.dayordate == DayOrDate::DATE)
	{
		wchar_t buf[16] = { L'\0' }; //YYYYMMDD
		unsigned int buffsize = 16;

		wcsftime(buf, buffsize, L"%F", &input.Buy.Date);
		output += BP + wstring(buf) + sep + to_wstring(input.Buy.Price) + L"\n";

		memset(buf, 0, buffsize * sizeof(wchar_t));

		wcsftime(buf, buffsize, L"%F", &input.Sell.Date);
		output += SP + wstring(buf) + sep + to_wstring(input.Sell.Price) + L"\n";
	}
	return output;
}

///////////////////////////////////////////////////////////////////////////////
/// Operand types /////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
std::wstring 
to_wstring(OPERAND_TYPE opertype)
{
	switch (opertype)
	{
		case OPERAND_TYPE::INT: return{ L"Integer" }; break;
		case OPERAND_TYPE::UINT: return{ L"Unsigned Integer" }; break;
		case OPERAND_TYPE::CHAR: return{ L"Character" }; break;
		case OPERAND_TYPE::STR: return{ L"String" }; break;
		case OPERAND_TYPE::EXIT: return{ L"Exit" }; break;
		case OPERAND_TYPE::END: 
		default:
			return{ L"Unknown" }; break;
	}
}

OperandType::OperandType(){}
OperandType::~OperandType(){}

OperandType::OperandType(OperandType const& src) { 
	*this = src; 
};
OperandType& 
OperandType::operator=(OperandType const& src)
{
	if (this != &src){
		m_names = src.m_names;
	}
	return *this;
}

OPERAND_TYPE 
OperandType::InvalidType() const
{ 
	return OPERAND_TYPE::END; 
}
bool 
OperandType::IsValidType(OPERAND_TYPE opertype) const
{ 
	return (m_names.count(opertype) > 0); 
}
bool 
OperandType::IsValidTypeText(std::wstring const& opertext) const
{
	for (auto &it:m_names){
		if (0 == estd::compare_ci(opertext, it.second.first))
			return true;
	}
	for (auto &it : m_names){
		if (0 == estd::compare_ci(opertext, it.second.second))
			return true;
	}
	return false;
}
OPERAND_TYPE 
OperandType::Type(std::wstring const& opertext) const
{
	for (auto &it : m_names){
		if (0 == estd::compare_ci(opertext, it.second.first))
			return it.first;
	}
	for (auto &it : m_names){
		if (0 == estd::compare_ci(opertext, it.second.second))
			return it.first;
	}
	return InvalidType();
}
std::wstring 
OperandType::TypeText(OPERAND_TYPE opertype) const
{
	if (m_names.count(opertype) > 0)
		return m_names.at(opertype).first;
	else
		return InvalidName;
}

std::vector<OperMenuItem> 
OperandType::MenuEntries(std::vector<OPERAND_TYPE> const &SkipEntries) const
{
	std::vector<OperMenuItem> MenuEntries;
	int MenuId = 1;
	for (auto &it : m_names)
	{
		OperMenuItem menuEntry;
		bool skip = false;
		for (auto &skipentry : SkipEntries){
			if (skipentry == it.first){
				skip = true;
				break;
			}
		}
		if (!skip){
			menuEntry.Id = MenuId++;
			menuEntry.EnumType = it.first;
			menuEntry.Text = it.second.first;
			menuEntry.IdText = it.second.second;
			MenuEntries.push_back(menuEntry);
		}
	}
	std::sort(MenuEntries.begin(), MenuEntries.end());
	return MenuEntries;
}

std::vector<OperandMenuItem>
OperandType::MenuData(std::vector<OPERAND_TYPE> const& SkipEntries) const
{
	std::vector<OperandMenuItem> MenuEntries;
	int MenuId = 1;
	for (auto &it : m_names)
	{
		OperandMenuItem menuEntry;
		bool skip = false;
		for (auto &skipentry : SkipEntries){
			if (skipentry == it.first){
				skip = true;
				break;
			}
		}
		if (!skip){
			menuEntry.idnum = MenuId++;
			menuEntry.id = it.first;
			menuEntry.long_text = it.second.first;
			menuEntry.short_text = it.second.second;
			MenuEntries.push_back(menuEntry);
		}
	}
	std::sort(MenuEntries.begin(), MenuEntries.end());
	return MenuEntries;
}
OperandMenuItem
OperandType::ExitItemData(
std::vector<OPERAND_TYPE> const& SkipEntries) const
{
	unsigned int MenuId = 1;
	for (auto &it : m_names)
	{
		OperandMenuItem menuEntry;
		bool skip = false;
		for (auto &skipentry : SkipEntries){
			if (skipentry == it.first){
				skip = true;
				break;
			}
		}			
		if (!skip && it.first != OPERAND_TYPE::EXIT){
			MenuId++;
		}
	}
	return{ OPERAND_TYPE::EXIT, MenuId,
		m_names.at(OPERAND_TYPE::EXIT).first,
		m_names.at(OPERAND_TYPE::EXIT).second
	};
}
	
unsigned int 
OperandType::OperandTypeCount()
{ 
	return m_names.size();
}

///////////////////////////////////////////////////////////////////////////////
/// Major operations //////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void 
MajorOps::build_name_map()
{
	if (name_map.size() == GetTotalItems()) return;
	name_map.empty();
	name_map = std::map <MAJOR_OPS, std::wstring>{
		{ MAJOR_OPS::SORT, L"" },
		{ MAJOR_OPS::SEARCH, L"" },
		{ MAJOR_OPS::BINOPS, L"" },
		{ MAJOR_OPS::LINKEDLIST, L"" },
		{ MAJOR_OPS::STRINGS, L"" },
		{ MAJOR_OPS::TWISTERS, L"" },
		{ MAJOR_OPS::BINTREE, L"" },
		{ MAJOR_OPS::CLRS_CH4, L"" },
		{ MAJOR_OPS::PALINDROME, L"" },
		{ MAJOR_OPS::EXIT, L"" },
	};
	for (auto & it : name_map){
		it.second = to_wstring(it.first);
	}
	return;
}
void 
MajorOps::build_ops_map()
{
	if (ops_map.size() == GetTotalItems()) return;
	ops_map.empty();
	ops_map = std::map < MAJOR_OPS, MajorOpsFunc > {
		{ MAJOR_OPS::EXIT, nullptr },
	};
}

bool 
MajorOps::IsValidId(MAJOR_OPS id) const { 
	return (id != GetInvalidId()); 
}
bool 
MajorOps::IsValidName(std::wstring const& name){ 
	return(IsValidId(GetId(name))); 
}

MAJOR_OPS
MajorOps::GetId(std::wstring const& name, bool besteffort)
{
	using namespace std;
	if (name.empty()) return GetInvalidId();
	build_name_map();

	for (auto const& it : name_map){
		wstring temp(it.second);
		if ((besteffort && (0 == estd::trimandcompare(name, temp)))
			|| (!besteffort && (0 == estd::compare_ci(name, temp)))){
			return it.first;
		}
	}
	return GetInvalidId();
}

std::wstring 
MajorOps::GetName(MAJOR_OPS id){ 
	return (IsValidId(id) ? to_wstring(id) : Common_Res::UNKNONWN()); 
}

MajorOps::MajorOpsFunc
MajorOps::GetOps(MAJOR_OPS id){ 
	return (IsValidId(id) ? (build_ops_map(), ops_map[id]) : nullptr); 
}

std::vector<MajorOpsMenuItem> 
MajorOps::GetMenuItems(const std::vector<MAJOR_OPS> & SkipItems)
{
	std::vector<MajorOpsMenuItem> MenuItems;
	unsigned int MenuId = 1;

	AddMenuItem(MenuItems, MenuId, 
		MajorOpsMenuItem(MAJOR_OPS::SORT, 0, L"SO", L""), SkipItems);
	AddMenuItem(MenuItems, MenuId, 
		MajorOpsMenuItem(MAJOR_OPS::SEARCH, 0, L"SE", L""), SkipItems);
	AddMenuItem(MenuItems, MenuId, 
		MajorOpsMenuItem(MAJOR_OPS::BINOPS, 0, L"BO", L""), SkipItems);
	AddMenuItem(MenuItems, MenuId, 
		MajorOpsMenuItem(MAJOR_OPS::LINKEDLIST, 0, L"LL", L""), SkipItems);
	AddMenuItem(MenuItems, MenuId, 
		MajorOpsMenuItem(MAJOR_OPS::STRINGS, 0, L"STR", L""), SkipItems);
	AddMenuItem(MenuItems, MenuId, 
		MajorOpsMenuItem(MAJOR_OPS::TWISTERS, 0, L"TW", L""), SkipItems);
	AddMenuItem(MenuItems, MenuId, 
		MajorOpsMenuItem(MAJOR_OPS::BINTREE, 0, L"BT", L""), SkipItems);
	AddMenuItem(MenuItems, MenuId, 
		MajorOpsMenuItem(MAJOR_OPS::CLRS_CH4, 0, L"CH4", L""), SkipItems);
	AddMenuItem(MenuItems, MenuId, 
		MajorOpsMenuItem(MAJOR_OPS::EXIT, 0, L"E", L""), SkipItems);

	std::sort(MenuItems.begin(), MenuItems.end());
	return MenuItems;
}

void 
MajorOps::AddMenuItem(std::vector<MajorOpsMenuItem>& MenuItems, unsigned int &MenuId,
	MajorOpsMenuItem const& MenuItem, std::vector<MAJOR_OPS> const& SkipItems)
{
	for (auto &SkipItem : SkipItems){
		if (MenuItem.EnumType == SkipItem)
			return;
	}
	MenuItems.push_back(MajorOpsMenuItem(
			MenuItem.EnumType, MenuId++, MenuItem.IdText, GetName(MenuItem.EnumType)));
	return;
}

std::wstring 
to_wstring(MAJOR_OPS id)
{
	switch (id)
	{
		case MAJOR_OPS::SORT: return{L"Sorting"}; break;
		case MAJOR_OPS::SEARCH: return{L"Searching"}; break;
		case MAJOR_OPS::BINOPS: return{L"Binary Operations"}; break;
		case MAJOR_OPS::LINKEDLIST: return{L"Linked List"}; break;
		case MAJOR_OPS::STRINGS: return{L"Strings"}; break;
		case MAJOR_OPS::TWISTERS: return{L"Twisters"}; break;
		case MAJOR_OPS::BINTREE: return{L"Binary Tree"}; break;
		case MAJOR_OPS::CLRS_CH4: return{L"CLRS4" }; break;
		case MAJOR_OPS::PALINDROME: return{L"Palindrome"}; break;
		case MAJOR_OPS::EXIT: return{ L"Exit" }; break;
		case MAJOR_OPS::END: 
		default:
			return{L"Unknown"}; break;
	};
}

///////////////////////////////////////////////////////////////////////////////
/// Argument map //////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
std::wstring 
to_wstring(RUNTYPE runtype)
{
	switch (runtype)
	{
		case RUNTYPE::ConsoleOps: return{ L"ConsoleOps" }; break;
		case RUNTYPE::FileInput: return{ L"FileInput" }; break;
		case RUNTYPE::CommandLine: return{ L"CommandLine" }; break;
		case RUNTYPE::END:
		default:
			return{ L"RUNTYPE unknown" }; break;
	}
}

std::wstring 
RunMap::extranames()
{
	std::wstring temp(L"");
	for (size_t i = 0; i < m_extranames.size() - 1; i++){ 
		temp.append(m_extranames[i] + L","); 
	}
	temp.append(m_extranames[m_extranames.size() - 1]);
	return temp;
}

bool 
RunMap::matchnames(const std::wstring& candidate)
{
	if (0 == m_name.compare(0, m_name.length(), candidate)) 
		return true;
	else{
		for each (auto var in m_extranames){
			if (0 == var.compare(0, var.length(), candidate)) 
				return true;
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
/// SubOpOperand  Implementation //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SubOpOperands::SubOpOperands(){};
SubOpOperands::SubOpOperands(
	std::wstring const& input, OPERAND_TYPE type, std::wstring const& delims) 
		:m_input(input), m_type(type), m_delims(delims) 
{}
SubOpOperands::~SubOpOperands(){};

SubOpOperands::SubOpOperands(SubOpOperands const& src) {
	*this = src; 
}
SubOpOperands&
SubOpOperands::operator=(SubOpOperands const& src)
{
	if (this != &src)
	{
		m_input = src.m_input;
		m_type = src.m_type;
		m_delims = src.m_delims;
	}
	return *this;
}
	
std::wstring 
SubOpOperands::Input() const { return m_input; }
OPERAND_TYPE 
SubOpOperands::Type() const { return m_type; }
std::wstring 
SubOpOperands::Delims() const { return m_delims; }

void 
SubOpOperands::Set(std::wstring const& input, OPERAND_TYPE type, std::wstring const& delims)
{
	m_input = input;
	m_type = type;
	m_delims = delims;
}
	
STATUS_GEN 
SubOpOperands::GetList(INT_VEC& tokens, TokStyle style)
{
	WSTRING_VEC rawtokens = { L"" };
	auto status = TokenizedList(style, rawtokens);
	tokens.clear();

	for each (auto token in rawtokens)
	{
		try{
			tokens.push_back(std::stoi(token));
		}
		catch (std::invalid_argument){
			//std::wcout << L"Failed to convert [" << token << "]\n";
			status = STATUS_GEN::BAD_INPUT;
			break;
		}
		catch (std::out_of_range){
			//std::wcout << L"Failed to convert [" << token << "]. Too Big.\n";
			status = STATUS_GEN::BAD_INPUT;
			break;
		}
	}
	return status;
}

STATUS_GEN 
SubOpOperands::GetList(UINT_VEC& tokens, TokStyle style)
{
	WSTRING_VEC rawtokens = { L"" };
	auto status = TokenizedList(style, rawtokens);
	tokens.clear();

	for each (auto token in rawtokens)
	{
		try{
			tokens.push_back(estd::stou(token));
		}
		catch (std::invalid_argument){
			//std::wcout << L"Failed to convert [" << token << "]\n";
			status = STATUS_GEN::BAD_INPUT;
			break;
		}
		catch (std::out_of_range){
			//std::wcout << L"Failed to convert [" << token << "]. Too Big.\n";
			status = STATUS_GEN::BAD_INPUT;
			break;
		}
	}
	return status;
}

STATUS_GEN 
SubOpOperands::GetList(WCHAR_VEC& tokens, TokStyle style)
{
	WSTRING_VEC rawtokens = { L"" };
	auto status = TokenizedList(style, rawtokens);
	tokens.clear();
	for each (auto token in rawtokens){
		if (token.length() != 1){
			std::wcout << L"Failed to convert [" << token << "]\n";
			status = STATUS_GEN::BAD_INPUT;
			break;
		}
		tokens.push_back(token[0]);
	}
	return status;
}

STATUS_GEN 
SubOpOperands::GetList(WSTRING_VEC& tokens, TokStyle style)
{
	STATUS_GEN status = STATUS_GEN::SUCCESS;
	tokens.clear();
	status = TokenizedList(style, tokens);
	return status;
}

STATUS_GEN 
SubOpOperands::TokenizedList(TokStyle style, WSTRING_VEC& tokens)
{
	if (m_input.empty()){
		tokens.clear();
		return STATUS_GEN::SUCCESS;
	}
	estd::TokenizerOP TokenOp = estd::TokenizerOP::SIMPLE;
		
	switch (style)
	{
	case TokStyle::SIMPLE: 
		TokenOp = estd::TokenizerOP::SIMPLE; break;
	case TokStyle::TRIMMEDTOKENS: 
		TokenOp = estd::TokenizerOP::TRIMMED; break;
	case TokStyle::NONEMPTYTOKENS: 
		TokenOp = estd::TokenizerOP::NONEMPTY; break;
	case TokStyle::NONEMPTYTRIMMEDTOKENS: 
		TokenOp = estd::TokenizerOP::NONEMPTYTRIMMED; break;
	case TokStyle::UNIQUETOKENS: 
		TokenOp = estd::TokenizerOP::UNIQUE; break;
	case TokStyle::UNIQUETRIMMEDTOKENS:  
		TokenOp = estd::TokenizerOP::UNIQUETRIMMED; break;
	case TokStyle::UNIQUENONEMPTYTOKENS: 
		TokenOp = estd::TokenizerOP::UNIQUENONEMPTY; break;
	case TokStyle::UNIQUENONEMPTYTRIMMEDTOKENS: 
		TokenOp = estd::TokenizerOP::UNIQUENONEMPTYTRIMMED; break;
	default: 
		TokenOp = estd::TokenizerOP::SIMPLE; break;
	}

	tokens = estd::Tokenizer <std::wstring>(m_delims, TokenOp).Tokens(m_input);

	return STATUS_GEN::SUCCESS;
}

SubOpOperands::StyleMask
SubOpOperands::GetStyleMask(TokStyle style)
{
	StyleMask mask;
	mask.nonempty = mask.trimmed = mask.unique = false;

	if (style == TokStyle::TRIMMEDTOKENS)
		mask.trimmed = true;
	else if (style == TokStyle::NONEMPTYTOKENS)
		mask.nonempty = true;
	else if (style == TokStyle::NONEMPTYTRIMMEDTOKENS)
		mask.nonempty = mask.trimmed = true;
	else if (style == TokStyle::UNIQUETOKENS)
		mask.unique = true;
	else if (style == TokStyle::UNIQUETRIMMEDTOKENS)
		mask.trimmed = mask.unique = true;
	else if (style == TokStyle::UNIQUENONEMPTYTOKENS)
		mask.nonempty = mask.unique = true;
	else if (style == TokStyle::UNIQUENONEMPTYTRIMMEDTOKENS)
		mask.nonempty = mask.trimmed = mask.unique = true;

	return mask;
}

bool 
SubOpOperands::IsValidParams(TokStyle style)
{
	if (m_input.empty()) return false;
	if (!OperandType().IsValidType(m_type)) return false;
	if (m_delims.empty()) return false;
	return true;
}


///////////////////////////////////////////////////////////////////////////////
/// Output Block Information //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
std::wstring 
to_wstring(OutputType optype)
{
	switch (optype)
	{
		case OutputType::Console:	return{ OPFile_Res::Console() }; break;
		case OutputType::File:		return{ OPFile_Res::File() }; break;
		case OutputType::ConsoleFile: return{ OPFile_Res::ConsoleFile() }; break;
		case OutputType::END: 
		default:
			return{ L"Unknown" }; break;
	}
}
OutputBlockInfo::OutputBlockInfo()
{
	m_target = L"";
	m_filename = L"";
	m_Id = 0;
	m_optype = OutputType::END;
}

OutputType
OutputBlockInfo::Type(std::wstring const& type_text)
{
	for (auto const &it : TypeTextMap()){
		if (0 == estd::compare_ci(it.second, type_text))
			return it.first;
	}
	return OutputType::Console;
}

std::wstring
OutputBlockInfo::Text(OutputType optype){
	return to_wstring(optype);
}

OutputBlockInfo::TypeText_Map 
OutputBlockInfo::TypeTextMap()
{
	return (TypeText_Map
	{
		{ OutputType::Console, to_wstring(OutputType::Console) },
		{ OutputType::File, to_wstring(OutputType::File) },
		{ OutputType::ConsoleFile, to_wstring(OutputType::ConsoleFile) },
		{ OutputType::END, to_wstring(OutputType::END) }
	}
	);
}

///////////////////////////////////////////////////////////////////////////////
/// Output File handler ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct OutputHandler::Impl : public OPFile_Res
{
private:
	OutputType		m_optype;
	std::wstring	m_filename;

public:
	Impl():Impl(L"", OutputType::Console) {}
	Impl(std::wstring const& filename, OutputType optype)
		: m_filename(filename), m_optype(optype){
		if (m_filename.empty()) 
			m_filename = OPFile_Res::FileName();
	}
	~Impl(){}

	Impl(const Impl& src){ *this = src; }
	Impl& operator=(const Impl& src){
		if (this != &src){
			m_optype = src.m_optype;
			m_filename = src.m_filename;
		}
		return *this;
	}

public:
	bool 
		IsAvailable() { return true; }
	void 
		DeleteFile()
	{
		using namespace std;
		wofstream ofile;
		ofile.open(m_filename, wofstream::out | wofstream::trunc);
		if (ofile){
			ofile.close();
			string str;
			utils::WStringToString(m_filename, str);
			remove(str.c_str());
		}
	}

	void 
		Write(std::wstring const& data)
	{
		if (OutputType::Console == m_optype)
			WriteToConsole(data);
		else if (OutputType::File == m_optype)
			WriteToFile(data);
		else{
			WriteToConsole(data);
			WriteToFile(data);
		}
	}
	void 
		WriteLine(std::wstring const& data)
	{
		Write(data + L"\n");
	}
	void 
		WriteToFile(std::wstring const& data)
	{
		using namespace std;

		wofstream ofile;
		ofile.open(m_filename, wofstream::out | wofstream::ate | wofstream::app);
		if (ofile){
			ofile << data;
			ofile.close();
		}
	}
	void 
		WriteToConsole(std::wstring const& data)
	{
		std::wcout << data;
	}
};

//////////////////////////////////////////////////////////////////////////
// OutputFileHandler Constructors, Copy constructors and Destrcutors
OutputHandler::~OutputHandler(){}
OutputHandler::OutputHandler() : pImpl(std::make_unique<Impl>()){}
OutputHandler::OutputHandler(std::wstring const& FileName, OutputType optype)
	: pImpl(std::make_unique<Impl>(FileName, optype)) {}

OutputHandler::OutputHandler(const OutputHandler& src)
	: pImpl(std::unique_ptr<Impl>(new Impl(*src.pImpl))){}
OutputHandler& 
OutputHandler::operator=(const OutputHandler& src)
{
	if (this != &src)
		*pImpl = *src.pImpl;
	return *this;
}

//////////////////////////////////////////////////////////////////////////
// InputFileHandler Public methods
void OutputHandler::DeleteFile(){
	return pImpl->DeleteFile();
}
bool OutputHandler::IsAvailable(){
	return (pImpl->IsAvailable());
}
void OutputHandler::Write(std::wstring const& data){
	return pImpl->Write(data);
}
void OutputHandler::WriteLine(std::wstring const& data){
	return pImpl->WriteLine(data);
}
void OutputHandler::WriteToFile(std::wstring const& data){
	return pImpl->WriteToFile(data);
}
void OutputHandler::WriteToConsole(std::wstring const& data){
	return pImpl->WriteToConsole(data);
}

///////////////////////////////////////////////////////////////////////////////
/// Input File handler ////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct InputFileHandler::Impl : public IpFile_Res, IpFile_Err
{
private:
	std::wstring	m_InputFile;
	UINT_VEC		m_InputBlockIds;
	UINT_VEC		m_OutputBlockIds;
	LOG_LEVEL		m_LogLevel;

	typedef std::pair <std::wstring, std::wstring> Value_Pair;
	typedef std::map <std::wstring, std::wstring> Value_Map;
	typedef std::pair <int, Value_Map> Block_Pair;

	std::map <unsigned int, Value_Map> m_InputBlock_Map;
	std::map <unsigned int, Value_Map> m_OutputBlock_Map;

public:
	// Constructors, destructors and copy constructors
	Impl() : Impl(IpFile_Res::INPUT_FILE_NAME(), LOG_LEVEL::BLOCK) { }
	Impl(std::wstring const& inputfile, LOG_LEVEL loglevel = LOG_LEVEL::BLOCK)
		: m_InputFile(inputfile), m_LogLevel(loglevel) {}

	~Impl(){}

	Impl(const Impl& src){ *this = src; }
	Impl& operator=(const Impl& src)
	{
		if (this != &src){
			m_InputFile = src.m_InputFile;
			m_InputBlockIds = src.m_InputBlockIds;
			m_OutputBlockIds = src.m_OutputBlockIds;
			m_LogLevel = src.m_LogLevel;
			m_InputBlock_Map = src.m_InputBlock_Map;
			m_OutputBlock_Map = src.m_OutputBlock_Map;
		}
		return *this;
	}

public:
	unsigned int 
		InvalidBlockId(){ return 0; }
	bool 
		IsValidBlockId(unsigned int Block) { return InvalidBlockId() != Block; }
	bool 
		IsAvailable()
	{
		std::ifstream IpFile(m_InputFile);
		if (!IpFile){
			return false;
		}
		IpFile.close();
		return true;
	}

	void SetLogLevel(LOG_LEVEL loglevel){
		m_LogLevel = loglevel;
		return;
	}

	STATUS_GEN 
		BlockIds(UINT_VEC &BlockIds, BLOCK_TYPE BlockType, bool CheckAgain = false)
	{
		auto status = FillBlockIds(BlockType, CheckAgain);
		if (STATUS_GEN::SUCCESS == status)
			BlockIds = m_InputBlockIds;
		return status;
	}

	MAJOR_OPS 
		MajorOp(unsigned int BlockId)
	{
		if (STATUS_GEN::SUCCESS != FillValueMap(BlockId)
			|| m_InputBlock_Map.at(BlockId).count(MAJOR_OP_TAG()) == 0)
			return MajorOps().GetInvalidId();

		return MajorOps().GetId(m_InputBlock_Map.at(BlockId).at(MAJOR_OP_TAG()));
	}

	std::wstring 
		SubOp(unsigned int BlockId)
	{
		if (STATUS_GEN::SUCCESS != FillValueMap(BlockId) ||
			m_InputBlock_Map.at(BlockId).count(SUB_OP_TAG()) == 0)
			return std::wstring(L"");

		return estd::trim(m_InputBlock_Map.at(BlockId).at(SUB_OP_TAG()));
	}

	OPERAND_TYPE 
		GetOperandType(unsigned int BlockId)
	{
		if (STATUS_GEN::SUCCESS != FillValueMap(BlockId)
			|| m_InputBlock_Map.at(BlockId).count(OPERAND_TYPE_TAG()) == 0)
			return OperandType().InvalidType();

		return OperandType().Type(estd::trim(
			m_InputBlock_Map.at(BlockId).at(OPERAND_TYPE_TAG())));
	}

	int 
		GetOperandCount(unsigned int BlockId)
	{
		int operand_count = 0;
		FillValueMap(BlockId);
		if (m_InputBlock_Map.count(BlockId) == 0)
			return operand_count;
			
		for (const auto &it : (*m_InputBlock_Map.find(BlockId)).second){
			if ((0 == estd::compare_ci(MAJOR_OP_TAG(), it.first)) ||
				(0 == estd::compare_ci(SUB_OP_TAG(), it.first)) ||
				(0 == estd::compare_ci(OPERAND_TYPE_TAG(), it.first)))
				continue;
			operand_count++;
		}
			
		return operand_count;
	}

	std::vector<std::pair<std::wstring, std::wstring>>
		RawOperands(unsigned int BlockId)
	{
		using namespace std;

		wstring SSD = Logger_Res::SubStrDelim();
		wstring logmsg = __FUNCTIONW__ + SSD + L"Block " + SSD + to_wstring(BlockId);

		std::vector<std::pair<std::wstring, std::wstring>> list;
		FillValueMap(BlockId);
		if (m_InputBlock_Map.count(BlockId) == 0){
			WriteLog(logmsg + SSD + WrongBlockId(), LogType::ERR);
			return list;
		}

		for (auto const& it : m_InputBlock_Map.at(BlockId)){
			if (estd::start_with_ci(it.first, OPERAND_DATA_TAG())){
				list.push_back({ it.first, it.second });
			}
		}
		return list;
	}

	std::vector<std::pair<std::wstring, SubOpOperands>>
		Operands(unsigned int BlockId)
	{
		using namespace std;
		std::vector<std::pair<std::wstring, SubOpOperands>> list;
		for (auto const& it : RawOperands(BlockId)){
			SubOpOperands subopoperands(it.second, GetOperandType(BlockId));
			list.push_back({ it.first, subopoperands });
		}
		return list;
	}

	SubOpOperands 
		Operand(unsigned int BlockId, std::wstring const& operandtag)
	{
		using namespace std;
		wstring SSD = Logger_Res::SubStrDelim();

		SubOpOperands subopoperands(L"", OperandType().InvalidType());
		wstring logmsg = L"Block " + to_wstring(BlockId) + 
							SSD + L"operandtag " + operandtag;

		FillValueMap(BlockId);
		if (m_InputBlock_Map.count(BlockId) == 0){
			WriteLog(logmsg + SSD + WrongBlockId(), LogType::ERR);
			return subopoperands;
		}

		wstring tagname(estd::trim(operandtag));
		transform(tagname.begin(), tagname.end(), tagname.begin(), ::tolower);
		if (m_InputBlock_Map.at(BlockId).count(tagname) == 0){
			WriteLog(logmsg + SSD + InvalidTagName(), LogType::ERR);
			return subopoperands;
		}
		subopoperands.Set(estd::trim(m_InputBlock_Map.at(BlockId).at(tagname)),
					GetOperandType(BlockId));
			
		return subopoperands;
	}
	STATUS_GEN 
		GetOperand(unsigned int BlockId, std::wstring const& operandtag, SubOpOperands& subopoperands)
	{
		subopoperands = Operand(BlockId, operandtag);
		if (!OperandType().IsValidType(subopoperands.Type()) ||
			subopoperands.Input().empty())
			return STATUS_GEN::BAD_FILE;
		return STATUS_GEN::SUCCESS;
	}

	STATUS_GEN 
		GetSampleDataOptions(unsigned int BlockId, bool& UseSample, bool& UseSampleDefault)
	{
		using namespace std;
		UseSample = UseSampleDefault = false;

		STATUS_GEN status = FillValueMap(BlockId);
		if (STATUS_GEN::SUCCESS != status) return status;

		if(0 != m_InputBlock_Map.at(BlockId).count(USE_SAMPLE_TAG()))
			UseSample = estd::any_of_ci(
				m_InputBlock_Map.at(BlockId).at(USE_SAMPLE_TAG()), Common_Res::TRUE_MARKERS());

		if (0 != m_InputBlock_Map.at(BlockId).count(SAMPLE_DEFAULT_TAG()))
			UseSampleDefault = estd::any_of_ci(
				m_InputBlock_Map.at(BlockId).at(SAMPLE_DEFAULT_TAG()), Common_Res::TRUE_MARKERS());

		return status;
	}
	
	WSTRING_VEC
		SampleHints(unsigned int BlockId)
	{
		WSTRING_VEC sample_hints{};
		if (STATUS_GEN::SUCCESS == GetSampleHints(BlockId, sample_hints))
			return sample_hints;
		else
			return{};
	}
	STATUS_GEN 
		GetSampleHints(unsigned int BlockId, WSTRING_VEC& SampleHints)
	{
		using namespace std;
		auto SSD = Logger_Res::SubStrDelim();
		wstring logmsg = __FUNCTIONW__ + SSD + L"Block id " + SSD + to_wstring(BlockId);
		vector<wstring> block_entries;

		auto status = GetBlockEntries(block_entries, BlockId, BLOCK_TYPE::INPUT);
		if (STATUS_GEN::SUCCESS != status){
			WriteLog(logmsg + SSD + EmptyBlock(), LogType::ERR);
			return (status = STATUS_GEN::BAD_PARAM);
		}

		for (auto &it : block_entries)
		{
			if (!estd::start_with_ci(it, SAMPLE_HINT_TAG()))
				continue;
			auto TagLoc = it.find_first_of(Common_Res::TAG_SEP());
			if (wstring::npos == TagLoc){
				WriteLog(logmsg + SSD + InvalidLine() + SSD + it, LogType::ERR);
				continue;
			}
			SampleHints.push_back(estd::trim(
				it.substr(it.find_last_of(Common_Res::TAG_SEP()) + 1)));
		}
		return (status = STATUS_GEN::SUCCESS);
	}
	
	std::pair<unsigned int,STATUS_GEN> 
		GetOutputBlockId(unsigned int InputBlockId)
	{
		using namespace std;
		wstring LogHint{ __FUNCTIONW__ };
		std::pair<unsigned int, STATUS_GEN> retval = { InvalidBlockId(), STATUS_GEN::FAILURE };

		STATUS_GEN status = FillValueMap(InputBlockId);
		if (STATUS_GEN::SUCCESS != status)
			return retval;
		if (0 == m_InputBlock_Map.at(InputBlockId).count(OUTPUT_BLOCKID_TAG())){
			retval.second = STATUS_GEN::SUCCESS;
			return retval;
		}

		unsigned int OutputBlockId;
		try{
			OutputBlockId = estd::stou(m_InputBlock_Map.at(InputBlockId)[OUTPUT_BLOCKID_TAG()]);
			if (InvalidBlockId() == OutputBlockId){
				WriteLog(LogHint + Logger_Res::SubStrDelim() + ZeroBlockId(), LogType::ERR);
				return retval;
			}
		}
		catch (exception){
			WriteLog(LogHint + Logger_Res::SubStrDelim() + InvalidTagData(), LogType::ERR);
			return retval;
		}

		status = FillBlockIds(BLOCK_TYPE::OUTPUT);
		if ((STATUS_GEN::SUCCESS == status)
			&& (estd::is_available(OutputBlockId, m_OutputBlockIds)))
		{
			retval.first = OutputBlockId;
			retval.second = STATUS_GEN::SUCCESS;
		}
		return retval;
	}

	STATUS_GEN
		GetOutputBlockData(unsigned int opblock, OutputBlockInfo & blockInfo)
	{
		using namespace std;
		wstring LogHint = __FUNCTIONW__;
		
		auto status = FillValueMap(opblock, BLOCK_TYPE::OUTPUT);
		if (STATUS_GEN::SUCCESS == status)
		{
			blockInfo.m_Id = opblock;
			if (0 != m_OutputBlock_Map.at(opblock).count(OUTPUT_TARGET_TAG())){
				blockInfo.m_target = m_OutputBlock_Map.at(opblock)[OUTPUT_TARGET_TAG()];
			}
			if (0 != m_OutputBlock_Map.at(opblock).count(OUTPUT_FILEPATH_TAG())){
				blockInfo.m_filename = m_OutputBlock_Map.at(opblock)[OUTPUT_FILEPATH_TAG()];
			}
		}
		return status;
	}
	std::wstring
		TestCaseSettings(unsigned int BlockId, BLOCK_TYPE BlockType = BLOCK_TYPE::INPUT)
	{
		return TagValue(BlockId, SETTINGS_TAG(), BlockType);
	}
	
	std::wstring
		TagValue(unsigned int block, std::wstring const& tag, BLOCK_TYPE blocktype = BLOCK_TYPE::INPUT)
	{
		if (STATUS_GEN::SUCCESS != FillValueMap(block, blocktype))
			return{};

		switch (blocktype)
		{
		case BLOCK_TYPE::INPUT:
			if (m_InputBlock_Map.at(block).count(tag) == 0)
				return{};
			else
				return m_InputBlock_Map.at(block).at(tag);
		case BLOCK_TYPE::OUTPUT:
			if (m_OutputBlock_Map.at(block).count(tag) == 0)
				return{};
			else
				return m_OutputBlock_Map.at(block).at(tag);
		default:
			break;
		}
		return{};
	}

private:
	STATUS_GEN 
		FillBlockIds(BLOCK_TYPE BlockType, bool CheckAgain = false)
	{
		using namespace std;
		auto status = STATUS_GEN::SUCCESS;
		wstring LogHint{ __FUNCTIONW__ };
		wstring SSD{Logger_Res::SubStrDelim()};

		if (!CheckAgain && !m_InputBlockIds.empty()){
			return status;
		}

		ifstream IpFile(m_InputFile);
		if (!IpFile){
			return (status = STATUS_GEN::BAD_FILE);
		}

		string line;
		wstring startblockline;
		unsigned int BlockId = std::numeric_limits<unsigned int>::min();
		bool StartBlockFound = false;
		wstring block_start_tag = (BLOCK_TYPE::INPUT == BlockType) ? 
					INPUT_BLOCK_START() : OUTPUT_BLOCK_START();
		wstring block_end_tag = (BLOCK_TYPE::INPUT == BlockType) ? 
					INPUT_BLOCK_END() : OUTPUT_BLOCK_END();

		while (getline(IpFile, line))
		{
			if ((line = estd::trim(line)).empty())
				continue;
			wstring wline;
			utils::StringToWString(line, wline);
			if (estd::start_with(wline, Common_Res::COMMENT_START()))
				continue;

			if (estd::start_with_ci(wline, block_start_tag))
			{
				if (!StartBlockFound)
					StartBlockFound = true;
				else
					WriteLog(LogHint + SSD + MissingEndBlock() + SSD + wline, LogType::WARN);
				startblockline = wline;
				continue;
			}

			if (estd::start_with_ci(wline, block_end_tag))
			{
				if (!StartBlockFound && LOG_LEVEL::ALL == m_LogLevel)
					WriteLog(LogHint + SSD + ExtraEndBlock() + SSD + wline, LogType::WARN);
				else
				{
					wstring numstr = startblockline.substr(block_start_tag.length());
					try{
						BlockId = estd::stou(numstr);
						if (0 == BlockId){
							if (LOG_LEVEL::ALL == m_LogLevel)
								WriteLog(LogHint + SSD + ZeroBlockId() + SSD + startblockline, LogType::WARN);
						}
						else{
							if (m_InputBlockIds.end()
								!= find(m_InputBlockIds.begin(), m_InputBlockIds.end(), BlockId))
							{
								// Time to jump out since duplicate ids are dangerous
								if (LOG_LEVEL::ALL == m_LogLevel)
									WriteLog(LogHint + SSD + DupBlockId() + SSD + startblockline, LogType::ERR);
								status = STATUS_GEN::CONTAINER_ITEM_DUP;
								break;
							}
							m_InputBlockIds.push_back(BlockId);
						}
					}
					catch (exception e){
						if (LOG_LEVEL::ALL == m_LogLevel)
							WriteLog(LogHint + SSD + WrongBlockId() + SSD + startblockline, LogType::WARN);
					}
					StartBlockFound = false;
				}
				startblockline = L"";
			}
		}
		IpFile.close();
		return status;
	}

	STATUS_GEN 
		FillValueMap(unsigned int BlockId, BLOCK_TYPE BlockType = BLOCK_TYPE::INPUT)
	{
		using namespace std;

		if ((BLOCK_TYPE::INPUT == BlockType && m_InputBlock_Map.count(BlockId) > 0)
			|| (BLOCK_TYPE::OUTPUT == BlockType && m_OutputBlock_Map.count(BlockId) > 0))
				return STATUS_GEN::SUCCESS;

		wstring logmsg{ L"Block id " + to_wstring(BlockId) + L": " };
		Value_Map value_map;
		vector<wstring> block_entries;

		auto status = GetBlockEntries(block_entries, BlockId, BlockType);

		if (STATUS_GEN::SUCCESS == status)
		{
			for (auto &it : block_entries){
				auto TagLoc = it.find_first_of(Common_Res::TAG_SEP());
				if (wstring::npos == TagLoc){
					WriteLog(logmsg + L"Line [" + it + L"] is incorrectly formatted", LogType::ERR);
					continue;
				}
				value_map.insert({ estd::trim(it.substr(0, TagLoc - 1)),
					estd::trim(it.substr(it.find_last_of(Common_Res::TAG_SEP()) + 1)) });
			}
		}

		if (value_map.empty()){
			WriteLog(logmsg + IpFile_Err::EmptyBlock(), LogType::WARN);
			status = STATUS_GEN::BAD_PARAM;
		}
		else{
			m_InputBlock_Map.insert(Block_Pair(BlockId, value_map));
			status = STATUS_GEN::SUCCESS;
		}

		return status;
	}

	STATUS_GEN 
		GetBlockEntries(
		WSTRING_VEC& block_entries, unsigned int BlockId, BLOCK_TYPE BlockType)
	{
		using namespace std;
		STATUS_GEN status = STATUS_GEN::BAD_PARAM;
		wstring LogHint{__FUNCTIONW__};
		wstring SSD = Logger_Res::SubStrDelim();

		ifstream ifile(m_InputFile);
		if (!ifile.is_open()){
			WriteLog(LogHint + SSD + FileOpenError(), LogType::ERR);
			return (status = STATUS_GEN::BAD_FILE);
		}

		wstring block_start_tag =
			estd::tolower((BLOCK_TYPE::INPUT == BlockType) ? 
			INPUT_BLOCK_START() : OUTPUT_BLOCK_START());
		wstring block_end_tag =
			estd::tolower((BLOCK_TYPE::INPUT == BlockType) ? 
			INPUT_BLOCK_END() : OUTPUT_BLOCK_END());
		
		string line("");

		while (true)
		{
			bool start_found = false;
			block_entries.clear(); // Clear old data if any

			while (getline(ifile, line))
			{
				line = estd::trim(line);
				wstring wline{L""};
				utils::StringToWString(estd::trim(line), wline);
				if (wline.empty() || estd::start_with(wline, Common_Res::COMMENT_START())) 
					continue;

				if (estd::start_with_ci(wline, block_start_tag)){
					if (start_found) block_entries.clear();
					start_found = true;
				} else if (estd::start_with_ci(wline, block_end_tag)){
					if (!start_found) {
						block_entries.clear();
						continue;
					}
					else{
						start_found = false;
						break;
					}
				}
				if (start_found) block_entries.push_back(wline);
			}
			if (start_found)				// EOF reached without matching end block
				block_entries.clear();
			if (block_entries.size() == 0)	// Found no proper block
				break;

			try{					// First entry is block_start_tag
				if (estd::stou(block_entries[0].substr(block_start_tag.length())) != BlockId)
					continue;		// Try with next block
				else
					break;			// Data found
			}
			catch (exception){
				continue;			// Try with next block
			}
		}

		if (block_entries.size() != 0)
			block_entries.erase(block_entries.begin()); // erase entry for block_start_tag
		if (block_entries.size() != 0)
			status = STATUS_GEN::SUCCESS;

		ifile.close();
		return status;
	}
};

//////////////////////////////////////////////////////////////////////////
// InputFileHandler Constructors, Copy constructors and Destrcutors
InputFileHandler::InputFileHandler() : pImpl(std::make_unique<Impl>()){}
InputFileHandler::InputFileHandler(std::wstring const& inputfile, LOG_LEVEL loglevel)
	: pImpl(std::make_unique<Impl>(inputfile)) {}

InputFileHandler::InputFileHandler(InputFileHandler const&  src)
	: pImpl(std::unique_ptr<Impl>(new Impl(*src.pImpl))){}
InputFileHandler& 
InputFileHandler::operator=(InputFileHandler const&  src)
{
	if (this != &src)
		*pImpl = *src.pImpl;
	return *this;
}

InputFileHandler::~InputFileHandler(){}
//////////////////////////////////////////////////////////////////////////
// InputFileHandler Public methods
unsigned int InputFileHandler::InvalidBlockId(){ 
	return pImpl->InvalidBlockId(); 
}
bool InputFileHandler::IsAvailable(){
	return (pImpl->IsAvailable());
}
bool InputFileHandler::IsValidBlockId(unsigned int Block){
	return (pImpl->IsValidBlockId(Block));
}
void InputFileHandler::SetLogLevel(LOG_LEVEL loglevel){
	return pImpl->SetLogLevel(loglevel);
}

STATUS_GEN 
InputFileHandler::BlockIds(UINT_VEC &BlockIds, BLOCK_TYPE BlockType, bool CheckAgain){
	return (pImpl->BlockIds(BlockIds, BlockType, CheckAgain));
}
MAJOR_OPS 
InputFileHandler::MajorOp(unsigned int BlockId){
	return (pImpl->MajorOp(BlockId));
}
std::wstring 
InputFileHandler::SubOp(unsigned int BlockId){
	return (pImpl->SubOp(BlockId));
}
OPERAND_TYPE 
InputFileHandler::GetOperandType(unsigned int BlockId){
	return (pImpl->GetOperandType(BlockId));
}
unsigned int 
InputFileHandler::OperandCount(unsigned int BlockId){
	return pImpl->GetOperandCount(BlockId);
}
std::vector<std::pair<std::wstring, std::wstring>>
InputFileHandler::RawOperands(unsigned int Block){
	return pImpl->RawOperands(Block);
}
std::vector<std::pair<std::wstring, SubOpOperands>>
InputFileHandler::Operands(unsigned int Block){
	return pImpl->Operands(Block);
}
SubOpOperands 
InputFileHandler::Operand(unsigned int BlockId, const std::wstring& operand){
	return pImpl->Operand(BlockId, operand);
}
STATUS_GEN 
InputFileHandler::GetOperand(unsigned int BlockId, const std::wstring& argid, SubOpOperands& out){
	return pImpl->GetOperand(BlockId, argid, out);
}
STATUS_GEN 
InputFileHandler::GetSampleDataOptions(unsigned int BlockId, bool& UseSample, bool& UseSampleDefault){
	return pImpl->GetSampleDataOptions(BlockId, UseSample, UseSampleDefault);
}
STATUS_GEN 
InputFileHandler::GetSampleHints(unsigned int BlockId, WSTRING_VEC& SampleHints){
	return pImpl->GetSampleHints(BlockId, SampleHints);
}
WSTRING_VEC InputFileHandler::SampleHints(unsigned int BlockId){
	return pImpl->SampleHints(BlockId);
}

std::pair<unsigned int, STATUS_GEN> 
InputFileHandler::OutputBlockId(unsigned int ipblock){
	return pImpl->GetOutputBlockId(ipblock);
}
STATUS_GEN
InputFileHandler::GetOpBlockData(unsigned int opblock, OutputBlockInfo & blockInfo){
	return pImpl->GetOutputBlockData(opblock, blockInfo);
}

std::wstring
InputFileHandler::TestCaseSettings(unsigned int block, BLOCK_TYPE blocktype){
	return pImpl->TestCaseSettings(block, blocktype);
}

std::wstring
InputFileHandler::TagValue(unsigned int block, std::wstring const& tag, BLOCK_TYPE blocktype){
	return pImpl->TagValue(block, tag, blocktype);
}