#include <iostream>
using namespace std;


bool append(string * &str_arr, string s, int &numEntries, int &arraySize)
{
   if(numEntries<arraySize)
   {
       str_arr[numEntries] = s;
       numEntries++;
       return false;
   }
   else
   {
       string * newArray = new string[arraySize*2];
       for(int i=0;i<arraySize;i++)
       {
           newArray[i] = str_arr[i];
       }
       arraySize *=2;
       newArray[numEntries] = s;
       str_arr = newArray;
       numEntries++;
       return true;
   }
}

int main()
{
    return 0;
}