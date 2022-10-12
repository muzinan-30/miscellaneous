#pragma once
#include<exception>
using namespace std;
class errorOperationOnStack :public exception
{
public:
	const char* what()const
		throw()
	{
		return "Error Operation On Stack";
	}
};