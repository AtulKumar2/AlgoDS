// AlgoDS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Utilities.h"
#include "CommonDefs.h"
#include "Resources.h"
#include "TestHarness.h"
#include <fstream>
#include <memory>

///////////////////////////////////////////////////////////////////////////////
/// Forward declarations
std::vector<RunMap> GetStartingModes();
std::unique_ptr<TestBase> ModuleEntryPoints(MAJOR_OPS);
void DriveConsole(ModuleData);
void DriveCL(ModuleData);
void DriveFI(ModuleData);
class RunMap;

///////////////////////////////////////////////////////////////////////////////
/// Program Entry
int _tmain(int argc, _TCHAR* argv[])
{
	using namespace std;
	Logger().Start();

	ModuleData moduleData;
	WSTRING_VEC args;

	for (int i = 0; i < argc; i++)
	{
#if _UNICODE
		wstring temp(argv[i]);
#else
		wstring temp(new string(argv[i]));
#endif
		args.insert(args.begin() + i, temp);
		transform(args[i].begin(), args[i].end(), args[i].begin(), ::tolower);
	}

	moduleData.args = args;
	if (1 == argc){ // Ask user for input. Let it be console based program
		moduleData.runtype = RUNTYPE::ConsoleOps;
		DriveConsole(moduleData);
	}

	if (1 < argc)
	{
		RUNTYPE StartMode = RUNTYPE::END;
		wstring::size_type index = args[1].find_first_not_of(ARGSTART_CHAR, 0);
		if ((args[1][0] != ARGSTART_CHAR) || wstring::npos == index){
			wcout << L"Wrong arguments" << endl;
		}
		else{
			for each (auto it in GetStartingModes()){
				if (it.matchnames(args[1].substr(index))){
					StartMode = it.gettype();
					break;
				}
			}
		}

		moduleData.runtype = StartMode;
		switch (StartMode)
		{
		case RUNTYPE::ConsoleOps: DriveConsole(moduleData); break;
		case RUNTYPE::CommandLine: DriveCL(moduleData); break;
		case RUNTYPE::FileInput:
			Logger::SetMedium(Logger::OPMEDIUM::File);
			DriveFI(moduleData); break;
		default:
			wcout << L"Unknown operation" << endl; break;
		}
	}
	Logger().Close();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// Console based operations ///
void DriveConsole(ModuleData moduleData)
{
	using std::wcin;
	using std::wcout;
	
	// TODO: Check if we even have console environment. 
	// TODO: Skip otherwise

	wcout << L"Welcome to esoteric program. What should I do next?\n";
	wchar_t dowerepeat = 'y';
	do
	{
		MajorOps majorops;
		std::vector<MajorOpsMenuItem> MajorOpsMenuItems = 
			majorops.GetMenuItems(std::vector <MAJOR_OPS> {});
		
		for (auto &MenuItem : MajorOpsMenuItems){
			if (majorops.IsValidId(MenuItem.EnumType)){
				std::wcout << MenuItem.Id << ". " << MenuItem.Text << std::endl;
			}
		}
		wcout << L"Enter your choice of esoteric operations : ";
		unsigned int choice;
		wcin >> choice;
		moduleData.majorops = majorops.GetInvalidId();
		for (auto &MenuItem : MajorOpsMenuItems){
			if (MenuItem.Id == choice){
				moduleData.majorops = MenuItem.EnumType;
				break;
			}
		}
		if (MAJOR_OPS::EXIT == moduleData.majorops ||
			!majorops.IsValidId(moduleData.majorops))
			break;

		auto entry_points = ModuleEntryPoints(moduleData.majorops);
		entry_points->Run(moduleData);
		/*if (MAJOR_OPS::CLRS_CH4 == moduleData.majorops){
			Run_CLRS4(moduleData);
		}
		if (MAJOR_OPS::PALINDROME == moduleData.majorops){
			Run_Palindrome(moduleData);
		}
		else if (nullptr != majorops.GetOps(moduleData.majorops))
			majorops.GetOps(moduleData.majorops)(moduleData.args);*/

		wcout << std::endl << "RERUN PROGRAM (Y/N) : ";
		wcin >> dowerepeat;
	} while ((L'y' == dowerepeat) || (L'Y' == dowerepeat));
}

// Read input from files and proceed
// Input related to one module can only be given.
// Multiple inputs are possible
// OP = Operation Name
// 

void DriveFI(ModuleData moduleData)
{
	using namespace std;

	STATUS_GEN	status{ STATUS_GEN::SUCCESS };
	wstring			LogHint{ L"DriveFI" };
	wstring			SSD{ Logger_Res::SubStrDelim() };

	InputFileHandler ipfilehandler;
	ipfilehandler.SetLogLevel(InputFileHandler::LOG_LEVEL::ALL);

	if (!ipfilehandler.IsAvailable()){
		WriteLog(LogHint + SSD + to_wstring(STATUS_GEN::MISSING_FILE), LogType::ERR);
		return;
	}
	UINT_VEC blockids;
	if ((STATUS_GEN::SUCCESS != (status =
		ipfilehandler.BlockIds(blockids, InputFileHandler::BLOCK_TYPE::INPUT)))
		|| blockids.empty()){
		WriteLog(LogHint + Logger_Res::SubStrDelim() + IpFile_Err::MissingBlockIds(), LogType::ERR);
		return;
	}

	for (auto const& blockid : blockids)
	{
		wstring logmessage = L"Block " + to_wstring(blockid) + L" : ";
		moduleData.majorops = ipfilehandler.MajorOp(blockid);
		moduleData.InputBlockId = blockid;
		
		MajorOps majorops;
			
		if (!majorops.IsValidId(moduleData.majorops)){
			WriteLog(logmessage + Common_Res::INVALID_ID(), LogType::ERR);
		}

		WriteLog(logmessage + L"Execution start", LogType::INFO);

		auto entry_points = ModuleEntryPoints(moduleData.majorops);
		entry_points->Run(moduleData);
		//if (MAJOR_OPS::CLRS_CH4 == moduleData.majorops){
		//	Run_CLRS4(moduleData);
		//}
		//if (MAJOR_OPS::PALINDROME == moduleData.majorops){
		//	Run_Palindrome(moduleData);
		//}
		//else if (nullptr != majorops.GetOps(moduleData.majorops))
		//	majorops.GetOps(moduleData.majorops)(moduleData.args);
		WriteLog(logmessage + +L"Execution end", LogType::INFO);
	}
	return;
}

/// Command Line operations ///
void DriveCL(ModuleData moduleData)
{
	std::wcout << SGen_Res().UNIMPLEMENTED << std::endl;
	return;
}

///////////////////////////////////////////////////////////////////////////////
std::vector<RunMap> GetStartingModes()
{
	return 
	(
		std::vector<RunMap>
		{
			{ RUNTYPE::ConsoleOps, L"consoleops", { L"co" } },
			{ RUNTYPE::FileInput, L"fileinput", { L"fi" } },
			{ RUNTYPE::CommandLine, L"commandline", { L"cl" } }
		}
	);
}

std::unique_ptr<TestBase>
ModuleEntryPoints(MAJOR_OPS major_ops)
{
	using namespace std;

	switch (major_ops)
	{
		case MAJOR_OPS::SORT: return unique_ptr<Sorting_Test>( new Sorting_Test() );
		case MAJOR_OPS::SEARCH: return unique_ptr<Search_Test>( new Search_Test() ); 
		case MAJOR_OPS::BINOPS: return unique_ptr<BinOps_Test>( new BinOps_Test() );
		case MAJOR_OPS::LINKEDLIST: return unique_ptr<LinkedList_Test>( new LinkedList_Test() );
		case MAJOR_OPS::STRINGS: return unique_ptr<Strings_Test>( new Strings_Test() );
		case MAJOR_OPS::TWISTERS: return unique_ptr<Twisters_Test>( new Twisters_Test() );
		case MAJOR_OPS::BINTREE: return unique_ptr<BinTree_Test>( new BinTree_Test() );
		case MAJOR_OPS::CLRS_CH4: return unique_ptr<CLRS4_Test>( new CLRS4_Test() );
		case MAJOR_OPS::PALINDROME: return unique_ptr<Palindrome_Test>( new Palindrome_Test() );
		default:
			throw std::invalid_argument(__FUNCTION__ + string("Invalid major_ops"));
	};
}

///////////////////////////////////////////////////////////////////////////////
std::wstring
to_wstring(TRACE_STATE tracestate)
{
	switch (tracestate)
	{
	case TRACE_STATE::TRACE_NO: return{ L"TRACE_NO" }; break;
	case TRACE_STATE::TRACE_BASIC: return{ L"TRACE_BASIC" }; break;
	case TRACE_STATE::TRACE_DETAILED: return{ L"TRACE_DETAILED" }; break;
	default: return{ L"TRACE_UNKNOWN" }; break;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// Log Writer
// https://kjellkod.wordpress.com/2013/01/22/exploring-c11-part-2-localtime-and-time-again/
/// Static Initialization ///
bool Logger::LogStarted = false;
Logger::OPMEDIUM Logger::OpMed = Logger::OPMEDIUM::Console;

Logger::Logger()
	:Logger(OPMEDIUM::Console)
{}
Logger::Logger(OPMEDIUM opmed)
	: m_OpMed(opmed)
{
	m_LogFile = FileName();
	m_LogPath = FileLocation();
}
Logger::Logger(std::wstring const& LogFile, std::wstring const& LogPath, OPMEDIUM opmed)
	: m_LogFile(LogFile), m_LogPath(LogPath), m_OpMed(opmed)
{}
Logger::~Logger()
{}

Logger::Logger(const Logger& src) {
	*this = src;
}
Logger& Logger::operator=(const Logger& src)
{
	if (this != &src){
		m_LogFile = src.m_LogFile;
		m_LogPath = src.m_LogPath;
		m_OpMed = src.m_OpMed;
	}
	return *this;
}

void
Logger::Start()
{
	if (!Logger::LogStarted)
	{
		auto message = LogSeparator() + L"\n" + LogStart() +
			SS + date_text(utils::systemtime_now()) + L"\n" + LogSeparator();
		writetofile(message);
		Logger::LogStarted = true;
	}
}
void
Logger::Close()
{
	Start();
	auto message = LogSeparator() + L"\n" + LogEnd()
		+ SS + date_text(utils::systemtime_now()) + L"\n" + LogSeparator();
	writetofile(message);
}
void
Logger::Write(std::wstring const & message, TYPE type, time_t timer)
{
	Start();
	writetofile(date_text(timer) + SS + L"[" + type_text(type) + L"]" + SS + message);
	return;
}
void
Logger::SetMedium(OPMEDIUM opmed) { OpMed = opmed; }

std::wstring
Logger::date_text(time_t timer)
{
	std::tm tm_snapshot;
	localtime_s(&tm_snapshot, &timer);
	const wchar_t* format = L"%Y%m%d %H:%M:%S";
	return utils::put_time(&tm_snapshot, format);
}
void
Logger::writetofile(std::wstring const & message)
{
	using namespace std;

	wofstream logfile;
	logfile.open(m_LogFile, wofstream::out | wofstream::ate | wofstream::app);

	if (!logfile){
		wcout << message << std::endl;
	}
	else{
		logfile << message << std::endl;
		logfile.close();
	}
	return;
}

std::wstring
Logger::type_text(TYPE type)
{
	switch (type){
	case TYPE::INFO: return MessageInfo(); break;
	case TYPE::ERR: return MessageErr(); break;
	case TYPE::WARN: return MessageWarn(); break;
	case TYPE::DEBUG: return MessageDebug(); break;
	default: return{ L"" }; break;
	}
}

void WriteLog(const std::wstring & message, LogType type, time_t timer){
	Logger().Write(message, type, timer);
}
void SetLogMedium(Logger::OPMEDIUM opmed){
	Logger::SetMedium(opmed);
}

///////////////////////////////////////////////////////////////////////////////
/// Status messages
std::wstring
StatusGen::str(TYPE sgen)
{
	return type_wstring(sgen);
};

std::wstring
StatusGen::type_wstring(TYPE sgen)
{
	switch (sgen)
	{
	case TYPE::SUCCESS: return SGen_Res::SUCCESS();
	case TYPE::FAILURE: return SGen_Res::FAILURE();
	case TYPE::PROB_FAIL: return SGen_Res::PROB_FAIL();
	case TYPE::PROB_NO_SOLN: return SGen_Res::PROB_NO_SOLN();
	case TYPE::PROB_BAD_IP: return SGen_Res::PROB_BAD_IP();
	case TYPE::BAD_PARAM: return SGen_Res::BAD_PARAM();
	case TYPE::BAD_INPUT: return SGen_Res::BAD_INPUT();
	case TYPE::BAD_DATA: return SGen_Res::BAD_DATA();
	case TYPE::MISSING_FILE: return SGen_Res::MISSING_FILE();
	case TYPE::BAD_FILE: return SGen_Res::BAD_FILE();
	case TYPE::BAD_SAMPLE_HINT: return SGen_Res::BAD_SAMPLE_HINT();
	case TYPE::NO_SAMPLE_HINT: return SGen_Res::NO_SAMPLE_HINT();
	case TYPE::CONTAINER_NOITEM: return SGen_Res::CONTAINER_NOITEM();
	case TYPE::CONTAINER_ITEM_DUP: return SGen_Res::CONTAINER_ITEM_DUP();
	case TYPE::CONTAINER_EMPTY: return SGen_Res::CONTAINER_EMPTY();
	case TYPE::MEMORY_ALLOC_FAIL: return SGen_Res::MEMORY_ALLOC_FAIL();
	case TYPE::UNIMPLEMENTED: return SGen_Res::UNIMPLEMENTED();
	case TYPE::UNKNOWN_FAILURE: return SGen_Res::UNKNOWN_FAILURE();
	default:
		break;
	}
	return SGen_Res::UNKNOWN();
}
std::wstring
to_wstring(STATUS_GEN sgen)
{
	return StatusGen().type_wstring(sgen);
}
