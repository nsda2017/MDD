#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Node.h"
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
template<typename T>
class LinkedList
{
    public:
        LinkedList()
        {
            count = 0;
            first = 0;
            last = 0;
        }
        ~LinkedList()
        {
            if(first != last)
                delete first;
            delete last;
        }
        int size()
        {
            return count;
        }
        Node<T>* front()
        {
            return first;
        }
        Node<T>* back()
        {
            return last;
        }
        virtual void push_front(T value) = 0;
        virtual void push_back(T value) = 0;
        virtual void insert_before(Node<T>* node, T value) = 0;
        virtual void insert_after(Node<T>* node, T value) = 0;
        virtual void pop_front() = 0;
        virtual void pop_back() = 0;
        virtual void erase(Node<T>* node) = 0;
        virtual Node<T>*  search_domain(T value) = 0;
		virtual void push_IP(T value, Node<T>* node) = 0;
		//virtual void erase_isolated_graph(char file[])=0;
		virtual void erase_single_querydomain()=0;
		int count;
        Node<T>* first;
        Node<T>* last;
		
    protected:
       
    private:
};

#endif // LINKEDLIST_H
