#pragma once
#include "CommonDefs.h"
#include "Problems.h"

///////////////////////////////////////////////////////////////////////////////
/// Data transfer from main module to sub modules
struct ModuleData
{
	unsigned int	InputBlockId;
	RUNTYPE			runtype;
	TRACE_STATE		tracestate;
	MAJOR_OPS		majorops;
	std::wstring    ipfilename;
	std::wstring    opfilename;

	WSTRING_VEC		args;

	ModuleData()
	{
		InputBlockId = InputFileHandler().InvalidBlockId();
		runtype = RUNTYPE::END;
		tracestate = TRACE_STATE::TRACE_NO;
		majorops = MajorOps().GetInvalidId();
		ipfilename = IpFile_Res::INPUT_FILE_NAME();
		opfilename = OPFile_Res::FileName();

		args = {};
	}
	friend std::wstring to_wstring(ModuleData const&);
};

///////////////////////////////////////////////////////////////////////////////
/// Base Class for test harnesses
class TestCaseSettings
{
private:
	bool		m_verbose_output;
	bool		m_summary_output;
	size_t		m_basecase_size;
	size_t		m_op_repeatcount;

public:
	bool verbose_output();
	bool summary_output();

	size_t basecase_size();
	size_t op_repeatcount();
};

///////////////////////////////////////////////////////////////////////////////
/// Base Class for test harnesses
class TestBase
{
protected:
	struct MenuChoices
	{
		MenuId subop;
		MenuId operand;
	};
	struct MenuData
	{
		MenuId			menu_id;
		MenuItemInfo	item_info;
	};

public:
	TestBase() :TestBase(ModuleData{}){}
	TestBase(ModuleData	module_data);
	~TestBase(){}

	virtual std::wstring ClassName();
	virtual MAJOR_OPS MajorOP();

	virtual STATUS_GEN Run() = 0;
	virtual STATUS_GEN Run(ModuleData moduleData) = 0;

protected:
	STATUS_GEN RunConsole();
	STATUS_GEN RunCL();
	STATUS_GEN RunFI();

	STATUS_GEN
		RetrieveFileData();
	STATUS_GEN
		RetrieveFileData(ModuleData const&);
	MenuChoices
		GetMenuChoices();
	MenuChoices
		GetMenuChoices(std::list<ConsoleMenu::Flags> const&);
	MenuId
		GetMenuChoices(ConsoleMenu const &,
		ConsoleMenu::DisplaySort = ConsoleMenu::DisplaySort::ID, 
		std::list<ConsoleMenu::Flags> const& = {});

	STATUS_GEN
		UpdateOperandMenu(std::vector<OPERAND_TYPE> const& skip_entries = {});
	OPERAND_TYPE
		GetOperandType(MenuId const & menu_id) const;
	STATUS_GEN
		GetOperands(OPERAND_TYPE operand,
		bool use_sample = false, bool use_sample_default = true);
	void
		CleanOperandLists();

protected:
	ModuleData		m_module_data;

	// Updated in base class
	TRACE_STATE		m_tracestate;

	OutputBlockInfo m_output_block;
	WSTRING_VEC		m_subops;

	bool			m_use_sample;
	bool			m_use_sample_default;
	WSTRING_VEC		m_sample_hints;

	// May get updated in subclass or baseclass
	OPERAND_TYPE	m_operand_type;
	std::vector<std::pair<std::wstring, SubOpOperands>> m_operands;

	std::wstring	m_str_operand;
	INT_VEC			m_i_operands;
	UINT_VEC		m_ui_operands;
	WCHAR_VEC		m_c_operands;
	WSTRING_VEC		m_str_operands;
	std::vector<WSTRING_PAIR> m_testcase_settings;

	// Update initiated in subclasses
	std::vector<OperandMenuItem>	m_operand_menu_data;
	ConsoleMenu					m_top_menu;
	ConsoleMenu					m_operand_menu;
};

///////////////////////////////////////////////////////////////////////////////
class BinTree_Test : public TestBase
{
public:
	virtual STATUS_GEN
		Run();
	virtual STATUS_GEN
		Run(ModuleData);

private:
	STATUS_GEN
		RunConsole();
};

///////////////////////////////////////////////////////////////////////////////
class LinkedList_Test : public TestBase
{
public:
	virtual STATUS_GEN
		Run();
	virtual STATUS_GEN
		Run(ModuleData);

private:
	STATUS_GEN
		RunConsole();
};

///////////////////////////////////////////////////////////////////////////////
class Strings_Test : public TestBase
{
public:
	virtual STATUS_GEN
		Run();
	virtual STATUS_GEN
		Run(ModuleData);

private:
	STATUS_GEN
		RunConsole();
};

///////////////////////////////////////////////////////////////////////////////
class Palindrome_Test : public TestBase /*, private Palindrome*/
{
public:
	Palindrome_Test() : Palindrome_Test(ModuleData{}){}
	Palindrome_Test(ModuleData module_data);
	~Palindrome_Test(){}

	virtual STATUS_GEN Run();
	virtual STATUS_GEN Run(ModuleData module_data);

private:
	STATUS_GEN
		RunFI();
	STATUS_GEN
		GetOperands(OPERAND_TYPE);
	STATUS_GEN
		GetSampleOperands(Palindrome_Probs, HintData_VEC const & = {});
	STATUS_GEN
		DoProblems(Palindrome_Probs);

private:
	std::map<Palindrome_Probs, MenuItemInfo> m_subop_data;
	bool m_verbose_outout;
	bool m_print_palindrome;
};

///////////////////////////////////////////////////////////////////////////////
class Twisters_Test : public TestBase
{
public:
	virtual STATUS_GEN
		Run();
	virtual STATUS_GEN
		Run(ModuleData);
private:
	STATUS_GEN
		RunConsole();
};

///////////////////////////////////////////////////////////////////////////////
class Sorting_Test : public TestBase
{
public:
	Sorting_Test(TRACE_STATE tracestate = TRACE_STATE::TRACE_NO);
	~Sorting_Test();

	virtual STATUS_GEN Run();
	virtual STATUS_GEN Run(ModuleData);

private:
	STATUS_GEN 
		RunConsole();
	STATUS_GEN 
		DoProblems(SORTING_Probs algo, OPERAND_TYPE opertype);
	STATUS_GEN
		getSortingData(OPERAND_TYPE opertype);

private:
	using SortMenuItem = MenuItem < SORTING_Probs >;
	std::vector <SortMenuItem> AlgoMenuEntries;
	std::vector<OperMenuItem> OperMenuEntries;

	int		*i_sortList = NULL;
	char	*c_sortList = NULL;
	char	**str_sortList = NULL;
	size_t	SortListSize = 0;

	bool			m_sort_status;
	INT_VEC			m_i_input;
	UINT_VEC		m_ui_input;
	WCHAR_VEC		m_wc_input;
	WSTRING_VEC		m_wstr_input;

	void PrintInternalList(OPERAND_TYPE opertype);
	void FreeInternalLists();

	TRACE_STATE m_tracestate = TRACE_STATE::TRACE_NO;
};

///////////////////////////////////////////////////////////////////////////////
class Search_Test :public TestBase
{
public:
	Search_Test();
	~Search_Test();

	virtual STATUS_GEN Run();
	virtual STATUS_GEN Run(ModuleData);

private:
	STATUS_GEN 
		RunConsole();
	int 
		CallSearchFunc(SEARCH_Probs algo, OPERAND_TYPE opertype);

	STATUS_GEN 
		getSearchData(OPERAND_TYPE opertype);
	void 
		FreeInputData();

private:
	using SearchMenuItem = MenuItem < SEARCH_Probs >;
	std::vector <SearchMenuItem> AlgoMenuEntries;
	std::vector<OperMenuItem> OperMenuEntries;

	int i_searchItem = 0, *i_searchList = NULL;
	intmax_t im_searchItem = 0, *im_searchList = NULL;
	char c_searchItem = '\0', *c_searchList = NULL;
	char *str_searchItem = NULL, **str_searchList = NULL;
	size_t SearchListSize = 0, SearchItemSize = 0;

	std::map<SEARCH_Probs, std::map<OPERAND_TYPE, bool>>ImplementationStatus;
	typedef std::map<OPERAND_TYPE, bool> ROW;
	typedef std::map<SEARCH_Probs, ROW> OUTER;
};

///////////////////////////////////////////////////////////////////////////////
class BinOps_Test : public TestBase
{
public:
	BinOps_Test(TRACE_STATE tracestate = TRACE_STATE::TRACE_NO);
	~BinOps_Test();

	virtual STATUS_GEN
		Run();
	virtual STATUS_GEN
		Run(ModuleData);
private:
	STATUS_GEN
		RunConsole(BINOPS_Probs ops = BINOPS_Probs::END);

private:
	std::map<BINOPS_Probs, MenuData> m_topmenu_data;
	MenuId m_topmenu_exitid;
};

////////////////////////////////////////////////////////////////////////////////
class CLRS4_Test : public TestBase
{
public:
	CLRS4_Test() : CLRS4_Test(ModuleData{}){}
	CLRS4_Test(ModuleData module_data);
	~CLRS4_Test(){}

	virtual STATUS_GEN Run();
	virtual STATUS_GEN Run(ModuleData module_data);

private:
	STATUS_GEN
		RunConsole();
	STATUS_GEN
		RunFI();
	STATUS_GEN
		RunCL(){ return STATUS_GEN::UNIMPLEMENTED; }
	void
		CleanOperandLists();
	STATUS_GEN
		GetOperands(OPERAND_TYPE);
	HintSpecs
		GetHintSpecs(CLRS4_Probs);
	STATUS_GEN
		GetSampleOperands(CLRS4_Probs, HintData_VEC const& = {});
	STATUS_GEN
		CreateSample(CLRS4_Probs, HintData_VEC const& = {});
	STATUS_GEN
		DoProblems(CLRS4_Probs, HintData_VEC const& = {});

private:
	const size_t	MAT_MAXR = 8;
	const size_t	MAT_MAXC = 8;
	const int		MAT_MAXVAL = 10000;
	const int		MAT_MINVAL = -10000;

	std::map<CLRS4_Probs, MenuItemInfo> m_subop_data;
	
	bool		m_verbose_output;
	bool		m_summary_output;
	size_t		m_basecase_size;
	size_t		m_op_repeatcount;
	
	std::vector<Matrix>		m_matrix;
	std::vector<ComplexNum> m_complex_numbers;

	using TopMenuItem = MenuItem <CLRS4_Probs>;
	std::vector <TopMenuItem> m_TopMenuEntries;
	std::vector <OperMenuItem> m_OperMenuEntries;
};

////////////////////////////////////////////////////////////////////////////////
//void TestLinkedList(WSTRING_VEC);
//void TestBinaryTree(WSTRING_VEC);
//void TestStrings(WSTRING_VEC);
//void TestTwisters(WSTRING_VEC);
//void TestSearch(WSTRING_VEC);
//void TestSorting(WSTRING_VEC);
//void TestBinOps(WSTRING_VEC);
//void TestCLRS_Ch4(WSTRING_VEC);
//
//void Run_CLRS4(ModuleData module_data);
//void Run_Palindrome(ModuleData module_data);