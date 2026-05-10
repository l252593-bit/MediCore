#include"Validator.h"
#include<ctime>
using namespace std;

bool Validator::validateID(int id)
{
	if (id <= 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool Validator::validatePassword(string password)
{
	int count = 0;
	for (int i = 0; password[i] != '\0'; i++)
	{
		count++;
	}
	if (count < 6)
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool Validator::validatePositiveFloat(double amount)
{
	if (amount <= 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool Validator::validateMenuChoice(int choice, int min, int max)
{
	if (choice >= min && choice <= max)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool Validator::validateContact(string contact)
{
	int count = 0;
	for (int i = 0; contact[i] != '\0'; i++)
	{
		if (contact[i] < '0' || contact[i] > '9')
		{
			return false;
		}
		count++;
	}
	if (count != 11)
	{
		return false;
	}
	return true;
}
bool Validator::validateTimeSlot(string slot)
{
	if (slot == "09:00" || slot == "10:00" || slot == "11:00" ||
		slot == "12:00" || slot == "13:00" || slot == "14:00" ||
		slot == "15:00" || slot == "16:00")
	{
		return true;
	}
	return false;
}
bool Validator::validateDate(string date)
{
	// check length is exactly 10
	int len = 0;
	for (int i = 0; date[i] != '\0'; i++) len++;
	if (len != 10) return false;

	// check dashes are in right positions
	if (date[2] != '-' || date[5] != '-')
	{
		return false;
	}

	// check all other positions are digits
	for (int i = 0; i < 10; i++)
	{
		if (i == 2 || i == 5)
		{
			continue;
		}
		if (date[i] < '0' || date[i] > '9')
		{
			return false;
		}
	}

	// extract day, month, year
	int day = (date[0] - '0') * 10 + (date[1] - '0');
	int month = (date[3] - '0') * 10 + (date[4] - '0');
	int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 +
		(date[8] - '0') * 10 + (date[9] - '0');

	// get current year using ctime
	time_t t = time(0);
	tm now;
	localtime_s(&now, &t);
	int currentYear = now.tm_year + 1900;

	if (day < 1 || day > 31) return false;
	if (month < 1 || month > 12) return false;
	if (year < currentYear) return false;

	return true;
}
