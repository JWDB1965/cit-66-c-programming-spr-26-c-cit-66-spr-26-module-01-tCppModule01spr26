#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* next;
    Node* prev; 


void insertEnd(Node*& head, int value) {
    Node* newNode = new Node{value, nullptr, nullptr};
    
    if (!head) {
        head = newNode;
        return;
    }

    Node* temp = head;
    while (temp->next) {
        temp = temp->next;
    }

    temp->next = newNode;
    newNode->prev = temp; 
}


void deleteValue(Node*& head, int value) {
    if (!head) return;

    Node* curr = head;


    while (curr && curr->data != value) {
        curr = curr->next;
    }

  
    if (!curr) return;

 
    if (curr == head) {
        head = curr->next;
        if (head) head->prev = nullptr;
    } else {
      
        curr->prev->next = curr->next;
        if (curr->next) {
            curr->next->prev = curr->prev;
        }
    }

    delete curr;
}

void displayForward(Node* head) {
    cout << "Forward: ";
    while (head) {
        cout << head->data << " <-> ";
        head = head->next;
    }
    cout << "NULL" << endl;
}

int main() {
    Node* head = nullptr;

    insertEnd(head, 100);
    insertEnd(head, 200);
    insertEnd(head, 300);

    displayForward(head);

    cout << "Deleting 200..." << endl;
    deleteValue(head, 200);
    
    displayForward(head);

    return 0;
}
