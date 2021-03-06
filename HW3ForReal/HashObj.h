#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#define MAP_SIZE 4177
using namespace std;

class PhoneHash
{
public:
	PhoneHash();
	PhoneHash(ifstream& fromFile);
	~PhoneHash();
	void buildHash(ifstream& fromFile);
	void hashMapStats(ofstream& toFile) const;

private:
	int size;
	struct DataNode
	{
		string firstName;
		string lastName;
		int areaCode;
		int firstThree;
		int lastFour;
	};
	vector<DataNode> PhoneHashMap[MAP_SIZE]; //holds vectors composed of nodes

	void printNode(const DataNode& toPrint) const;
	void createNode(const string& first, const string& last, const int& area, const int& three, const int& four);
	void insertNode(DataNode& toInsert);
	int hashPlacement(const DataNode& toInsert);
	void buildHelper(const string& data);
	int lastNameBits(const char& toConvert);
	int firstNameBits(const char& toConvert);
};