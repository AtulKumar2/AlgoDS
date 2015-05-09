#pragma once
///////////////////////////////////////////////////////////////////////////////
// Various Resource strings
//
//
///////////////////////////////////////////////////////////////////////////////

#include <string>

#define ARGSTART_CHAR			L'-'

#define MAX_MENU_ITEM			512
#define MAXSIZE_LIST			4096
#define MAXSIZE_ITEM			256


struct Common_Res
{
	static std::vector<std::wstring> TRUE_MARKERS(){ return{ L"True", L"T", L"Yes", L"Y" }; }
	static std::vector<std::wstring> FALSE_MARKERS() { return{ L"False", L"F", L"No", L"N" }; }
	static std::wstring UNKNONWN(){ return  std::wstring(L"Unknown"); }
	static std::wstring INVALID_ID(){ return  std::wstring(L"Invalid Id"); }
	static std::wstring EXIT_LONG() { return  std::wstring(L"Exit"); }
	static std::wstring EXIT_SHORT() { return  std::wstring(L"Ex"); }

	static std::wstring COMMENT_START(){ return{ L"#" }; }
	static std::wstring TAG_SEP() { return{ L"=" }; }
	static std::wstring TUPLE_SEP() { return{ L";" }; }
	static std::wstring TUPLE_INNERSEP() { return{ L":" }; }

	static std::wstring DIM_SEP() { return{ L"," }; }
	static std::wstring SAME_OP() { return{ L"Identical output" }; }
	static std::wstring DIFF_OP() { return{ L"Different output" }; }
};

struct SGen_Res
{
	static std::wstring SUCCESS()				{ return L"Success"; }
	static std::wstring FAILURE()				{ return L"Failure"; }
	static std::wstring BAD_PARAM()				{ return L"Bad Parameter"; }
	static std::wstring BAD_INPUT()				{ return L"Bad Input"; }
	static std::wstring BAD_DATA()				{ return L"Bad Data"; }
	static std::wstring BAD_FILE()				{ return L"Bad File"; }
	static std::wstring BAD_SAMPLE_HINT()		{ return L"Bad Sample Hint"; }
	static std::wstring NO_SAMPLE_HINT()		{ return L"No Sample Hint"; }
	static std::wstring MISSING_FILE()			{ return L"Missing File"; }
	static std::wstring CONTAINER_NOITEM()		{ return L"No item present"; }
	static std::wstring CONTAINER_ITEM_DUP()	{ return L"Duplicate item"; }
	static std::wstring CONTAINER_EMPTY()	{ return L"Empty Container"; }
	static std::wstring MEMORY_ALLOC_FAIL() { return L"Memory Allocation Failure"; }
	static std::wstring UNIMPLEMENTED()		{ return L"Not implemented"; }
	static std::wstring UNKNOWN_FAILURE()	{ return L"Unknown Failure"; }
	static std::wstring PROB_FAIL()			{ return L"Problem failed"; }
	static std::wstring PROB_NO_SOLN()		{ return L"Problem has no solution"; }
	static std::wstring PROB_BAD_IP()		{ return L"Bad problem input"; }
	static std::wstring UNKNOWN()			{ return L"Unknown status"; }
};

struct Logger_Res
{
	static std::wstring FileName() { return  std::wstring(L"AlgoDS.log"); }
	static std::wstring FileLocation() { return  std::wstring(L"."); }
	static std::wstring MessageInfo() { return  std::wstring(L"Information"); }
	static std::wstring MessageErr() { return  std::wstring(L"Error"); }
	static std::wstring MessageWarn() { return  std::wstring(L"Warning"); }
	static std::wstring MessageDebug() { return  std::wstring(L"Debug"); }
	static std::wstring TimeFormat() { return  std::wstring(L"%Y%m%d %H:%M:%S"); }
	static std::wstring LogStart() { return  std::wstring(L"LOG START"); }
	static std::wstring LogEnd() { return  std::wstring(L"LOG END"); }
	static std::wstring LogSeparator() { return  std::wstring(L"=================================="); }
	static std::wstring SubStrDelim() { return  std::wstring(L" : "); }
};

struct Problem_Res
{
	static std::wstring E_PROB_UNRECOGNIZED() { return{ L"Problem Not recognized" }; }
	static std::wstring E_PROB_INVALID_IP() { return{ L"Invalid Problem Input" }; }
};

struct TestCaseSettings_Res
{
	static std::wstring VERBOSE(){ return{ L"Verbose" }; }
	static std::wstring PRINT_PALINDROME(){ return{ L"PrintPalindrome" }; }
	static std::wstring BESTCASE_SIZE(){ return{ L"BestcaseSize" }; }
	static std::wstring SUMMARY_OUTPUT(){ return{ L"SummaryOutput" }; }
	static std::wstring DETAILED_OUTPUT(){ return{ L"DetailedOutput" }; }
	static std::wstring REPEATOP_COUNT(){ return{ L"RepeatOp" }; }
};

struct OperandType_Res
{
	const std::wstring INTEGER_TAG	= std::wstring(L"Integer");
	const std::wstring CHAR_TAG		= std::wstring(L"Character");
	const std::wstring STRING_TAG	= std::wstring(L"String");
	const std::wstring EXIT_TAG		= std::wstring(L"Exit");
	const std::wstring UNKNOWN_TAG	= std::wstring(L"Unknown");
};
	

struct IpFile_Res
{
	static std::wstring INPUT_FILE_NAME() { return {L"AlgoDSInput.ini"}; }

	static std::wstring INPUT_BLOCK_START() { return {L"InputBlock START"}; }
	static std::wstring INPUT_BLOCK_END() { return {L"InputBlock END"}; }
	static std::wstring OUTPUT_BLOCK_START() { return {L"OutputBlock START"}; }
	static std::wstring OUTPUT_BLOCK_END() { return {L"OutputBlock END"}; }

	static std::wstring MAJOR_OP_TAG() { return {L"MajorOp"}; }
	static std::wstring SUB_OP_TAG() { return {L"SubOp"}; }

	static std::wstring OPERAND_TYPE_TAG() { return {L"OperandType"}; }
	static std::wstring OPERAND_DELIMITER_TAG() { return {L"OperandDelimiter"}; }
	static std::wstring OPERAND_DATA_TAG() { return{ L"OperandData" }; }

	static std::wstring OUTPUT_BLOCKID_TAG() { return {L"OutputBlockId"}; }
	static std::wstring OUTPUT_TARGET_TAG() { return {L"Target"}; }
	static std::wstring OUTPUT_FILEPATH_TAG() { return {L"Filepath"}; }

	static std::wstring USE_SAMPLE_TAG() { return {L"UseSample"}; }
	static std::wstring SAMPLE_DEFAULT_TAG() { return {L"SampleDefault"}; }
	static std::wstring SAMPLE_HINT_TAG() { return {L"SampleHint"}; }

	static std::wstring SETTINGS_TAG(){ return {L"Settings"}; }
};

struct IpFile_Err
{
	static std::wstring FileOpenError(){return std::wstring(L"Failed to open input file");}
	static std::wstring MissingBlockIds(){ return std::wstring(L"Unable to retrieve Block Ids"); }
	static std::wstring WrongBlockId(){ return std::wstring(L"Invalid Block id"); }
	static std::wstring InvalidTagName(){ return std::wstring(L"Invalid tag name"); }
	static std::wstring InvalidTagData(){ return std::wstring(L"Invalid tag data"); }
	static std::wstring InvalidLine(){ return std::wstring(L"Incorrectly formatted line"); }
	static std::wstring EmptyBlock(){ return std::wstring(L"No Block Data Found"); }
	static std::wstring EmptySampleHint(){ return std::wstring(L"No Sample Hint Found"); }
	static std::wstring MissingEndBlock(){ return std::wstring(L"No End block"); }
	static std::wstring ExtraEndBlock(){ return std::wstring(L"End block has no start block"); }
	static std::wstring DupBlockId(){ return std::wstring(L"Duplicate Block Id"); }
	static std::wstring ZeroBlockId(){ return std::wstring(L"Block Id <0> not allowed"); }
	static std::wstring OPERAND_BAD_FORMAT(){ return std::wstring(L"Badly formatted operand"); }
	static std::wstring OPERAND_WRONG_NUMBERS(){ return std::wstring(L"Incorrect number of operands"); }
};

struct OPFile_Res
{
	static std::wstring FileName() { return (L"AlgoDSOutput.txt"); }
	static std::wstring Console() { return (L"Console"); }
	static std::wstring File() { return (L"File"); }
	static std::wstring ConsoleFile() { return (L"ConsoleFile"); }
};

struct SubOpOperands_Res
{
	static std::wstring OPERAND_SEP(){ return{ L",;" }; };
};

struct SData_Res
{
	static std::wstring HintTotal(){ return std::wstring(L"Total"); }
	static std::wstring HintMaxStrLength(){ return  std::wstring(L"MaxStrLength"); }
	static std::wstring HintString(){ return  std::wstring(L"Text"); }
	static std::wstring HintCharLow(){ return std::wstring(L"Low"); }
	static std::wstring HintCharHigh(){ return  std::wstring(L"High"); }
	static std::wstring HintIntLow(){	return std::wstring(L"Low"); }
	static std::wstring HintIntHigh(){ return  std::wstring(L"High"); }
	static std::wstring HintUIntLow(){ return std::wstring(L"Low"); }
	static std::wstring HintUIntHigh(){ return  std::wstring(L"High"); }
	static std::wstring HintSeed(){ return  std::wstring(L"Seed"); }
	static std::wstring HintMaxValue(){ return  std::wstring(L"MaxValue"); }
	static std::wstring HintMinValue(){ return  std::wstring(L"MinValue"); }
	static std::wstring HintMatrixRows(){ return  std::wstring(L"MatrixRows"); }
	static std::wstring HintMatrixCols(){ return  std::wstring(L"MatrixCols"); }
	static std::wstring HintMatrix1(){ return  std::wstring(L"Matrix1"); }
	static std::wstring HintMatrix2(){ return  std::wstring(L"Matrix2"); }
	static std::wstring HintStrassenBaseSize() { return std::wstring(L"StrassenBaseSize"); }

	static std::wstring HintUnk(){ return  std::wstring(L"Unknown"); }
	static std::wstring HintInnerSep() { return std::wstring(L":"); }

	static std::wstring ErrMalformedHint() { return std::wstring(L"Not a proper Sample hint"); }
	static std::wstring ErrBadHintType() { return std::wstring(L"Wrong sample hint type"); }
	static std::wstring ErrBadHintData() { return std::wstring(L"Wrong sample hint data"); }
	static std::wstring ErrHintNotHinted() { return std::wstring(L"Valid hint but not in includehint"); }
};

struct Menu_Res
{
	static std::wstring Rpt() { return std::wstring(L"Repeat"); }
	static std::wstring Ops() { return std::wstring(L"Operations"); }
	static std::wstring SOps() { return std::wstring(L"Sub Operations"); }
	static std::wstring UIp() { return std::wstring(L"Enter Choice"); }
	static std::wstring SD() { return std::wstring(L"Sample Data"); }
	static std::wstring SAM() { return std::wstring(L"Samples"); }
	static std::wstring TOT() { return std::wstring(L"Total"); }
	static std::wstring MAX() { return std::wstring(L"Maximum"); }
	static std::wstring MIN() { return std::wstring(L"Minimum"); }
	static std::wstring VAL() { return std::wstring(L"Value"); }
	static std::wstring SL() { return std::wstring(L"String Length"); }
	static std::wstring EN() { return std::wstring(L"Enter"); }
	static std::wstring DEF() { return std::wstring(L"Default"); }
	static std::wstring SEP() { return std::wstring(L"Separated by"); }
	static std::wstring BI() { return std::wstring(L"Bad Input"); }
	static std::wstring UL() { return std::wstring(L"------------------"); }
	static std::wstring YN() { return std::wstring(L"(Y/N)"); }
	static std::wstring PI() { return std::wstring(L"(+ve Int)"); }
	static std::wstring INT() { return std::wstring(L"(Int)"); }
	static std::wstring CH() { return std::wstring(L"(Char)"); }
	static std::wstring STR() { return std::wstring(L"(String)"); }
	static std::wstring SS() { return std::wstring(L" "); }
	static std::wstring PE() { return std::wstring(L": "); }
	static std::wstring HY() { return std::wstring(L"-"); }
	static std::wstring QM() { return std::wstring(L"?"); }
	static std::wstring SBR() { return std::wstring(L"("); }
	static std::wstring EBR() { return std::wstring(L")"); }
	static std::wstring UNK() { return std::wstring(L"Unknown"); }
	static std::wstring NumTextSep() {return std::wstring(L"."); }
	static std::wstring DefSep() { return std::wstring(L","); }
	static std::wstring InputBreak() { return std::wstring(L"\n"); }
};