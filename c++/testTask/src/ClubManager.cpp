#include "ClubManager.h"

void ClubManager::SetPrice(int price_)
{
	price = price_;
}

void ClubManager::SetRoomsNumber(int rooms_number_)
{
	rooms_number = rooms_number_;
	clients_queue.reserve(rooms_number);
	tables_and_revenue.resize(rooms_number);

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


	for (auto& event_ : incoming_events)
	{
		switch (event_.event_id)
		{
		case (EventType::ClientCome) :
		{
			EventId1(event_);
		}
		case (EventType::ClientSatDown):
		{
			EventId2(event_);
		}
		case (EventType::ClientWait):
		{
			EventId3(event_);
		}
		case (EventType::ClientLeft):
		{
			EventId4(event_);
		}
		default:
			break;
		}
	}
}

void ClubManager::EventId1(Event& event_)
{
	if (clients_on_table.find(event_.client_id) == clients_on_table.end())
	{
		EventId13(event_.time, Errors::YouShallNotPass);
		return;
	}

	if (CheckTime(event_.time))
	{
		EventId13(event_.time, Errors::NotOpenYet);
		return;
	}

	PrintFullIncomingLine(event_);

	if (CheckClientsQueue())
	{
		EventId11(event_);
		return;
	}	
}

void ClubManager::EventId2(Event& event_)
{
	if (!CheckClient(event_.client_id))
	{
		EventId13(event_.time, Errors::ClientUnknown);
		return;
	}

	if (!SatAtSpecificTable(event_))
	{
		EventId13(event_.time, Errors::PlaceIsBusy);
		return;
	}

	PrintFullIncomingLine(event_);
}

void ClubManager::EventId3(Event& event_)
{
	if (CheckClientsQueue())
	{
		EventId13(event_.time, Errors::ICanWaitNoLonger);
		return;
	}

	clients_queue.push_back(event_);
	PrintFullIncomingLine(event_);
}

void ClubManager::EventId4(Event& event_)
{
	const auto& client_id = event_.client_id;

	if (!CheckClient(client_id))
	{
		EventId13(event_.time, Errors::ClientUnknown);
		return;
	}

	TableTime table_time = CalculateTable(event_);
	DeleteClient(event_);
	PrintFullIncomingLine(event_);
	EventId12(table_time);
}

void ClubManager::EventId11(Event& event_)
{
	CalculateTable(event_);
	DeleteClient(event_);
	PrintFullIncomingLine(event_);
}

void ClubManager::EventId12(TableTime table_time)
{
	if (!clients_queue.empty())
	{
		auto& first_client_in_queue = clients_queue.front().client_id;
		clients_on_table[first_client_in_queue] = table_time;
		tables_for_clients[table_time.number] = first_client_in_queue;
		clients_queue.pop_back();

		Event& event_ = OutgoingEventConstructor({ table_time.start_time,
			EventType::ClientSatDownOutgoing,
			first_client_in_queue ,
			table_time.number ,
			"" });

		PrintFullIncomingLine(event_);
	}
}

void ClubManager::EventId13(int time, Errors error_)
{
	std::cout << ConvertIntToTime(time) << " 13 " << error_ << std::endl;
}

bool ClubManager::SatAtSpecificTable(Event& event_)
{
	if (tables_for_clients.find(event_.target_table_number) != tables_for_clients.end())
	{
		tables_for_clients[event_.target_table_number] = event_.client_id;
		return true;
	}
	return false;
}

bool ClubManager::CheckTime(int time)
{
	return time < working_time.first || time > working_time.second;
}

bool ClubManager::CheckClientsQueue()
{
	return clients_queue.size() != rooms_number;
}

bool ClubManager::CheckClient(const std::string& client_id)
{
	return clients_on_table.find(client_id) != clients_on_table.end();
}

void ClubManager::PrintFullIncomingLine(Event& event_)
{
	std::cout << event_.full_incoming_line << std::endl;
}

void ClubManager::DeleteClient(Event& event_)
{
	tables_for_clients.erase(clients_on_table[event_.client_id].number);
	clients_on_table.erase(event_.client_id);
}

TableTime ClubManager::CalculateTable(Event& event_)
{
	auto table_and_start_time = clients_on_table.find(event_.client_id);

	int& stop_time = event_.time;
	int& start_time = table_and_start_time->second.start_time;
	int table_working_time = stop_time - start_time;
	int spend = table_working_time * price;

	int table_number = table_and_start_time->second.number;
	tables_and_revenue[table_number] += TableTotalSpend{ spend, table_working_time };

	return { table_number, stop_time };
}

void ClubManager::CloseClub()
{
	for (auto& [table, client_id] : tables_for_clients)
	{
		Event& event_ = OutgoingEventConstructor({ working_time.second,
			EventType::ClientLeftOutgoing,
			client_id ,
			table ,
			"" });
		EventId11(event_);
	}
}

Event& ClubManager::OutgoingEventConstructor(Event event_to_save)
{
	incoming_events.push_back(event_to_save);
	return incoming_events.back();
}

void ClubManager::CalculateTotal()
{
	std::cout << working_time.second << std::endl;
	for (int i = 0; i < tables_and_revenue.size(); i++)
	{
		auto& [time, revenue] = tables_and_revenue[i];
		std::cout << i << " " << revenue << " " << time << std::endl;
	}
}