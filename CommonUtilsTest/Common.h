#pragma once
#include <iostream>

// STD OUT redirection to string ///////////////////////////////////////////////
// http://stackoverflow.com/questions/5419356/redirect-stdout-stderr-to-a-string
struct cout_redirect 
{
	cout_redirect(std::streambuf * new_buffer)
		: old(std::cout.rdbuf(new_buffer))
	{ }

	~cout_redirect() {
		std::cout.rdbuf(old);
	}

private:
	std::streambuf *old;
};

struct wcout_redirect
{
	wcout_redirect(std::wstreambuf * new_buffer)
		: old(std::wcout.rdbuf(new_buffer))
	{ }

	~wcout_redirect() {
		std::wcout.rdbuf(old);
	}

private:
	std::wstreambuf *old;
};

////////////////////////////////////////////////////////////////////////////////
class IntStringPair
{
public:
	IntStringPair(int id, std::wstring value)
		:m_id(id), m_value_w(value){
		mark_all_unset();
		m_id_set = m_value_w_set = true;
	}
	IntStringPair(int id, std::string value)
		:m_id(id), m_value(value){
		mark_all_unset();
		m_id_set = m_value_set = true;
	}
	IntStringPair(unsigned int id, std::wstring value)
		:m_u_id(id), m_value_w(value){
		mark_all_unset();
		m_u_id_set = m_value_w_set = true;
	}
	IntStringPair(unsigned int id, std::string value)
		:m_u_id(id), m_value(value){
		mark_all_unset();
		m_u_id_set = m_value_set = true;
	}

	std::wstring 
		to_wstring()
	{
		using namespace std;
		wstring result{ L"" };
		if (m_id_set){
			result += std::to_wstring(m_id);
		}else if (m_u_id_set){
			result += std::to_wstring(m_u_id);
		}
		if (m_value_w_set){
			result += m_value_w;
		}
		return result;
	}
	std::string
		to_string()
	{
		using namespace std;
		string result{ "" };
		if (m_id_set){
			result += std::to_string(m_id);
		}
		else if (m_u_id_set){
			result += std::to_string(m_u_id);
		}
		if (m_value_set){
			result += m_value;
		}
		return result;
	}

private:
	void mark_all_unset()
	{
		m_u_id_set = m_id_set
			= m_value_set = m_value_w_set = false;
	}

private:
	unsigned int	m_u_id;		bool m_u_id_set;
	int				m_id;		bool m_id_set;
	std::wstring	m_value_w;	bool m_value_w_set;
	std::string		m_value;	bool m_value_set;
};