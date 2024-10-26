#include "hashMap.hpp"
#include <fstream>
#include <ctime>

using namespace std;

int loadDictionary(string fname, HashMap<string, int> *map);
int calcLD(string word1, string word2);
void spellChecker(HashMap<string, int> *dictionary);

int main() {
	double start, end, elapsed;
	string dictionaryFile = "dictionary.txt";
	HashMap<string, int> *dictionary = new HashMap<string, int> (1000);

	cout << "Loading dictionary file..." << '\n';

	start = clock();
	int loadStatus = loadDictionary(dictionaryFile, dictionary);
	end = clock();
	elapsed = end - start;
	elapsed /= CLOCKS_PER_SEC;

	if (loadStatus == -1) {
		cout << "Failed to load dictionary file!" << '\n';
		delete dictionary;
		return 1;
	}

	cout << "Dictionary loaded in " << elapsed << " seconds." << '\n';
	cout << "Dictionary contains " << dictionary->mapSize() << " entries hashed into " << dictionary->mapCapacity() << " buckets." << '\n';
	cout << "Table load: " << dictionary->mapTableLoad() << '\n';

	spellChecker(dictionary);

	delete dictionary;
	return 0;
}

int loadDictionary(string fname, HashMap<string, int> *map) {
	string inputBuffer = "";
	ifstream dictionaryFile(fname);
	if (dictionaryFile.is_open()) {
		while (!dictionaryFile.eof()) {
			getline(dictionaryFile, inputBuffer);
			map->mapPut(inputBuffer, 1);
		}
		dictionaryFile.close();	
		return 0;
	}

	else
		return -1;
}

int calcLD(string word1, string word2) {
	int word1Len = word1.length();
	int word2Len = word2.length();

	if (word1Len == 0)
		return word2Len;

	if (word2Len == 0)
		return word1Len;

	int matrix[word1Len + 1][word2Len + 1];

	for (int i = 0; i <= word1Len; i++)
		matrix[i][0] = i;

	for (int i = 0; i <= word2Len; i++)
		matrix[0][i] = i;

	for (int i = 1; i <= word1Len; i++) {
		char c1 = word1[i - 1];

		for (int j = 1; j <= word2Len; j++) {
			char c2 = word2[j - 1];

			if (c1 == c2)
				matrix[i][j] = matrix[i - 1][j - 1];

// 					   h a t
// 					 c 1 2 3
// 					 a 2 1 2
// 					 t 3 2 1

			else {
				int deletion = matrix[i - 1][j] + 1;
				int insert = matrix[i][j - 1] + 1;
				int sub = matrix[i-1][j - 1] + 1;
				
				int min = deletion;
				if (insert < min)
					min = insert;

				if (sub < min)
					min = sub;

				matrix[i][j] = min;
			}
		}
	}

	return matrix[word1Len][word2Len];
}

void spellChecker(HashMap<string, int> *dictionary) {
	string inputBuffer = "";
	bool quit = false;

	while (!quit) {
		cout << "Enter a word to spell check or \"quit\" to exit: ";
		cin >> inputBuffer;

		if (inputBuffer.compare("quit") == 0)
			quit = true;

		else if (dictionary->mapContains(inputBuffer))
			cout << "\n\"" << inputBuffer << "\"" << " is spelled correctly.\n" << '\n';

		else {
			cout << "\nDid you mean: " << '\n';
			
			for (int i = 0; i < dictionary->mapCapacity(); i++) {
				HashLink<string, int> *seeker = dictionary->mapTableLink(i);

				while (seeker) {
					string seekerKey = seeker->getKey();

					if (seekerKey.length() >= inputBuffer.length() && seekerKey[0] == inputBuffer[0]) {
						int LD = calcLD(inputBuffer, seeker->getKey());
                    	if ((LD == 1 || LD == 2)) 
                    		cout << seekerKey << '\n';
                	}

                	seeker = seeker->getNext();
				}
			}
			cout << '\n';
		} 
	}
}