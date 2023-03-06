#include "ClubManager.h"

void ClubManager::SetPrice(int price_)
{
	price = price_;
}

void ClubManager::SetRoomsNumber(int rooms_number_)
{
	rooms_number = rooms_number_;
}

void ClubManager::SetWorkingTime(std::pair<int, int> working_time_)
{
	working_time = working_time_;
}

void ClubManager::AddEvent(Event event_)
{
	events.push_back(event_);
}