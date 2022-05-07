#include "ini.h"

using namespace std;

namespace ini {

Section& Document::AddSection(string name)
{
    sections_.insert(name, {});
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
    //string str;
    //getline(input, str);
    //string sec_name;
    Section invis;
    Section& sec_ref = invis;
    Document doc;
    for (string line; getline(input, line) && line[1] != '/';) 
    {
        auto pos_sec = line.find_first_not_of('[');
        if (pos_sec != string::npos)
        {
            string sec_name = line.substr(pos_sec, line.find_last_not_of(']'));
            sec_ref = doc.Document::AddSection(sec_name);
            continue;
        }        
        line.erase(remove_if(line.begin(), 
                              line.end(),
                              [](auto x){return isspace(x);}),
               line.end());
        
        auto pos_key = line.find('=');
        
        auto key_name = line.substr(0, pos_key - 1);
        auto value = line.substr(pos_key + 1, line.size() - 1);

        sec_ref = {key_name, value};     
    }
    return doc;
}
}
// место для реализаций методов и функций библиотеки ini
// не забудьте, что они должны быть помещены в namespace ini