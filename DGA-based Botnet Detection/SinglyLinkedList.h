#ifndef SINGLELINKEDLIST_H
#define SINGLELINKEDLIST_H
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
using namespace std;

template<typename T>
class SinglyLinkedList : public LinkedList<T>
{
    public:
        
        void push_back(T value)
        {
            Node<T>* node = new Node<T>(value);
            if (LinkedList<T>::count == 0)
                LinkedList<T>::first = node;
            else
                LinkedList<T>::last->next = node;
            LinkedList<T>::last = node;
            ++LinkedList<T>::count;
        }
        void push_IP(T value, Node<T>* node)
        {
            
            if (node->previous==0)
            {
            	Node<T>* ipnode = new Node<T>(value);
            	node->previous=ipnode;
            	node->ipcount++;
        	}
        	else
			{
				if(find_previous_IP(value, node)!=0)
				{
					node->ipcount++;
					Node<T>* ipnode = new Node<T>(value);
					find_previous_IP(value, node)->previous=ipnode ;
					
				}
			}
			
			
        }
        Node<T>* search_domain(T value)
		{
			Node<T>* preNode = LinkedList<T>::first;
            while (preNode != 0)
            {
                if (value == preNode->value)
                    return preNode;
                preNode = preNode->next;
            }
        	return 0;
		} 
		void push_front(T value)
        {
            Node<T>* node = new Node<T>(value);
            if (LinkedList<T>::count == 0)
                LinkedList<T>::last = node;
            else
                node->next = LinkedList<T>::first;
            LinkedList<T>::first = node;
            ++LinkedList<T>::count;
        }
        void insert_before(Node<T>* node, T value)
        {
            Node<T>* newNode = new Node<T>(value);
            if (LinkedList<T>::first != node)
            {
                Node<T>* preNode = find_previous_node(node);
                preNode->next = newNode;
            }
            newNode->next = node;
            ++LinkedList<T>::count;
        }
        void insert_after(Node<T>* node, T value)
        {
            Node<T>* newNode = new Node<T>(value);
            newNode->next = node->next;
            node->next = newNode;
            ++LinkedList<T>::count;
        }
        void pop_front()
        {
            if (LinkedList<T>::count == 0)
                throw std::out_of_range ("empty list");
            else if (LinkedList<T>::count == 1)
            {
                LinkedList<T>::first = 0;
                LinkedList<T>::last = 0;
            }
            else
            {
                Node<T>* node = LinkedList<T>::first->next;
                //LinkedList<T>::first->next = 0;
                LinkedList<T>::first = node;
            }
            --LinkedList<T>::count;
        }
        void pop_back()
        {
            if (LinkedList<T>::count == 0)
                throw std::out_of_range ("empty list");
            else if (LinkedList<T>::count == 1)
            {
                LinkedList<T>::first = 0;
                LinkedList<T>::last = 0;
            }
            else
            {
                Node<T>* node = find_previous_node(LinkedList<T>::last);
                node->next = 0;
                LinkedList<T>::last = node;
            }
            --LinkedList<T>::count;
        }
        void erase(Node<T>* node)
        {
            if (node == LinkedList<T>::first)
                pop_front();
            else if (node == LinkedList<T>::last)
                pop_back();
            else
            {
                Node<T>* preNode = find_previous_node(node);
                if (preNode == 0)
                    throw std::out_of_range ("node not in list");
                preNode->next = node->next;
                //node->next = 0;
                --LinkedList<T>::count;
            }
        }
        void erase_isolated_graph(char file[])
        {
        	Node<T>* isonode = LinkedList<T>::first;
        	while(isonode != 0)
        	{
        		Node<T>* chenode = isonode->previous;
        		if(isonode->ipcount==1 && check_isolate_ip(chenode->value)==1)
        			erase(isonode);
        		isonode=isonode->next;
        	}
        }
        void erase_single_querydomain()
        {
        	Node<T>* singledomain = LinkedList<T>::first;
        	while(singledomain!=0)
        	{
        		if(singledomain->ipcount==1)
        			erase(singledomain);
        		singledomain=singledomain->next;
        	}
        }
    protected:
    private:
        Node<T>* find_previous_node(Node<T>* node)
        {
            Node<T>* preNode = LinkedList<T>::first;
            while (preNode != 0)
            {
                if (node == preNode->next)
                    break;
                preNode = preNode->next;
            }
            return preNode;
        }
        Node<T>* find_previous_IP(T value, Node<T>* node)
        {
        	Node<T>* preIPNode = node;
        	while (preIPNode->previous != 0)
            {
				preIPNode = preIPNode->previous;
                if (value == preIPNode->value)
                {
                	return 0;
            	}
            }
            return preIPNode;
        }
        /*int check_isolate_ip(T value)
        {
			int sameipcount=0;
			//cin.getline("123test",20);
			fstream fin;
			char buffer[400];
			fin.open("123.txt",ios::in);
			if(!fin)
				cout<<"readfile error!!!!!!";
			else
			{	
				while(!fin.eof())
				{                   					 
					fin.getline(buffer,sizeof(buffer));   					 //讀取一行 
  					char *pch = strtok(buffer," ");	
					int i=1;
					string domainname, ipaddress;
					while (pch != 0)
					{
    					if(i==12)
						{
    						ipaddress.assign(pch);
    						ipaddress.erase(ipaddress.find("#"),ipaddress.size());//移除ip後面接的port 
						}                          
    					if(i==14)
    					domainname.assign(pch);
   						pch = strtok (NULL," ");
    					i++;
    				
  					}
  					if(domainname!="." && value==ipaddress)
  						sameipcount++;
				}
			}
			return sameipcount;
        }*/
};










#endif // SINGLELINKEDLIST_H
