#ifndef NODE_H
#define NODE_H
#include <cstring>
#include <string>
template<typename T>
class Node
{
    public:
        Node(T v)
        {
            value = v;
            previous = 0;
            next = 0;
            ipcount = 0;
        }
        ~Node()
        {
            if(previous != next)
                delete previous;
            delete next;
        }
        Node<T>* previous;
        Node<T>* next;
        T value;
        int ipcount;
    protected:
    private:
};

#endif // NODE_H
