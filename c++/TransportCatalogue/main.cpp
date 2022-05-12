#include "transport_catalogue.h"
#include "geo.h"
#include "input_reader.h"
#include "stat_reader.h"

#include <sstream>
#include <cassert>
#include <iostream>
#include <fstream>

using namespace std;

int main() {
	/*istringstream input{
		"10\n"
		"Stop Tolstopaltsevo : 55.611087 , 37.208290 \n"
		"Stop Marushkino : 55.595884, 37.209755\n"
		"Bus 256 : Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n"
		"Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n"
		"Stop Rasskazovka : 55.632761, 37.333324\n"
		"Stop Biryulyovo Zapadnoye : 55.574371, 37.651700\n"
		"Stop Biryusinka : 55.581065, 37.648390\n"
		"Stop Universam : 55.587655, 37.645687\n"
		"Stop Biryulyovo Tovarnaya : 55.592028, 37.653656\n"
		"Stop Biryulyovo Passazhirskaya : 55.580999, 37.659164\n"
		"3\n"
		"Bus 256\n"
		"Bus 750\n"
		"Bus 751\n" 
	};*/
	ifstream input("e:/git/Home/c++/TransportCatalogue/tsA_case2_input.txt");
	string line;
	getline(input, line);
	int number = stoi(line);

	TransportCatalogue cat;
	InputReader input_reader;

	for (int i = 0; i < number; i++)
	{
		getline(input, line);
		input_reader.ParseStopOrSaveBus(line, cat);
	}

	input_reader.ParseBus(cat);

	getline(input, line);
	number = stoi(line);

	StatReader stat_reader;

	for (int i = 0; i < number; i++)
	{
		getline(input, line);
		stat_reader.OutStatReader(line, cat);
	}
}