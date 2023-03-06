#include "Parse.h"

bool Parser::CheckWord(std::ifstream& input, std::string_view control)
{
	std::string line;
	std::getline(input, line);
	if (line != control)
	{
		std::cout << line;
		return false;
	}
	return true;
}

int Parser::ReadNumberOnLine(std::istream& input) {
	std::string line;
	std::getline(input, line);
	return std::stoi(line);
}

int Parser::ConvertTimeToMinute(std::string& line)
{
	size_t colon = line.find(':');
	int symbols_of_minute = 2;
	int hours = std::stoi(line.substr(0, symbols_of_minute));
	int minutes = std::stoi(line.substr(colon + 1, symbols_of_minute).data());
	return hours * 60 + minutes;
};

std::pair<int, int> Parser::ParseWorkingTime(std::istream& input)
{
	std::string line, open_time, close_time;
	std::getline(input, line);
	size_t space = line.find(' ');
	//int symbols_of_time = 5;
	open_time = line.substr(0, symbols_of_time);
	close_time = line.substr(space + 1, symbols_of_time);
	return { ConvertTimeToMinute(open_time) , ConvertTimeToMinute(close_time) };
}

Event Parser::ParseEvent(std::string line)
{	
	std::string time = line.substr(0, symbols_of_time);
	line = line.substr(symbols_of_time + 1, line.size() - symbols_of_time);
	size_t space = line.find(' ');
	std::string event_id = line.substr(0, space);
	std::string client_id = line.substr(space + symbols_of_word_client  + 1, line.size() - space);
	return { ConvertTimeToMinute(time), std::stoi(event_id), std::stoi(client_id) };
}

void Parser::ParseEvents(std::istream& input)
{
	std::string line;
	while (line != "END EVENTS");
	{
		std::getline(input, line);
		clubManager.AddEvent(ParseEvent(line));
	}
}

bool Parser::ParseInputFile(std::ifstream& input)
{
	if (!CheckWord(input, "BEGIN CFG"))
	{
		return false;
	}
	if (!CheckWord(input, "BEGIN ROOM"))
	{
		return false;
	}
	int rooms_number = ReadNumberOnLine(input);
	if (!CheckWord(input, "END ROOM"))
	{
		return false;
	}
	if (!CheckWord(input, "BEGIN SCHEDULE"))
	{
		return false;
	}
	auto [start, stop] = ParseWorkingTime(input);
	if (!CheckWord(input, "END SCHEDULE"))
	{
		return false;
	}
	if (!CheckWord(input, "BEGIN PRICE"))
	{
		return false;
	}
	int price = ReadNumberOnLine(input);
	if (!CheckWord(input, "END PRICE"))
	{
		return false;
	}
	if (!CheckWord(input, "END CFG"))
	{
		return false;
	}
	if (!CheckWord(input, "BEGIN EVENTS"))
	{
		return false;
	}
	ParseEvents(input);
}