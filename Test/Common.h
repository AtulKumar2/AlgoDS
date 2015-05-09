#pragma once
#include <string>
#include <random>
#include <limits>
#include <chrono>

template <typename T>
std::wostream&
operator<<(std::wostream& os, std::vector<typename T> const& input)
{
	std::wstring output;
	for (size_t i = 0; i < input.size() - 1; i++){
		output += (std::to_wstring(input.at(i)) + L",");
	}
	output += std::to_wstring(input.at(input.size() - 1));
	return (os << output);
}

std::wostream&
operator<<(std::wostream& os, std::vector<wchar_t> const& input);
std::wostream&
operator<<(std::wostream& os, std::vector<std::wstring> const& input);


///////////////////////////////////////////////////////////////////////////////
class PrintTimer
{
public:
	virtual std::wstring
		to_wstring(std::chrono::hours::rep value, std::chrono::hours dummy) = 0;
	virtual std::wstring
		to_wstring(std::chrono::minutes::rep value, std::chrono::minutes dummy) = 0;
	virtual std::wstring
		to_wstring(std::chrono::seconds::rep value, std::chrono::seconds dummy) = 0;
	virtual std::wstring
		to_wstring(std::chrono::microseconds::rep value, std::chrono::microseconds dummy) = 0;
	virtual std::wstring
		to_wstring(std::chrono::milliseconds::rep value, std::chrono::milliseconds dummy) = 0;
};

template<typename clock_type, typename dur>
class ExecutionTimer
{
private:
	typename clock_type::time_point m_start;
	typename clock_type::time_point m_end;
	PrintTimer * m_printer;

public:
	ExecutionTimer() : ExecutionTimer(nullptr) {};
	ExecutionTimer(PrintTimer * printer){
		m_start = clock_type::now();;
		m_end = clock_type::now();;
		m_printer = printer;
	};

	void set_start(){
		m_start = clock_type::now();
	}
	void set_start(typename clock_type::time_point start){
		m_start = start;
	}
	void set_end(){
		m_end = clock_type::now();
	}
	void set_end(typename clock_type::time_point end){
		m_end = end;
	}

	typename dur::rep elapsed()
	{
		return std::chrono::duration_cast<dur>(m_end - m_start).count();
	};
	std::wstring to_wstring()
	{
		if (m_printer){
			auto ret = m_printer->to_wstring(elapsed(), std::chrono::duration_cast<dur>(std::chrono::seconds(1)));
			return ret;
		}
		else{
			return L"Printing facilities not available";
		}
	}
};

class PrintExecutionTimer :public PrintTimer
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
		to_wstring(std::chrono::microseconds::rep value, std::chrono::microseconds dummy);
	virtual std::wstring
		to_wstring(std::chrono::milliseconds::rep value, std::chrono::milliseconds dummy);
};

///////////////////////////////////////////////////////////////////////////////
class RunBase
{
public:
	virtual void Run() = 0;
};

class Run1 : public RunBase
{
public:
	virtual void Run();
};
class Run2 : public RunBase
{
public:
	void Run();
};

///////////////////////////////////////////////////////////////////////////////
class RandomData
{
private:
	std::default_random_engine generator;
public:
	int Generate(std::vector<int>& list, size_t total,
		int lowest = std::numeric_limits<int>::min(),
		int highest = std::numeric_limits<int>::max());
	int Generate(std::vector<wchar_t>& list, size_t total);
	int Generate(std::vector<std::wstring> &list, size_t total, int maxlength);
private:
	void CreateGenerator(std::vector<int> seed_init = {},
		std::initializer_list<int> seed_init_seq = {});
	void GetRandomValues(std::vector<int>& randomvalues, size_t total, int lowest, int highest);
};

struct test234
{
	const std::wstring abc = L"abc";
	static std::wstring res1(){ return std::wstring(L"res1"); }
};