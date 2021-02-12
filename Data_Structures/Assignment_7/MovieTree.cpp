#include <iostream>
#include <fstream>
#include "MovieTree.hpp"

using namespace std;

/* ------------------------------------------------------ */
MovieTree::MovieTree()
{
	root = NULL;
}

void deleteLinkedList(LLMovieNode* currentNode)
{
	LLMovieNode* next;
	while(currentNode != NULL)
  {
		next = currentNode->next;
		delete currentNode;
		currentNode = next;
	}
}

void deleteTree(TreeNode *node)
{
	if (node == NULL) return;
	deleteTree(node->leftChild);
    deleteTree(node->rightChild);
    deleteLinkedList(node->head);
    delete node;
}

MovieTree::~MovieTree()
{
  deleteTree(root);
  root = NULL;
}

void printMovieInventoryHelper(TreeNode* currNode)
{
  if(currNode != NULL)
  {
    printMovieInventoryHelper(currNode->leftChild);
    if(currNode->head != NULL)
    {
      cout << "Movies starting with letter: " << currNode->titleChar << endl;
      LLMovieNode * crawler = currNode->head;
      while(currNode->head != NULL)
      {
        cout << " >> " << crawler->title << " " << crawler->rating << endl;
        if(crawler->next != NULL)
        {
          crawler = crawler->next;
        }
        else
        {
          break;
        }
      }
    }
    printMovieInventoryHelper(currNode->rightChild);
  }
}

void MovieTree::printMovieInventory()
{
    printMovieInventoryHelper(root);
}

TreeNode * createTreeNode(char title)
{
  TreeNode * newTreeNode = new TreeNode;
  newTreeNode->titleChar = title;
}

void llAddMovie(TreeNode * currNode, int ranking, string title, int year, float rating)
{
  if(currNode->head == NULL)
    {
      LLMovieNode * newMovieNode = new LLMovieNode(ranking, title, year, rating);
      currNode->head = newMovieNode;
    }
  else if(currNode->head != NULL) 
  {
    LLMovieNode * newMovieNode = new LLMovieNode(ranking, title, year, rating);
    LLMovieNode * crawler = currNode->head;
    LLMovieNode * prev = crawler;
    bool added = false;
    while(crawler != NULL && !added)
    {
      added = false;
      if(title < currNode->head->title)
      {
        newMovieNode->next = currNode->head;
        currNode->head = newMovieNode;
        added = true;
      }
      if(newMovieNode->title > prev->title && newMovieNode->title < crawler->title)
      {
        newMovieNode->next = crawler;
        prev->next = newMovieNode;
        added = true;
      }
      prev = crawler;
      crawler = crawler->next;
      if(crawler == NULL && !added)
      {
        newMovieNode->next = crawler;
        prev->next = newMovieNode;
        added = true;
      }
    }
    return; 
  }
}

TreeNode* addMovieHelper(TreeNode* currNode, int ranking, string title, int year, float rating)
{
  char titleC = title[0];
  if(currNode == NULL)
  {
    currNode = createTreeNode(titleC);
    llAddMovie(currNode, ranking, title, year, rating); 
    return currNode;
  }
  else if(currNode->titleChar == titleC)
  {
     llAddMovie(currNode, ranking, title, year, rating);
     return currNode; 
  }
  else if(currNode->titleChar < titleC)
  {
    TreeNode * rChild = addMovieHelper(currNode->rightChild, ranking, title, year, rating);
    currNode->rightChild = rChild;
    rChild->parent = currNode;
  }
  else if(currNode->titleChar > titleC)
  {
    TreeNode * lChild = addMovieHelper(currNode->leftChild, ranking, title, year, rating);
    currNode->leftChild = lChild;
    lChild->parent = currNode;
  }
  return currNode;
}

void MovieTree::addMovie(int ranking, string title, int year, float rating)
{
  root = addMovieHelper(root, ranking, title, year, rating);
}

TreeNode* getMinValueNode(TreeNode* currNode)
{
    if(currNode->leftChild == NULL)
    {
      return currNode;
    }
    return getMinValueNode(currNode->leftChild);
}

bool doesMovieExist(LLMovieNode* currNode, string title)
{
  while(currNode != NULL)
  {
    if(currNode->title == title)
    {
      return true;
    }
    currNode = currNode->next;
  }
  return false;
}

bool deleteMovieNode(LLMovieNode* currNode, string title)
{
  LLMovieNode* prev = currNode;
  bool found = false;
  if(currNode->title == title && currNode->next == NULL)
  {
    delete currNode;
    return true;
  }
  while (currNode != NULL && !found)
  {
    if(currNode->title == title)
    {
      found = true;
      prev->next = currNode->next;
      delete currNode;
    }
    prev = currNode;
    currNode = currNode->next;
  }
  return false; 
}

bool leftOrRight(TreeNode* parent, TreeNode* child)
{
  if(parent->leftChild == child)
  {
    return false;
  }
  else if(parent->rightChild == child)
  {
    return true;
  }
}

TreeNode* deleteNodeHelper(TreeNode* currNode, char titleChar)
{
  if(currNode == NULL)
  {
    return NULL;
  }
  else if(titleChar < currNode->titleChar)
  {
    currNode->leftChild = deleteNodeHelper(currNode->leftChild, titleChar);
  }

  else if(titleChar > currNode->titleChar)
  {
    currNode->rightChild = deleteNodeHelper(currNode->rightChild, titleChar);
  }
  else
  {
    //No Child
    if(currNode->leftChild == NULL && currNode->rightChild == NULL)
    { 
      TreeNode* parent = currNode->parent;
      if(parent != NULL && leftOrRight(parent, currNode) == true)
      {
        parent->rightChild = NULL;
      }
      else if(parent != NULL && leftOrRight(parent, currNode) == false)
      {
        parent->leftChild = NULL;
      }
      delete currNode;
      currNode = NULL;
    }
    //Right Child Only
    else if(currNode->leftChild == NULL && currNode->rightChild != NULL)
    {
      TreeNode* parent = currNode->parent;
      if(parent != NULL && leftOrRight(parent, currNode) == true)
      {
        parent->rightChild = currNode->rightChild;
        currNode->rightChild->parent = parent;
      }
      else if(parent != NULL &&  leftOrRight(parent, currNode) == false)
      {
        parent->leftChild = currNode->rightChild;
        currNode->rightChild->parent = parent;
      }
      TreeNode* temp = currNode;
      currNode = temp->rightChild;
      delete temp;
    }
    //Left Child Only
    else if(currNode->rightChild == NULL && currNode->leftChild != NULL)
    {
      TreeNode* parent = currNode->parent;
      if(parent != NULL && leftOrRight(parent, currNode) == true)
      {
        parent->rightChild = currNode->leftChild;
        currNode->leftChild->parent = parent;
      }
      else if(parent != NULL && leftOrRight(parent, currNode) == false)
      {
        parent->leftChild = currNode->leftChild;
        currNode->leftChild->parent = parent;
      }
      TreeNode* temp = currNode;
      currNode = temp->leftChild;
      delete temp;
    }
    //Both Children
    else
    {
      TreeNode* temp = getMinValueNode(currNode->rightChild);
      currNode->head = temp->head;
      currNode->titleChar = temp->titleChar;
      currNode->rightChild = deleteNodeHelper(currNode->rightChild, temp->titleChar);
    }
  }
  return currNode;
}

void MovieTree::deleteMovie(std::string title)
{
  char titleC = title[0];
  TreeNode* search = searchChar(titleC);
  if(search == NULL)
  {
    cout << "Movie: " << title << " not found, cannot delete." << endl;
    return;
  }
  LLMovieNode* find = search->head;
  if(!doesMovieExist(find, title))
  {
    cout << "Movie: " << title << " not found, cannot delete." << endl;
    return;
  }
  if(find->title == title && find->next != NULL)
  {
    search->head = search->head->next;
    delete find;
    find = NULL;
  }
  else if(deleteMovieNode(find, title))
  {
    root = deleteNodeHelper(root, titleC);
  }
}

void MovieTree::leftRotation(TreeNode* curr)
{
  TreeNode* pivot = curr->rightChild;
  if(curr->rightChild == NULL)
  {
    return;
  }
  if(curr->parent == NULL)
  {
    curr->rightChild = pivot->leftChild;
    curr->parent = pivot;
    pivot->leftChild = curr;
    pivot->parent = NULL;
    root = pivot;
  }
  else
  { 
    bool rOrL;
    if(curr->parent->leftChild == curr)
    {
      rOrL = false;
    }
    else if(curr->parent->rightChild == curr)
    {
      rOrL = true;
    }
    if(rOrL == true)
    {
      curr->parent->rightChild = pivot;
      pivot->parent = curr->parent;
      curr->parent = pivot;
      curr->rightChild = pivot->leftChild;
      pivot->leftChild = curr;
    }
    else if(rOrL == false)
    {
      curr->parent->leftChild = pivot;
      pivot->parent = curr->parent;
      curr->parent = pivot;
      curr->rightChild = pivot->leftChild;
      pivot->leftChild = curr;
    }
  }
}

//------ Given Methods--------//
void _grader_inorderTraversal(TreeNode * root)
{
	if (root == NULL) {
		return;
	}

	_grader_inorderTraversal(root->leftChild);
	cout << root->titleChar << " ";
	_grader_inorderTraversal(root->rightChild);
}


void MovieTree::inorderTraversal() {
	_grader_inorderTraversal(root);
	cout << endl;
}

TreeNode* searchCharHelper(TreeNode* curr, char key)
{
    if (curr == NULL)
        return curr;
    else if(curr->titleChar == key)
        return curr;
    else if (curr->titleChar > key)
        return searchCharHelper(curr->leftChild, key);
    else
        return searchCharHelper(curr->rightChild, key);
}

TreeNode* MovieTree::searchChar(char key)
{
    return searchCharHelper(root, key);
}
