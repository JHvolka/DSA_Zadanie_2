/*
 * Prevzaté z https://gist.github.com/Ashwin-op/70581aba99c793d64a2e428e72786ab0
 *
 * Taken from https://gist.github.com/Ashwin-op/70581aba99c793d64a2e428e72786ab0
 */

#include <iostream>
#include <string>
#include <math.h> // floor()

using namespace std;

// EDIT I put all code inside HashSC namespace to avoid collisions with other data structures
namespace HashSC {
    struct Node {
        int key;      // number
        string value; // value
        Node *next;   // pointer to remember memory address of next node

        Node() : key(0), value(""), next(0) {};

        // EDIT Int replaced by size_t
        Node(int Key, string Value) : key(Key), value(Value), next(0) {};

        Node(Node const &data) : key(data.key), value(data.value),
                                 next(data.next) {};
    };

    class HashTable {
    private:
        int size,     // size: size of table, count: number of data
        count;    // count/size = load factor
        Node **table; // pointer to pointer, hash table

        int hashFunction(int key); // Multiplication method
        void tableDoubling();

        void tableShrinking();

        void reHashing(int size_orig);

    public:
        // EDIT Empty constructor did not initialize anything, so I added initializer list and initialized table
        HashTable() :size(128),count(0) {
            table = new Node *[size]; // allocate the first demension of table
            for (int i = 0; i < size; i++)
                table[i] = 0; // ensure every slot points to NULL
        };

        HashTable(int m) : size(m), count(0) {
            table = new Node *[size]; // allocate the first demension of table
            for (int i = 0; i < size; i++)
                table[i] = 0; // ensure every slot points to NULL
        }

        ~HashTable();

        void Insert(Node data); // consider tableDoubling()
        void Delete(int key);   // consider tableShrinking()
        string Search(int key);

        void displayTable();
    };

    void HashTable::Insert(Node data) {
        count++;
        if (count > size) {                    // consider load factor
            tableDoubling(); // if n/m > 1, then double the size of table
        }

        int index = hashFunction(data.key); // get index of slot
        Node *newNode = new Node(data);     // create new node to store data

        // push_front()
        if (table[index] ==
            NULL) {                           // eg: list: (empty), add4
            table[index] = newNode; // eg: list: 4->NULL
        }
        else {                                    // eg: list: 5->9->NULL  , add 4
            Node *next = table[index]->next; //     list: 5->4->9->NULL
            table[index]->next = newNode;
            newNode->next = next;
        }
    }

    void HashTable::Delete(int key) {
        int index = hashFunction(key); // get index of slot
        Node *current = table[index],  // use two pointer for traversal in list
        *previous = NULL;

        while (current != NULL && current->key != key) {
            previous = current;      // traversal in list, 3 cases:
            current = current->next; // 1. data not found
        }                            // 2. data found at first node in list
        // 3. data found at other position in list

        if (current == NULL) { // eg: list:5->2->9->NULL, want to delete 3
            cout << "Data not found!\n\n";
            return;
        }
        else {
            if (previous ==
                NULL) {                                 // eg: list:5->2->9->NULL, want to delete 5
                table[index] = current->next; // after deleting 5, list:2->9->NULL
            }                                 // current points to 5

            else {                                   // eg: list:5->2->9->NULL, want to delete 2
                previous->next = current->next; // after deleting 2, list:5->9->NULL
            }                                   // current points to 2
            delete current;
            current = 0;
        }

        count--;
        if (count < size / 4) {                     // consider load factor
            tableShrinking(); // if n/m < 4, then shrink the table
        }
    }

    string HashTable::Search(int key) {
        int index = hashFunction(key); // get index of slot
        Node *current = table[index];  // current points to the first node in list

        while (current != NULL) { // traversal in list
            if (current->key == key)
                return current->value;
            current = current->next;
        }
        return "\nNo such data!";
    }

    int HashTable::hashFunction(int key) {
        // Multiplication method
        double A = 0.6180339887, frac = key * A - floor(key * A);
        return floor(this->size * frac);
    }

    void HashTable::tableDoubling() {
        int size_orig = size; // size_orig represents the original size of table
        size *= 2;            // double the size of table
        reHashing(size_orig); // create new table with new larger size
    }

    void HashTable::tableShrinking() {
        int size_orig = size; // size_orig represents the original size of table
        size /= 2;            // shrink the size of table
        reHashing(size_orig); // create new table with new smaller size
    }

    void HashTable::reHashing(int size_orig) {
        Node **newtable = new Node *[size]; // allocate memory for new table
        for (int i = 0; i < size; i++) {                    // initializetion
            newtable[i] = 0; // ensure every node in slot points to NULL
        }

        for (int i = 0;
             i < size_orig; i++) { // visit every node in the original table

            Node *curr_orig = table[i], // curr_orig: current node in original table
            *prev_orig = NULL;      // prev_orig: following curr_orig

            while (curr_orig !=
                   NULL) { // traversal in list of each slot in original table
                prev_orig = curr_orig->next; // curr_orig will be directly move to new table
                // need prev_orig to keep pointer in original table

                int index = hashFunction(
                        curr_orig->key); // get index of slot in new table

                // push_front(), do not allocate new memory space for data
                // directly move node in original table to new table
                if (newtable[index] == NULL) { // means newtable[index] is empty
                    newtable[index] = curr_orig;
                    newtable[index]->next = 0; // equivalent to curr_orig->next = 0;
                }
                    // if there is no initialization for newtable, segmentation faults might happen
                    // because newtable[index] might not point to NULL
                    // but newtable[index] is empty
                else {                                       // if newtable[index] is not empty
                    Node *next = newtable[index]->next; // push_front()
                    newtable[index]->next = curr_orig;
                    curr_orig->next = next;
                }
                curr_orig = prev_orig; // visit the next node in list in original table
            }
        }
        delete[] table;         // release memory of original table
        this->table = newtable; // point table of object to new table
    }

    HashTable::~HashTable() {
        for (int i = 0; i <
                        size; i++) {                             // visit every node in table and release the memory of each node
            Node *current = table[i]; // point *current to first node in list
            while (current != NULL) { // traversal in list
                Node *previous = current;
                current = current->next;
                delete previous;
                previous = 0;
            }
        }
        delete[] table;
    }

    void HashTable::displayTable() {
        for (int i = 0; i < size; i++) { // visit every node in table
            cout << "Slot " << i << ": ";
            Node *current = table[i];
            while (current != NULL) {
                cout << "(" << current->key << "," << current->value << ") ";
                current = current->next;
            }
            cout << endl;
        }
        cout << endl;
    }
}

// EDIT removed main function