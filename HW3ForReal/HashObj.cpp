#pragma once
#include "HashObj.h"

/*
Public Functions
*/

PhoneHash::PhoneHash()
{

}

PhoneHash::~PhoneHash()
{

}


/*
buildHash recieves an infile stream fromFile and pulls out lines
one at a time as strings that are passed into the buildHelper function.
*/
void PhoneHash::buildHash(ifstream& fromFile)
{
	// while there are more lines to read from fromFile, continue loop.
	while (fromFile)
	{
		string rawLine = ""; // initialize
		getline(fromFile, rawLine); // feed line into rawLine

		if (rawLine == "") // if the line is empty, return.
		{
			return;
		}

		// use buildHelper to convert string into DataNode object
		// to be stored in the hash.
		buildHelper(rawLine);
	}
}

/*
buildHelper recieves strings from buildHash. It assumes that every line
is formatted in such a way that sections are clearly delinated by these
special characters:		','		'\t'	')'		'-'

Using these characters, it identifies the proper substrings and extracts
them into independent strings. The phone number itself is broken into
three strings which are then cast as integers. After the conversions are
complete, it then passes these components to the createNode() function.
*/
void PhoneHash::buildHelper(const string& data)
{
	// use index, (find_first_of - index) to obtain substrings from the line of data
	int index = 0;
	int terminus = data.find_first_of(','); // for the first name

	string first = data.substr(index, terminus);
	index += terminus + 2; // +2 to find position of character after space
						  // finds tab character and subtracts index to find length
	terminus = data.find_first_of('\t') - index;

	string last = data.substr(index, terminus);
	index += terminus + 2; // +2 to move past the tab and the '('
						  // finds ')' character and subtracts index to find length
	terminus = data.find_first_of(')') - index;

	string area = data.substr(index, terminus);
	index += terminus + 2; // +2 to skip ) and ' '
						  // finds instance of dash
	terminus = data.find_first_of('-') - index;

	string threedig = data.substr(index, terminus);
	index += terminus + 1; // +1 since number is adjacent to dash
	terminus = data.length() - index; // find length from index to end of str.

	string fourdig = data.substr(index, terminus);

	// Convert Numeric Strings into Ints

	int areaCode = stoi(area);
	int threePart = stoi(threedig);
	int fourPart = stoi(fourdig);

	createNode(first, last, areaCode, threePart, fourPart);
}

/*
hashMapStats reads out the size for every cell of the hash table.
*/
void PhoneHash::hashMapStats(ofstream& toFile) const
{
	//counter for empty cells
	int emptyCellCount = 0;


	for (int i = 0; i < MAP_SIZE; i++)
	{
		toFile << i << "," << PhoneHashMap[i].size() << endl;
		if (PhoneHashMap[i].size() == 0) { emptyCellCount++; }
	}

	cout << endl;
	cout << "Empty Cells: " << emptyCellCount << endl;
	cout << "Filled Cells: " << MAP_SIZE - emptyCellCount << endl;
	cout << "Filled Cells Ratio: " << 1 - emptyCellCount/float(MAP_SIZE) << endl;
}


/*
#########################################
Private Functions
#########################################
*/

/*
Simply reconstructs the old input given the DataNode to piece back together.
*/
void PhoneHash::printNode(const DataNode& toPrint) const
{
	cout << toPrint.lastName << ", " << toPrint.firstName << "\t("
		<< toPrint.areaCode << ") " << toPrint.firstThree << "-"
		<< toPrint.lastFour << endl;
}

/*
Receives raw data from buildHelper and composes a new DataNode.
Once the new DataNode is created, it is passed to insertNode.
*/
void PhoneHash::createNode(const string& first, const string& last, const int& area, const int& three, const int& four)
{
	DataNode *temp = new DataNode;
	temp->firstName = first;
	temp->lastName = last;
	temp->areaCode = area;
	temp->firstThree = three;
	temp->lastFour = four;

	insertNode(*temp);
}

/*
Receives the newly created DataNode and runs hashPlacement on it to
obtain the location in the table it should be placed. Once obtained, it
opens the vector at the specified location and appends the new DataNode.
*/
void PhoneHash::insertNode(DataNode& toInsert)
{
	PhoneHashMap[hashPlacement(toInsert)].push_back(toInsert);
}


/*
The hashing function.
*/
int PhoneHash::hashPlacement(const DataNode& toInsert)
{
	
	return ((toInsert.firstThree * 10000 + toInsert.lastFour) % MAP_SIZE);
	
	/*
	int result = 0;
	result += lastNameBits(toInsert.lastName.at(0));

	result = result << 4;	// shifts by 3-digits size
	result += toInsert.firstThree - 423;

	result = result << 4;	// shifts by 4-digits size
	result += ((toInsert.lastFour - 2234) / 11);

	result = result << 5;	// shifts by First Letter size
	result += firstNameBits(toInsert.firstName.at(0));

	result = result << 1;	// shifts by Area Code size
	result += toInsert.areaCode % 2;

	return ( result % MAP_SIZE );
	*/
}

/*
	Special converter to return an integer associated with a char.
	Assumes input is a capital letter.
*/
int PhoneHash::lastNameBits(const char& toConvert)
{
	switch (toConvert)
	{
		case 'A':
			return 0;
		case 'B':
			return 1;
		case 'E':
			return 2;
		case 'G':
			return 3;
		case 'J':
			return 4;
		case 'L':
			return 5;
		case 'S':
			return 6;
		case 'T':
			return 7;
		case 'Z':
			return 8;
	}
}

/*
	Converts first letter of First Name to integer. Assumes input is a
	capital letter.
*/
int PhoneHash::firstNameBits(const char& toConvert)
{
	return(toConvert - 'A');
}

/*
	HASH FUNCTION GRAVEYARD

	ITERATION 1################################
	int item = toInsert.lastFour % 97;
	return item;
	

	ITERATION 2################################
	int result = 0;
	int power = (toInsert.firstThree) % 5;
	int item = (toInsert.lastFour) % 503;
	int section = 1 + ((toInsert.areaCode) % 2);

	result = ((item ^ power) / section) % 97;
	return result;

	ITERATION 3################################
	int quadrant = 1 + (toInsert.firstThree % 4);
	int item = toInsert.lastFour % 100;
	return (((quadrant * 25) + item) % 97);
	

*/