#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <numeric>

using namespace std;

class BookBase
{
public:
	void SetReadUser(int id, int page)
	{
		if (id_and_page.size() < id + 1)
			id_and_page.resize(id + 1);
		if (page_and_amount.size() < page + 1)
			page_and_amount.resize(page + 1);

		int id_count = page_and_amount[id_and_page[id]];
		if (id_count != 0)
		{
			id_count--;
			number_id--;
		}

		id_and_page[id] = page;
		page_and_amount[page]++;
		number_id++;
	}

	void CheerUser(int id)
	{
		if (id_and_page.size() < id)
		{
			result.push_back(0);
			return;
		}
		int less_users = accumulate(
			page_and_amount.begin(), 
			next(page_and_amount.begin(), id_and_page[id]), 
			0);

		if (less_users == number_id - 1)
		{
			result.push_back(1);
			return;
		}
		result.push_back(less_users / static_cast<double>(number_id - 1));
	}

	void PrintResult()
	{
		for (double less : result)
			cout << setprecision(6) << less << endl;
	}

private:
	vector<int> id_and_page;
	vector<int> page_and_amount;
	vector<double> result;
	int number_id = 0;
};

void ParseRequsets(BookBase& bb, string line)
{
	if (line.substr(0, 5) == "CHEER")
	{
		double id = stod(line.substr(5));
		bb.CheerUser(id);
	}
	else
	{
		line = line.substr(5);
		double id = stod(line);
		size_t space = line.find_first_of(' ');
		double page = stod(line.substr(space));
		bb.SetReadUser(id, page);
	}
}

int main() {
	std::string line;
	std::getline(cin, line);
	int requsts_number = stoi(line);
	BookBase bb;
	for (int i = 0; i < requsts_number; i++)
	{
		getline(cin, line);
		ParseRequsets(bb, line);
	}
	bb.PrintResult();
}