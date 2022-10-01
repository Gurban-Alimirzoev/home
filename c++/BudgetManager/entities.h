#pragma once

struct DayInfo {
    double earned;
    double spent;
};

inline DayInfo operator+(const DayInfo left, const DayInfo right) {
    DayInfo output;
    output.earned = left.earned + right.earned;
    output.spent = left.spent + right.spent;
    return output;
}