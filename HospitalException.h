#pragma once
#include <iostream>
#include <string>
using namespace std;

class HospitalException
{
protected:
	string message;
public:
	HospitalException(string msg)
	{
		message = msg;
	}
	virtual string what()
	{
		return message;
	}
};

class FileNotFoundException :public HospitalException
{
public:
	FileNotFoundException(string msg):HospitalException(msg){}
};
class InsufficientFundsException :public HospitalException
{
public:
	InsufficientFundsException(string msg) :HospitalException(msg) {}
};

class InvalidInputException :public HospitalException
{
public:
	InvalidInputException(string msg) :HospitalException(msg) {}
};

class SlotUnavailableException :public HospitalException
{
public:
	SlotUnavailableException(string msg) :HospitalException(msg) {}
};
