#include <functional>
#include <set>
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <cassert>
#include <vector>
#include <bitset>
#include <list>
#include <string>
#include <tuple>
#include <algorithm>

struct MenuItemId 
{
	unsigned int m_val;
};
inline bool operator<(MenuItemId const& lhs, MenuItemId const& rhs){
	return lhs.m_val < rhs.m_val;
}
inline bool operator==(MenuItemId const& lhs, MenuItemId const& rhs){
	return lhs.m_val == rhs.m_val;
}
inline bool operator!=(MenuItemId const& lhs, MenuItemId const& rhs){ return !operator==(lhs, rhs); }
inline bool operator>(MenuItemId const& lhs, MenuItemId const& rhs){ return !operator<(lhs, rhs); }

struct MenuItemInfo
{
	std::wstring longtext;
	std::wstring shorttext;
};

class MyMenu
{
public:	
	enum class DisplaySort{ ID, LongText, ShortText };
	enum class Flags{	Horizontal = 0, Vertical, 
						LongText, ShorTText,
						ShowHeading, NoShowHeading,
						Total };
	const MenuItemId InvalidId = MenuItemId{ 0 };
	const MenuItemInfo InvalidInfo = MenuItemInfo{ L"", L"" };
private:
	typedef std::bitset<static_cast<size_t>(Flags::Total)> FlagMask; 

public:
	MyMenu() : MyMenu(L"", { { InvalidId, InvalidInfo } }){};
	MyMenu(std::wstring const&, std::vector<std::pair<MenuItemId, MenuItemInfo>> const&, std::list<Flags> const& = {});
	~MyMenu();

	void	Add(MenuItemId const&, MenuItemInfo const&);
	void	Add(std::vector<std::pair<MenuItemId, MenuItemInfo>> const&);
	void	Delete(std::vector<MenuItemId> const&);
	void	SetFlags(std::list<Flags> const& flags = {});
	void	ModifyFlagMask(FlagMask &, std::list<Flags> const& = {});
	void	SetHeading(std::wstring const& heading = L"");

	MenuItemId Choice(DisplaySort = DisplaySort::ID, std::list<Flags> const& = {});

private:
	std::wstring				m_heading;
	std::map<MenuItemId, MenuItemInfo>		m_items;
	FlagMask					m_display_options;

private:
	inline std::wstring separator(){ return (L"-----------------------"); }
};


MenuItemId MyMenu::Choice(DisplaySort display_sort, std::list<Flags> const& flags)
{
	using namespace std;

	FlagMask display_options{ m_display_options };
	ModifyFlagMask(display_options, flags);

	bool show_heading = display_options[static_cast<size_t>(Flags::ShowHeading)];
	bool show_largetext = display_options[static_cast<size_t>(Flags::LongText)];
	bool show_vertical = display_options[static_cast<size_t>(Flags::Vertical)];

	if (show_heading && !m_heading.empty())
		wcout << m_heading << endl;

	typedef tuple<unsigned int, wstring, MenuItemId> display_item;
	vector<display_item> display_items;

	unsigned int cnt = 1;
	for (auto const& it : m_items){
		auto id = it.first;
		display_items.push_back(make_tuple(
			cnt++, (show_largetext ? it.second.longtext : it.second.shorttext), it.first));
	}

	switch (display_sort)
	{
	case MyMenu::DisplaySort::LongText:
	case MyMenu::DisplaySort::ShortText:
		sort(display_items.begin(), display_items.end(),
			[](display_item const & first, display_item const & second) {
			return (get<1>(first) < get<1>(second)); });
		break;
	default:
		break;
	}

	for (auto const& it : display_items){
		wcout << to_wstring(get<0>(it)) << L". " << get<1>(it);
		if (show_vertical)	wcout << endl;
		else wcout << L" ";
	}
	unsigned int choice = 0;
	if (show_vertical)
		wcout << L"Enter choice : ";
	else
		wcout << endl << L"Enter choice : ";

	wcin >> choice;
	for (auto const& it : display_items){
		if (get<0>(it) == choice){
			choice = get<0>(it); break;
		}
	}
	return { choice };
}

MyMenu::MyMenu(
	std::wstring const& heading, std::vector<std::pair<MenuItemId, MenuItemInfo>> const& items, std::list<Flags> const& flags)
{
	m_heading = m_heading;
	Add(items);
	m_display_options.reset();
	SetFlags({ Flags::Horizontal, Flags::LongText, Flags::ShowHeading });
	SetFlags(flags);
}

MyMenu::~MyMenu(){
}

void MyMenu::Add(MenuItemId const& id, MenuItemInfo const& item){
	Add({ { id, item } });
}
void MyMenu::Add(std::vector<std::pair<MenuItemId, MenuItemInfo>> const& items){
	for (auto const& it : items){
		if (it.first != InvalidId)
			m_items.insert(std::make_pair(it.first, it.second));
	}
}

void MyMenu::Delete(std::vector<MenuItemId> const& item_ids)
{
	for(auto & it : item_ids){
		for (auto iter = m_items.begin(); iter != m_items.end();){
			if ((*iter).first == it){
				iter = m_items.erase(iter);
			}
			else{
				iter++;
			}
		}
	}
}

void MyMenu::SetFlags(std::list<Flags> const& flags)
{
	ModifyFlagMask(m_display_options, flags);
}

void MyMenu::ModifyFlagMask(FlagMask & MaskVar, std::list<Flags> const& NewFlags)
{
	for (auto const& it : NewFlags)
	{
		switch (it)
		{
		case Flags::Horizontal:
			MaskVar.set(static_cast<size_t>(Flags::Horizontal));
			MaskVar.reset(static_cast<size_t>(Flags::Vertical)); break;
		case Flags::Vertical:
			MaskVar.set(static_cast<size_t>(Flags::Vertical)); 
			MaskVar.reset(static_cast<size_t>(Flags::Horizontal)); break;
		case Flags::LongText:
			MaskVar.set(static_cast<size_t>(Flags::LongText));
			MaskVar.reset(static_cast<size_t>(Flags::ShorTText)); break;
		case Flags::ShorTText:
			MaskVar.set(static_cast<size_t>(Flags::ShorTText)); 
			MaskVar.reset(static_cast<size_t>(Flags::LongText)); break;
		case Flags::ShowHeading:
			MaskVar.set(static_cast<size_t>(Flags::ShowHeading));
			MaskVar.reset(static_cast<size_t>(Flags::NoShowHeading)); break;
		case Flags::NoShowHeading:
			MaskVar.set(static_cast<size_t>(Flags::NoShowHeading));
			MaskVar.reset(static_cast<size_t>(Flags::ShowHeading)); break;
		default:
			break;
		}
	}
}

void MyMenu::SetHeading(std::wstring const& heading){
	m_heading = heading;
}

void test_mymenu()
{
	MyMenu my_menu;
	my_menu.Add({ 1 }, { L"Item1_long", L"item1_short" });
	my_menu.Add({ 
		std::make_pair(MenuItemId{ 2 }, MenuItemInfo{ L"Item2_long", L"item2_short" }),
		std::make_pair(MenuItemId{ 3 }, MenuItemInfo{ L"Item3_long", L"item3_short" }),
		std::make_pair(MenuItemId{ 4 }, MenuItemInfo{ L"Exit", L"Ex" }),
		}
	);
	auto ret = my_menu.Choice();
	std::wcout << L"Choice is " << ret.m_val << std::endl;
	getchar();
	
	my_menu.SetHeading(L"Test mymenu");
	ret = my_menu.Choice(MyMenu::DisplaySort::ID, 
		{ MyMenu::Flags::Vertical, MyMenu::Flags::ShowHeading, MyMenu::Flags::ShorTText });
	std::wcout << L"Choice is " << ret.m_val << std::endl;
	getchar();

	return;
}

///////////////////////////////////////////////////////////////////////////////
template<typename T>
struct MenuItem
{

	unsigned int	Id = INT_MAX;
	std::wstring	IdText = L"";
	std::wstring	Text = L"";
	T				EnumType;

	MenuItem(){};
	MenuItem(T Type, unsigned int MenuId, const std::wstring& MenuTextId, const std::wstring& MenuText) :
		EnumType(Type), Id(MenuId), IdText(MenuTextId), Text(MenuText){	};
	~MenuItem(){};

	bool operator <(const MenuItem & second){
		return(Id < second.Id);
	}
	bool operator ==(const MenuItem & second){
		return((Id == second.Id) && (IdText == second.IdText) && (Text == second.Text) && (EnumType == second.EnumType));
	}
};

enum class EnumTest1
{
	START,
	ITEM1 = START, ITEM2, ITEM3,
	END
};
enum class EnumTest2
{
	START,
	ITEM1 = START, ITEM2, ITEM3,
	END
};

void TestEnum2()
{
	using MenuItem1 = MenuItem < EnumTest1 > ;
	MenuItem1 EnumTest1Menu1;
	EnumTest1Menu1.EnumType = EnumTest1::ITEM1;
	EnumTest1Menu1.Id = 1;
	EnumTest1Menu1.IdText = L"I1";
	EnumTest1Menu1.Text = L"Item1";
	MenuItem1 EnumTest1Menu2(EnumTest1::ITEM2, 2, L"I2", L"Item2");

	typedef std::map <EnumTest1, MenuItem1> MAPV;
	MAPV mapv;
	mapv.insert(MAPV::value_type(EnumTest1::ITEM1, EnumTest1Menu1));
	mapv.insert(MAPV::value_type(EnumTest1::ITEM2, EnumTest1Menu2));
	std::wcout << L"map" << std::endl;
	for (auto &it: mapv)
	{
		std::wcout << it.second.Id << L" " << it.second.IdText << L" " << it.second.Text << std::endl;
	}

	std::vector<MenuItem1> vec1;
	vec1.push_back(MenuItem1(EnumTest1::ITEM1, 3, L"I3", L"Item3"));
	vec1.push_back(MenuItem1(EnumTest1::ITEM1, 1, L"I1", L"Item1"));
	vec1.push_back(MenuItem1(EnumTest1::ITEM1, 4, L"I4", L"Item4"));
	vec1.push_back(MenuItem1(EnumTest1::ITEM2, 2, L"I2", L"Item2"));
	std::wcout << L"Vector" << std::endl;
	for (auto &it : vec1)
	{
		std::wcout << it.Id << L" " << it.IdText << L" " << it.Text << std::endl;
	}
	std::sort(vec1.begin(), vec1.end());
	std::wcout << L"Sorted Vector" << std::endl;
	for (auto &it : vec1)
	{
		std::wcout << it.Id << L" " << it.IdText << L" " << it.Text << std::endl;
	}
}

template<typename T>
struct enum_identity 
{
	typedef T type;
};

namespace details 
{
	void begin();
	void end();
}

template<typename Enum>
struct enum_iterator : std::iterator<std::bidirectional_iterator_tag, Enum> 
{
	enum_iterator() :c(end()) { }

	enum_iterator(Enum c) :c(c) {
		assert(c >= begin() && c <= end());
	}

	enum_iterator &operator=(Enum c) {
		assert(c >= begin() && c <= end());
		this->c = c;
		return *this;
	}

	static Enum begin() {
		using details::begin; // re-enable ADL
		return begin(enum_identity<Enum>());
	}

	static Enum end() {
		using details::end; // re-enable ADL
		return end(enum_identity<Enum>());
	}

	enum_iterator &operator++() {
		assert(c != end() && "incrementing past end?");
		c = static_cast<Enum>(c + 1);
		return *this;
	}

	enum_iterator operator++(int) {
		assert(c != end() && "incrementing past end?");
		enum_iterator cpy(*this);
		++*this;
		return cpy;
	}

	enum_iterator &operator--() {
		assert(c != begin() && "decrementing beyond begin?");
		c = static_cast<Enum>(c - 1);
		return *this;
	}

	enum_iterator operator--(int) {
		assert(c != begin() && "decrementing beyond begin?");
		enum_iterator cpy(*this);
		--*this;
		return cpy;
	}

	Enum operator*() {
		assert(c != end() && "cannot dereference end iterator");
		return c;
	}

	Enum get_enum() const {
		return c;
	}

private:
	Enum c;
};

template<typename Enum>
bool operator==(enum_iterator<Enum> e1, enum_iterator<Enum> e2) {
	return e1.get_enum() == e2.get_enum();
}

template<typename Enum>
bool operator!=(enum_iterator<Enum> e1, enum_iterator<Enum> e2) {
	return !(e1 == e2);
}

enum Color 
{
	Color_Begin,
	Color_Red = Color_Begin,
	Color_Orange,
	Color_Yellow,
	Color_Green,
	Color_Blue,
	Color_Indigo,
	Color_Violet,
	Color_End
};
Color begin(enum_identity<Color>){
	return Color::Color_Begin;
}
Color end(enum_identity<Color>){
	return Color::Color_End;
}

std::map < Color, std::wstring> mapc =
{
	{ Color::Color_Blue, L"Blue" },
	{ Color::Color_Red, L"Red" }
};

//namespace std 
//{
//	template<>
//	struct iterator_traits<Color>  
//	{
//		typedef Color  value_type;
//		typedef int    difference_type;
//		typedef Color *pointer;
//		typedef Color &reference;
//		typedef std::bidirectional_iterator_tag iterator_category;
//	};
//}
//
//Color &operator++(Color &c) 
//{
//	assert(c != Color_End);
//	c = static_cast<Color>(c + 1);
//	return c;
//}
//
//Color operator++(Color &c, int) 
//{
//	assert(c != Color_End);
//	++c;
//	return static_cast<Color>(c - 1);
//}
//
//Color &operator--(Color &c) 
//{
//	assert(c != Color_Begin);
//	return c = static_cast<Color>(c - 1);
//}
//
//Color operator--(Color &c, int) 
//{
//	assert(c != Color_Begin);
//	--c;
//	return static_cast<Color>(c + 1);
//}
//
//Color operator*(Color c) 
//{
//	assert(c != Color_End);
//	return c;
//}


void print(Color c)
{
	std::wcout << (mapc.count(c) > 0 ? mapc.at(c) : L"NA") << std::endl;
	/*if (mapc.count(c) > 0)
		std::wcout << mapc.at(c) << std::endl;*/
	//std::cout << c << std::endl;
}

void TestEnum1() 
{
	//std::for_each(Color_Begin, Color_End, &print);
	enum_iterator<Color> b = Color::Color_Begin, e;
	while (b != e)
		print(*b++);
}


// Generic enumerator implementation
template <class T>
class Enum 
{ /// http://www.drdobbs.com/when-enum-just-isnt-enough-enumeration-c/184403955
protected:
	// Constructors
	explicit Enum(int Value);
	
private:
	// Predicate for finding the corresponding instance
	struct Enum_Predicate_Corresponds :
		public std::unary_function<const Enum<T>*, bool> 
	{
		Enum_Predicate_Corresponds(int Value) : m_value(Value) { }
		bool operator()(const Enum<T>* E)
		{
			return E->Get_Value() == m_value;
		}
	private:
		const int m_value;
	};
	
	// Comparison functor for the set of instances
	struct Enum_Ptr_Less :
		public std::binary_function<const Enum<T>*, const Enum<T>*, bool> 
	{
		bool operator()(const Enum<T>* E_1, const Enum<T>* E_2)
		{
			return E_1->Get_Value() < E_2->Get_Value();
		}
	};

public:
	// Compiler-generated copy constructor and operator= are OK.
	typedef std::set<const Enum<T>*, Enum_Ptr_Less> instances_list;
	typedef typename instances_list::const_iterator const_iterator;
	
	// Access to int value
	int Get_Value(void) const { return m_value; }
	
	static int Min(void) { return (*s_instances.begin())->m_value; }
	static int Max(void) { return (*s_instances.rbegin())->m_value; }
	
	static const Enum<T>* Corresponding_Enum(int Value)
	{
		const_iterator it = find_if(s_instances.begin(), s_instances.end(),
			Enum_Predicate_Corresponds(Value));
		return (it != s_instances.end()) ? *it : NULL;
	}
	
	static bool Is_Valid_Value(int Value) { return Corresponding_Enum(Value) != NULL; }
	
	// Number of elements
	static typename instances_list::size_type size(void) {
		return s_instances.size();
	}
	
	// Iteration
	static const_iterator begin(void) { return s_instances.begin(); }
	static const_iterator end(void) { return s_instances.end(); }

private:
	int m_value;
	static instances_list s_instances;
};

template <class T> inline 
Enum<T>::Enum(int Value) : m_value(Value)
{
	s_instances.insert(this);
}


//////////////////////////////////////
// Testing generic enum implementation
class Test_Enum : public Enum<Test_Enum> 
{
private:
	explicit Test_Enum(int Value) : Enum<Test_Enum>(Value) { }

public:
	static const Test_Enum enum_11;
	static const Test_Enum enum_12;
	static const Test_Enum enum_18;
	static const Test_Enum enum_20;
	static const Test_Enum enum_21;
};

Enum<Test_Enum>::instances_list Enum<Test_Enum>::s_instances;

const Test_Enum Test_Enum::enum_11(11);
const Test_Enum Test_Enum::enum_12(12);
const Test_Enum Test_Enum::enum_18(18);
const Test_Enum Test_Enum::enum_20(20);
const Test_Enum Test_Enum::enum_21(21);

/*
std::wostream& operator<<(std::wostream& out, const Test_Enum value)
{
	typedef std::map<Test_Enum, std::wstring> MAPV;
	static MAPV mapv;
	if (mapv.size() == 0)
	{
		#define INSERT_ELEMENT(p) strings[p] = #p
		INSERT_ELEMENT(Test_Enum::enum_11);
		INSERT_ELEMENT(Test_Enum::enum_11);
		INSERT_ELEMENT(Test_Enum::enum_11);
		#undef INSERT_ELEMENT
		mapv.insert(MAPV::value_type(Test_Enum(Test_Enum::enum_11), L"11"));
		
	}

	return out << mapv[value];
}
*/

void TestGenericEnum()
{
	using std::cout;
	using std::endl;
	int Cur_Elem = 0;
	for (Test_Enum::const_iterator i = Test_Enum::begin();
		i != Test_Enum::end(); ++i)
	{
		Cur_Elem++;
		cout << "Test_Enum element #" << Cur_Elem << " value = "
			<< (*i)->Get_Value() << endl;
	}

	cout << "Total #elements = " << Test_Enum::size() << endl;
	cout << "Min enum value = " << Test_Enum::Min() << endl;
	cout << "Max enum value = " << Test_Enum::Max() << endl;

	for (int i = Test_Enum::Min(); i <= Test_Enum::Max(); ++i)
	{
		cout << i;
		if (Test_Enum::Is_Valid_Value(i)) cout << " is ";
		else cout << " isn't ";
		cout << "a valid value for Test_Enum." << endl;
	}

	Test_Enum E(Test_Enum::enum_11);
	cout << "Value of E = " << E.Get_Value() << endl;

	E = Test_Enum::enum_20;
	cout << "Value of E = " << E.Get_Value() << endl;

	// Illegal code
	// bool B = E; // won't compile, illegal implicit conversion
	// E++; // won't compile, cannot treat Test_Enum as an int
	// Test_Enum X(17); // won't compile, ctor is private

	return;
}



