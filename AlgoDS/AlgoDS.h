#ifndef __ALGODS_H
#define __ALGODS_H

#include "stdafx.h"

namespace AlgoDS
{
#define MOPS_NAME_LENGTH		128

#define MOPS_NAME_SORT			"Sorting"
#define MOPS_NAME_SEARCH		"SEARCHING"
#define MOPS_NAME_BINOPS		"Binary operations"
#define MOPS_NAME_LINKEDLIST	"Linked List"
#define MOPS_NAME_STRINGS		"Strings"
#define MOPS_NAME_TWISTERS		"Twisters"
#define MOPS_NAME_BINTREE		"Binary Tree"
#define MOPS_NAME_END			"Exit"

	enum MAJOR_OPS{
		MOPS_START=-1,
		MOPS_SORT,
		MOPS_SEARCH,
		MOPS_BINOPS,
		MOPS_LINKEDLIST,
		MOPS_STRINGS,
		MOPS_TWISTERS,
		MOPS_BINTREE,
		MOPS_END
	};
	void TestLinkedList();
	void TestBinaryTree();
	void TestStrings();
	void TestTwisters();
	void TestSearch();
	void TestSorting();
	void TestBinOps();

	typedef void (*TestMajorOps)();

	struct _MAJOR_OPS_INFO{
		MAJOR_OPS		index;
		char			opsname[MOPS_NAME_LENGTH];
		TestMajorOps	mopsfunc;
	};
	typedef struct _MAJOR_OPS_INFO MAJOR_OPS_INFO;
}

#endif __ALGODS_H