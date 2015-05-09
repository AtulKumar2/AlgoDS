#include "stdafx.h"
#include "Common.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
void Run1::Run(){ 
	std::wcout << L"Inside " << __FUNCTIONW__ << std::endl; 
}

void Run2::Run(){
	std::wcout << L"Inside " << __FUNCTIONW__ << std::endl; 
}

////////////////////////////////////////////////////////////////////////////////
// Check if recursive call to calss function runs at same speed as normal function
class Factorial
{
public:
	long long get(unsigned int num)
	{
		if (num <= 1)
			return 1l;
		else
			return num*get(num - 1);
	}
};

long long factorial(unsigned int num)
{
	if (num <= 1)
		return 1l;
	else
		return num*factorial(num - 1);
}

////////////////////////////////////////////////////////////////////////////////
#pragma optimize( "", off )
void RunOOP()
{
	using namespace std;
	using namespace std::chrono;
	
	ExecutionTimer<high_resolution_clock, microseconds> exe_timer_micro(new PrintExecutionTimer());
	vector<unsigned int> num { 35, 40 };
	unsigned int iter = 2000000;
	
	vector<long long> product{ 0, 0 };
	vector<long long> result{ 0, 0 };

	wcout << L"Calling free function" << endl;
	exe_timer_micro.set_start();
	product[0] = factorial(num[0]);
	product[1] = factorial(num[1]);
	for (size_t i = 0; i < iter; i++)
	{
		result[0] = factorial(num[0]);
		result[1] = factorial(num[1]);
		if (product != result){
			product = { 0, 0 };
			break;
		}
	}
	exe_timer_micro.set_end();
	wcout << L"Factorial of " << num[0] << L" is : " << to_wstring(product[0]) << endl;
	wcout << L"Factorial of " << num[1] << L" is : " << to_wstring(product[1]) << endl;
	wcout << to_wstring(exe_timer_micro.elapsed()) << L" microseconds" << endl;
	wcout << exe_timer_micro.to_wstring() << endl;
	
	wcout << endl;
	seconds sleep_time(10);
	wcout << L"Sleeping for " << PrintExecutionTimer().to_wstring(seconds(10).count(), seconds(1)) << endl;
	std::this_thread::sleep_for(seconds(10));
	wcout << endl;

	wcout << L"Calling class method" << endl;
	exe_timer_micro.set_start();
	product[0] = Factorial().get(num[0]);
	product[1] = Factorial().get(num[1]);
	for (size_t i = 0; i < iter; i++)
	{
		result[0] = Factorial().get(num[0]);
		result[1] = Factorial().get(num[1]);
		if (product != result){
			product = { 0, 0 };
			break;
		}
	}
	exe_timer_micro.set_end();
	wcout << L"Factorial of " << num[0] << L" is : " << to_wstring(product[0]) << endl;
	wcout << L"Factorial of " << num[1] << L" is : " << to_wstring(product[1]) << endl;
	wcout << to_wstring(exe_timer_micro.elapsed()) << L" microseconds" << endl;
	wcout << exe_timer_micro.to_wstring() << endl;
}
#pragma optimize( "", on )
