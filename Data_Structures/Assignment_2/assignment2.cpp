#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

struct wordItem
{
    std::string word;
    int count;
};

void getStopWords(const char * ignoreWordFileName, std::string ignoreWords[]);

bool isStopWord(std::string word, std::string ignoreWords[]);

int getTotalNumberNonStopWords(wordItem uniqueWords[], int length);

void arraySort(wordItem uniqueWords[], int length);

void printNext10(wordItem uniqueWords[], int N, int totalNumWords);

int main (int argc, char * argv[])
{
    if(argc != 4)
    {
        std::cout << "Usage: Assignment2Solution <number of words> <inputfilename.txt> <ignoreWordsfilename.txt>" << std::endl;
    }
    std::string ignoreWords[50];
    getStopWords(argv[3], ignoreWords);
    std::ifstream inStream_Words(argv[2]);
    std::string word;
    int max = 100, n = 0, doubled = 0, totalNumWords = 0;
    wordItem * ptr_words = new wordItem[max];
    while(inStream_Words >> word)
    {
        if(isStopWord(word, ignoreWords) == false)
        {
            bool exists = false;
            for(int i = 0; i < n; i++)
            {
                if(ptr_words[i].word == word)
                {
                    ptr_words[i].count += 1;
                    exists = true;
                }
            }
            if(exists == false)
            {
                n++;
                ptr_words[n - 1].word = word;
                ptr_words[n - 1].count += 1;
            }

        }
        if(n >= max)
        {
            max = max * 2;
            wordItem * temp = new wordItem[max];
            for(int i = 0; i < n; i++)
            {
                temp[i] = ptr_words[i];
            }
            delete [] ptr_words;
            ptr_words = temp;
            doubled++;
        }
    }
    inStream_Words.close();
    totalNumWords = getTotalNumberNonStopWords(ptr_words, n);
    arraySort(ptr_words, n);
    std::cout << "Array doubled: " << doubled << std::endl;
    std::cout << '#' << std::endl;
    std::cout << "Unique non-common words: " << n << std::endl;
    std::cout << '#' << std::endl;
    std::cout << "Total non-common words: " << totalNumWords << std::endl;
    std::cout << '#' << std::endl;
    std::cout << "Probability of next 10 words from rank " << argv[1] << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    printNext10(ptr_words,  std::stoi(argv[1]), totalNumWords);
}

void getStopWords(const char * ignoreWordFileName, std::string ignoreWords[])
{
    std::ifstream inStream_Ignore(ignoreWordFileName);
    std::string word;
    int n = 0;
    if(inStream_Ignore.fail())
    {
        std::cout << "Failed to open " << ignoreWordFileName << std::endl;
    }
    while(std::getline(inStream_Ignore, word))
    {
        ignoreWords[n] = word;
        n++;
    }
}

bool isStopWord(std::string word, std::string ignoreWords[])
{
    bool found = false;
    for(int i = 0; i < 50; i++)
    {
        if(word == ignoreWords[i])
        {
            found = true;
            break;
        }   
    }
    return found;
}

int getTotalNumberNonStopWords(wordItem uniqueWords[], int length)
{
    int total = 0;
    for(int i = 0; i < length; i++)
    {
        total += uniqueWords[i].count;
    }
    return total;
}

void arraySort(wordItem uniqueWords[], int length)
{
    wordItem temp;
    for(int k = 0; k < length - 1; k++)
    {
        for(int i = 0; i < length - k - 1; i++)
        {
            if(uniqueWords[i].count < uniqueWords[i + 1].count) 
            {
                temp = uniqueWords[i];
                uniqueWords[i] = uniqueWords[i + 1];
                uniqueWords[i + 1] = temp;
            }
        }
    }
}

void printNext10(wordItem uniqueWords[], int N, int totalNumWords)
{
    for(int i = N; i < N + 10; i++)
    {
        float prob = (float) uniqueWords[i].count / (float) totalNumWords;
        std::cout << std::setprecision(4) << std::fixed << prob << " - " << uniqueWords[i].word << std::endl;
    }
}
