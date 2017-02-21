#include "HashObj.h"
#include <fstream>
#include <iostream>
using namespace std;

int main() {
	// Begin by prepping incoming data to be handled by the classes.

	ifstream infile("phonebook.txt");

	if (!infile) {
		cout << "Whoops couldn't open the file." << endl;
		return 1;
	}

	//Create PhoneHash object
	PhoneHash H;

	//Create Output.txt
	ofstream outfile("output.csv");

	//Build the Hashmap

	H.buildHash(infile);
	H.hashMapStats(outfile);

	infile.close();
	return 0;
}