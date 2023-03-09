#include "ClubManager.h"

void ClubManager::SetPrice(int price_)
{
	price = price_;
}

void ClubManager::SetRoomsNumber(int rooms_number_)
{
	rooms_number = rooms_number_;
	tables_and_revenue.resize(rooms_number + 1);

}

void ClubManager::SetWorkingTime(std::pair<int, int> working_time_)
{
	working_time = working_time_;
}

void ClubManager::AddEvent(Event event_)
{
	incoming_events.push_back(event_);
}

void ClubManager::ExecuteEvents()
{
	std::cout << ConvertIntToTime(working_time.first) << std::endl;
	for (auto event_ : incoming_events)
	{
		switch (event_.event_id)
		{
		case (EventType::ClientCome) :
		{
			EventId1(std::move(event_));
			break;
		}
		case (EventType::ClientSatDown):
		{
			EventId2(std::move(event_));
			break;
		}
		case (EventType::ClientWait):
		{
			EventId3(std::move(event_));
			break;
		}
		case (EventType::ClientLeft):
		{
			EventId4(std::move(event_));
			break;
		}
		default:
			break;
		}
	}
}

void ClubManager::EventId1(Event event_)
{
	PrintFullIncomingLine(event_);
	clients_in_club.insert(event_.client_id);
	if (CheckTime(event_.time))
	{
		EventId13(event_.time, Errors::NotOpenYet);
		return;
	}

	if (clients_on_table.find(event_.client_id) != clients_on_table.end())
	{
		EventId13(event_.time, Errors::YouShallNotPass);
		return;
	}
	if (!CheckQueue())
	{
		EventId11(event_);
		return;
	}	
}

void ClubManager::EventId2(Event event_)
{
	PrintFullIncomingLine(event_);
	if (!CheckClientOnClub(event_.client_id))
	{
		EventId13(event_.time, Errors::ClientUnknown);
		return;
	}

	if (!SatAtSpecificTable(event_))
	{
		EventId13(event_.time, Errors::PlaceIsBusy);
		return;
	}
}

void ClubManager::EventId3(Event event_)
{
	PrintFullIncomingLine(event_);
	if (!CheckQueue() || CheckTable())
	{
		EventId13(event_.time, Errors::ICanWaitNoLonger);
		return;
	}

	clients_queue.push_back(std::move(event_));
}

void ClubManager::EventId4(Event event_)
{
	if (!CheckClientOnClub(event_.client_id))
	{
		EventId13(event_.time, Errors::ClientUnknown);
		return;
	}

	TableTime table_time = CalculateTable(event_);
	DeleteClient(event_);
	PrintFullIncomingLine(event_);
	if (!clients_queue.empty())
	{
		EventId12(table_time);
	}	
}

void ClubManager::EventId11(Event event_)
{
	CalculateTable(event_);
	DeleteClient(event_);
	PrintFullIncomingLine(event_);
}

void ClubManager::EventId12(TableTime table_time)
{
	if (!clients_queue.empty())
	{
		auto first_client_in_queue = clients_queue.front().client_id;
		clients_on_table[first_client_in_queue] = table_time;
		tables_for_clients[table_time.number] = first_client_in_queue;
		clients_queue.pop_front();

		Event& event_ = OutgoingEventConstructor({ first_client_in_queue ,
			"",
			EventType::ClientSatDownOutgoing,
			table_time.start_time,			
			table_time.number});

		PrintFullIncomingLine(event_);
	}
}

void ClubManager::EventId13(int time, Errors error_)
{
	std::cout << ConvertIntToTime(time) << " 13 " << error_ << std::endl;
}

bool ClubManager::SatAtSpecificTable(Event& event_)
{
	if (tables_for_clients.find(event_.target_table_number) == tables_for_clients.end())
	{
		tables_for_clients[event_.target_table_number] = event_.client_id;
		clients_on_table[event_.client_id] = { event_.target_table_number, event_.time };
		if (!clients_queue.empty())
			clients_queue.pop_front();
		return true;
	}
	return false;
}

bool ClubManager::CheckTime(int time) const
{
	return time < working_time.first || time > working_time.second;
}

bool ClubManager::CheckQueue() const
{
	return clients_queue.size() != rooms_number;
}

bool ClubManager::CheckTable() const
{
	return tables_for_clients.size() != rooms_number;
}

bool ClubManager::CheckClientOnTable(const std::string& client_id) const
{
	return clients_on_table.find(client_id) != clients_on_table.end();
}

bool ClubManager::CheckClientOnClub(const std::string& client_id) const
{
	return clients_in_club.find(client_id) != clients_in_club.end();
}

void ClubManager::PrintFullIncomingLine(Event& event_)
{
	std::cout << event_.full_incoming_line << std::endl;
}

void ClubManager::DeleteClient(Event& event_)
{
	tables_for_clients.erase(clients_on_table[event_.client_id].number);
	clients_on_table.erase(event_.client_id);
	clients_in_club.erase(event_.client_id);

}

TableTime ClubManager::CalculateTable(Event& event_)
{
	auto table_and_start_time = clients_on_table.find(event_.client_id);

	int& stop_time = event_.time;
	int& start_time = table_and_start_time->second.start_time;
	int table_working_time = (stop_time - start_time);
	int what_more = (table_working_time % 60 ? 1 : 0);
	int table_working_time_in_hours = table_working_time / 60 + what_more;
	int spend = table_working_time_in_hours * price;

	int table_number = table_and_start_time->second.number;
	tables_and_revenue[table_number] += TableTotalSpend{ spend, table_working_time };

	return { table_number, stop_time };
}

void ClubManager::CloseClub()
{
	for (int i = 0; i < tables_for_clients.size(); i++)
	{
		auto iter_ = tables_for_clients.begin();
		auto& client_id = iter_->second;
		auto& table_number = iter_->first;
		Event& event_ = OutgoingEventConstructor(std::move(Event{ client_id,
			"",
			EventType::ClientLeftOutgoing,
			working_time.second,			
			table_number }));
		EventId11(event_);
	}
}

Event& ClubManager::OutgoingEventConstructor(Event event_to_save)
{
	event_to_save.full_incoming_line = ConvertIntToTime(event_to_save.time);
	event_to_save.full_incoming_line += " " + std::to_string(int(event_to_save.event_id));
	event_to_save.full_incoming_line += " " + event_to_save.client_id;
	event_to_save.full_incoming_line += " " + std::to_string(event_to_save.target_table_number);
	incoming_events.push_back(event_to_save);
	return incoming_events.back();
}

void ClubManager::CalculateTotal()
{
	std::cout << ConvertIntToTime(working_time.second) << std::endl;
	for (int i = 1; i < tables_and_revenue.size(); i++)
	{
		auto& [revenue, time] = tables_and_revenue[i];
		std::cout << i << " " << revenue << " " << ConvertIntToTime(time) << std::endl;
	}
}