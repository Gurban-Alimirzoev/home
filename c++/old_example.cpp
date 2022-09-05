#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <string_view>
#include <utility>

using namespace std;
using filesystem::path;

path operator""_p(const char* data, std::size_t sz) {
    return path(data, data + sz);
}

bool operator>(const path& lhs,
    const path& rhs)
{
    return lhs.filename().string() < rhs.filename().string();
}

void PrintTree(ostream& dst, const path& p, const filesystem::file_status& status, int offset)
{
    error_code err;
    vector <path> result_file;
    vector <path> result_dir;
    if (err) {
        return;
    }
    dst << string(offset, ' ') << p.filename().string() << endl;
    offset += 2;
    for (const auto& dir_entry : filesystem::directory_iterator(p))
    {
        if (dir_entry.is_regular_file())
            result_file.push_back(dir_entry);
        else
        {
            result_dir.push_back(dir_entry);
        }
    }

    sort(result_file.begin(), result_file.end());
    sort(result_dir.begin(), result_dir.end());
    reverse(result_file.begin(), result_file.end());
    reverse(result_dir.begin(), result_dir.end());
    for (auto _p : result_file)
    {
        dst << string(offset, ' ') << _p.filename().string() << endl;
    }
    for (auto _p : result_dir)
    {
        PrintTree(dst, _p, status, offset);
    }
}

void PrintTree(ostream& dst, const path& p)
{
    int offset = 0;
    error_code err;
    auto status = filesystem::status(p, err);
    PrintTree(dst, p, status, offset);
}

int main() {
    error_code err;
    filesystem::remove_all("test_dir", err);
    filesystem::create_directories("test_dir"_p / "a"_p, err);
    filesystem::create_directories("test_dir"_p / "b"_p, err);

    ofstream("test_dir"_p / "b"_p / "f1.txt"_p);
    ofstream("test_dir"_p / "b"_p / "f2.txt"_p);
    ofstream("test_dir"_p / "a"_p / "f3.txt"_p);

    ostringstream out;
    PrintTree(cout, "test_dir"_p);
    PrintTree(out, "test_dir"_p);
    assert(out.str() ==
        "test_dir\n"
        "  b\n"
        "    f2.txt\n"
        "    f1.txt\n"
        "  a\n"
        "    f3.txt\n"sv
    );
}
