#include "ini.h"

using namespace std;

namespace ini {

Section& Document::AddSection(string name)
{
    
}
    
const Section& Document::GetSection(const string& name) const
{
    if (sections_.contains(name))
    {
        return sections_[name];
    }
    unordered_map<string, Section> zero;
    return zero;
}
    
size_t Document::GetSectionCount() const
{
    return sections_.size();
}    
    
Document Load(std::istream& input)
{
    string str;
    getline(input, str);
    
    
}
}
// место для реализаций методов и функций библиотеки ini
// не забудьте, что они должны быть помещены в namespace ini