#include "Header.h"
#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <string>

void iteratormagic()
{
	using std::map;
	using std::wstring;
	using std::wcout;

	typedef map<int, int> ValueMap;
	ValueMap Prices = ValueMap{
		{ 1, 100 }, { 2, 87 }, { 3, 156 }, { 4, 234 }, { 5, 65 }, { 6, 144 }, {7,98}
	};
	int diff = INT_MIN;
	ValueMap::const_iterator itleft = Prices.end(), itright = Prices.end();
	for (auto left = Prices.begin(); left != --(Prices.end()); left++){
		auto temp = left;
		for (auto right = ++temp; right != Prices.end(); right++){
			if (((*right).second - (*left).second) > diff) {
				diff = ((*right).second - (*left).second);
				itleft = left;
				itright = right;
			}
		}
	}
	wcout << (*itleft).second << L"\t" << (*itright).second << std::endl;
	return;
}

std::vector<int> 
GetDailyVariations(const std::map<unsigned int, unsigned int>& DayPrices)
{
	std::vector<int> variations;
	if (DayPrices.size() >= 2)
	{
		auto& it = DayPrices.begin();
		++it;
		while (it != DayPrices.end()){
			auto prev = it;
			variations.push_back((it++)->second - (--prev)->second);
		}
	}
	return variations;
}

void TestContainers()
{
	for (auto &it : GetDailyVariations({ { 1, 3 }, { 2, 4 }, { 4, 6 }, { 6, 9 } }))
		std::wcout << std::to_wstring(it) << std::endl;
	//iteratormagic();
}