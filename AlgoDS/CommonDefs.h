#pragma once

#include "Resources.h"
#include "Utilities.h"
#include "estd.h"

#include <string>
#include <vector>
#include <map>
#include <list>
#include <memory>
#include <ctime>
#include <set>
#include <algorithm>
#include <iostream>
#include <bitset>


///////////////////////////////////////////////////////////////////////////////
#define NOT_FOUND				-1 
#define NOT_IMPLEMENTED			-2

///////////////////////////////////////////////////////////////////////////////
using INT_VEC = std::vector < int >;
using UINT_VEC = std::vector < unsigned int >;
using WCHAR_VEC = std::vector < wchar_t >;
using WSTRING_VEC = std::vector < std::wstring >;
using WSTRING_PAIR = std::pair	< std::wstring, std::wstring >;
using INT_MATRIX = std::vector < std::vector < int > >;
using UINT_MATRIX = std::vector < std::vector < unsigned int > >;

///////////////////////////////////////////////////////////////////////////////
struct _IntArr
{
	int *list;
	unsigned int length;
};
typedef struct _IntArr IntArr;

struct _ChArr
{
	char *list;
	unsigned int length;
};
typedef struct _ChArr ChArr;

///////////////////////////////////////////////////////////////////////////////
#define FREE(ptr) if (ptr) {free(ptr); ptr=NULL;}

///////////////////////////////////////////////////////////////////////////////
enum class TRACE_STATE { TRACE_NO, TRACE_BASIC, TRACE_DETAILED };
std::wstring to_wstring(TRACE_STATE);

////////////////////////////////////////////////////////////////////////////////
using estd::PrintTimer;
class PrintExecutionTimer : public PrintTimer
{
public:
	const std::wstring SS = std::wstring(L" ");

	virtual std::wstring
		to_wstring(std::chrono::hours::rep value, std::chrono::hours dummy);
	virtual std::wstring
		to_wstring(std::chrono::minutes::rep value, std::chrono::minutes dummy);
	virtual std::wstring
		to_wstring(std::chrono::seconds::rep value, std::chrono::seconds dummy);
	virtual std::wstring
		to_wstring(std::chrono::milliseconds::rep value, std::chrono::milliseconds dummy);
	virtual std::wstring
		to_wstring(std::chrono::microseconds::rep value, std::chrono::microseconds dummy);
	virtual std::wstring
		to_wstring(std::chrono::nanoseconds::rep value, std::chrono::nanoseconds dummy);
};

///////////////////////////////////////////////////////////////////////////////
/// Log Writer
/// http://www.cplusplus.com/reference/ctime/time/
class LoggerBase
{
public:
	enum class TYPE
	{
		INFO, ERR, WARN, DEBUG
	};
	enum class OPMEDIUM
	{
		Console, File, EventLog,
		ConsoleFile, ConsoleEventLog, FileEventLog
	};

	virtual void Write(std::wstring const & message, 
				TYPE type = TYPE::INFO, time_t timer = time(NULL)) = 0;
	virtual void Start() = 0;
	virtual void Close() = 0;
};
using LogType = LoggerBase::TYPE;

class Logger : public LoggerBase, public Logger_Res
{
public:
	static bool LogStarted;
	static OPMEDIUM OpMed;

public:
	Logger();
	Logger(OPMEDIUM opmed);
	Logger(std::wstring const& LogFile, std::wstring const& LogPath, 
		OPMEDIUM = OPMEDIUM::Console);
	~Logger();

	Logger(Logger const& src);
	Logger& operator=(Logger const& src);

	virtual void 
		Write(std::wstring const &, TYPE = TYPE::INFO, time_t = time(NULL));
	virtual void 
		Start();
	virtual void 
		Close();
	static void 
		SetMedium(OPMEDIUM opmed);

private:
	std::wstring
		date_text(time_t timer);
	void
		writetofile(std::wstring const & message);
	std::wstring
		type_text(TYPE type);

private:
	const std::wstring SS = std::wstring(L" ");

	std::wstring	m_LogFile;
	std::wstring	m_LogPath;
	OPMEDIUM		m_OpMed;
};
void WriteLog(std::wstring const&, LogType = LogType::INFO, time_t = time(NULL));
void SetMedium(Logger::OPMEDIUM);

//////////////////////////////////////////////////////////////////////////////
/// Status messages
class StatusGen : public SGen_Res
{
public:
	enum class TYPE
	{
		START,
		SUCCESS = START, FAILURE,
		PROB_FAIL, PROB_NO_SOLN, PROB_BAD_IP, 
		BAD_PARAM, BAD_INPUT, BAD_DATA, 
		MISSING_FILE, BAD_FILE,
		BAD_SAMPLE_HINT,NO_SAMPLE_HINT, 
		CONTAINER_NOITEM, CONTAINER_ITEM_DUP, CONTAINER_EMPTY,
		MEMORY_ALLOC_FAIL,
		UNIMPLEMENTED, UNKNOWN_FAILURE,
		END
	};

public:
	std::wstring
		str(TYPE sgen);
	friend std::wstring 
		to_wstring(TYPE sgen);
private:
	std::wstring type_wstring(TYPE sgen);
};
using STATUS_GEN = StatusGen::TYPE;
std::wstring to_wstring(STATUS_GEN sgen);

///////////////////////////////////////////////////////////////////////////////
/// Argument map
enum class RUNTYPE
{
	ConsoleOps, FileInput, CommandLine, 
	END
};
std::wstring to_wstring(RUNTYPE);

class RunMap
{
public:
	RunMap(RUNTYPE op, std::wstring const& name)
		:m_op(op), m_name(name) { }
	RunMap(RUNTYPE op, std::wstring const& name, WSTRING_VEC const& extranames)
		:m_op(op), m_name(name), m_extranames(extranames) {}

	RUNTYPE gettype() { return m_op; }
	void addextranames(WSTRING_VEC extranames) { m_extranames = extranames; }

	std::wstring extranames();
	bool matchnames(std::wstring const& candidate);

private:
	RUNTYPE			m_op;
	std::wstring	m_name;
	WSTRING_VEC		m_extranames;
};

//////////////////////////////////////////////////////////////////////////////
/// Major operations or top level operations
typedef void(*TestMajorOps)(WSTRING_VEC);
template<typename T> struct MenuItem;

enum class MAJOR_OPS
{
	SORT, SEARCH, BINOPS, LINKEDLIST,			// 0 to 3 
	STRINGS, TWISTERS, BINTREE, CLRS_CH4,		// 4 to 7
	PALINDROME,
	EXIT, END
};
std::wstring to_wstring(MAJOR_OPS id);

class MajorOps
{
public:
	typedef void(*MajorOpsFunc)(WSTRING_VEC);

private:
	using MajorOpsMenuItem = MenuItem < MAJOR_OPS >;

	std::map <MAJOR_OPS, std::wstring> name_map;
	std::map <MAJOR_OPS, MajorOpsFunc> ops_map;
	
public:
	MajorOps(){};

	size_t 
		GetTotalItems() const { return (static_cast<size_t>(MAJOR_OPS::END)); };
	MAJOR_OPS 
		GetInvalidId() const { return (MAJOR_OPS::END); };
	bool 
		IsValidId(MAJOR_OPS) const;
	bool 
		IsValidName(std::wstring const&);
	std::vector<MajorOpsMenuItem> 
		GetMenuItems(std::vector<MAJOR_OPS> const& SkipItems);
	MAJOR_OPS 
		GetId(std::wstring const& name, bool besteffort = true);
	std::wstring 
		GetName(MAJOR_OPS id);
	MajorOpsFunc 
		GetOps(MAJOR_OPS id);

private:
	void 
		build_name_map();
	void 
		build_ops_map();
	void 
		buildmaps() { build_name_map(); build_ops_map(); }
	void 
		AddMenuItem(std::vector<MajorOpsMenuItem>& MenuItems, unsigned int & MenuId,
		MajorOpsMenuItem const& MenuItem, std::vector<MAJOR_OPS> const& SkipItems);
};
using MajorOpsMenuItem = MenuItem < MAJOR_OPS >;

///////////////////////////////////////////////////////////////////////////////
/// Operands from input
enum class OPERAND_TYPE
{
	INT, UINT, CHAR, STR,
	EXIT, END
};
std::wstring to_wstring(OPERAND_TYPE);

struct OperandMenuItem
{
	OPERAND_TYPE id;
	unsigned int idnum;
	std::wstring long_text;
	std::wstring short_text;
	
	bool 
		operator <(OperandMenuItem const& second)
	{
		return(id < second.id);
	}
	bool 
		operator ==(OperandMenuItem const& second)
	{
		return((id == second.id) && (idnum == second.idnum) &&
			(long_text == second.long_text) && (short_text == second.short_text));
	}
};

class OperandType
{
private:
	typedef std::map<OPERAND_TYPE, std::pair<std::wstring, std::wstring>> Value_Map;

	Value_Map m_names = Value_Map
	{
		{ OPERAND_TYPE::INT, { L"Integer", L"Int" } },
		{ OPERAND_TYPE::CHAR, { L"Character", L"Char" } },
		{ OPERAND_TYPE::STR, { L"String", L"Str" } },
		{ OPERAND_TYPE::UINT, { L"Unsigned Integer", L"UInt" } },
		{ OPERAND_TYPE::EXIT, { Common_Res::EXIT_LONG(), Common_Res::EXIT_SHORT() } }
	};
	std::wstring InvalidName = L"Unknown";

	using OperMenuItem = MenuItem<OPERAND_TYPE>;

public:
	OperandType();
	~OperandType();

	OperandType(OperandType const& src);
	
	OperandType& 
		operator=(OperandType const& src);

	OPERAND_TYPE 
		InvalidType() const;
	bool 
		IsValidType(OPERAND_TYPE opertype) const;
	bool 
		IsValidTypeText(std::wstring const& opertext) const;

	OPERAND_TYPE 
		Type(std::wstring const& opertext) const;
	std::wstring 
		TypeText(OPERAND_TYPE opertype) const;
	std::vector<OperMenuItem> 
		MenuEntries(std::vector<OPERAND_TYPE> const& SkipEntries = {}) const;

	std::vector<OperandMenuItem> 
		MenuData(std::vector<OPERAND_TYPE> const& SkipEntries = {}) const;
	OperandMenuItem
		ExitItemData(std::vector<OPERAND_TYPE> const& SkipEntries = {}) const;

	unsigned int 
		OperandTypeCount();
};
using OperMenuItem = MenuItem<OPERAND_TYPE>;

///////////////////////////////////////////////////////////////////////////////
enum class OPERAND_COMPLEX
{
	INT_TUPLE, UINT_TUPLE, CHAR_TUPLE, STR_TUPLE,
	INT_MULTI, UINT_MULTI, CHAR_MULTI, STR_MULTI,
	END
};
std::wstring 
to_wstring(OPERAND_COMPLEX);

///////////////////////////////////////////////////////////////////////////////
// Subop operands
class SubOpOperands
{
public:
	enum class TokStyle 
	{
		SIMPLE, TRIMMEDTOKENS, NONEMPTYTOKENS, NONEMPTYTRIMMEDTOKENS,
		UNIQUETOKENS, UNIQUETRIMMEDTOKENS, UNIQUENONEMPTYTOKENS, UNIQUENONEMPTYTRIMMEDTOKENS
	};

public:
	SubOpOperands();
	SubOpOperands(std::wstring const& input, OPERAND_TYPE opertype, 
			std::wstring const& delims = SubOpOperands_Res::OPERAND_SEP());
	~SubOpOperands();

	SubOpOperands(SubOpOperands const& src);
	SubOpOperands& operator=(SubOpOperands const& src);

public:
	std::wstring 
		Input() const;
	OPERAND_TYPE 
		Type() const;
	std::wstring 
		Delims(void) const;

	void 
		Set(std::wstring const&, OPERAND_TYPE, 
			std::wstring const& = SubOpOperands_Res::OPERAND_SEP());

	STATUS_GEN 
		GetList(INT_VEC&, TokStyle = TokStyle::NONEMPTYTRIMMEDTOKENS);
	STATUS_GEN
		GetList(UINT_VEC&, TokStyle = TokStyle::NONEMPTYTRIMMEDTOKENS);
	STATUS_GEN 
		GetList(WCHAR_VEC&, TokStyle = TokStyle::NONEMPTYTRIMMEDTOKENS);
	STATUS_GEN 
		GetList(WSTRING_VEC&, TokStyle = TokStyle::NONEMPTYTRIMMEDTOKENS);

private:
	struct StyleMask
	{
		bool unique = false;
		bool trimmed = false;
		bool nonempty = false;
	};

	std::wstring	m_input = L"";
	OPERAND_TYPE	m_type = OperandType().InvalidType();
	std::wstring    m_delims = SubOpOperands_Res::OPERAND_SEP();

	STATUS_GEN
		TokenizedList(TokStyle style, WSTRING_VEC& tokens);
	StyleMask
		GetStyleMask(TokStyle style);
	bool
		IsValidParams(TokStyle style);
};

///////////////////////////////////////////////////////////////////////////////
/// Writing output
enum class OutputType
{ 
	Console, 
	File, 
	ConsoleFile, 
	END 
};
std::wstring 
to_wstring(OutputType optype);

struct OutputBlockInfo
{
	unsigned int	m_Id;
	std::wstring	m_target;
	OutputType		m_optype;
	std::wstring	m_filename;

	OutputBlockInfo();
	OutputType 
		Type(std::wstring const& type_text);
	std::wstring 
		Text(OutputType optype);

private:
	typedef std::map<OutputType, std::wstring> TypeText_Map;
	TypeText_Map TypeTextMap();
};

///////////////////////////////////////////////////////////////////////////////
/// Output file handler
class OutputHandler
{
private:
	struct Impl;
	std::unique_ptr<Impl> pImpl;

public:
	OutputHandler();
	OutputHandler(std::wstring const & FileName, OutputType optype);
	~OutputHandler();

	OutputHandler(OutputHandler const& src);

	OutputHandler& 
		operator=(OutputHandler const& src);

	void 
		DeleteFile();
	bool 
		IsAvailable();

	void 
		Write(std::wstring const& data);
	void
		WriteLine(std::wstring const& data=L"");
	void 
		WriteToFile(std::wstring const& data);
	void 
		WriteToConsole(std::wstring const& data);
};

/////////////////////////////////////////////////////////////////////////////// 
/// Reading input from input file
class InputFileHandler : protected IpFile_Res
{
public:
	enum class BLOCK_TYPE{ INPUT, OUTPUT, END };
	enum class LOG_LEVEL{ ALL, BLOCK, END };

private:
	struct Impl;
	std::unique_ptr<Impl> pImpl;

public:
	InputFileHandler();
	InputFileHandler(std::wstring const& inputfile, 
		LOG_LEVEL loglevel = LOG_LEVEL::BLOCK);
	~InputFileHandler();

	InputFileHandler(InputFileHandler const& src);
	InputFileHandler& operator=(InputFileHandler const& src);

public:
	unsigned int 
		InvalidBlockId();
	bool 
		IsValidBlockId(unsigned int Block);
	bool 
		IsAvailable();

	void 
		SetLogLevel(LOG_LEVEL loglevel);

	STATUS_GEN 
		BlockIds(UINT_VEC &out, BLOCK_TYPE blockType, 
		bool CheckAgain = false);
	UINT_VEC
		BlockIds(BLOCK_TYPE blockType, bool CheckAgain = false);

	MAJOR_OPS 
		MajorOp(unsigned int Block);

	std::wstring 
		SubOp(unsigned int Block);
	OPERAND_TYPE 
		GetOperandType(unsigned int Block);
	
	unsigned int 
		OperandCount(unsigned int Block);
	std::vector<std::pair<std::wstring, std::wstring>>
		RawOperands(unsigned int Block);
	std::vector<std::pair<std::wstring, SubOpOperands>>
		Operands(unsigned int Block);
	SubOpOperands 
		Operand(unsigned int Block, std::wstring const& argid);
	STATUS_GEN 
		GetOperand(unsigned int Block, std::wstring const& argid, 
		SubOpOperands& out);

	STATUS_GEN 
		GetSampleDataOptions(unsigned int Block, bool& UseSample, 
		bool& UseSampleDefault);
	WSTRING_VEC 
		SampleHints(unsigned int ipblock);
	STATUS_GEN 
		GetSampleHints(unsigned int ipblock, WSTRING_VEC &);

	std::pair<unsigned int, STATUS_GEN> 
		OutputBlockId(unsigned int ipblock);
	STATUS_GEN 
		GetOpBlockData(unsigned int opblock, OutputBlockInfo &);
	std::wstring
		TestCaseSettings(unsigned int block, BLOCK_TYPE = BLOCK_TYPE::INPUT);

	std::wstring
		TagValue(unsigned int block, std::wstring const& tag, 
		BLOCK_TYPE = BLOCK_TYPE::INPUT);
};

///////////////////////////////////////////////////////////////////////////////
struct MaxSubArrayInfo
{
	size_t low;
	size_t high;
	int sum;

	MaxSubArrayInfo() :MaxSubArrayInfo(0, 0, std::numeric_limits<int>::min()){}
	MaxSubArrayInfo(size_t a_low, size_t a_high, int a_sum) :
		low(a_low), high(a_high), sum(a_sum){}
};

///////////////////////////////////////////////////////////////////////////////
struct PriceItem
{
	unsigned int seq;
	unsigned int price;

	PriceItem();
	PriceItem(unsigned int a_seq, unsigned int a_price);
	PriceItem(PriceItem const&);
	
	PriceItem& 
		operator=(PriceItem const&);
	bool 
		operator<(PriceItem const&);
	bool 
		operator==(PriceItem const&);
};
typedef std::vector<PriceItem> PriceSeq;

///////////////////////////////////////////////////////////////////////////////
struct MaxDiffPair
{
	PriceItem first;
	PriceItem last;
};
std::wstring to_wstring(MaxDiffPair const& diff_pair);


///////////////////////////////////////////////////////////////////////////////
/// Price related structures
typedef std::map<struct tm, unsigned int> DatePriceMap;
typedef std::map<unsigned int, unsigned int> DayPriceMap;
INT_VEC GetDailyVariations(const DatePriceMap& DatePrices);
INT_VEC GetDailyVariations(const DayPriceMap& DayPrices);

enum class DayOrDate{ DAY, DATE, END };
class PricePoint
{
public:
	DayOrDate dayordate;
	union
	{
		struct tm Date;
		unsigned int Day;
	};
	unsigned int Price;

public:
	PricePoint();
	PricePoint(const PricePoint& src);
	~PricePoint();

	PricePoint& 
		operator=(const PricePoint& src);
	bool 
		operator<(const PricePoint & second);
	bool 
		operator==(const PricePoint & second);
};
struct MaxProfitPoints
{
	class PricePoint Buy;
	class PricePoint Sell;
};
std::wstring to_wstring(MaxProfitPoints const&);
inline std::wostream&  
operator<<(std::wostream& os, MaxProfitPoints const& input)
{
	return os << to_wstring(input);
}

///////////////////////////////////////////////////////////////////////////////
/// Generate Sample Data
struct HintData
{
	OPERAND_TYPE	m_type;
	std::wstring	m_name;
	union
	{
		unsigned int	m_uintval;
		int				m_intval;
		wchar_t			m_wcval;
	};
	std::wstring m_textval;

	friend std::wstring to_wstring(HintData const&);
};
typedef std::vector<HintData> HintData_VEC;
typedef std::map<std::wstring, OPERAND_TYPE> HintSpecs;
std::wstring to_wstring(HintData const&);

class SampleData : protected SData_Res
{
public:
	struct Limits
	{
		static size_t maxsize_set(){ return 100; }
		static size_t maxlen_str(){ return 20; }
		static int i_low() { return std::numeric_limits<int>::min(); }
		static int i_high() { return std::numeric_limits<int>::max(); }
		static unsigned int ui_low() { return std::numeric_limits<unsigned int>::min(); }
		static unsigned int ui_high() { return std::numeric_limits<unsigned int>::max(); }
	};	
	
public:
	SampleData() : SampleData(L"", {}){}
	SampleData(HintSpecs const& hintspecs) : SampleData(L"", hintspecs){}
	SampleData(std::wstring const& Hints, HintSpecs const& hintspecs);
	~SampleData() = default;

public:
	HintData_VEC 
		GetHintList(HintSpecs const& hintspecs);
	HintData_VEC 
		GetHintList(std::wstring const& hints);
	HintData_VEC 
		GetHintList(std::wstring const& hints, HintSpecs const& hintspecs);

	HintData_VEC 
		Generate(HintSpecs const& IncHints);

	size_t 		
		Generate(INT_VEC& Samples, size_t Total = Limits::maxsize_set(),
					int Min = Limits::i_low(), int Max = Limits::i_high());
	size_t		
		Generate(WSTRING_VEC& Samples, size_t Total = Limits::maxsize_set(), 
					size_t MaxStrLength = Limits::maxlen_str());
	size_t 		
		Generate(WCHAR_VEC& Samples, size_t Total = Limits::maxsize_set());

private:
	std::wstring	m_hints;
	HintSpecs		m_hintspecs;
};
using SDLimits = SampleData::Limits;
std::wstring to_wstring(HintData_VEC const&);

///////////////////////////////////////////////////////////////////////////////
/// A menu item
template<typename T>
struct MenuItem
{
	unsigned int	Id;
	std::wstring	IdText;
	std::wstring	Text;
	T				EnumType;

	MenuItem()
	{
		Id			= std::numeric_limits<unsigned int>::max();
		IdText		= L"";
		Text		= L"";
		EnumType	= T::END;
	};
	MenuItem(T Type, unsigned int MenuId, 
		std::wstring const& MenuTextId, std::wstring const& MenuText) 
		:EnumType(Type), Id(MenuId), IdText(MenuTextId), Text(MenuText) 
	{};
	~MenuItem(){};

	bool operator <(const MenuItem & second){
		return(Id < second.Id);
	}
	bool operator ==(const MenuItem & second)
	{
		return((Id == second.Id) && (IdText == second.IdText) && 
			(Text == second.Text) && (EnumType == second.EnumType));
	}
};

///////////////////////////////////////////////////////////////////////////////
/// Get User choices on console menu
enum class MenuMsgOpts
{
	REPEAT_SUBOP, REPEAT_MAJOROP,
	HEADING_MAJOROP, USER_INPUT,
	SAMPLE_DATA,
	SAMPLE_DATA_DEFAULT, SAMPLE_DATA_TOTAL,
	SAMPLE_DATA_LOWEST, SAMPLE_DATA_HIGHEST,
	SAMPLE_DATA_MAXSTRLENGTH,
	OPERAND_INPUT_UINT, OPERAND_INPUT_INT,
	OPERAND_INPUT_CHAR, OPERAND_INPUT_STR,
	BAD_INPUT,
	END
};

class UserMenu : public Menu_Res
{
public:
	UserMenu() : UserMenu(MajorOps().GetInvalidId()){}
	UserMenu(MAJOR_OPS MajorOp) :m_MajorOp(MajorOp){ }

public:
	std::wstring MenuMsgStr(MenuMsgOpts option, std::wstring const& additional = L"");
	std::wstring MenuMsgStr(MAJOR_OPS MajorOp, MenuMsgOpts option, std::wstring const& additional = L"");

	template <typename T>
	T TopMenuChoice(std::vector<MenuItem<typename T>> const& TopMenuEntries)
	{
		using namespace std;
		wcout << MenuMsgStr(MenuMsgOpts::HEADING_MAJOROP) << endl;

		unsigned int menuread_i;
		utils::clear_wcin();
		for (auto &it : TopMenuEntries)
			wcout << it.Id << NumTextSep() << it.Text << SS();
		wcout << endl << MenuMsgStr(MenuMsgOpts::USER_INPUT);
		wcin >> menuread_i;
		for (auto &it : TopMenuEntries)
			if (it.Id == menuread_i) return(it.EnumType);
		return T::END;
	}
	
	void SampleChoice(bool& OptForSample, bool& DeafaultSamples);
	void SampleStyle(size_t& Total, int& Lowest, int& Highest, OPERAND_TYPE opertype);
	void SampleStyle(size_t& Total, unsigned int& Lowest, unsigned int& Highest);
	void SampleStyle(size_t& Total, size_t& MaxStrlength);

	OPERAND_TYPE OperandMenuChoice(std::vector <OperMenuItem> const& OperMenuEntries);
	void GetOperands(UINT_VEC&, std::wstring const& seps = Menu_Res::DefSep());
	void GetOperands(INT_VEC&, std::wstring const& seps= Menu_Res::DefSep());
	void GetOperands(WCHAR_VEC&, std::wstring const& seps= Menu_Res::DefSep());
	void GetOperands(WSTRING_VEC&, std::wstring const& seps= Menu_Res::DefSep());

private:
	MAJOR_OPS	m_MajorOp;
};
std::wstring inline 
MenuMsgStr(MAJOR_OPS MajorOp, MenuMsgOpts option, std::wstring const& additional = L""){
	return UserMenu(MajorOp).MenuMsgStr(option, additional);
}

///////////////////////////////////////////////////////////////////////////////
/// Console Menu
struct MenuId
{
	unsigned int m_val;
};
inline bool 
operator<(MenuId const& lhs, MenuId const& rhs){
	return lhs.m_val < rhs.m_val;
}
inline bool 
operator==(MenuId const& lhs, MenuId const& rhs){
	return lhs.m_val == rhs.m_val;
}
inline bool 
operator!=(MenuId const& lhs, MenuId const& rhs){ return !operator==(lhs, rhs); }
inline bool 
operator>(MenuId const& lhs, MenuId const& rhs){ return !operator<(lhs, rhs); }

struct MenuItemInfo
{
	std::wstring longtext;
	std::wstring shorttext;

	friend bool any_of(MenuItemInfo const&, std::wstring const&);
	friend bool any_of_ci(MenuItemInfo const&, std::wstring);
};
bool any_of(MenuItemInfo const& c, std::wstring const& item);
bool any_of_ci(MenuItemInfo const& c, std::wstring item);

class ConsoleMenu
{
public:
	enum class DisplaySort{ ID, LongText, ShortText };
	enum class Flags
	{
		Horizontal = 0, Vertical,
		LongText, ShorTText,
		ShowHeading, NoShowHeading,
		Total
	};

private:
	const MenuId m_invalidid = MenuId{ 0 };
	const MenuItemInfo m_invalidinfo = MenuItemInfo{ L"", L"" };
	typedef std::bitset<static_cast<size_t>(Flags::Total)> FlagMask;

public:
	ConsoleMenu() : ConsoleMenu(L"", { { { 0 }, { L"", L"" } } }, { 0 }){}
	ConsoleMenu(
		std::wstring const& heading, 
		std::vector<std::pair<MenuId, 
		MenuItemInfo>> const&, 
		MenuId const&,
		std::list<Flags> const& = {});
	~ConsoleMenu();

	ConsoleMenu(ConsoleMenu const&);
	ConsoleMenu& 
		operator=(ConsoleMenu const&);

	STATUS_GEN	
		Add(MenuId const&, MenuItemInfo const&);
	STATUS_GEN	
		Add(std::vector<std::pair<MenuId, MenuItemInfo>> const&);
	STATUS_GEN	
		Delete(std::vector<MenuId> const&);
	void 
		SetFlags(std::list<Flags> const& = {});
	void 
		SetHeading(std::wstring const& = L"");
	void 
		SetExitID(MenuId const&);

	MenuId 
		Choice(DisplaySort = DisplaySort::ID, std::list<Flags> const& = {}) const;
	MenuId
		InvalidId() const;
	
	std::pair<bool, std::wstring> 
		IsValid() const;
	bool 
		IsExitId(MenuId const&) const;
	bool 
		IsValidId(MenuId const&) const;

private:
	void 
		ModifyFlagMask(FlagMask &, std::list<Flags> const& = {}) const;

private:
	std::wstring						m_heading;
	std::map<MenuId, MenuItemInfo>		m_items;
	MenuId								m_exit_id;
	FlagMask							m_display_options;
};

///////////////////////////////////////////////////////////////////////////////
///
struct _GenericList
{
	union _list{
		int * Int;
		char * Char;
		char ** Str;
	} list;
	OPERAND_TYPE opertype;
};
typedef struct _GenericList GenericList;

