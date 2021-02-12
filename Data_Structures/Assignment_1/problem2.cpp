#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

struct Park
{
std::string parkname;
std::string state;
int area;
};
void addPark(Park parks[], std::string parkname, std::string state, int area, int length)
{
    Park newPark;
    newPark.parkname = parkname;
    newPark.state = state;
    newPark.area = area;
    parks[length] = newPark;
}
void printList(const Park parks[], int length)
{
    for(int i = 0; i < length; i++)
    {
        std::cout << parks[i].parkname <<" [" << parks[i].state << "] area: "<< parks[i].area << std::endl;
    }
}
int main(int argc, char *argv[])
{
    std::string line, parkname, state, area_str;
    int area, length = 0;
    Park parks[100];
    std::ifstream inFile(argv[1]);
    std::ofstream outFile(argv[2]);
    std::string lowerBound_str = argv[3], upperBound_str = argv[4];
    int lowerBound = stoi(lowerBound_str);
    int upperBound = stoi(upperBound_str);
    while(std::getline(inFile, line))
    {
        std::stringstream s(line);
        std::getline(s, parkname, ',');
        std::getline(s, state, ',');
        std::getline(s, area_str, '\n');
        area = stoi(area_str);
        addPark(parks, parkname, state, area, length);
        length++;
    }
    printList(parks, length);
    for(int i = 0; i < length; i++)
    {
        if(parks[i].area >= lowerBound && parks[i].area <= upperBound)
        {
            outFile << parks[i].parkname << "," << parks[i].state <<  "," << parks[i].area << std::endl;
        }
    }
    outFile.close();
}