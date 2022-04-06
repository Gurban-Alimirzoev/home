#include "process_queries.h"

using namespace std;

vector<vector<Document>> ProcessQueries(
	const SearchServer& search_server,
	const vector<string>& queries)
{
	vector<vector<Document>> result(queries.size());
	transform(
		queries.begin(), queries.end(),
		result.begin(),
		[&search_server](const string& query)
		{ return search_server.FindTopDocuments(query); });
	return result;
}