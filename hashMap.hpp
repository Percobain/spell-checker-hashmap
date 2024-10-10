#pragma once
#include "hashLink.h"
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

#define HASH_FUNCTION hashFunction1
#define MAX_TABLE_LOAD .75
template <typename K, typename V> //allows to create generic code that can work with any data type

class HashMap 
{
    public:
        /*
        * Parameterized HashMap constructor
        */
        HashMap(int capacity) 
        {
            mCapacity = capacity; 
            mSize = 0; //Tracks the number of key-value pairs currently in the hash map
            mTable = new HashLink<K, V> *[capacity]();
            for (int i = 0; i < capacity; i++)		
                mTable[i] = nullptr;
        }

        ~HashMap() 
        { 
            HashLink<K, V> *temp, *next;

            for (int i = 0; i < mCapacity; i++) 
            {
                temp = mTable[i];
                while (temp != nullptr) 
                {
                    next = temp->getNext();
                    delete temp;
                    temp = next;
                }
            }
            delete[] mTable;
        }


    /*
	 * Returns a pointer to the value of the link with the given key. Returns nullptr if no 
	 * link with the given key is in the table.
     */
    V* mapGet(const K &key) {

            // get index of bucket
            int index = HASH_FUNCTION(key) % mapCapacity(); 
            if (index < 0)
                index += mapCapacity();

            HashLink<K, V> *temp = mTable[index];
            while (temp != nullptr) {
                if (temp->getKey().compare(key) == 0) {
                    V tempVal = temp->getValue();
                    return &tempVal;
                }

                temp = temp->getNext();
            }
            return nullptr;
        }

    /*
	 * Updates the given key-value pair in the hash table if a link with the given key
	 * already exists. Otherwise allocates a new link with the given key-value pair and adds it
	 * to the corresponding bucket's linked list.*/

    void mapPut(const K key, const V value) {

		// resize table if table load exceeds max threshold (default .75)
		if (mapTableLoad() >= MAX_TABLE_LOAD)
			resizeTable(mCapacity * 2);

		// get index of bucket
		int index = HASH_FUNCTION(key) % mapCapacity();
		if (index < 0)
			index += mCapacity;

		// update link value if key exists in table
		if (mapContains(key)) {
			HashLink<K, V> *entry = mTable[index];
			entry->setValue(value);
		}

		// otherwise add new link to bucket
		else {
			HashLink<K, V> *newLink = new HashLink<K, V> (key, value);

			if (!mTable[index]) 
				mTable[index] = newLink;

			else {
				// add new link to the end 
				HashLink<K, V> *temp = mTable[index];
				while (temp->getNext() != nullptr) 
					temp = temp->getNext();

				temp->setNext(newLink);
			}
			mSize++;
		}
	}

    /*
	 * Attemps to locate the key-value pair specified by the key parameter from the hash table.
	 * Returns true if the key-value pair is found in the table and false otherwise*/
    bool mapRemove(const K &key) 
    {
		if (mapContains(key)) {
			int index = HASH_FUNCTION(key) % mapCapacity();
			if (index < 0)
				index += mCapacity;

			HashLink<K, V> *temp = mTable[index];
			HashLink<K, V> *prev = nullptr;
			while (temp != nullptr) {

                // Check if the current node's key matches the key to be removed
				if (temp->getKey().compare(key) == 0) {
					if (prev)
						prev->setNext(temp->getNext());

					mTable[index] = temp->getNext();  // If removing the head, update the head of the list
					delete temp;
					mSize--;
					return true;
				}

				else {
					prev = temp;
					temp = temp->getNext();
				}
			}
		}

		return false;
	}

};