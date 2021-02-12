/****************************************************************/
/*                CountryNetwork Implementation                 */
/****************************************************************/
/* TODO: Implement the member functions of class CountryNetwork */
/*     This class uses a linked-list of Country structs to      */
/*     represet communication paths between nations             */
/****************************************************************/

#include "CountryNetwork.hpp"

using namespace std;

/*
 * Purpose: Constructer for empty linked list
 * @param none
 * @return none
 */
CountryNetwork::CountryNetwork()
{
    // TODO
    head = NULL;
}

/*
 * Purpose: Add a new Country to the network
 *   between the Country *previous and the Country that follows it in the network.
 * @param previous name of the Country that comes before the new Country
 * @param countryName name of the new Country
 * @return none
 */
void CountryNetwork::insertCountry(Country* previous, string countryName)
{
    // TODO
    Country* new_Country = new Country(); 
    new_Country->name = countryName;
    if(previous == NULL)
    {
        new_Country->next = head;  
        head = new_Country;  
        cout << "adding: " << countryName << " (HEAD)" << endl;
    }
    else
    {
        new_Country->next = previous->next;  
        previous->next = new_Country;
        cout << "adding: " << countryName << " (prev: " << previous->name << ")" << endl;
    }
}

/*
 * Purpose: populates the network with the predetermined countries
 * @param none
 * @return none
 */
void CountryNetwork::loadDefaultSetup()
{
    // TODO
    insertCountry(head, "United States");
    Country * ptr_Country = head;
    insertCountry(ptr_Country, "Canada");
    ptr_Country = ptr_Country->next;
    insertCountry(ptr_Country, "Brazil");
    ptr_Country = ptr_Country->next;
    insertCountry(ptr_Country, "India");
    ptr_Country = ptr_Country->next;
    insertCountry(ptr_Country, "China");
    ptr_Country = ptr_Country->next;
    insertCountry(ptr_Country, "Australia");
    ptr_Country = ptr_Country->next;
}

/*
 * Purpose: Search the network for the specified country and return a pointer to that node
 * @param countryName name of the country to look for in network
 * @return pointer to node of countryName, or NULL if not found
 */
Country* CountryNetwork::searchNetwork(string countryName)
{
    // TODO
    Country * ptr = head;
    bool found = false;
    while(ptr != NULL)
    {
        if(ptr->name == countryName)
        {
            found = true;
            break;
        }
        ptr = ptr->next;
    }
    if(found == false)
    {
        ptr = NULL;
        return ptr;
    }
    else
    {
        return ptr;
    }
}

/*
 * Purpose: Transmit a message across the network to the
 *   receiver. Msg should be stored in each country it arrives
 *   at, and should increment that country's count.
 * @param receiver name of the country to receive the message
 * @param message the message to send to the receiver
 * @return none
 */
void CountryNetwork::transmitMsg(string receiver, string message)
{
    // TODO
    if(head == NULL) 
    {
        cout << "Empty list" << endl;
        return;
    }
    Country *start = head;
    while(start != NULL && start->name.compare(receiver) != 0) 
    {
        start->message = message;
        start->numberMessages += 1;
        cout << start->name << " [# messages received: " << start->numberMessages << "] received: " << start->message << endl;
        start = start->next;
    }
    start->message = message;
    start->numberMessages += 1;
    cout << start->name << " [# messages received: " << start->numberMessages << "] received: " << start->message << endl;
}

/*
 * Purpose: prints the current list nicely
 * @param ptr head of list
 */
void CountryNetwork::printPath()
{
    // TODO
    cout << "== CURRENT PATH ==" << endl;
    if(head == NULL)
    {
        cout << "nothing in path" << endl;
        cout << "===" << endl;
        return;
    }
    Country *start = head;
    while(start != NULL) {
        cout << start->name << " -> ";
        start = start->next;
    }
    cout << "NULL" << endl << "===" << endl;
}
