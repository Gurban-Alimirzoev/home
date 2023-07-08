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
        static const Section n = {};
        if (sections_.count(name))
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
        string sec_name;
        Document doc;
        for (string line; getline(input, line) && line[0] != '/';)
        {
            if (line.empty())
            {
                continue;
            }
            size_t pos_sec = line.find('[');
            size_t pos_sec_end = line.find(']');
            if (pos_sec != string::npos)
            {
                sec_name = line.substr(pos_sec + 1, pos_sec_end - pos_sec - 1);
                doc.Document::AddSection(sec_name);
                continue;
            }

            Section& sec_ref = doc.Document::AddSection(sec_name);
            size_t pos_key = line.find('=');

            string key_with_n = line.substr(0, pos_key);
            size_t key_fpos = 0;
            if (isspace(key_with_n[key_fpos]))
            {
                key_fpos = key_with_n.find_first_not_of(' ');;
            }

            size_t key_epos = key_with_n.size();
            if (isspace(key_with_n[key_epos - 1]))
            {
                key_epos = key_with_n.find_last_not_of(' ');
            }

            string key_name = key_with_n.substr(key_fpos, key_epos - key_fpos + 1);



            string value_with_n = line.substr(pos_key + 1, line.size());

            size_t value_fpos = 0;
            if (isspace(value_with_n[value_fpos]))
            {
                value_fpos = value_with_n.find_first_not_of(' ');;
            }

            size_t value_epos = value_with_n.size();
            if (isspace(value_with_n[value_epos - 1]))
            {
                value_epos = value_with_n.find_last_not_of(' ');
            }

            string value = value_with_n.substr(value_fpos, value_epos - value_fpos + 1);
            if (!value.empty() && !key_name.empty())
            {
                sec_ref.insert({ key_name, value });
            }
        }
        return doc;
    }
}
// место для реализаций методов и функций библиотеки ini
// не забудьте, что они должны быть помещены в namespace ini