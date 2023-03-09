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

std::pair<int, int> Parser::ParseWorkingTime(std::istream& input)
{
	std::string line, open_time, close_time;
	std::getline(input, line);
	size_t space = line.find(' ');
	open_time = line.substr(0, symbols_of_time);
	close_time = line.substr(space + 1, symbols_of_time);
	return { ConvertTimeToInt(open_time) , ConvertTimeToInt(close_time) };
}

Event Parser::ParseEvent(std::string line_)
{	
	std::string line(line_);

	std::string time = ParseLine(line);
	std::string event_id = ParseLine(line);
	std::string client_id = ParseLine(line);

	EventType event_type = EventType(std::stoi(event_id));
	int table_number = 0;
	if (event_type == EventType::ClientSatDown)
	{
		table_number = std::stoi(ParseLine(line));

	}
	return std::move(Event{ client_id, line_, event_type, ConvertTimeToInt(time), table_number });
}

std::string Parser::ParseLine(std::string& line)
{
	size_t space = line.find(' ');
	std::string result = line.substr(0, space);
	line = line.substr(space + 1, line.size() - space);
	return result;
}

void Parser::ParseEvents(std::istream& input)
{
	std::string line;
	while (std::getline(input, line))
	{		
		line != "END EVENTS" ? clubManager.AddEvent(ParseEvent(line)) : clubManager.CloseClub();
	}
}

void Parser::ParseInputFile(std::ifstream& input)
{
	CheckWord(input, "BEGIN CFG");
	CheckWord(input, "BEGIN ROOM");
	int rooms_number = ReadNumberOnLine(input);
	clubManager.SetRoomsNumber(rooms_number);
	CheckWord(input, "END ROOM");
	CheckWord(input, "BEGIN SCHEDULE");
	auto [start, stop] = ParseWorkingTime(input);
	clubManager.SetWorkingTime({ start, stop });
	CheckWord(input, "END SCHEDULE");
	CheckWord(input, "BEGIN PRICE");
	int price = ReadNumberOnLine(input);
	clubManager.SetPrice(price);
	CheckWord(input, "END PRICE");
	CheckWord(input, "END CFG");
	CheckWord(input, "BEGIN EVENTS");
	ParseEvents(input);
}