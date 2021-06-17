/** @file BST.cpp. */
//#include "BST.h"     // header file

using namespace std;

template <typename Comparable>
BinarySearchTree<Comparable>::BinarySearchTree() : root(nullptr)
{
}  // end default constructor

template <typename Comparable>
BinarySearchTree<Comparable>::BinarySearchTree(const BinarySearchTree& tree)
{
	copyTree(tree.root, root);
}  // end copy constructor

template <typename Comparable>
BinarySearchTree<Comparable>::~BinarySearchTree()
{
	destroyTree(root);
}  // end destructor

template <typename Comparable>
bool BinarySearchTree<Comparable>::isEmpty() const
{
	return (root == nullptr);
}  // end searchTreeIsEmpty

template <typename Comparable>
void BinarySearchTree<Comparable>::searchTreeInsert(const Comparable& newItem)
{
	insertItem(root, newItem);
}  // end searchTreeInsert

template <typename Comparable>
void BinarySearchTree<Comparable>::searchTreeDelete(const Comparable& searchKey)
{
	deleteItem(root, searchKey);
}  // end searchTreeDelete

template <typename Comparable>
bool BinarySearchTree<Comparable>::searchTreeRetrieve(Comparable& treeItem) const
{
	// if retrieveItem throws a TreeException, it is
	// ignored here and passed on to the point in the code
	// where searchTreeRetrieve was called
	return retrieveItem(root, treeItem);
}  // end searchTreeRetrieve

template <typename Comparable>
void BinarySearchTree<Comparable>::preorderTraverse(FunctionType visit)
{
	preorder(root, visit);
}  // end preorderTraverse

template <typename Comparable>
void BinarySearchTree<Comparable>::inorderTraverse(FunctionType visit)
{
	inorder(root, visit);
}  // end inorderTraverse

template <typename Comparable>
void BinarySearchTree<Comparable>::postorderTraverse(FunctionType visit)
{
	postorder(root, visit);
}  // end postorderTraverse

template <typename Comparable>
void BinarySearchTree<Comparable>::insertItem(TreeNode *& treePtr,
	const Comparable& newItem)
{
	if (treePtr == nullptr)
	{  // position of insertion found; insert after leaf

	   // create a new node
		try
		{
			treePtr = new TreeNode(newItem, nullptr, nullptr);
		}
		catch (bad_alloc e)
		{
			throw TreeException(
				"TreeException: insertItem cannot allocate memory");
		}  // end try
	}
	// else search for the insertion position
	else if (newItem < treePtr->item)
		// search the left subtree
		insertItem(treePtr->leftChildPtr, newItem);

	else  // search the right subtree
		insertItem(treePtr->rightChildPtr, newItem);
}  // end insertItem

template <typename Comparable>
void BinarySearchTree<Comparable>::deleteItem(TreeNode *& treePtr,
	const Comparable& searchKey)
	// Calls: deleteNodeItem.
{
	if (treePtr == nullptr)
		throw TreeException("TreeException: delete failed");  // empty tree

	else if (searchKey == treePtr->item)
		// item is in the root of some subtree
		deleteNodeItem(treePtr);  // delete the item

								  // else search for the item
	else if (searchKey < treePtr->item)
		// search the left subtree
		deleteItem(treePtr->leftChildPtr, searchKey);

	else  // search the right subtree
		deleteItem(treePtr->rightChildPtr, searchKey);
}  // end deleteItem

template <typename Comparable>
void BinarySearchTree<Comparable>::deleteNodeItem(TreeNode *& nodePtr)
// Algorithm note: There are four cases to consider:
//   1. The root is a leaf.
//   2. The root has no left child.
//   3. The root has no right child.
//   4. The root has two children.
// Calls: processLeftmost.
{
	TreeNode     *delPtr;
	Comparable	 replacementItem;

	// test for a leaf
	if ((nodePtr->leftChildPtr == nullptr) &&
		(nodePtr->rightChildPtr == nullptr))
	{
		delete nodePtr;
		nodePtr = nullptr;
	}  // end if leaf
	   // test for no left child
	else if (nodePtr->leftChildPtr == nullptr)
	{
		delPtr = nodePtr;
		nodePtr = nodePtr->rightChildPtr;
		delPtr->rightChildPtr = nullptr;
		delete delPtr;
	}  // end if no left child

	   // test for no right child
	else if (nodePtr->rightChildPtr == nullptr)
	{
		delPtr = nodePtr;
		nodePtr = nodePtr->leftChildPtr;
		delPtr->leftChildPtr = nullptr;
		delete delPtr;
	}  // end if no right child

	   // there are two children:
	   // retrieve and delete the inorder successor
	else
	{
		processLeftmost(nodePtr->rightChildPtr,
			replacementItem);
		nodePtr->item = replacementItem;
	}  // end if two children
}  // end deleteNodeItem

template <typename Comparable>
void BinarySearchTree<Comparable>::processLeftmost(TreeNode *& nodePtr, Comparable& treeItem)
{
	if (nodePtr->leftChildPtr == nullptr)
	{
		treeItem = nodePtr->item;
		TreeNode *delPtr = nodePtr;
		nodePtr = nodePtr->rightChildPtr;
		delPtr->rightChildPtr = nullptr;  // defense
		delete delPtr;
	}

	else
		processLeftmost(nodePtr->leftChildPtr, treeItem);
}  // end processLeftmost

template <typename Comparable>
bool BinarySearchTree<Comparable>::retrieveItem(TreeNode *treePtr,
	Comparable& treeItem) const
{
	if (treePtr == nullptr)
		return false;

	else if (treeItem == treePtr->item) {
		// item is in the root of some subtree
		treeItem = treePtr->item;
		return true;
	}

	else if (treeItem < treePtr->item)
		// search the left subtree
		return retrieveItem(treePtr->leftChildPtr,	treeItem);

	else  // search the right subtree
		return retrieveItem(treePtr->rightChildPtr, treeItem);
}  // end retrieveItem

template <typename Comparable>
BinarySearchTree<Comparable>& BinarySearchTree<Comparable>::operator=(const BinarySearchTree& rhs)
{
	if (this != &rhs)
	{
		destroyTree(root);  // deallocate left-hand side
		copyTree(rhs.root, root);  // copy right-hand side
	}  // end if
	return *this;
}  // end operator=

template <typename Comparable>
void BinarySearchTree<Comparable>::copyTree(TreeNode *treePtr,
	TreeNode *& newTreePtr) const
{
	// preorder traversal
	if (treePtr != nullptr)
	{  // copy node
		try
		{
			newTreePtr = new TreeNode(treePtr->item, nullptr, nullptr);
			copyTree(treePtr->leftChildPtr, newTreePtr->leftChildPtr);
			copyTree(treePtr->rightChildPtr, newTreePtr->rightChildPtr);
		}
		catch (bad_alloc e)
		{
			throw TreeException(
				"TreeException: copyTree cannot allocate memory");
		}  // `end try
	}
	else
		newTreePtr = nullptr;  // copy empty tree
}  // end copyTree

template <typename Comparable>
void BinarySearchTree<Comparable>::destroyTree(TreeNode *& treePtr)
{
	// postorder traversal
	if (treePtr != nullptr)
	{
		destroyTree(treePtr->leftChildPtr);
		destroyTree(treePtr->rightChildPtr);
		delete treePtr;
		treePtr = nullptr;
	}  // end if
}  // end destroyTree

template <typename Comparable>
typename BinarySearchTree<Comparable>::TreeNode *BinarySearchTree<Comparable>::rootPtr() const
{
	return root;
}  // end rootPtr

template <typename Comparable>
void BinarySearchTree<Comparable>::setRootPtr(TreeNode *newRoot)
{
	root = newRoot;
}  // end setRoot

template <typename Comparable>
void BinarySearchTree<Comparable>::getChildPtrs(TreeNode *nodePtr,
	TreeNode *& leftPtr,
	TreeNode *& rightPtr) const
{
	leftPtr = nodePtr->leftChildPtr;
	rightPtr = nodePtr->rightChildPtr;
}  // end getChildPtrs

template <typename Comparable>
void BinarySearchTree<Comparable>::setChildPtrs(TreeNode *nodePtr,
	TreeNode *leftPtr,
	TreeNode *rightPtr)
{
	nodePtr->leftChildPtr = leftPtr;
	nodePtr->rightChildPtr = rightPtr;
}  // end setChildPtrs

template <typename Comparable>
void BinarySearchTree<Comparable>::preorder(TreeNode *treePtr,
	FunctionType visit)
{
	if (treePtr != nullptr)
	{
		visit(treePtr->item);
		preorder(treePtr->leftChildPtr, visit);
		preorder(treePtr->rightChildPtr, visit);
	}  // end if
}  // end preorder

template <typename Comparable>
void BinarySearchTree<Comparable>::inorder(TreeNode *treePtr,
	FunctionType visit)
{
	if (treePtr != nullptr)
	{
		inorder(treePtr->leftChildPtr, visit);
		visit(treePtr->item);
		inorder(treePtr->rightChildPtr, visit);
	}  // end if
}  // end inorder

template <typename Comparable>
void BinarySearchTree<Comparable>::postorder(TreeNode *treePtr,
	FunctionType visit)
{
	if (treePtr != nullptr)
	{
		postorder(treePtr->leftChildPtr, visit);
		postorder(treePtr->rightChildPtr, visit);
		visit(treePtr->item);
	} // end if
}  // end postorder


////////////////////////////////////////////////////
//Add your code here
////////////////////////////////////////////////////

// Private function that takes a node and takes the sum of
// the depths of the items in its left and right subtrees.
template <typename Comparable>
int BinarySearchTree<Comparable>::totalLevel(TreeNode * treePtr, int initLevel) const
{
	// Stores sum of levels for current branch
	int branchLevelSum = 0;

	// If the pointer points to nothing
	if (treePtr == nullptr)
		// Return a zero because the child is empty
		// and is therefore not a node
		return 0;

	// Take the sum of the left subtree levels
	branchLevelSum += totalLevel(treePtr->leftChildPtr, initLevel + 1);

	// Take the sum of the right subtree levels
	branchLevelSum += totalLevel(treePtr->rightChildPtr, initLevel + 1);

	// Add the current subtree level to the sum
	branchLevelSum += initLevel;

	// Return sum of current branch levels
	return branchLevelSum;

}

//get the total level of the BST pointed by the pointer treePtr, 
//which is defined as the sum of levels of all nodes
//initLevel is the level of the root node of treePtr in the original
//tree
template <typename Comparable>
int BinarySearchTree<Comparable>::totalLevel(void) const
{
	// Stores the sum of all the levels
	int levelSum = 0;

	// Perform recursive operation on children
	// of the root node (because root has no depth)
	levelSum += totalLevel(root->leftChildPtr, 1);		// Left subtree level sum
	levelSum += totalLevel(root->rightChildPtr, 1);	// Right subtree level sum

	// Return value that results from the operation
	return levelSum;
}






// End of implementation file.
