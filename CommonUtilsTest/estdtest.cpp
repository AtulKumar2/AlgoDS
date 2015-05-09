#include "stdafx.h"
#include "CppUnitTest.h"
#include "Common.h"
#include "..\CommonUtils\estd.h"
#include <exception>
#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include <list>
#include <climits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CommonUtilsTest
{
	TEST_CLASS(estd_operator_structm)
	{
	private:
		struct tm maketm(int year, int mon=1, int mday=1, 
			int hour = 0, int min = 0, int sec = 0)
		{
			struct tm result{0};
			result.tm_year = year - 1900;
			result.tm_mon = mon - 1;
			result.tm_mday = mday;
			result.tm_hour = hour;
			result.tm_min = min;
			result.tm_sec = sec;

			return result;
		}
	public:
		TEST_METHOD(T1_structtm_op_eq)
		{
			struct tm first = maketm(2000);
			struct tm second = maketm(2000);
			Assert::IsTrue(first == second);
		}
		TEST_METHOD(T1_structtm_op_ne)
		{
			struct tm first = maketm(2000);
			struct tm second = maketm(2001);
			Assert::IsTrue(first != second);
		}
		TEST_METHOD(T1_structtm_op_lt)
		{
			struct tm first = maketm(2000);
			struct tm second = maketm(2001);
			Assert::IsTrue(first < second);
		}
		TEST_METHOD(T1_structtm_op_gte)
		{
			struct tm first = maketm(2001);
			struct tm second = maketm(2000);
			Assert::IsTrue(first >= second);
		}
	};

	TEST_CLASS(estd_operator_vector_stdout)
	{
	public:
		TEST_METHOD(T1_ostream_uintvec)
		{
			using namespace std;

			wstringstream buffer;
			wcout_redirect redirect_dummy{ buffer.rdbuf() };

			wstring expected{ L"1,2,3,4" };
			wcout << vector<unsigned int> { 1, 2, 3, 4 };
			wstring actual = buffer.str();

			Assert::AreEqual(actual, expected);
		}

		TEST_METHOD(T1_ostream_wcharvec)
		{
			using namespace std;
			
			wstringstream buffer;
			wcout_redirect redirect_dummy{ buffer.rdbuf() };

			wstring expected{ L"a,b,c,d" };
			wcout << vector<wchar_t> { L'a', L'b', L'c', L'd' };
			wstring actual = buffer.str();

			Assert::AreEqual(actual, expected);
		}

		TEST_METHOD(T1_ostream_wstringvec)
		{
			using namespace std;

			wstringstream buffer;
			wcout_redirect redirect_dummy{ buffer.rdbuf() };

			wstring expected{ L"aA,bB,cC,dD" };
			wcout << vector<wstring> { L"aA", L"bB", L"cC", L"dD" };
			wstring actual = buffer.str();

			Assert::AreEqual(actual, expected);
		}
	};

	TEST_CLASS(estd_operator_trim)
	{
		TEST_METHOD(T1_trim_w)
		{
			Assert::AreEqual(std::wstring(L"abcd"), estd::trim(L"  abcd "));
		}
		TEST_METHOD(T1_trim)
		{
			Assert::AreEqual(std::string("abcd"), estd::trim(" abcd  "));
		}
		TEST_METHOD(T1_ltrim_w)
		{
			Assert::AreEqual(std::wstring(L"ab cd"), estd::trim(L"  ab cd"));
		}
		TEST_METHOD(T1_ltrim)
		{
			Assert::AreEqual(std::string("a bcd"), estd::trim(" a bcd"));
		}
		TEST_METHOD(T1_rtrim_w)
		{
			Assert::AreEqual(std::wstring(L"abc d"), estd::trim(L"abc d  "));
		}
		TEST_METHOD(T1_rtrim)
		{
			Assert::AreEqual(std::string("abcd"), estd::trim("abcd "));
		}
	};

	TEST_CLASS(estd_string_conversion)
	{
	public:
		TEST_METHOD(T1_to_wstring_true)
		{
			Assert::AreEqual(std::wstring(L"true"), estd::to_wstring(true));
		}
		TEST_METHOD(T1_to_wstring_false)
		{
			Assert::AreEqual(std::wstring(L"false"), estd::to_wstring(false));
		}

	/*	TEST_METHOD(T2_to_wstring_object)
		{
			std::vector<IntStringPair> test_list{
				{ 1, L"A" },
				{ 2, L"B" },
				{ 3, L"C" },
				{ 4, L"D" },
			};
			Assert::AreEqual(std::wstring(L"1A;2B;3C;4D"), estd::to_wstring(test_list, L";"));
		}
		TEST_METHOD(T2_to_string_object)
		{
			std::vector<IntStringPair> test_list{
				{ 1u, "Z" },
				{ 2u, "Y" },
				{ 3u, "X" },
				{ 4u, "W" },
			};
			Assert::AreEqual(std::string("1Z;2Y;3X;4W"), estd::to_string(test_list, ";"));
		}*/
		TEST_METHOD(T2_to_wstring_uintvec)
		{
			std::vector<unsigned int> test_list{ 1u, 2u, 3u, 4u };
			Assert::AreEqual(std::wstring{L"1,2,3,4"},
				estd::to_wstring(test_list));
		}
		/*TEST_METHOD(T2_to_wstring_doublevec)
		{
			using namespace std;
			vector<double> test_list{ 1.23456789, -2.34567891, -3.4512, 4.5678923451123 };

			wstring expected{ L"" }, sep{ L"," };
			for (size_t i = 0; i < test_list.size() - 1; i++){
				expected += std::to_wstring(static_cast<float>(test_list[i])) + sep;
			}
			expected += std::to_wstring(static_cast<float>(test_list[test_list.size() - 1]));

			Assert::AreEqual(expected, estd::to_wstring(test_list));
		}*/
		TEST_METHOD(T2_to_string_negintvec)
		{
			std::vector<int> test_list{ -1, -2, -3, -4 };
			Assert::AreEqual(std::string{ "-1,-2,-3,-4" },
				estd::to_string(test_list));
		}
		TEST_METHOD(T2_to_string_floatvec)
		{
			using namespace std;
			vector<float> test_list{ 1.2345f, 2.3451f, -3.4512f, -4.5123f };
			
			string expected{ "" }, padding{ "" }, sep{","};
			padding.insert(padding.begin(), FLT_DIG - 4, '0');
			for (auto const& it :
				vector < string > { "1.2345", "2.3451", "-3.4512" }){
				expected += it + padding + sep;
			}
			expected += std::string{"-4.5123"} + padding;
			
			Assert::AreEqual(expected, estd::to_string(test_list));
		}

		TEST_METHOD(T3_to_wstring_wstrvec)
		{
			std::vector<std::wstring> test_list{ L"abcd", L"bcda", L"cdab", L"dabc" };
			Assert::AreEqual(std::wstring(L"abcd:bcda:cdab:dabc"), 
				estd::to_wstring(test_list, std::wstring(L":")));
		}
		TEST_METHOD(T3_to_string_strvec)
		{
			std::vector<std::string> test_list{ "abcd", "bcda", "cdab", "dabc" };
			Assert::AreEqual(std::string("abcd,bcda,cdab,dabc"), 
				estd::to_string(test_list));
		}

		TEST_METHOD(T4_to_wstring_wstrset)
		{
			std::set<std::wstring> test_list{ L"abcd", L"bcda", L"cdab", L"dabc" };
			Assert::AreEqual(std::wstring(L"abcd:bcda:cdab:dabc"),
				estd::to_wstring(test_list, std::wstring(L":")));
		}
		TEST_METHOD(T3_to_string_strset)
		{
			std::set<std::string> test_list{ "abcd", "bcda", "cdab", "dabc" };
			Assert::AreEqual(std::string("abcd,bcda,cdab,dabc"), estd::to_string(test_list));
		}
		
		TEST_METHOD(T4_to_string_charvec)
		{
			std::vector<char> test_list{ 'a', 'b', 'c', 'd' };
			Assert::AreEqual(std::string("a,b,c,d"), estd::to_string(test_list));
		}
		TEST_METHOD(T4_to_wstring_wcharvec)
		{
			std::vector<wchar_t> test_list{ L'a', L'b', L'c', L'd' };
			Assert::AreEqual(std::wstring(L"a,b,c,d"), estd::to_wstring(test_list));
		}
		TEST_METHOD(T4_to_string_charset)
		{
			std::set<char> test_list{ 'a', 'b', 'c', 'd' };
			Assert::AreEqual(std::string("a,b,c,d"), estd::to_string(test_list));
		}
		TEST_METHOD(T4_to_wstring_wcharset)
		{
			std::set<wchar_t> test_list{ L'a', L'b', L'c', L'd' };
			Assert::AreEqual(std::wstring(L"a,b,c,d"), estd::to_wstring(test_list));
		}
	};
	
	TEST_CLASS(estd_case_conversion)
	{
	public:
		TEST_METHOD(T1_tolower_w)
		{
			Assert::AreEqual(std::wstring(L"abcd"), 
				estd::tolower(std::wstring(L"AbCd")));
		}
		TEST_METHOD(T1_tolower)
		{
			Assert::AreEqual(std::string("abcd"),
				estd::tolower(std::string("AbCd")));
		}
		TEST_METHOD(T1_toupper_w)
		{
			Assert::AreEqual(std::wstring(L"ABCD"),
				estd::toupper(std::wstring(L"AbCd")));
		}
		TEST_METHOD(T1_toupper)
		{
			Assert::AreEqual(std::string("ABCD"),
				estd::toupper(std::string("AbCd")));
		}

		TEST_METHOD(T2_tolower_wstrvec)
		{
			using namespace std;
			vector < wstring > expected{L"abcd",L"bcda",L"cdab", L"dabc"};
			vector < wstring > input {L"Abcd",L"bCda", L"cdAb", L"dabC"};
			Assert::IsTrue(expected == estd::tolower(input));
		}
		TEST_METHOD(T2_tolower_strvec)
		{
			using namespace std;
			vector < string > expected {"abcd", "bcda", "cdab", "dabc" };
			vector < string > input {"Abcd", "bCda", "cdAb", "dabC" };
			Assert::IsTrue(expected==estd::tolower(input));
		}
		TEST_METHOD(T2_toupper_wstrvec)
		{
			using namespace std;
			vector < wstring > expected{ L"ABCD", L"BCDA", L"CDAB", L"DABC" };
			vector < wstring > input{ L"Abcd", L"bCda", L"cdAb", L"dabC" };
			Assert::IsTrue(expected == estd::toupper(input));
		}
		TEST_METHOD(T2_toupper_strvec)
		{
			using namespace std;
			vector < string > expected{ "ABCD", "BCDA", "CDAB", "DABC" };
			vector < string > input{ "Abcd", "bCda", "cdAb", "dabC" };
			Assert::IsTrue(expected == estd::toupper(input));
		}

		//TEST_METHOD(T3_tolower_wcharlist)
		//{
		//	using namespace std;
		//	Assert::AreEqual(
		//		list < wchar_t > {L'a', L'b', L'c', L'd' },
		//		estd::tolower(list < wchar_t > {L'A', L'b', L'C', L'd' }));
		//}
		//TEST_METHOD(T3_tolower_charlist)
		//{
		//	using namespace std;
		//	Assert::AreEqual(
		//		list < char > {'a', 'b', 'c', 'd' },
		//		estd::tolower(list < char > {'a', 'B', 'c', 'D' }));
		//}
		//TEST_METHOD(T3_toupper_wcharlist)
		//{
		//	using namespace std;
		//	Assert::AreEqual(
		//		list < wchar_t > {L'A', L'B', L'C', L'D'},
		//		estd::tolower(list < wchar_t > {L'A', L'b', L'C', L'd' }));
		//}
		//TEST_METHOD(T3_toupper_charset)
		//{
		//	using namespace std;
		//	Assert::AreEqual(
		//		set < char > {'A', 'B', 'C', 'D'},
		//		estd::tolower(set < char > {'a', 'B', 'c', 'D' }));
		//}
	};

	TEST_CLASS(etsd_numeric_conversion)
	{
	public:
		TEST_METHOD(T1_stou_w)
		{
			Assert::AreEqual(5000u, estd::stou(std::wstring{L"5000"}));
		}
		TEST_METHOD(T1_stou)
		{
			Assert::AreEqual(5000u, estd::stou(std::string{ "5000" }));
		}
	};

	TEST_CLASS(etsd_padding)
	{
	public:
		TEST_METHOD(T1_zeropadding_w)
		{
			Assert::AreEqual(std::wstring{ L"00023" }, estd::prepend_zero_w(23, 5));
		}
		TEST_METHOD(T1_zeropadding)
		{
			Assert::AreEqual(std::string{ "00023" }, estd::prepend_zero(23, 5));
		}

		TEST_METHOD(T2_prepend_space_w)
		{
			Assert::AreEqual(std::wstring{ L"   23" }, estd::prepend_space_w(L"23", 5));
		}
		TEST_METHOD(T2_prepend_space)
		{
			Assert::AreEqual(std::string{ "   23" }, estd::prepend_space("23", 5));
		}

		TEST_METHOD(T3_append_space_w)
		{
			Assert::AreEqual(std::wstring{ L"23   " }, estd::append_space_w(L"23", 5));
		}
		TEST_METHOD(T3_append_space)
		{
			Assert::AreEqual(std::string{ "23   " }, estd::append_space("23", 5));
		}
	};

	TEST_CLASS(etsd_repeat)
	{
	public:
		TEST_METHOD(T1_repeat_w)
		{
			Assert::AreEqual(std::wstring{ L"AbcdAbcdAbcdAbcd" },
				estd::repeat(std::wstring{ L"Abcd" }, 4));
		}
		TEST_METHOD(T1_repeat)
		{
			Assert::AreEqual(std::string{ "AbcdAbcdAbcdAbcd" },
				estd::repeat(std::string{ "Abcd" }, 4));
		}
	};

	TEST_CLASS(estd_compare)
	{
	public:
		TEST_METHOD(T1_compare_ci_w_t)
		{
			Assert::AreEqual(0, estd::compare_ci(
				std::wstring{ L"0AbCd efgh1" }, std::wstring{ L"0aBcD efGh1" }));
		}
		TEST_METHOD(T1_compare_ci_w_f)
		{
			Assert::AreNotEqual(0, estd::compare_ci(
				std::wstring{ L"Ab Cd efghi" }, std::wstring{ L"8aBcDefGh&" }));
		}
		TEST_METHOD(T1_compare_ci_t)
		{
			Assert::AreEqual(0, estd::compare_ci(
				std::string{ "AbC defgh " }, std::string{ "aBc DefGh " }));
		}
		TEST_METHOD(T1_compare_ci_f)
		{
			Assert::AreNotEqual(0, estd::compare_ci(
				std::string{ "Ab Cde fgh" }, std::string{ "0a BcDe fGh" }));
		}
	};

	TEST_CLASS(estd_trimandcompare)
	{
	public:
		TEST_METHOD(T1_trimandcompare_w_t)
		{
			Assert::AreEqual(0, estd::trimandcompare(
				std::wstring{ L"0 AbCd efg h1" }, std::wstring{ L"  0 AbCd efg h1" }));
		}
		TEST_METHOD(T1_trimandcompare_w_f)
		{
			Assert::AreNotEqual(0, estd::trimandcompare(
				std::wstring{ L"  Ab Cd efghi " }, std::wstring{ L"aB cD efGhi" }));
		}
		TEST_METHOD(T1_trimandcompare_t)
		{
			Assert::AreEqual(0, estd::trimandcompare(
				std::string{"0 AbCd efg h1"}, std::string{"  0 AbCd efg h1"}));
		}
		TEST_METHOD(T1_trimandcompare_f)
		{
			Assert::AreNotEqual(0, estd::trimandcompare(
				std::string{"  Ab Cd efghi " }, std::string{"aB cD efGhi" }));
		}

		TEST_METHOD(T2_trimandcompare_ci_w_t)
		{
			Assert::AreEqual(0, estd::trimandcompare_ci(
				std::wstring{ L"0 AbCd efg h1  " }, std::wstring{ L"  0 AbCd eFg H1" }));
		}
		TEST_METHOD(T2_trimandcompare_ci_w_f)
		{
			Assert::AreNotEqual(0, estd::trimandcompare_ci(
				std::wstring{ L"  Ab Cd efghi " }, std::wstring{ L"aB cD efGhi" }));
		}
		TEST_METHOD(T2_trimandcompare_ci_t)
		{
			Assert::AreEqual(0, estd::trimandcompare_ci(
				std::string{ "0 AbCd efg h1  " }, std::string{ "  0 AbCd eFg H1" }));
		}
		TEST_METHOD(T2_trimandcompare_ci_f)
		{
			Assert::AreNotEqual(0, estd::trimandcompare_ci(
				std::string{ "  Ab Cd efghi " }, std::string{ "aB cD efGhi" }));
		}
	};

	TEST_CLASS(estd_mathematical)
	{
	public:

		TEST_METHOD(T1_ispowerof_int_t_1)
		{
			Assert::IsTrue(estd::is_power_of(8, 2));
		}
		TEST_METHOD(T1_ispowerof_int_t_2)
		{
			Assert::IsTrue(estd::is_power_of(-27, -3));
		}
		TEST_METHOD(T1_ispowerof_int_f_1)
		{
			Assert::IsFalse(estd::is_power_of(8, 3));
		}
		TEST_METHOD(T1_ispowerof_int_f_2)
		{
			Assert::IsFalse(estd::is_power_of(-9, 3));
		}

		TEST_METHOD(T2_ispowerof_uint_t_1)
		{
			Assert::IsTrue(estd::is_power_of(8u, 2u));
		}
		TEST_METHOD(T2_ispowerof_uint_f_1)
		{
			Assert::IsFalse(estd::is_power_of(8u, 3u));
		}

		TEST_METHOD(T3_ispowerof_nonint_t_1)
		{
			Assert::IsTrue(estd::is_power_of(.81, .9));
		}
		TEST_METHOD(T3_ispowerof_nonint_t_2)
		{
			Assert::IsTrue(estd::is_power_of(-.027, -.3));
		}
		TEST_METHOD(T3_ispowerof_nonint_t_3)
		{
			Assert::IsTrue(estd::is_power_of(6.25, 2.5));
		}
		TEST_METHOD(T3_ispowerof_nonint_t_4)
		{
			Assert::IsTrue(estd::is_power_of(39.0625, -2.5));
		}

		TEST_METHOD(T4_ispowerof_nonint_f_1)
		{
			Assert::IsFalse(estd::is_power_of(.81, 3.0));
		}
		TEST_METHOD(T4_ispowerof_nonint_f_2)
		{
			Assert::IsFalse(estd::is_power_of(-.5, 1.0));
		}
		TEST_METHOD(T4_ispowerof_nonint_f_3)
		{
			Assert::IsFalse(estd::is_power_of(-2.5, 0.5));
		}
		TEST_METHOD(T4_ispowerof_nonint_f_4)
		{
			Assert::IsFalse(estd::is_power_of(2.5, 0.0));
		}
	};

	TEST_CLASS(estd_positional)
	{
	public:
		TEST_METHOD(T1_is_available_vec_t)
		{
			std::vector<double> data_list{ 1.2, 3.4, -4.5, 6.7 };
			Assert::IsTrue(estd::is_available(3.4, data_list));
		}
		TEST_METHOD(T1_is_available_vec_f)
		{
			std::vector<double> data_list{ 1.2, 3.4, -4.5, 6.7 };
			Assert::IsFalse(estd::is_available(0.5, data_list));
		}
		TEST_METHOD(T1_is_available_list_t)
		{
			std::list<double> data_list{ 1.2, 3.4, -4.5, 6.7 };
			Assert::IsTrue(estd::is_available(-4.5, data_list));
		}
		TEST_METHOD(T1_is_available_list_f)
		{
			std::list<double> data_list{ 1.2, 3.4, -4.5, 6.7 };
			Assert::IsFalse(estd::is_available(0, data_list));
		}

		TEST_METHOD(T2_any_of_w_t)
		{
			using namespace std;
			vector<wstring> data_items{ L"pqrstu", L"upqrst", L"tupqrs", L"stupqr" };
			Assert::IsTrue(estd::any_of(L"pqrstu", data_items));
		}
		TEST_METHOD(T2_any_of_w_f)
		{
			using namespace std;
			vector<wstring> data_items{ L"pqrstu", L"upqrst", L"tupqrs", L"stupqr" };
			Assert::IsFalse(estd::any_of(L"pqrst", data_items));
		}

		TEST_METHOD(T3_any_of_t)
		{
			using namespace std;
			vector<string> data_items{ "pqrstu", "upqrst", "tupqrs", "stupqr" };
			Assert::IsTrue(estd::any_of("pqrstu", data_items));
		}
		TEST_METHOD(T3_any_of_f)
		{
			using namespace std;
			vector<string> data_items{ "pqrstu", "upqrst", "tupqrs", "stupqr" };
			Assert::IsFalse(estd::any_of("pqrst", data_items));
		}

		TEST_METHOD(T4_any_of_ci_w_t)
		{
			using namespace std;
			vector<wstring> data_items{ L"pQrstu", L"upqRst", L"tupqrS", L"stUpqr" };
			Assert::IsTrue(estd::any_of_ci(L"pqRStu", data_items));
		}
		TEST_METHOD(T4_any_of_ci_w_f)
		{
			using namespace std;
			vector<wstring> data_items{ L"pqrstu", L"upqrst", L"tupqrs", L"stupqr" };
			Assert::IsFalse(estd::any_of_ci(L"PqrT", data_items));
		}

		TEST_METHOD(T5_any_of_ci_t)
		{
			using namespace std;
			vector<string> data_items{ "pqrsTu", "Upqrst", "tupQrs", "stuPqr" };
			Assert::IsTrue(estd::any_of_ci("pqRstU", data_items));
		}
		TEST_METHOD(T5_any_of_ci_f)
		{
			using namespace std;
			vector<string> data_items{ "pqRstu", "upqrSt", "Tupqrs", "stuPqr" };
			Assert::IsFalse(estd::any_of_ci("pRstu", data_items));
		}

		TEST_METHOD(T6_start_with_w_t)
		{
			std::wstring src{ L"efghijkl" }, search_str{ L"efg" };
			Assert::IsTrue(estd::start_with(src, search_str));
		}
		TEST_METHOD(T6_start_with_w_f)
		{
			std::wstring src{ L"efghijkl" }, search_str{ L"eFg" };
			Assert::IsFalse(estd::start_with(src, search_str));
		}
		TEST_METHOD(T6_start_with_t)
		{
			std::string src{ "efghijkl" }, search_str{ "efg" };
			Assert::IsTrue(estd::start_with(src, search_str));
		}
		TEST_METHOD(T6_start_with_f)
		{
			std::string src{ "efghijkl" }, search_str{ "eFg" };
			Assert::IsFalse(estd::start_with(src, search_str));
		}

		TEST_METHOD(T7_start_with_ci_w_t)
		{
			std::wstring src{ L"efghijkl" }, search_str{ L"EfG" };
			Assert::IsTrue(estd::start_with_ci(src, search_str));
		}
		TEST_METHOD(T7_start_with_ci_w_f)
		{
			std::wstring src{ L"efg" }, search_str{ L"efgijkl" };
			Assert::IsFalse(estd::start_with_ci(src, search_str));
		}
		TEST_METHOD(T7_start_with_ci_t)
		{
			std::string src{ "EfGhIjKl" }, search_str{ "efg" };
			Assert::IsTrue(estd::start_with_ci(src, search_str));
		}
		TEST_METHOD(T7_start_with_ci_f)
		{
			std::string src{ "ef gh ijkl" }, search_str{ "efg" };
			Assert::IsFalse(estd::start_with_ci(src, search_str));
		}

	};

	TEST_CLASS(estd_tokenizer)
	{
	private:
		using TokOP=estd::TokenizerOP;
		using WSTR_VEC = std::vector < std::wstring > ;
		using STR_VEC = std::vector < std::string >;
	
	public:
		TEST_METHOD(T1_SIMPLE_w)
		{
			estd::Tokenizer<std::wstring> tokenizer;
			auto actual = tokenizer.Tokens({ L"a,b,c,d" }, { L"," });
			auto expected = WSTR_VEC{ L"a", L"b", L"c", L"d" };
			Assert::IsTrue(expected == actual);
		}
		TEST_METHOD(T1_TRIMMED_w)
		{
			estd::Tokenizer<std::wstring> tokenizer(TokOP::TRIMMED);
			auto expected = tokenizer.Tokens({ L"a , b, c ,d" }, L",");
			auto actual = WSTR_VEC{ L"a", L"b", L"c", L"d" };
			Assert::IsTrue(expected == actual);
		}
		TEST_METHOD(T1_NONEMPTY_w)
		{
			estd::Tokenizer<std::wstring> tokenizer(L";", TokOP::NONEMPTY);
			auto expected = tokenizer.Tokens({ L"a; ; ;b;c;d" });
			auto actual = WSTR_VEC{ L"a", L"b", L"c", L"d" };
			Assert::IsTrue(expected == actual);
		}
		TEST_METHOD(T1_NONEMPTYTRIMMED_w)
		{
			estd::Tokenizer<std::wstring> tokenizer(TokOP::NONEMPTYTRIMMED);
			auto expected = tokenizer.Tokens({ L"a ; ; ; b;  c ;d" }, L";");
			auto actual = WSTR_VEC{ L"a", L"b", L"c", L"d" };
			Assert::IsTrue(expected == actual);
		}
		TEST_METHOD(T1_UNIQUE_w)
		{
			estd::Tokenizer<std::wstring> tokenizer(L"=", TokOP::UNIQUE);
			auto expected = tokenizer.Tokens({ L"a=b=a=c=d=b" });
			auto actual = WSTR_VEC{ L"a", L"b", L"c", L"d" };
			Assert::IsTrue(expected == actual);
		}
		TEST_METHOD(T1_UNIQUETRIMMED_w)
		{
			estd::Tokenizer<std::wstring> tokenizer(TokOP::UNIQUETRIMMED);
			auto expected = tokenizer.Tokens({ L"a;b ; a ; c ;d; b" }, L";");
			auto actual = WSTR_VEC{ L"a", L"b", L"c", L"d" };
			Assert::IsTrue(expected == actual);
		}
		TEST_METHOD(T1_UNIQUENONEMPTY_w)
		{
			estd::Tokenizer<std::wstring> tokenizer(L"=", TokOP::UNIQUENONEMPTY);
			auto expected = tokenizer.Tokens({ L"a;b;;;a;c;d; ; ;b" }, L";");
			auto actual = WSTR_VEC{ L"a", L"b", L"c", L"d" };
			Assert::IsTrue(expected == actual);
		}
		TEST_METHOD(T1_UNIQUENONEMPTYTRIMMED_w)
		{
			estd::Tokenizer<std::wstring> tokenizer(TokOP::UNIQUENONEMPTYTRIMMED);
			auto expected = tokenizer.Tokens({ L"a ;b ; a ; c ;d; ; ; b" }, L";");
			auto actual = WSTR_VEC{ L"a", L"b", L"c", L"d" };
			Assert::IsTrue(expected == actual);
		}

		TEST_METHOD(T2_SIMPLE)
		{
			estd::Tokenizer<std::string> tokenizer;
			auto actual = tokenizer.Tokens({ "a,b,c,d" }, { "," });
			auto expected = STR_VEC{ "a", "b", "c", "d" };
			Assert::IsTrue(expected == actual);
		}
		TEST_METHOD(T2_TRIMMED)
		{
			estd::Tokenizer<std::string> tokenizer(TokOP::TRIMMED);
			auto expected = tokenizer.Tokens({ "a , b, c ,d" }, ",");
			auto actual = STR_VEC{ "a", "b", "c", "d" };
			Assert::IsTrue(expected == actual);
		}
		TEST_METHOD(T2_NONEMPTY)
		{
			estd::Tokenizer<std::string> tokenizer(";", TokOP::NONEMPTY);
			auto expected = tokenizer.Tokens({ "a; ; ;b;c;d" });
			auto actual = STR_VEC{ "a", "b", "c", "d" };
			Assert::IsTrue(expected == actual);
		}
		TEST_METHOD(T2_NONEMPTYTRIMMED)
		{
			estd::Tokenizer<std::string> tokenizer(TokOP::NONEMPTYTRIMMED);
			auto expected = tokenizer.Tokens({ "a ; ; ; b;  c ;d" }, ";");
			auto actual = STR_VEC{ "a", "b", "c", "d" };
			Assert::IsTrue(expected == actual);
		}
		TEST_METHOD(T2_UNIQUE)
		{
			estd::Tokenizer<std::string> tokenizer("=", TokOP::UNIQUE);
			auto expected = tokenizer.Tokens({ "a=b=a=c=d=b" });
			auto actual = STR_VEC{ "a", "b", "c", "d" };
			Assert::IsTrue(expected == actual);
		}
		TEST_METHOD(T2_UNIQUETRIMMED)
		{
			estd::Tokenizer<std::string> tokenizer(TokOP::UNIQUETRIMMED);
			auto expected = tokenizer.Tokens({ "a;b ; a ; c ;d; b" }, ";");
			auto actual = STR_VEC{ "a", "b", "c", "d" };
			Assert::IsTrue(expected == actual);
		}
		TEST_METHOD(T2_UNIQUENONEMPTY)
		{
			estd::Tokenizer<std::string> tokenizer("=", TokOP::UNIQUENONEMPTY);
			auto expected = tokenizer.Tokens({ "a;b;;;a;c;d; ; ;b" }, ";");
			auto actual = STR_VEC{ "a", "b", "c", "d" };
			Assert::IsTrue(expected == actual);
		}
		TEST_METHOD(T2_UNIQUENONEMPTYTRIMMED)
		{
			estd::Tokenizer<std::string> tokenizer(TokOP::UNIQUENONEMPTYTRIMMED);
			auto expected = tokenizer.Tokens({ "a ;b ; a ; c ;d; ; ; b" }, ";");
			auto actual = STR_VEC{ "a", "b", "c", "d" };
			Assert::IsTrue(expected == actual);
		}
	};

	TEST_CLASS(estd_execution_timer)
	{
	private:
		const std::chrono::seconds basic_sleep_time{ 2 };

		class PrintExecutionTimer : public estd::PrintTimer
		{
		private:
			const std::wstring SS = std::wstring{ L" " };
		public:
			virtual std::wstring
				to_wstring(std::chrono::hours::rep value, std::chrono::hours dummy){
				return (std::to_wstring(value) + SS + L"hours");
			}
			virtual std::wstring
				to_wstring(std::chrono::minutes::rep value, std::chrono::minutes dummy){
				return (std::to_wstring(value) + SS + L"minutes");
			}
			virtual std::wstring
				to_wstring(std::chrono::seconds::rep value, std::chrono::seconds dummy){
				return (std::to_wstring(value) + SS + L"seconds");
			}
			virtual std::wstring
				to_wstring(std::chrono::microseconds::rep value, std::chrono::microseconds dummy){
				return (std::to_wstring(value) + SS + L"microseconds");
			}
			virtual std::wstring
				to_wstring(std::chrono::milliseconds::rep value, std::chrono::milliseconds dummy){
				return (std::to_wstring(value) + SS + L"milliseconds");
			}
			virtual std::wstring
				to_wstring(std::chrono::nanoseconds::rep value, std::chrono::nanoseconds dummy){
				return (std::to_wstring(value) + SS + L"nanoseconds");
			}
		};

	public:
		TEST_METHOD(Test_1)
		{
			using namespace std::chrono;
			using namespace std;

			estd::ExecutionTimer<high_resolution_clock, nanoseconds> timer;
			timer.set_start();
			this_thread::sleep_for(basic_sleep_time);
			timer.set_end();
			Assert::IsTrue(timer.elapsed() >= duration_cast<nanoseconds>(basic_sleep_time).count());
		}

		TEST_METHOD(Test_2)
		{
			using namespace std::chrono;
			using namespace std;

			estd::ExecutionTimer<system_clock, milliseconds> timer;
			timer.set_start();
			this_thread::sleep_for(basic_sleep_time);
			timer.set_end();
			Assert::IsTrue(timer.elapsed() >= duration_cast<milliseconds>(basic_sleep_time).count());
		}

		TEST_METHOD(Test_3)
		{
			using namespace std::chrono;
			using namespace std;

			estd::ExecutionTimer<steady_clock, minutes> timer;
			timer.set_start();
			this_thread::sleep_for(basic_sleep_time*45);
			timer.set_end();
			Assert::IsTrue(timer.elapsed() >= duration_cast<minutes>(basic_sleep_time*45).count());
		}

		TEST_METHOD(Test_4)
		{
			using namespace std::chrono;
			using namespace std;

			estd::ExecutionTimer<high_resolution_clock, microseconds> timer;
			timer.set_start(high_resolution_clock::now());
			this_thread::sleep_for(basic_sleep_time);
			timer.set_end(high_resolution_clock::now());
			Assert::IsTrue(timer.elapsed() >= duration_cast<minutes>(basic_sleep_time * 45).count());
		}

		TEST_METHOD(Test_5)
		{
			using namespace std::chrono;
			using namespace std;

			estd::ExecutionTimer<system_clock, microseconds> timer;
			timer.set_start(system_clock::now());
			this_thread::sleep_for(basic_sleep_time);
			timer.set_end(system_clock::now());
			Assert::IsTrue(std::to_wstring(timer.elapsed()) == timer.to_wstring());
		}

		TEST_METHOD(Test_6)
		{
			using namespace std::chrono;
			using namespace std;

			auto printer = std::shared_ptr<estd::PrintTimer>(new PrintExecutionTimer());
			estd::ExecutionTimer<steady_clock, hours> timer(printer);
			timer.set_start(steady_clock::now());
			this_thread::sleep_for(basic_sleep_time);
			timer.set_end(steady_clock::now());
			Assert::IsTrue(printer->to_wstring(timer.elapsed(), hours(1)) == timer.to_wstring());
		}

		TEST_METHOD(Test_7)
		{
			using namespace std::chrono;
			using namespace std;

			auto printer = std::shared_ptr<estd::PrintTimer>(new PrintExecutionTimer());
			estd::ExecutionTimer<high_resolution_clock, seconds> timer;
			timer.set_printer(printer);
			timer.set_start(high_resolution_clock::now());
			this_thread::sleep_for(basic_sleep_time);
			timer.set_end(high_resolution_clock::now());
			Assert::IsTrue(printer->to_wstring(timer.elapsed(), seconds(1)) == timer.to_wstring());
		}

		TEST_METHOD(Test_8)
		{
			using namespace std::chrono;
			using namespace std;

			auto printer = std::shared_ptr<estd::PrintTimer>(new PrintExecutionTimer());
			estd::ExecutionTimer<system_clock, microseconds> timer;
			timer.set_start(system_clock::now());
			this_thread::sleep_for(basic_sleep_time);
			timer.set_end(system_clock::now());
			Assert::IsTrue(printer->to_wstring(timer.elapsed(), microseconds(1)) == timer.to_wstring(printer));
		}
	};

	TEST_CLASS(estd_allocator_arrays)
	{
	public:
		TEST_METHOD(T1_blank_empty)
		{
			std::vector<int> actual{ };
			auto actual_ptr = estd::AllocateArrays().blank(actual);
			Assert::IsNull(actual_ptr);
		}

		TEST_METHOD(T2_blank_int)
		{
			std::vector<int> actual{ -1,-2,-3,-4,5,6,7,8 }, expected{};
			expected.assign(actual.size(), 0);
			auto actual_ptr = estd::AllocateArrays().blank(actual);
			Assert::IsNotNull(actual_ptr);
			for (size_t i = 0; i < expected.size(); i++) {
				Assert::AreEqual(actual_ptr[i], expected[i]);
			}
			delete[] actual_ptr;
		}
		TEST_METHOD(T2_blank_uint)
		{
			std::vector<unsigned int> actual{ 1u,2u,3u,4u,5u,6u,7u,8u }, expected{};
			expected.assign(actual.size(), 0);
			auto actual_ptr = estd::AllocateArrays().blank(actual);
			Assert::IsNotNull(actual_ptr);
			for (size_t i = 0; i < expected.size(); i++) {
				Assert::AreEqual(actual_ptr[i], expected[i]);
			}
			delete[] actual_ptr;
		}
		TEST_METHOD(T2_blank_char)
		{
			std::vector<char> actual{ '1','2','3','4','5','6','7','8' }, expected{};
			expected.assign(actual.size(), 0);
			auto actual_ptr = estd::AllocateArrays().blank(actual);
			Assert::IsNotNull(actual_ptr);
			for (size_t i = 0; i < expected.size(); i++){
				Assert::AreEqual(actual_ptr[i], expected[i]);
			}
			delete[] actual_ptr;
		}
		TEST_METHOD(T2_blank_wchar)
		{
			std::vector<wchar_t> actual{ L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8' }, expected{};
			expected.assign(actual.size(), 0);
			auto actual_ptr = estd::AllocateArrays().blank(actual);
			Assert::IsNotNull(actual_ptr);
			for (size_t i = 0; i < expected.size(); i++) {
				Assert::AreEqual(actual_ptr[i], expected[i]);
			}
			delete[] actual_ptr;
		}

		TEST_METHOD(T3_blank_chararray)
		{
			std::vector<std::string> actual{ "12","345","5678","7" }, expected{};
			expected.resize(actual.size());
			for (size_t i = 0; i < expected.size(); i++){
				expected[i].assign(actual[i].size(), 0);
			}

			auto actual_ptr = estd::AllocateArrays().blank(actual);
			Assert::IsNotNull(actual_ptr);
			for (size_t i = 0; i < expected.size(); i++) {
				for (size_t j = 0; j < expected[i].size(); j++) {
					Assert::AreEqual(actual_ptr[i][j], expected[i][j]);
				}
				delete[] actual_ptr[i];
			}
			delete[] actual_ptr;
		}
		TEST_METHOD(T3_blank_wchararray)
		{
			std::vector<std::wstring> actual{ L"12",L"345",L"5678",L"7" }, expected{};
			expected.resize(actual.size());
			for (size_t i = 0; i < expected.size(); i++) {
				expected[i].assign(actual[i].size(), 0);
			}

			auto actual_ptr = estd::AllocateArrays().blank(actual);
			Assert::IsNotNull(actual_ptr);
			for (size_t i = 0; i < expected.size(); i++) {
				for (size_t j = 0; j < expected[i].size(); j++) {
					Assert::AreEqual(actual_ptr[i][j], expected[i][j]);
				}
				delete[] actual_ptr[i];
			}
			delete[] actual_ptr;
		}

		TEST_METHOD(T4_filled_int)
		{
			std::vector<int> actual{ -1,-2,-3,-4,5,6,7,8 }, expected{};
			expected.assign(actual.begin(), actual.end());
			auto actual_ptr = estd::AllocateArrays().filled(actual);
			Assert::IsNotNull(actual_ptr);
			for (size_t i = 0; i < expected.size(); i++) {
				Assert::AreEqual(actual_ptr[i], expected[i]);
			}
			delete[] actual_ptr;
		}
		TEST_METHOD(T4_filled_uint)
		{
			std::vector<unsigned int> actual{ 1u,2u,3u,4u,5u,6u,7u,8u }, expected{};
			expected.assign(actual.begin(), actual.end());
			auto actual_ptr = estd::AllocateArrays().filled(actual);
			Assert::IsNotNull(actual_ptr);
			for (size_t i = 0; i < expected.size(); i++) {
				Assert::AreEqual(actual_ptr[i], expected[i]);
			}
			delete[] actual_ptr;
		}
		TEST_METHOD(T4_filled_char)
		{
			std::vector<char> actual{ '1','2','3','4','5','6','7','8' }, expected{};
			expected.assign(actual.begin(), actual.end());
			auto actual_ptr = estd::AllocateArrays().filled(actual);
			Assert::IsNotNull(actual_ptr);
			for (size_t i = 0; i < expected.size(); i++) {
				Assert::AreEqual(actual_ptr[i], expected[i]);
			}
			delete[] actual_ptr;
		}
		TEST_METHOD(T4_filled_wchar)
		{
			std::vector<wchar_t> actual{ L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8' }, expected{};
			expected.assign(actual.begin(), actual.end());
			auto actual_ptr = estd::AllocateArrays().filled(actual);
			Assert::IsNotNull(actual_ptr);
			for (size_t i = 0; i < expected.size(); i++) {
				Assert::AreEqual(actual_ptr[i], expected[i]);
			}
			delete[] actual_ptr;
		}

		TEST_METHOD(T5_filled_chararray)
		{
			std::vector<std::string> actual{ "12","345","5678","7" }, expected{};
			expected.resize(actual.size());
			for (size_t i = 0; i < expected.size(); i++) {
				expected[i].assign(actual[i].begin(), actual[i].end());
			}

			auto actual_ptr = estd::AllocateArrays().filled(actual);
			Assert::IsNotNull(actual_ptr);
			for (size_t i = 0; i < expected.size(); i++) {
				for (size_t j = 0; j < expected[i].size(); j++) {
					Assert::AreEqual(actual_ptr[i][j], expected[i][j]);
				}
				delete[] actual_ptr[i];
			}
			delete[] actual_ptr;
		}
		TEST_METHOD(T5_filled_wchararray)
		{
			std::vector<std::wstring> actual{ L"12",L"345",L"5678",L"7" }, expected{};
			expected.resize(actual.size());
			for (size_t i = 0; i < expected.size(); i++) {
				expected[i].assign(actual[i].begin(), actual[i].end());
			}

			auto actual_ptr = estd::AllocateArrays().filled(actual);
			Assert::IsNotNull(actual_ptr);
			for (size_t i = 0; i < expected.size(); i++) {
				for (size_t j = 0; j < expected[i].size(); j++) {
					Assert::AreEqual(actual_ptr[i][j], expected[i][j]);
				}
				delete[] actual_ptr[i];
			}
			delete[] actual_ptr;
		}
	};
}