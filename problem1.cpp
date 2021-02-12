#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int insertIntoSortedArray(float myArray[], int numEntries, float newValue)
{
    if(numEntries >= 100)
    {
        return numEntries;
    }
    int i;
    for(i = numEntries - 1; (i >= 0 && myArray[i] < newValue); i--)
    {
        myArray[i + 1] = myArray[i];
    }
    myArray[i + 1] = newValue;
    return (numEntries + 1);
}

int main(int argc, char *argv[])
{
    float array[100];   
    std::string line;
    int entries = 0; 
    float value = 0;
    std::ifstream inFile(argv[1]);
    if(inFile.fail())
    {
        std::cout << "Failed to open the file." << std::endl;
    }
    while(std::getline(inFile, line))
    {
        value = stof(line);
        insertIntoSortedArray(array, entries, value);
        entries++;
        for(int i = 0; i < entries; i++)
        {
            std::cout << array[i];
            if(entries > 1 && (i + 1 != entries))
            {
                std::cout << ",";
            }
        }
        std::cout << "\n";
    }
}