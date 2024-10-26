#pragma once
#include "hashLink.h"
#include <iostream>
#include <string>

using namespace std;

#define HASH_FUNCTION hashFunction1
#define MAX_TABLE_LOAD .75

template <typename K, typename V>
class HashMap {
public:
	HashMap(int capacity) {
		mCapacity = capacity;
		mSize = 0;
		mTable = new HashLink<K, V> *[capacity]();
		for (int i = 0; i < capacity; i++)		
			mTable[i] = nullptr;
	}

	~HashMap() { 
		HashLink<K, V> *temp, *next;

		for (int i = 0; i < mCapacity; i++) {
			temp = mTable[i];
			while (temp != nullptr) {
				next = temp->getNext();
				delete temp;
				temp = next;
			}
		}
		delete[] mTable;
	}

	V* mapGet(const K &key) {
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

	void mapPut(const K key, const V value) {
		if (mapTableLoad() >= MAX_TABLE_LOAD)
			resizeTable(mCapacity * 2);

		int index = HASH_FUNCTION(key) % mapCapacity();
		if (index < 0)
			index += mCapacity;

		if (mapContains(key)) {
			HashLink<K, V> *entry = mTable[index];
			entry->setValue(value);
		}

		else {
			HashLink<K, V> *newLink = new HashLink<K, V> (key, value);

			if (!mTable[index]) 
				mTable[index] = newLink;

			else {
				HashLink<K, V> *temp = mTable[index];
				while (temp->getNext() != nullptr) 
					temp = temp->getNext();

				temp->setNext(newLink);
			}
			mSize++;
		}
	}

	bool mapRemove(const K &key) {
		if (mapContains(key)) {
			int index = HASH_FUNCTION(key) % mapCapacity();
			if (index < 0)
				index += mCapacity;

			HashLink<K, V> *temp = mTable[index];
			HashLink<K, V> *prev = nullptr;
			while (temp != nullptr) {
				if (temp->getKey().compare(key) == 0) {
					if (prev)
						prev->setNext(temp->getNext());

					mTable[index] = temp->getNext();
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

	bool mapContains(const K &key) {
		int index = HASH_FUNCTION(key) % mapCapacity();
		if (index < 0)
			index += mCapacity;

		HashLink<K, V> *temp = mTable[index];
		while (temp != nullptr) {
			if (temp->getKey().compare(key) == 0)
				return true;

			temp = temp->getNext();
		}

		return false;
	}

	int mapSize() const { return mSize; }

	int mapCapacity() const { return mCapacity; } 

	HashLink<K, V>* mapTableLink(int index) const { return mTable[index]; }

	int mapEmptyBuckets() const {
		int empty = 0;
		for (int i = 0; i < mapCapacity(); i++) {
			if (mTable[i] == nullptr)
				empty++;
		}

		return empty;
	}

	double mapTableLoad() const {
		double links = (double)mapSize();
		double buckets = (double)mapCapacity();
		return (links / buckets);
	}

	void hashMapCleanup(HashLink<K, V> **map, int capacity) {
		HashLink<K, V> *temp, *next;

		for (int i = 0; i < capacity; i++) {
			temp = map[i];
			while (temp != nullptr) {
				next = temp->getNext();
				delete temp;
				temp = next;
			}
		}
	}

	void resizeTable(int newCapacity) {
		HashLink<K, V> **oldTable = mTable;
		int oldCapacity = mCapacity;
		mCapacity = newCapacity;

		mTable = new HashLink<K, V> *[newCapacity]();

		for (int i = 0; i < newCapacity; i++)		
			mTable[i] = nullptr;

		mSize = 0;
		for (int i = 0; i < oldCapacity; i++) {
			HashLink<K, V> *temp = oldTable[i];

			while (temp != nullptr) {
				K tempKey = temp->getKey();
				V tempVal = temp->getValue();
				mapPut(tempKey, tempVal);
				temp = temp->getNext();
			}
		}

		hashMapCleanup(oldTable, oldCapacity);
		delete[] oldTable;
	}


	friend std::ostream& operator<<(std::ostream& os, const HashMap<K, V>& map) {
		for (int i = 0; i < map.mapCapacity(); i++) {
			HashLink<K, V> *entry = map.mTable[i];
			if (entry != nullptr) {
				os << "Bucket " << i << " -> ";
				while (entry != nullptr) {
					os << "(" << entry->getKey() << ", " << entry->getValue() << ") -> ";
					entry = entry->getNext();
				}
				os << "nullptr";
				os << '\n';
			}
		}
		os << '\n';
		return os;
	}

	int hashFunction1(const K &key) {
		int r = 0;
		for (int i = 0; key[i] != '\0'; i++)
			r += key[i];

		return r;
	}

	int hashFunction2(const K &key) {
		int r = 0;
		for (int i = 0; key[i] != '\0'; i++)
			r += (i + 1) * key[i];

		return r;
	}

private:
	HashLink<K, V>** mTable;
	int mSize;
	int mCapacity;
};