/****************************************************************/
/*                   Assignment 3 Driver File                   */
/****************************************************************/
/* TODO: Implement menu options as described in the writeup     */
/****************************************************************/

#include "CountryNetwork.hpp"
#include <iostream>
#include <fstream>

void displayMenu();  // declaration for main's helper-function

int main(int argc, char* argv[])
{
    // Object representing our network of cities.
    // (Linked list representation is in CountryNetwork)
    CountryNetwork CountryNet;

    // TODO
    int input = 0;
    string message_Input;
    string country_Input;
    string newCountry;
    Country * previous = new Country();
    string previous_Name;
    bool found = false;
    bool filled = false;
    while(input != 5)
    {
        cin >> input;
        cin.ignore();
        displayMenu();
        if(input == 1)
        {
            CountryNet.loadDefaultSetup();
            CountryNet.printPath();
            filled = true;
        }
        else if(input == 2)
        {
            CountryNet.printPath();
        }
        else if(input == 3)
        {
            cout << "Enter name of the country to receive the message:" << endl;
            getline(cin, country_Input);
            '\n';
            cout << "Enter the message to send:" << endl;
            getline(cin, message_Input);
            cout << "" << endl;
            if(CountryNet.searchNetwork(country_Input) == NULL && filled == true)
            {
                cout << "Country not found" << endl;
            }
            else
            {
                CountryNet.transmitMsg(country_Input, message_Input);
            }
        }
        else if(input == 4)
        {
            cout << "Enter a new country name:" << endl;
            getline(cin, newCountry);
            cout << "Enter the previous country name (or First):" << endl;
            while(found == false)
            {
                getline(cin, previous_Name);
                previous_Name[0] = toupper(previous_Name[0]);
                if(previous_Name == "First")
                {
                    previous = NULL;
                    found = true;
                }
                else if(CountryNet.searchNetwork(previous_Name) == NULL)
                {
                    cout << "INVALID(previous country name)...Please enter a VALID previous country name!" << endl;
                }
                else
                {
                    found = true;
                    previous = CountryNet.searchNetwork(previous_Name);
                }
            }
            CountryNet.insertCountry(previous, newCountry);
            CountryNet.printPath();
            found = false;
            filled = true;
        }
        else if(input == 5)
        {
            cout << "Quitting..." << endl;
            cout << "Goodbye!" << endl;
        }
    }
    return 0;
}


/*
 * Purpose; displays a menu with options
 */
void displayMenu()
{
    cout << "Select a numerical option:" << endl;
    cout << "+=====Main Menu=========+" << endl;
    cout << " 1. Build Network " << endl;
    cout << " 2. Print Network Path " << endl;
    cout << " 3. Transmit Message " << endl;
    cout << " 4. Add Country " << endl;
    cout << " 5. Quit " << endl;
    cout << "+-----------------------+" << endl;
    cout << "#> ";
}
