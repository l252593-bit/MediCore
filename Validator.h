#pragma once
#include<iostream>
#include<string>
using namespace std;

class Validator
{
public:
	bool validateDate(string date);
	bool validateTimeSlot(string slot);
	bool validateContact(string contact);
	bool validateID(int id);
	bool validatePassword(string password);
	bool validatePositiveFloat(double amount);
	bool validateMenuChoice(int choice, int min, int max);
};
