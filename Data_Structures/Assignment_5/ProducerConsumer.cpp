#include "ProducerConsumer.hpp"
#include <iostream>

using namespace std;

ProducerConsumer::ProducerConsumer()
{
    queueFront = 0;
    queueEnd = 0;
}
bool ProducerConsumer::isEmpty()
{
    if(counter == 0)
        return true;
    else
        return false;
}
bool ProducerConsumer::isFull()
{
    if(counter == SIZE)
        return true;
    else
        return false;
}
void ProducerConsumer::enqueue(std::string player)
{
    if(!isFull() && !isEmpty())
    {
        queue[queueEnd] = player;
        if(queueEnd == SIZE-1)
        {
            queueEnd = 0;
        }
        else
        {
            queueEnd++;
        }
        counter++;
    }
    else if(!isFull() && isEmpty())
    {
        queue[0] = player;
        queueEnd++;
        counter++;
    }
    else
    {
        cout << "Queue full, cannot add new item" << endl;
    }
}
void ProducerConsumer::dequeue()
{
    if(!isEmpty())
    {
        queue[queueFront] = "";
        if(queueFront == SIZE-1)
            queueFront = 0;
        else
            queueFront++;
        counter--;
    }
    else
    {
        cout << "Queue empty, cannot dequeue an item" << endl;
    }
}
int ProducerConsumer::queueSize()
{
    if(isEmpty())
        return 0;
    else
        return counter;
}
string ProducerConsumer::peek()
{
    if(!isEmpty())
        return queue[queueFront];
    else{
        cout << "Queue empty, cannot peek" << endl;
        return "";
    }
}