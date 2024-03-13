#include "NotationConverter.hpp"
#include <iostream>
#include <string>

class Node {
    public:
    Node(std::string data) : data(data), next(nullptr), prev(nullptr) {}
    std::string data;
    Node *next;
    Node *prev;
};

class DLinkedList {
    public:
    DLinkedList(): head(nullptr) {}

    int size() {
        if(!head) throw std::runtime_error("List is empty");
        int n = 0;

        Node *curr = head;
        while(curr) {
            n++;
            curr = curr->next;
        }
        return n;
    }

    std::string front() {
        if(!head) throw std::runtime_error("List is empty");

        Node *curr = head;

        while(curr->next) {
            curr = curr->next;
        }

        return curr->data;
    }

    std::string back() {
        if(!head) throw std::runtime_error("List is empty");

        return head->data;
    }

    void insertBack(std::string value) {
        Node *newNode = new Node(value);

        if(!head) {
            head = newNode;
            return;
        }

        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }

    void insertFront(std::string value) {
        Node *newNode = new Node(value);

        if(!head) {
            head = newNode;
            return;
        }

        Node *curr = head;

        while(curr->next) {
            curr = curr->next;
        }

        curr->next = newNode;
        newNode->prev = curr;
    }

    void removeBack() {
        if(!head) throw std::runtime_error("List is empty");

        Node *temp = head;
        head = head->next;
        head->prev = nullptr;
        delete temp;
    }

    void removeFront() {
        if(!head) throw std::runtime_error("List is empty");

        if(!head->next) {
            Node *temp = head;
            head = nullptr;
            delete temp;
            return;
        }

        Node *curr = head;

        while(curr->next) {
            curr = curr->next;
        }

        Node *temp = curr;
        curr->prev->next = nullptr;
        temp->prev = nullptr;
        delete temp;
    }

    ~DLinkedList() {
        while(head) {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
    }
    private:
    Node *head;
};

class LinkedDeque {
    public:
    LinkedDeque(): n(0) {};

    void insertFront(std::string value) {
        D.insertFront(value); n++;
    }

    void insertBack(std::string value) {
        D.insertBack(value); n++;
    }

    void removeFront() {
        D.removeFront(); n--;
    }

    void removeBack() {
        D.removeBack(); n--;
    }

    std::string front() {
        return D.front();
    }

    std::string back() {
        return D.back();
    }

    private:
    int n;
    DLinkedList D;
};


class NotationConverter : public NotationConverterInterface {
public:
    std::string postfixToInfix(std::string inStr) override {
        for(int i = 0; i < inStr.size(); i++) {
            std::string ch = std::string(1, inStr[i]);
            if(ch == " ") {
                continue;
            }
            else if(ch == "*" || ch == "/" || ch == "+" || ch == "-") {
                std::string a,b;
                b = D.front();
                D.removeFront();
                a = D.front();
                D.removeFront();
                D.insertFront("(" + a + " " + ch + " " + b + ")");
            } else {
                D.insertFront(ch);
            }
        }
        return D.front();
    }

    std::string postfixToPrefix(std::string inStr) override {
        for(int i = 0; i < inStr.size(); i++) {
            std::string ch = std::string(1, inStr[i]);
            if(ch == " ") {
                continue;
            }
            else if (ch == "*" || ch == "/" || ch == "+" || ch == "-") {
                std::string a,b;
                b = D.front();
                D.removeFront();
                a = D.front();
                D.removeFront();
                D.insertFront(ch + " " + a + " " + b);
            } else {
                D.insertFront(ch);
            }
        }
        return D.front();
    }

    std::string deleteSpace(std::string str) {
        std::string str_no_space;
        for(int i = 0; i < str.size(); i++)
            if(str[i] != ' ') str_no_space.push_back(str[i]);

        return str_no_space;
    }

    std::string infixToPostfix(std::string inStr) override {
        std::string prefix = infixToPrefix(inStr);
        return prefixToPostfix(prefix);
    }

    std::string infixToPrefix(std::string inStr) override {
               inStr = deleteSpace(inStr);
        // Starting from index 1 and ending with index (n - 1) is fine
        // since the first and last char will always be '(' and ')'.
        for(int i = 1; i < inStr.size() - 1; i++) {
            std::string prev_ch = std::string(1, inStr[i - 1]);

            std::string ch = std::string(1, inStr[i]);

            std::string next_ch = std::string(1, inStr[i + 1]);

            if(ch == "+" || ch == "-" || ch == "*" || ch == "/") {
                if(prev_ch == ")" && next_ch == "(") {
                    D.insertFront(ch);
                } else if(prev_ch == ")") {
                    D.insertFront(ch);
                    D.insertBack(next_ch);
                } else if(next_ch == "(") {
                    D.insertFront(ch);
                    D.insertBack(prev_ch);
                } else {
                    D.insertBack(ch);
                    D.insertBack(prev_ch);
                    D.insertBack(next_ch);
                }
            }
        }
        int count = D.size();

        std::string prefix;

        for(int i = 0; i < count - 1; i++) {
            prefix = prefix + D.front() + " ";
            D.removeFront();
        }
        prefix = prefix + D.front();
        D.removeFront();

        return prefix;
    }

    std::string prefixToInfix(std::string inStr) override {
        for(int i = inStr.size() - 1; i >= 0; i--) {
            std::string ch = std::string(1, inStr[i]);
            if(ch == " ") continue;
            else if (ch == "*" || ch == "/" || ch == "+" || ch == "-") {
                std::string a,b;
                a = D.front();
                D.removeFront();
                b = D.front();
                D.removeFront();
                D.insertFront("(" + a + " " + ch + " " + b + ")");
            }
            else D.insertFront(ch);
        }
        return D.front();
    }

    std::string prefixToPostfix(std::string inStr) override {
        for(int i = inStr.size() - 1; i >= 0; i--) {
            std::string ch = std::string(1, inStr[i]);
            if(ch == " ") continue;
            else if (ch == "*" || ch == "/" || ch == "+" || ch == "-") {
                std::string a,b;
                a = D.front();
                D.removeFront();
                b = D.front();
                D.removeFront();
                D.insertFront(a + " " + b + " " + ch);
            }
            else D.insertFront(ch);
        }
        return D.front();
    }

    private:
    DLinkedList D;
};
