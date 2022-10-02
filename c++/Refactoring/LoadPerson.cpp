struct LoadingParams
{
    string_view db_name;
    int db_connection_timeout;
    bool db_allow_exceptions;
    DBLogLevel db_log_level;
    int min_age;
    int max_age;
    string_view name_filter;


    LoadingParams() = default;

    LoadingParams& SetDataBaseName(string_view db_name_)
    {
        this->db_name = db_name_;
        return *this;
    }

    LoadingParams& SetDataBaseConnectionTimeout(int db_connection_timeout_)
    {
        this->db_connection_timeout = db_connection_timeout_;
        return *this;
    }

    LoadingParams& SetDataBaseAllowExecptions(bool db_allow_exceptions_)
    {
        this->db_allow_exceptions = db_allow_exceptions_;
        return *this;
    }

    LoadingParams& SetDataBaseLogLevel(DBLogLevel db_log_level_)
    {
        this->db_log_level = db_log_level_;
        return *this;
    }

    LoadingParams& SetMinAge(int min_age_)
    {
        this->min_age = min_age_;
        return *this;
    }

    LoadingParams& SetMaxAge(int max_age_)
    {
        this->max_age = max_age_;
        return *this;
    }

    LoadingParams& SetNameFilter(string_view name_filter_)
    {
        name_filter = name_filter_;
        return *this;
    }
};

class LoadPersons
{
public:
    LoadPersons(LoadingParams params)
    {
        Load();
    }

    vector<Person> Load()
    {
        if (!ConnectDB())
            return {};
        ostringstream query_str;
        DBQuery query = MakeQuery(query_str);
        return LoadConstainer();
    }

    bool ConnectDB();
    DBQuery MakeQuery(ostringstream& query_str);
    vector<Person> LoadConstainer();

private:
    LoadingParams params;
    DBConnector connector;
    DBHandler db;
    DBQuery query;
    vector<Person> persons;
};

bool LoadPersons::ConnectDB()
{
    DBConnector connector(db_allow_exceptions, db_log_level);
    DBHandler db;
    if (db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(db_name, db_connection_timeout);
    }
    else {
        db = connector.Connect(db_name, db_connection_timeout);
    }
    if (!db_allow_exceptions && !db.IsOK()) {
        return false;
    }
    return true;
}

DBQuery LoadPersons::MakeQuery(ostringstream& query_str)
{
    query_str << "from Persons "s
        << "select Name, Age "s
        << "where Age between "s << params.min_age << " and "s << params.max_age << " "s
        << "and Name like '%"s << db.Quote(params.name_filter) << "%'"s;
    return query(query_str.str());
}

vector<Person> LoadPersons::LoadConstainer()
{
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({ move(name), age });
    }
    return persons;
}