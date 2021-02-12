#include "RPNCalculator.hpp"
#include <iostream>
#include <string>

using namespace std;


RPNCalculator::RPNCalculator()
{
    stackHead = NULL;
}

RPNCalculator::~RPNCalculator()
{
    while(!isEmpty())
    {
        pop();
    }
}

bool RPNCalculator::isEmpty()
{
    if(stackHead == NULL)
        return true;
    else
        return false;
}

void RPNCalculator::push(float number)
{
    Operand *temp = new Operand();
    temp->number = number;
    temp->next = stackHead;
    stackHead = temp;
}

void RPNCalculator::pop()
{
    if(!isEmpty()){
        Operand *temp = new Operand();
        temp = stackHead;
        stackHead = stackHead->next;
        delete temp;
    }else{
        cout << "Stack empty, cannot pop an item." << endl;
    }
}

Operand* RPNCalculator::peek()
{
    if(!isEmpty()){
        return stackHead;
    }else{
        cout << "Stack empty, cannot peek." << endl;
        return NULL;
    }
}

bool RPNCalculator::compute(string symbol)
{
    float num1;
    float num2;
    float ans;

    if(symbol == "+"){
        if(isEmpty()){
            cout << "err: not enough operands" << endl;
            return false;
        }
        num1 = stackHead->number;
        pop();
        if(isEmpty()){
            cout << "err: not enough operands" << endl;
            push(num1);
            return false;
        }
        num2 = stackHead->number;
        pop();
        ans = num1+num2;
        push(ans);
        return true;
    }else if(symbol == "*"){
        if(isEmpty()){
            cout << "err: not enough operands" << endl;
            return false;
        }
        num1 = stackHead->number;
        pop();
        if(isEmpty()){
            cout << "err: not enough operands" << endl;
            push(num1);
            return false;
        }
        num2 = stackHead->number;
        pop();
        ans = num1*num2;
        push(ans);
        return true;
    }else if(symbol == "="){
        return true;
    }else{
        cout << "err: invalid operation" << endl;
        return false;
    }
}