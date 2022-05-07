#include "ini.h"

using namespace std;

namespace ini
{

    Section& Document::AddSection(string name)
    {
        sections_.insert({ name, {} });
        return sections_[name];
    }

    const Section& Document::GetSection(const string& name) const
    {
        //unordered_map<string, Section> zero;
        static const Section n;
        if (sections_.at(name) != n)
        {
            return sections_.at(name);
        }
        return n;
    }

    size_t Document::GetSectionCount() const
    {
        return sections_.size();
    }

    Document Load(std::istream& input)
    {
        //Section invis;
        //Section& sec_ref = invis;
        string sec_name;
        Document doc;
        for (string line; getline(input, line) && line[0] != '/';)
        {
            line.erase(remove_if(line.begin(),
                line.end(),
                [](auto x) {return isspace(x); }),
                line.end());

            auto pos_sec = line.find('[');
            if (pos_sec != string::npos)
            {
                string sec_name = line.substr(pos_sec + 1, line.find_last_not_of(']'));
                Section& sec_ref = doc.Document::AddSection(sec_name);
                continue;
            }

            Section& sec_ref = doc.Document::GetSection(sec_name);
            auto pos_key = line.find('=');

            string key_name = line.substr(0, pos_key);

            auto pos_value = line.find_last_not_of("\n");
            string value = line.substr(pos_key + 1, pos_value);
            if (!value.empty())
            {
                sec_ref.insert({ key_name, value });
            }
        }
        return doc;
    }
}
// место для реализаций методов и функций библиотеки ini
// не забудьте, что они должны быть помещены в namespace ini