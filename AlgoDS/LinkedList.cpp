#include "stdafx.h"
#include <cassert>
#include "CommonDefs.h"
#include "TestHarness.h"
#include "Utilities.h"

using std::wcout;
using std::endl;

///////////////////////////////////////////////////////////////////////////////
class LinkedList
{
public:
	struct node {
		int data;
		struct node* next;
	};

	LinkedList(struct node *a_head=NULL): head(a_head){}

	// Basic Utility functions
	int Length();
	static int Length(struct node* head)
	{ LinkedList LL(head); return LL.Length(); }

	void Push(int newData);
	static void Push(struct node** headRef, int newData)
	{ LinkedList LL(*headRef); LL.Push(newData); }

	void BuildOneTwoThree(int i);
	static struct LinkedList::node* BuildOneTwoThree()
	{ LinkedList LL; LL.BuildOneTwoThree(0); return LL.head; }
		
	std::wstring GetList(); 
	static std::wstring GetList(struct node* head)
	{ LinkedList LL(head); return LL.GetList(); }
	//Given an int and a reference to the head pointer (i.e. a struct
	// node** pointer to the head pointer), add a new node at the head of the list
	//void Push(struct node** headRef, int newData); 
	void AddAtHead(int i);
	static struct node* AddAtHead()
	{LinkedList LL; LL.AddAtHead(0); return LL.head; }

	void BuildWithSpecialCase(int i);
	static struct node* BuildWithSpecialCase()
	{  LinkedList LL; LL.BuildWithSpecialCase(0); return LL.head; }

	void BuildWithDummyNode(int i); 
	static struct node* BuildWithDummyNode()
	{  LinkedList LL; LL.BuildWithDummyNode(0); return LL.head; }

	void BuildWithLocalRef(int i);
	static struct node* BuildWithLocalRef()
	{  LinkedList LL; LL.BuildWithLocalRef(0); return LL.head; }

	int Count(int searchFor);
	static int Count(struct node* head, int searchFor)
	{  LinkedList LL(head); return LL.Count(searchFor); }

	int GetNth(int index);
	static int GetNth(struct node* head, int searchFor)
	{  LinkedList LL(head); return LL.GetNth(searchFor); }

	void DeleteList();
	static void DeleteList(struct node** headRef)
	{  LinkedList LL(*headRef); LL.DeleteList(); *headRef = LL.head; }

	int Pop();
	static int Pop(struct node** headRef)
	{  LinkedList LL(*headRef); int i = LL.Pop(); *headRef = LL.head; return i;}

	void InsertNth(int index, int data);
	static void InsertNth(struct node** headRef, int index, int data)
	{  LinkedList LL(*headRef); LL.InsertNth(index, data); *headRef = LL.head; }

private:
	struct node * head;
};

///////////////////////////////////////////////////////////////////////////////
/*
Return the number of nodes in a list (while-loop version)
*/
int LinkedList::Length() 
{
	int count = 0;
	struct node* current = head;
	while (NULL != current) {
		count++;
		current = current->next;
	}
	return(count);
}

/*
Build and return the list {1, 2, 3}
*/
void LinkedList::BuildOneTwoThree(int i) 
{
	//struct node* head = NULL; // Start with the empty list
	//Push(&head, 3); // Use Push() to add all the data
	//Push(&head, 2);
	//Push(&head, 1);

	Push(3); // Use Push() to add all the data
	Push(2);
	Push(1);
	return;
}

/*
Given a reference (pointer to pointer) to the head
of a list and an int, push a new node on the front of the list.
Creates a new node with the int, links the list off the .next of the
new node, and finally changes the head to point to the new node.
*/
void LinkedList::Push(int newData) {
	struct node* newNode =
		(struct node*) malloc(sizeof(struct node)); // allocate node
	newNode->data = newData; // put in the data
	newNode->next = head; // link the old list off the new node
	head = newNode; // move the head to point to the new node
}

/*
Displays list members in order from head to tail
*/
std::wstring LinkedList::GetList()
{
	std::wstring temp = L"{ ";
	struct node* current = head;
	while (NULL != current) {
		temp += L" ";
		current = current->next;
	}
	return ( temp += L"}" );
}

/*
	Build — At Head With Push()
*/
void LinkedList::AddAtHead(int dummy) 
{
	head = NULL;
	int i;
	for (i=1; i<6; i++) {
		Push(&head, i);
	}
	// head == {5, 4, 3, 2, 1};
	//return(head);
}

/*
Special code for head node.
*/
void LinkedList::BuildWithSpecialCase(int dummy) 
{
	head = NULL;
	struct node* tail;
	int i;
	// Deal with the head node here, and set the tail pointer
	Push(&head, 1);
	tail = head;
	// Do all the other nodes using 'tail'
	for (i=2; i<6; i++) {
		Push(&(tail->next), i); // add node at tail->next
		tail = tail->next; // advance tail to point to last node
	}
	//return(head); // head == {1, 2, 3, 4, 5};
}

/*
Use a temporary dummy node at the head of the list during the computation.
*/
void LinkedList::BuildWithDummyNode(int dummy_val) 
{
	struct node dummy; // Dummy node is temporarily the first node
	struct node* tail = &dummy; // Start the tail at the dummy.
	// Build the list on dummy.next (aka tail->next)
	int i;
	dummy.next = NULL;
	for (i=1; i<6; i++) {
		Push(&(tail->next), i);
		tail = tail->next;
	}
	// The real result list is now in dummy.next
	// dummy.next == {1, 2, 3, 4, 5};
	head = dummy.next;
}

/*
unify all the node cases without using a dummy node at all. 
For this technique, we use a local "reference pointer" which always points to the last
pointer in the list instead of to the last node.
*/
void LinkedList::BuildWithLocalRef(int dummy) 
{
	head = NULL;
	struct node** lastPtrRef= &head; // Start out pointing to the head pointer
	int i;
	for (i=1; i<6; i++) {
		Push(lastPtrRef, i); // Add node at the last pointer in the list
		lastPtrRef= &((*lastPtrRef)->next); // Advance to point to the
		// new last pointer
	}
	// head == {1, 2, 3, 4, 5};
	//return(head);
}

// ========================================================================
// Section 2
// ========================================================================
/*
Given a list and an int, return the number of times that int occurs
in the list.
*/
int LinkedList::Count(int searchFor) 
{
	int count = 0;
	struct node* current = head;
	while (NULL != current) {
		if (searchFor == current->data) count++;
		current = current->next;
	}
	return(count);
}
void CountTest() 
{
	using std::wcout;
	using std::endl;
	wcout << L"CountTest" << endl;
	struct LinkedList::node * myList = LinkedList::BuildOneTwoThree(); // build {1, 2, 3}
	int count = LinkedList::Count(myList, 2); // returns 1 since there's 1 '2' in the list
	wcout << L"Number of 2s in List is " << count << endl;
}

// Given a list and an index, return the data
// in the nth node of the list. The nodes are numbered from 0.
// Assert fails if the index is invalid (outside 0..lengh-1).
int LinkedList::GetNth(int index)
{
	assert(index >= 0);
	int count = 0;
	struct node* current = head;
	while (NULL != current) {
		if (index == count++) break;
		current = current->next;
	}
	assert(NULL != current);
	return (current->data);
}
void GetNthTest() 
{
	using std::wcout;
	using std::endl;

	wcout << L"GetNthTest" << endl;
	struct LinkedList::node * myList = LinkedList::BuildOneTwoThree(); // build {1, 2, 3}
	int lastNode = LinkedList::GetNth(myList, 2); // returns the value 3
	wcout << L"Data @ location 3 in List is " << lastNode << endl;
}

// Takes a list, deallocates all of its memory and sets its
// head pointer to NULL (the empty list)
void LinkedList::DeleteList()
{
	struct node* temp = head;
	while (NULL != temp)
	{
		struct node* temp1 = temp->next;
		free(temp);
		temp = temp1;
	}
	head = temp;
}

void DeleteListTest() 
{
	wcout << L"DeleteListTest" << endl;
	struct LinkedList::node * myList = LinkedList::BuildOneTwoThree(); // build {1, 2, 3}
	LinkedList::DeleteList(&myList); // deletes the three nodes and sets myList to NULL
	wcout << L"New List is " << LinkedList::GetList(myList).c_str() << endl;
}

/*
The opposite of Push(). Takes a non-empty list
and removes the front node, and returns the data
which was in that node.
*/
int LinkedList::Pop()
{
	assert(NULL != head);
	struct node* temp = (head->next);
	int data = (head->data);
	free (head);
	head = temp;
	return data;
}

void PopTest() 
{
	wcout << L"PopTest" << endl;
	struct LinkedList::node * head = LinkedList::BuildOneTwoThree(); // build {1, 2, 3}
	wcout << L"Popped " << LinkedList::Pop(&head) << endl; // deletes "1" node and returns 1
	wcout << L"Popped " << LinkedList::Pop(&head) << endl; // deletes "2" node and returns 2
	wcout << L"Popped " << LinkedList::Pop(&head) << endl; // deletes "3" node and returns 3
	wcout << L"List is now " << LinkedList::GetList(head).c_str() << L" Of length "
		<< LinkedList::Length(head) << endl;
}

/*
A more general version of Push().
Given a list, an index 'n' in the range 0..length,and a data element, 
add a new node to the list so that it has the given index.
*/
void LinkedList::InsertNth(int index, int data) 
{
	assert(index >= 0 && index <= Length());
	struct node* temp = (struct node*)malloc (sizeof(struct node*));
	assert (NULL != temp);
	temp->data = data;
	temp->next = NULL;

	int count = -1;
	struct node* iter = head;
	while (NULL != iter) {
		if (count+1 == index) break; 
		count++;
		iter = iter->next;
	}
}
void InsertNthTest() 
{
	wcout << L"InsertNthTest" << endl;
	struct LinkedList::node* head = NULL; // start with the empty list
	LinkedList::InsertNth(&head, 0, 13); // build {13)
	LinkedList::InsertNth(&head, 1, 42); // build {13, 42}
	LinkedList::InsertNth(&head, 1, 5); // build {13, 5, 42}
	LinkedList::DeleteList(&head); // clean up after ourselves
}

///////////////////////////////////////////////////////////////////////////////
STATUS_GEN
LinkedList_Test::Run()
{
	STATUS_GEN status = STATUS_GEN::BAD_INPUT;
	switch (m_module_data.runtype)
	{
	case RUNTYPE::ConsoleOps:
	{
		wchar_t dowerepeat = L'y';
		do
		{
			status = RunConsole();
			utils::clear_wcin();
			std::wcout << MenuMsgStr(m_module_data.majorops, MenuMsgOpts::REPEAT_MAJOROP);
			std::wcin >> dowerepeat;
		} while (L'y' == dowerepeat || L'Y' == dowerepeat);
	}
	break;
	case RUNTYPE::FileInput: status = TestBase::RunFI(); break;
	case RUNTYPE::CommandLine: status = TestBase::RunCL(); break;
	default:
		break;
	}
	return status;
}
STATUS_GEN
LinkedList_Test::Run(ModuleData module_data)
{
	m_module_data = module_data;
	return Run();
}

STATUS_GEN
LinkedList_Test::RunConsole()
{
	CountTest();
	GetNthTest();
	DeleteListTest();
	PopTest();
	InsertNthTest();
	return STATUS_GEN::SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
void TestLinkedList(WSTRING_VEC args)
{
	CountTest();
	GetNthTest();
	DeleteListTest();
	PopTest();
	InsertNthTest();
	return;
}