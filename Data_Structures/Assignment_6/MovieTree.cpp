#include "MovieTree.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// MovieNode: node struct that will be stored in the MovieTree BST

MovieTree::MovieTree() {
  //write your code
  root = NULL;
}
void Destroy(MovieNode *& tree)
{
  if(tree!=NULL)
  {
    Destroy(tree->left);
    Destroy(tree->right);
    delete (tree);
  }
}

MovieTree::~MovieTree() {
  //write your code
  Destroy(root);
}

void printMovieInventoryHelper(MovieNode* currNode)
{
  if(currNode)
  {
    printMovieInventoryHelper(currNode->left);
    cout << "Movie: " << currNode->title << " " << currNode->rating << endl;
    printMovieInventoryHelper(currNode->right);
  }
}

void MovieTree::printMovieInventory() {
   //write your code
  if(root == NULL)
  {
    cout << "Tree is Empty. Cannot print" << endl;
  }
  else
  {
    printMovieInventoryHelper(root);
  }
}

MovieNode * createMovieNode(int ranking, string title, int year, float rating)
{
  MovieNode * newMovieNode = new MovieNode(ranking, title, year, rating);
  return newMovieNode;
}

MovieNode* addMovieNodeHelper(MovieNode* currNode, int ranking, string title, int year, float rating)
{
  if(currNode == nullptr)
  {
    return createMovieNode(ranking, title, year, rating);
  }
  else if(currNode->title.compare(title) < 0)
  {
    currNode->right = addMovieNodeHelper(currNode->right, ranking, title, year, rating);
  }
  else if(currNode->title.compare(title) > 0)
  {
    currNode->left = addMovieNodeHelper(currNode->left, ranking, title, year, rating);
  }
  return currNode;
}

void MovieTree::addMovieNode(int ranking, string title, int year, float rating) {
  //write your code
  root = addMovieNodeHelper(root, ranking, title, year, rating);
}

MovieNode* findMovieHelper(MovieNode * currNode, string title)
{
  if(currNode == NULL || currNode->title == title)
  {
    return currNode; 
  } 
  else if(currNode->title.compare(title) < 0)
  {
    return findMovieHelper(currNode->right, title);
  }  
  return findMovieHelper(currNode->left, title);
}

void MovieTree::findMovie(string title) {
  //write your code
  MovieNode * crawler = findMovieHelper(root, title);
  if(crawler == NULL)
  {
    cout << "Movie not found." << endl;
  }
  else
  {
    cout << "Movie Info:" << endl;
    cout << "==================" << endl;
    cout << "Ranking:" << crawler->ranking << endl;
    cout << "Title :" << crawler->title << endl;
    cout << "Year :" << crawler->year << endl;
    cout << "rating :" << crawler->rating << endl;
  }  
}

void queryMoviesHelper(MovieNode* currNode, float rating, int year)
{
  if(currNode == NULL)
  {
    return;
  }
  if(currNode->rating >= rating && currNode->year > year)
  {
    cout << currNode->title << "(" << currNode->year << ") " << currNode->rating << endl;
  }
  queryMoviesHelper(currNode->left, rating, year);
  queryMoviesHelper(currNode->right, rating, year);
}

void MovieTree::queryMovies(float rating, int year) {
  //write your code
   MovieNode * crawler = root;
   if(crawler == NULL)
   {
     cout << "Tree is Empty. Cannot query Movies" << endl;
   }
   else
   {
     cout << "Movies that came out after " << year << " with rating at least " << rating << ":" << endl;
     queryMoviesHelper(crawler, rating, year);
   }  
}

float averageRatingHelper(MovieNode* currNode, float &ratingTotal, int &numRatings)
{
  if(currNode == NULL)
  {
    return ratingTotal;
  }
  averageRatingHelper(currNode->left, ratingTotal, numRatings);
  ratingTotal += currNode->rating;
  numRatings++;
  averageRatingHelper(currNode->right, ratingTotal, numRatings);
  return ratingTotal; 
}

void MovieTree::averageRating() {
  //write your code
  MovieNode * crawler = root;
  float average = 0;
  float ratingTotal = 0;
  int numRatings = 0;
  if(crawler == NULL)
  {
    cout << "Average rating:0.0" << endl;
  }
  else
  {
    averageRatingHelper(crawler, ratingTotal, numRatings);
    average = ratingTotal / numRatings;
    cout << "Average rating:" << average << endl;
  }
}

void printLevelNodesHelper(MovieNode* currNode, int level)
{
  if(currNode == NULL)
  {
    return;
  }
  if(level == 0)
  {
    cout << "Movie: " << currNode->title << " " << currNode->rating << endl;
  }
  else
  {
    printLevelNodesHelper(currNode->left, level-1);
    printLevelNodesHelper(currNode->right, level-1);
  }
}

void MovieTree::printLevelNodes(int level) {
  //write your code]
  printLevelNodesHelper(root, level);
}