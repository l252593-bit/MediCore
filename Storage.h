#pragma once

template<class T>
class Storage
{
private:
	T data[100];
	int count;
public:
	Storage()
	{
		count = 0;
	}
	bool add(T item)
	{
		if (count >= 100)
		{
			return false;
		}
		data[count] = item;
		count++;
		return true;
	}
	bool removeByID(int id)
	{
		bool exists = false;
		for (int i = 0; i < count; i++)
		{
			if (data[i].getID() == id)
			{
				exists = true;
				int j = i;
				for (; j < count - 1; j++)
				{
					T temp = data[j];
					data[j] = data[j + 1];
					data[j + 1] = temp;
				}
				count--;
				return true;
			}
		}
		if (exists == false)
		{
			return false;
		}

	}
	T* findByID(int id)
	{
		for (int i = 0; i < count; i++)
		{
			if (data[i].getID() == id)
			{
				return &data[i];
			}
		}
		return nullptr;
	}
	T* getAll()
	{
		return data;
	}
	int size()
	{
		return count;
	}
};