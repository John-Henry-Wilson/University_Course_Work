/****************************************************************/
/*                Producer Consumer Driver File                 */
/****************************************************************/
/* TODO: Implement menu options as described in the writeup     */
/****************************************************************/

#include "ProducerConsumer.hpp"
#include <iostream>
#include <string>
// you may include more libraries as needed

using namespace std;

/*
 * Purpose: displays a menu with options
 * @param none
 * @return none
 */
void menu()
{
	cout << "*----------------------------------------*" << endl;
	cout << "Choose an option:" << endl;
    cout << "1. Producer (Produce items into the queue)" << endl;
	cout << "2. Consumer (Consume items from the queue)" << endl;
	cout << "3. Return the queue size and exit" << endl;
	cout << "*----------------------------------------*" << endl;
}
// Use getline for reading
int main(int argc, char const *argv[])
{
	ProducerConsumer queuePC;
	string input = "", numItems = "", enque = "", consume = "";
	int con = 0;
	while(input != "3")
	{
		menu();
		getline(cin, input);
		if (input == "1")
		{
			cout << "Enter the number of items to be produced:" << endl;
			getline(cin, numItems);
			for(int i = 0; i < stoi(numItems); i++)
			{
				cout << "Item" << i + 1 << ":" << endl;
				getline(cin, enque);
				queuePC.enqueue(enque);
			}

		}
		else if (input == "2")
		{
			cout << "Enter the number of items to be consumed:" << endl;
			getline(cin, consume);
			con = stoi(consume);
			while(!queuePC.isEmpty() &&  con != 0)
			{
				cout << "Consumed: " << queuePC.peek() << endl;
				queuePC.dequeue();
				con--;
				if(queuePC.isEmpty())
				{
					cout<< "No more items to consume from queue" << endl;
					break;
				}
			}
		}
		else if (input == "3")
		{
			cout << "Number of items in the queue:" << queuePC.queueSize();
		}
	}
}
