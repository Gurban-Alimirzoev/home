#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>

using std::cin;
using std::cout;

int Interactor(int result_code = 0,
	int interactor_code = 0,
	int checker_code = 0)
{
	int result;
	switch (interactor_code)
	{
		case 0 :
			result = result_code != 0 ? 3 : checker_code;
			break;
		case 1 :
			result = checker_code;
			break;
		case 4 :
			result = result_code != 0 ? 3 : 4;
			break;
		case 6 :
			result = 0;
			break;
		case 7 :
			result = 1;
			break;
		default :
			result = interactor_code;
	}
	return result;	
}

int main()
{
	int result_code = 0;
	int interactor_code = 0;
	int checker_code = 0;
	cin >> result_code >> interactor_code >> checker_code;
	cout << Interactor(result_code, interactor_code, checker_code);
}