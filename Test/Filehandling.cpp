#include <string>
#include <iostream>

//class DoubleOutput
//{
//public:
//	enum class OPType{Console, File, ConsoleFile, end};
//
//public:
//	DoubleOutput(): m_optype(OPType::Console) {}
//	DoubleOutput(OPType optype, std::wstring const & filename)
//		:m_optype(optype), m_filename(filename){}
//
//	template <typename T>
//	friend DoubleOutput & operator<<(T const& value)
//	{
//		//using namespace std;
//		if ((OPType::Console == m_optype) || (OPType::ConsoleFile == m_optype))
//			std::cout << value;
//		/*if ((OPType::File == m_optype) || (OPType::ConsoleFile == m_optype)){
//			wofstream logfile;
//			logfile.open(m_filename, wofstream::out | wofstream::ate | wofstream::app);
//			logfile << value;
//		}*/
//	}
//
//private:
//	std::wstring m_filename;
//	OPType m_optype;
//};
//
//int test_DoubleOutput()
//{
//	DoubleOutput double_output;
//	double_output << "console";
//	return 0;
//}