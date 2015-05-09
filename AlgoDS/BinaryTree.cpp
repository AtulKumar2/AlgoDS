#include "stdafx.h"
#include <cassert>
#include <iostream>
#include "CommonDefs.h"
#include "TestHarness.h"
#include "Utilities.h"

using std::wcout;
using std::endl;

///////////////////////////////////////////////////////////////////////////////
class BST
{
	//http://cslibrary.stanford.edu/110/BinaryTrees.html
public:
	struct node 
	{ 
		int data; 
		struct node* left; 
		struct node* right; 
	};

	static int lookup(struct node* node, int target);
	struct node* NewNode(int data);
	struct node* insert(struct node* node, int data);
	struct node* build123(wchar_t implementation='a');
	int size(struct node* node);
	int maxDepth(struct node* node);
	int minValue(struct node* node);
	int maxValue(struct node* node); 
	void printTree(struct node* node);
	void printPostorder(struct node* node);
	void printPreorder(struct node* node);

	struct node * gettree_from_preorder_and_inorder
		(const std::list<int> &preorder, const std::list<int> &inorder);
	struct node * gettree_from_postorder_and_inorder
		(const std::list<int> &postorder, const std::list<int> &inorder);
};

///////////////////////////////////////////////////////////////////////////////
/* 
Given a BST tree, return true if node is found in the tree. Recurs 
down the tree, chooses the left or right branch by comparing the target to each node. 
*/ 
int BST::lookup(struct node* node, int target) 
{ 
	if (node == NULL) { 
		return(false); 
	} else { 
		if (target == node->data) return(true); 
		else { 
			// otherwise recur down the correct subtree 
			if (target < node->data) return(lookup(node->left, target)); 
			else return(lookup(node->right, target)); 
		} 
	} 
} 

/* 
Prepares a new node.
*/ 
struct BST::node* 
BST::NewNode(int data) 
{ 
	struct node* node = new(struct node);    // "new" is like "malloc" 
	assert (node != NULL);
	node->data = data; 
	node->left = node->right = NULL; 
	return(node); 
} 

/* 
Give a BST and a number, inserts new node in the correct place in the tree. 
Returns the new root pointer which the caller should then use 
(the standard trick to avoid using reference parameters). 
*/ 
struct BST::node* 
BST::insert(struct node* node, int data) 
{ 
	if (node == NULL) { 
		return(NewNode(data)); 
	} else { 
		// 2. Otherwise, recur down the tree 
		if (data <= node->data) node->left = insert(node->left, data); 
		else node->right = insert(node->right, data); 

		return(node); // return the (unchanged) node pointer 
	} 
} 

/*
Write the code in three different ways... 
	a: by calling newNode() three times, and using three pointer variables 
	b: by calling newNode() three times, and using only one pointer variable 
	c: by calling insert() three times passing it the root pointer to build up the tree 
*/
struct BST::node* 
BST::build123(wchar_t implementation)
{
	assert (L'a' <= implementation && implementation <= L'c');
	struct node * root = NULL;
	if (L'a' == implementation){
		struct node * temp1 = NewNode(1);
		struct node * temp2 = NewNode(2);
		struct node * temp3 = NewNode(3);
		temp2->left = temp1;
		temp2->right = temp3;
		root = temp2;
	} 
	if(L'b' == implementation){
		root = NewNode(2);
		root->left = NewNode(1);
		root->right = NewNode(3);
	}
	if(L'c' == implementation){
		root = insert(NULL, 2);
		root = insert(root, 3);
		root = insert(root, 2);
	}
	return root;
}

/*
Given a binary tree, count the number of nodes in the tree. 
*/
int BST::size(struct node* node)
{
	if (NULL == node){
		return 0;
	}
	return( size(node->left) + size(node->right) + 1 );
}

/*
Given a binary tree, compute its "maxDepth" -- the number of nodes along 
the longest path from the root node down to the farthest leaf node. 
*/
int BST::maxDepth(struct node* node)
{
	if (NULL == node){
		return 0;
	}

	int l_maxDepth = NULL != node->left ? maxDepth(node->left) : 0;
	int r_maxDepth = NULL != node->right ? maxDepth(node->right) : 0;

	return ( 1 + (l_maxDepth <= r_maxDepth ? l_maxDepth: r_maxDepth));
}

/*
Given a non-empty binary search tree (an ordered binary tree), 
return the minimum data value found in that tree
*/
int BST::minValue(struct node* node)
{
	assert (NULL != node);
	struct node * temp = node;
	while (NULL != temp->left){
		temp = temp->left;
	}
	return temp->data;
}
/*
Given a non-empty binary search tree (an ordered binary tree), 
return the maximum data value found in that tree
*/
int BST::maxValue(struct node* node)
{
	assert (NULL != node);
	struct node * temp = node;
	while (NULL != temp->right){
		temp = temp->right;
	}
	return temp->data;
}

/*
Given a BST (aka an "ordered binary tree"), in "inorder", 
iterate over the nodes to print them out in increasing order.
*/
void BST::printTree(struct node* node)
{
	if (NULL == node){
		return;
	}
	printTree(node->left);
	std::wcout << node->data << L" ";
	printTree(node->right);
}
/*
Given a binary tree, print out the nodes of the tree according to a bottom-up "postorder" traversal 
*/
void BST::printPostorder(struct node* node)
{
	if (NULL == node){
		return;
	}
	printPostorder(node->left);
	printPostorder(node->right);
	std::wcout << node->data << L" ";
}
	
	/*
Given a binary tree, print out the nodes of the tree according to a root-first "preorder" traversal 
*/
void BST::printPreorder(struct node* node)
{
	if (NULL == node){
		return;
	}
	std::wcout << node->data << L" ";
	printPreorder(node->left);
	printPreorder(node->right);
}

/*
http://www.coders2020.com/construct-a-tree-given-its-inorder-and-preorder-traversal-strings-similarly-construct-a-tree-given-its-inorder-and-post-order
Construct a tree given its inorder and preorder traversal strings.
*/
using std::list;
using std::find;

struct BST::node * BST::gettree_from_preorder_and_inorder(
	const list<int> &preorder, const list<int> &inorder)
{
	struct node *root = NULL;
	if(!(preorder.size() && inorder.size()))
		return root;
				
	root = NewNode(*(preorder.begin()));
	for (list<int>::const_iterator iter= (++preorder.begin()); iter != preorder.end(); iter++)
	{
		struct node *pivot = root;
		list<int>::const_iterator pivot_loc, node_loc;
		do{
			pivot_loc = find(inorder.begin(), inorder.end(), pivot->data);
			node_loc = find(inorder.begin(), pivot_loc, *iter);
			if (node_loc != pivot_loc){ //Left tree
				if (NULL == pivot->left){
					pivot->left = NewNode(*iter);
					break;
				}else{
					pivot = pivot->left;
				}
			}else{ //Right tree
				node_loc = find(pivot_loc, inorder.end(), *iter);
				if (NULL == pivot->right){
					pivot->right = NewNode(*iter);
					break;
				}else{
					pivot = pivot->right;
				}
			}
		}while(true);
	}
	return root;
}

void test_gettree_from_preorder_and_inorder()
{
	wcout << L"test_gettree_from_preorder_and_inorder" << endl;
	// inorder = g d h b e i a f j c
	int temp[10] = {7, 4, 8, 2, 5, 9, 1, 6, 10, 3};
	list<int> inorder(temp, temp+10);
	// preorder = a b d g h e i c f j
	int temp1[10] = {1, 2, 4, 7, 8, 5, 9, 3, 6, 10};
	list<int> preorder(temp1, temp1+10);
	// postorder = g h d i e b j f c a
	int temp2[10] = {7, 8, 4, 9, 5, 2, 10, 6, 3, 1};
	list<int> postorder(temp2, temp2+10);

	BST bst;
	struct BST::node *root = bst.gettree_from_preorder_and_inorder(preorder, inorder);
	wcout << L"InOrder "; bst.printTree(root); wcout << endl; 
	wcout << L"PostOrder "; bst.printPostorder(root); wcout << endl;
	wcout << L"PreOrder "; bst.printPreorder(root); wcout << endl;
	return;
}

/*
http://www.coders2020.com/construct-a-tree-given-its-inorder-and-preorder-traversal-strings-similarly-construct-a-tree-given-its-inorder-and-post-order
Construct a tree given its inorder and postorder traversal strings.
*/
struct BST::node * BST::gettree_from_postorder_and_inorder(
	const list<int> &postorder, const list<int> &inorder)
{
	struct node *root = NULL;
	if(!(postorder.size() && inorder.size()))
		return root;
				
	root = NewNode(*(--(postorder.end())));
	for (list<int>::const_iterator iter= (--(--postorder.end())); ; iter--)
	{
		struct node *pivot = root;
		list<int>::const_iterator pivot_loc, node_loc;
		do{
			pivot_loc = find(inorder.begin(), inorder.end(), pivot->data);
			node_loc = find(inorder.begin(), pivot_loc, *iter);
			if (node_loc != pivot_loc){ //Left tree
				if (NULL == pivot->left){
					pivot->left = NewNode(*iter);
					break;
				}else{
					pivot = pivot->left;
				}
			}else{ //Right tree
				node_loc = find(pivot_loc, inorder.end(), *iter);
				if (NULL == pivot->right){
					pivot->right = NewNode(*iter);
					break;
				}else{
					pivot = pivot->right;
				}
			}
		}while(true);

		if(iter == postorder.begin())
			break;
	}
	return root;
}

void test_gettree_from_postorder_and_inorder()
{
	wcout << L"test_gettree_from_postorder_and_inorder" << endl;
	// inorder = g d h b e i a f j c
	int temp[10] = {7, 4, 8, 2, 5, 9, 1, 6, 10, 3};
	list<int> inorder(temp, temp+10);
	// preorder = a b d g h e i c f j
	int temp1[10] = {1, 2, 4, 7, 8, 5, 9, 3, 6, 10};
	list<int> preorder(temp1, temp1+10);
	// postorder = g h d i e b j f c a
	int temp2[10] = {7, 8, 4, 9, 5, 2, 10, 6, 3, 1};
	list<int> postorder(temp2, temp2+10);

	BST bst;
	struct BST::node *root = bst.gettree_from_postorder_and_inorder(postorder, inorder);
	wcout << L"InOrder "; bst.printTree(root); wcout << endl; 
	wcout << L"PostOrder "; bst.printPostorder(root); wcout << endl;
	wcout << L"PreOrder "; bst.printPreorder(root); wcout << endl;
	return;
}

///////////////////////////////////////////////////////////////////////////////
STATUS_GEN
BinTree_Test::Run()
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
BinTree_Test::Run(ModuleData module_data)
{
	m_module_data = module_data;
	return Run();
}

STATUS_GEN
BinTree_Test::RunConsole()
{
	test_gettree_from_preorder_and_inorder();
	test_gettree_from_postorder_and_inorder();
	return STATUS_GEN::SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
void TestBinaryTree(WSTRING_VEC args)
{
	test_gettree_from_preorder_and_inorder();
	test_gettree_from_postorder_and_inorder();
	return;
}