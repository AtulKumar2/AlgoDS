#pragma once
#include <string>
#include <algorithm>
#include <vector>

namespace estd
{
	//////////////////////////////////////////////////////////////////////
	template <typename T> std::wstring
		to_wstring(T const& C, std::wstring const& delim = L",")
	{
		std::wstring ret = L"";
		for (auto const &it : C){
			ret += (std::to_wstring(it) + delim);
		}
		if (!ret.empty()){
			ret.erase(ret.end() - delim.size(), ret.end());
		}
		return ret;
	}
	template <typename T> std::string
		to_string(T const& C, std::string const& delim = ",")
	{
		std::string ret = "";
		for (auto const &it : C){
			ret += std::to_string(it) + delim;
		}
		if (!ret.empty()){
			ret.erase(ret.end() - delim.size(), ret.end());
		}
		return ret;
	}

	/// Conversion ////////////////////////////////////////////////////////////
	std::wstring inline tolower(std::wstring val){
		std::transform(val.begin(), val.end(), val.begin(), ::towlower);
		return val;
	}
	std::wstring inline toupper(std::wstring val){
		std::transform(val.begin(), val.end(), val.begin(), ::towupper);
		return val;
	}
	std::vector<std::wstring> inline
		tolower(std::vector<std::wstring> val){
		for (auto &it : val)
			std::transform(it.begin(), it.end(), it.begin(), ::towlower);
		return val;
	}
	std::vector<std::wstring> inline
		toupper(std::vector<std::wstring>& val){
		for (auto &it : val)
			std::transform(it.begin(), it.end(), it.begin(), ::towupper);
		return val;
	}
}