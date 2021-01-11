#ifndef _Tree_H_
#define _Tree_H_
#include <iostream>
#include <vector>
#include <queue>
#include <string>
class Tree
{
    char data;
    std::vector<Tree*> children;

    Tree* addChild(const char c)
    {
        children.push_back(new Tree(c));
        return children[children.size()-1];
    }

    Tree* getChild(const char c)
    {
        for(int i=0; i<children.size(); i++)
        {
            if(children[i]->data == c)
                return children[i];
        }
        return nullptr;
    }
    void deleteTree()
    {
        for(int i=0; i<children.size(); i++)
            delete children[i];
    }
public:
    Tree(const char data) : data(data) {}
    ~Tree()
    {
        deleteTree();
    }
    void addPath(std::string str)
    {
        if(str.size()==0)
            return;

        Tree* child = getChild(str[0]);
        if(child == nullptr)
            child = addChild(str[0]);
        child->addPath(str.substr(1));
    }
    void print()
    {
        std::cout<<data<<"-";
        for(int i=0; i<children.size(); i++)
            children[i]->print();

    }

};
#endif // _Tree_H_
