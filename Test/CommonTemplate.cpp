#include "stdafx.h"
#include "Common.h"
#include "Header.h"
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <locale>
#include <iterator>
#include <chrono>

///////////////////////////////////////////////////////////////////////////////
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
operator<<(std::wostream& os, std::vector<wchar_t> const& input)
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
operator<<(std::wostream& os, std::vector<std::wstring> const& input)
{
	std::wstring output;
	for (size_t i = 0; i < input.size() - 1; i++){
		output += (input.at(i) + L",");
	}
	output += input.at(input.size() - 1);
	return (os << output);
}

/////////////////////////////////////////////////////////////////////////////////
//enum class TEMP_TYPE
//{
//	TYPE1,
//	TYPE2,
//	TYPE3
//};
//
//class MajorOpsbase
//{
//public:
//	virtual void test() = 0;
//};
//class MajorOps1 :public MajorOpsbase
//{
//public:
//	void test(){
//		std::wcout << L"MajorOps1" << std::endl;
//	}
//};
//class MajorOps2: public MajorOpsbase
//{
//public:
//	void test(){
//		std::wcout << L"MajorOps2" << std::endl;
//	}
//};
//class MajorOps3 : public MajorOpsbase
//{
//public:
//	void test(){
//		std::wcout << L"MajorOps3" << std::endl;
//	}
//};

///////////////////////////////////////////////////////////////////////////////
std::wstring tolower(std::wstring val){
	std::transform(val.begin(), val.end(), val.begin(), ::towlower);
	return val;
}
std::wstring toupper(std::wstring val){
	std::transform(val.begin(), val.end(), val.begin(), ::towupper);
	return val;
}
std::vector<std::wstring> 
tolower(std::vector<std::wstring> val){
	for (auto &it:val)
		std::transform(it.begin(), it.end(), it.begin(), ::towlower);
	return val;
}
std::vector<std::wstring> 
toupper(std::vector<std::wstring>& val){
	for (auto &it : val)
		std::transform(it.begin(), it.end(), it.begin(), ::towupper);
	return val;
}

bool find_ci(const std::vector<std::wstring>& c, std::wstring val)
{
	val = tolower(val);
	return std::any_of(c.begin(), c.end(), 
		[&,val](const std::wstring& src){return (tolower(src) == val); });
}

///////////////////////////////////////////////////////////////////////////////
std::wstring to_wstring(bool b){
	return (b ? L"true" : L"false");
}
template <typename T>
std::wstring to_wstring(T C){
//std::wstring to_wstring(std::vector<T> C){
	std::wcout << __FUNCTIONW__ << std::endl;
	std::wstring ret = L"";
	for (auto const &it : C){
		ret += std::to_wstring(it) + L",";
	}
	if (!ret.empty())
		ret.erase(ret.end() - 1);
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
class t_class1
{
public:
	template <typename T>
	void method1(T const & C);
};
template <typename T>
void t_class1::method1(T const & C)
{
	return;
}

///////////////////////////////////////////////////////////////////////////////
//template<typename clock_type, typename dur>
//struct ExecutionTimer
//{
//	typename clock_type::time_point start;
//	typename clock_type::time_point end;
//
//	typename clock_type::rep
//		diff()
//	{
//		//auto ret = std::chrono::duration_cast<dur>(end - start).count();
//		return std::chrono::duration_cast<dur>(end - start).count();
//	};
//};

///////////////////////////////////////////////////////////////////////////////
std::wstring
PrintExecutionTimer::to_wstring(std::chrono::hours::rep value, std::chrono::hours dummy)
{
	using namespace std::chrono;
	auto ret = std::to_wstring(value) + SS + L"hours";
	return ret;
}
std::wstring
PrintExecutionTimer::to_wstring(std::chrono::minutes::rep value, std::chrono::minutes dummy)
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
PrintExecutionTimer::to_wstring(std::chrono::seconds::rep value, std::chrono::seconds dummy)
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
PrintExecutionTimer::to_wstring(std::chrono::milliseconds::rep value, std::chrono::milliseconds dummy)
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
PrintExecutionTimer::to_wstring(std::chrono::microseconds::rep value, std::chrono::microseconds dummy)
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

///////////////////////////////////////////////////////////////////////////////
void templatetest()
{
	using namespace std::chrono;
	using namespace std;

	std::vector<float> test_list{ 1.2345678f, 2.3456781f, -3.4567812f, -4.5678123f };
	std::wcout << estd::to_wstring(test_list) << std::endl;
	std::wcout << std::to_wstring(1.2345678f) << std::endl;

	//ExecutionTimer<high_resolution_clock, microseconds> exe_timer_micro;
	//ExecutionTimer<high_resolution_clock, milliseconds> exe_timer_milli(new PrintExecutionTimer());
	//exe_timer_micro.set_start();
	//exe_timer_milli.set_start();

	//for (size_t i = 0; i < (std::numeric_limits<unsigned int>::max()); i++)
	//{
	//	if (0 == i % (32 * std::numeric_limits<unsigned short>::max())){
	//		wcout << L".";
	//		exe_timer_micro.set_end(high_resolution_clock::now());
	//		exe_timer_milli.set_end(high_resolution_clock::now());
	//	}
	//}
	//wcout << endl;
	//wcout << to_wstring(exe_timer_micro.elapsed()) << L" microseconds" << endl;
	//wcout << exe_timer_micro.to_wstring() << endl;

	//wcout << to_wstring(exe_timer_milli.elapsed()) << L" milliseconds" << endl;
	//wcout << exe_timer_milli.to_wstring() << endl;

	//wcout << to_wstring(vector < int > { 1, 2, 3, 4 }) << endl;;

	//wcout << test234().abc << endl;

	//vector<wstring> strlist = { L"Yes", L"Y", L"True", L"T" };
	//wcout << to_wstring(find_ci(strlist, L"y")) << endl;
	//wcout << to_wstring(find_ci(strlist, L"Y")) << endl;
	//wcout << to_wstring(find_ci(strlist, L"tRue")) << endl;

	//wcout << tolower(strlist) << endl;
	//wcout << toupper(strlist) << endl;

	//std::map<TEMP_TYPE, MajorOpsbase*> classmap = {
	//	{ TEMP_TYPE::TYPE1, new MajorOps1 },
	//	{ TEMP_TYPE::TYPE2, new MajorOps2 },
	//	{ TEMP_TYPE::TYPE3, new MajorOps3 },
	//};

	//MajorOps1().test(); classmap[TEMP_TYPE::TYPE1]->test();
	//MajorOps2().test(); classmap[TEMP_TYPE::TYPE2]->test();
	//MajorOps3().test(); classmap[TEMP_TYPE::TYPE3]->test();

	//for (auto &it : classmap){
	//	it.second->test();
	//}

	return;
}