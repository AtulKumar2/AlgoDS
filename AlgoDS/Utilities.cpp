#include "stdafx.h"
#include "Utilities.h"

///////////////////////////////////////////////////////////////////////////////
///  Left shift overload specialization
std::wostream&  
operator<<(std::wostream& os, const std::vector<int>& input)
{
	std::wstring output;
	for (size_t i = 0; i < input.size() - 1; i++){
		output += (std::to_wstring(input.at(i)) + L",");
	}
	output += std::to_wstring(input.at(input.size() - 1));
	return (os << output);
}
std::wostream&  
operator<<(std::wostream& os, const std::vector<unsigned int>& input)
{
	std::wstring output;
	for (size_t i = 0; i < input.size() - 1; i++){
		output += (std::to_wstring(input.at(i)) + L",");
	}
	output += std::to_wstring(input.at(input.size() - 1));
	return (os << output);
}

std::wostream&  
operator<<(std::wostream& os, const std::vector<wchar_t>& input)
{
	std::wstring output;
	for (size_t i = 0; i < input.size() - 1; i++){
		output.push_back(input.at(i));
		output += L",";
	}
	output.push_back(input.at(input.size() - 1));
	return (os << output);
}
std::wostream&  
operator<<(std::wostream& os, const std::vector<std::wstring>& input)
{
	std::wstring output;
	for (size_t i = 0; i < input.size() - 1; i++){
		output += (input.at(i) + L",");
	}
	output += input.at(input.size() - 1);
	return (os << output);
}

///////////////////////////////////////////////////////////////////////////////
/// Trimming Specialization
std::wstring trim(std::wstring const &s)
{
	auto  wsfront = std::find_if_not(s.begin(), s.end(), [](int c){return std::isspace(c); });
	return std::wstring(wsfront,
		std::find_if_not(s.rbegin(), std::wstring::const_reverse_iterator(wsfront),
		[](int c){return std::isspace(c); }).base());
}
std::wstring ltrim(std::wstring &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}
std::wstring rtrim(std::wstring &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

std::string trim(std::string const &s)
{
	auto  wsfront = std::find_if_not(s.begin(), s.end(), [](int c){return std::isspace(c); });
	return std::string(wsfront,
		std::find_if_not(s.rbegin(), std::string::const_reverse_iterator(wsfront),
		[](int c){return std::isspace(c); }).base());
}
std::string ltrim(std::string &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}
std::string rtrim(std::string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

///////////////////////////////////////////////////////////////////////////////
/// Compare function specializations
int compare(const std::wstring &s1, const std::wstring &s2, bool casesensitive)
{
	using namespace std;
	std::wstring temp_s1(s1), temp_s2(s2);
	if (!casesensitive){
		transform(temp_s1.begin(), temp_s1.end(), temp_s1.begin(), ::tolower);
		transform(temp_s2.begin(), temp_s2.end(), temp_s2.begin(), ::tolower);
	}
	return temp_s1.compare(temp_s2);
}
int trimandcompare(const std::wstring &s1, const std::wstring &s2, bool casesensitive)
{
	return compare<std::wstring>(trim(s1), trim(s2), casesensitive);
}
int compare(const std::string &s1, const std::string &s2, bool casesensitive)
{
	using namespace std;
	std::string temp_s1(s1), temp_s2(s2);
	if (!casesensitive){
		transform(temp_s1.begin(), temp_s1.end(), temp_s1.begin(), ::tolower);
		transform(temp_s2.begin(), temp_s2.end(), temp_s2.begin(), ::tolower);
	}
	return temp_s1.compare(temp_s2);
}
int trimandcompare(const std::string &s1, const std::string &s2, bool casesensitive)
{
	return compare<std::string>(trim(s1), trim(s2), casesensitive);
}

///////////////////////////////////////////////////////////////////////////////
/// RandomData Implementation
void 
RandomData::CreateGenerator(std::vector<int> seed_init, 
		std::initializer_list<int> seed_init_seq)
{
	using namespace std::chrono;
	using namespace std;

	if (seed_init.size() != 0){
		seed_seq seedseq(seed_init.begin(), seed_init.end());
		generator = default_random_engine(seedseq);
	}
	else if (seed_init_seq.size() != 0){
		seed_seq seedseq(seed_init_seq);
		generator = default_random_engine(seedseq);
	}
	else{
		//static system_clock::rep savedseed, currseed;
		static unsigned int savedseed, currseed;
		do{
			currseed = system_clock::now().time_since_epoch().count() % 
						std::numeric_limits<unsigned int>::max() ;
		} while (currseed == savedseed);
		savedseed = currseed;
		generator = default_random_engine(currseed);
	}
}

void 
RandomData::GetRandomValues(std::vector<int>& randomvalues, size_t total,
		int lowest, int highest)
{
	std::uniform_int_distribution<int> distribution(lowest, highest);
	auto dice = std::bind(distribution, generator);
	for (size_t i = 0; i < total; i++){
		randomvalues.push_back(dice());
	}
}
void 
RandomData::GetRandomValues(std::vector<unsigned int>& randomvalues, size_t total,
	unsigned int lowest, unsigned int highest)
{
	std::uniform_int_distribution<unsigned int> distribution(lowest, highest);
	auto dice = std::bind(distribution, generator);
	for (size_t i = 0; i < total; i++){
		randomvalues.push_back(dice());
	}
}

int 
RandomData::GetARandomNum(int lowest, int highest)
{
	static int lastrandomdelivered;
	int curr_random;
	do{
		CreateGenerator();
		std::uniform_int_distribution<int> distribution(lowest, highest);
		curr_random = distribution(generator);
	} while (lastrandomdelivered == curr_random);
	return curr_random;
}

size_t
RandomData::Generate(std::vector<int>& list, size_t total,
int lowest, int highest)
{
	CreateGenerator();
	GetRandomValues(list, total, lowest, highest);
	return list.size();
}
size_t 
RandomData::Generate(std::vector<unsigned int>& list, size_t total,
	unsigned int lowest, unsigned int highest)
{
	CreateGenerator();
	GetRandomValues(list, total, lowest, highest);
	return list.size();
}

std::vector<int>
RandomData::Generate(size_t total, int lowest, int highest)
{
	std::vector<int> list;
	Generate(list, total, lowest, highest);
	return list;
}
std::vector<unsigned int>
RandomData::Generate(size_t total, unsigned int lowest, unsigned int highest)
{
	std::vector<unsigned int> list;
	Generate(list, total, lowest, highest);
	return list;
}
size_t 
RandomData::Generate(std::vector<wchar_t>& list, size_t total)
{ //'a' 97 to 'z' 122 'A' to 'Z' 65 to 90
	std::vector<int> intlist;
	GetRandomValues(intlist, total, 65, 122);

	for (const auto& it : intlist)
	{
		if (it <= 90)
			list.push_back('A' + (it - 65));
		else if (it <= 93 && it > 90)
			list.push_back('A' + (it - 90));
		else if (it > 93 && it < 97)
			list.push_back('a' + (it - 93));
		else
			list.push_back('a' + (it - 97));
	}
	return list.size();
}
size_t 
RandomData::Generate(std::vector<std::wstring> &list,
		size_t total, int maxlength)
{
	std::vector<int> itemsizes, itemseeders;
	std::vector<wchar_t> itemchars;

	Generate(itemsizes, total, 1, maxlength);
	for (const auto& size : itemsizes)
	{
		GetRandomValues(itemseeders, size);
		CreateGenerator(itemseeders);
		Generate(itemchars, size);
		std::wstring temp = L"";
		for (const auto& itemchar : itemchars){
			temp += itemchar;
		}
		itemchars.clear();
		itemseeders.clear();
		list.push_back(temp);
	}
	return 0;
}
std::wstring 
RandomData::Generate(size_t length, wchar_t low, wchar_t high)
{
	std::vector<unsigned int> intlist;
	CreateGenerator();
	GetRandomValues(intlist, length, 
		static_cast<unsigned int>(low), static_cast<unsigned int>(high));
	std::wstring ret{ L"" };

	for (const auto& it : intlist){
		ret += static_cast<wchar_t>(it);
	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
void ClearWcin()
{
	std::wcin.clear();
	std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), L'\n');
}

////////////////////////////////////////////////////////////////////////////////
namespace estd
{

/// Conversion ////////////////////////////////////////////////////////////
std::wstring tolower(std::wstring val){
	std::transform(val.begin(), val.end(), val.begin(), ::towlower);
	return val;
}
std::wstring toupper(std::wstring val){
	std::transform(val.begin(), val.end(), val.begin(), ::towupper);
	return val;
}
std::string tolower(std::string val){
	std::transform(val.begin(), val.end(), val.begin(), ::tolower);
	return val;
}
std::string toupper(std::string val){
	std::transform(val.begin(), val.end(), val.begin(), ::toupper);
	return val;
}

WSTR_VEC tolower(WSTR_VEC& val){
	for (auto &it : val)
		std::transform(it.begin(), it.end(), it.begin(), ::towlower);
	return val;
}
WSTR_VEC toupper(WSTR_VEC& val){
	for (auto &it : val)
		std::transform(it.begin(), it.end(), it.begin(), ::towupper);
	return val;
}

/// any_of //////////////////////////////////////////////////////////////////
bool any_of_ci(const std::vector<std::wstring>& c, std::wstring val)
{
	if (c.size() == 0 || val.size() == 0)
		return false;
	val = tolower(val);
	return std::any_of(c.begin(), c.end(),
		[&, val](const std::wstring& src){return (tolower(src) == val); });
}
bool any_of(const std::vector<std::wstring>& c, std::wstring const& val){
	if (c.size() == 0 || val.size() == 0)
		return false;
	return std::any_of(c.begin(), c.end(),
		[&, val](const std::wstring& src){return (src == val); });
}

/// Custom output /////////////////////////////////////////////////////////
std::wstring to_wstring(bool b)
{
	return (b ? L"true" : L"false");
}
std::wstring
to_wstring(std::string const& s)
{
	std::wstring wsTmp(s.begin(), s.end());
	return (wsTmp);
}
std::string
to_string(std::wstring const& ws)
{
	std::string sTmp(ws.begin(), ws.end());
	return sTmp;
}

/// numeric conversion ////////////////////////////////////////////////////
unsigned stou(std::wstring const & str, size_t * idx, int base) {
	unsigned long result = std::stoul(str, idx, base);
	if (result > std::numeric_limits<unsigned>::max()) {
		throw std::out_of_range("stou");
	}
	return result;
}

unsigned stou(std::string const & str, size_t * idx, int base) {
	unsigned long result = std::stoul(str, idx, base);
	if (result > std::numeric_limits<unsigned>::max()) {
		throw std::out_of_range("stou");
	}
	return result;
}

}

////////////////////////////////////////////////////////////////////////////////
/// Namespace utils  ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
namespace Utils
{
////////////////////////////////////////////////////////////////////////////
// Time functions
// https://kjellkod.wordpress.com/2013/01/22/exploring-c11-part-2-localtime-and-time-again/
std::time_t 
systemtime_now()
{
	using namespace std::chrono;
	time_point<system_clock> system_now = system_clock::now();
	return system_clock::to_time_t(system_now);
}

std::wstring 
put_time(const std::tm* date_time, const wchar_t* c_time_format)
{
	std::wostringstream woss;
	woss << std::put_time(const_cast<std::tm*>(date_time), c_time_format);
	return woss.str();
}
std::wstring
date_text(time_t timer)
{
	std::tm tm_snapshot;
	localtime_s(&tm_snapshot, &timer);
	const wchar_t* format = L"%Y%m%d %H:%M:%S";
	return put_time(&tm_snapshot, format);
}
////////////////////////////////////////////////////////////////////////////
std::string 
ltrim(std::string &s) 
{
	s.erase(s.begin(), std::find_if(
		s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}
std::string 
rtrim(std::string &s) 
{
	s.erase(std::find_if(
		s.rbegin(), s.rend(), 
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}
/////////////////////////////////
std::wstring 
ltrim(std::wstring &s) 
{
	s.erase(s.begin(), std::find_if(
		s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}
std::wstring 
rtrim(std::wstring &s) 
{
	s.erase(std::find_if(
		s.rbegin(), s.rend(), 
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

////////////////////////////////////////////////////////////////////////////
// string <-> wstring conversions
int 
StringToWString(const std::string &s, std::wstring &ws)
{
	std::wstring wsTmp(s.begin(), s.end());
	ws = wsTmp;
	return 0;
}
int 
WStringToString(const std::wstring &ws, std::string &s)
{
	std::string sTmp(ws.begin(), ws.end());
	s = sTmp;
	return 0;
}

////////////////////////////////////////////////////////////////////////////
// Compare functions for use in qsort
int CmpFuncInt(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
}
int CmpFuncChar(const void *a, const void *b)
{
	return (*(char *)a - *(char *)b);
}
int CmpFuncStr(const void *a, const void *b)
{
	return (strcmp(*(char **)a, *(char **)b));
}

////////////////////////////////////////////////////////////////////////////
// Compare two equal length lists
bool CmpEqLenList(int *List1, int* List2, size_t ListLength)
{
	for (size_t i = 0; i < ListLength; i++){
		if (List1[i] != List2[i]) return false;
	}
	return true;
}
bool CmpEqLenList(char **List1, char** List2, size_t ListLength)
{
	for (size_t i = 0; i < ListLength; i++){
		if (0 != strcmp(List1[i], List2[i])) return false;
	}
	return true;
}
	
////////////////////////////////////////////////////////////////////////////
// Print Lists
void printlist(int *list, unsigned int low, unsigned int high)
{
	if (low > high || !list)
		printf("Zero length List\n");
	else{
		for (unsigned int i = low; i < high; i++){
			printf("%d,", list[i]);
		}
		printf("%d\n", list[high]);
	}
}
void printlist(char *list, unsigned int low, unsigned int high)
{
	if (low > high || !list)
		printf("Zero length List\n");
	else{
		for (unsigned int i = low; i < high; i++){
			printf("%c,", list[i]);
		}
		printf("%c\n", list[high]);
	}
}
void printlist(char **list, unsigned int low, unsigned int high)
{
	if (low > high || !list || !*list)
		printf("Zero length List\n");
	else{
		for (unsigned int i = low; i < high; i++){
			printf("%s,", list[i]);
		}
		printf("%s\n", list[high]);
	}
}

/// Swap values ////////////////////////////////////////////////////////////
void 
swapvalues(char * first, char * second)
{
	char temp = *second;
	*first = *second;
	*second = temp;
}

void 
swapvalues(int * first, int * second)
{
	int temp = *second;
	*first = *second;
	*second = temp;
}

/* 
Allocate space equal to the length of value and assign it to target.
Deallocation should be handled by caller.
*/
STATUS_UTILITY 
AllocateAndAssignCString(char **target, char *value, unsigned int maxsize)
{
	STATUS_UTILITY retVal = SUTIL_FAILURE;
	if(!value || !target || !(*target) || 0 == maxsize)
		return SUTIL_BAD_INPUT;
	unsigned int length = strnlen_s(value, maxsize);
	if (!(*target = (char *)malloc(sizeof(char) * (length + 1))))
		return SUTIL_MEMORY_ALLOC_FAIL;
	if (-1 != sprintf_s(*target, length + 1, "%s", value))
		retVal = SUTIL_SUCCESS;
		
	return retVal;
}

/*
Array must have been allocated using malloc
*/
STATUS_UTILITY 
DeallocateCStringArray(char *target[], unsigned int length)
{
	STATUS_UTILITY retVal = SUTIL_FAILURE;
	//assert(target && 0 != length);
	if (target && 0 != length)
	{
		for (unsigned int i = 0; i < length; i++)
		{
			FREE(target[i]);
		}
	}
	return (retVal = SUTIL_SUCCESS);
}
}