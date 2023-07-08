using namespace std;

enum class DateTimeErrors {
    YEAR_IS_TOO_SMALL,    
    YEAR_IS_TOO_BIG,
    MONTH_IS_TOO_SMALL,
    MONTH_IS_TOO_BIG,
    DAY_IS_TOO_SMALL,
    DAY_IS_TOO_BIG,
    HOUR_IS_TOO_SMALL,
    HOUR_IS_TOO_BIG,
    MINUTE_IS_TOO_SMALL,
    MINUTE_IS_TOO_BIG,
    SECOND_IS_TOO_SMALL,
    SECOND_IS_TOO_BIG
};

optional<DateTimeErrors> CheckDateTimeErrors(const DateTime& dt)
{
    using DT = DateTimeErrors;
    if (dt.year < 1) {
        return DT::YEAR_IS_TOO_SMALL;
    }

    if (dt.year > 9999) {
        return DT::YEAR_IS_TOO_BIG;
    }

    if (dt.month < 1) {
        return DT::MONTH_IS_TOO_SMALL;
    }
    if (dt.month > 12) {
        return DT::MONTH_IS_TOO_BIG;
    }

    const bool is_leap_year = IsLeapYear(const DateTime & dt);
    const array month_lengths = { 31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (dt.day < 1) {
        return DT::DAY_IS_TOO_SMALL;
    }
    if (dt.day > month_lengths[dt.month - 1]) {
        return DT::DAY_IS_TOO_BIG;
    }

    if (dt.hour < 0) {
        return DT::HOUR_IS_TOO_SMALL;
    }
    if (dt.hour > 23) {
        return DT::HOUR_IS_TOO_BIG;
    }

    if (dt.minute < 0) {
        return DT::MINUTE_IS_TOO_SMALL;
    }
    if (dt.minute > 59) {
        return DT::MINUTE_IS_TOO_BIG;
    }

    if (dt.second < 0) {
        return DT::SECOND_IS_TOO_SMALL;
    }
    if (dt.second > 59) {
        return DT::SECOND_IS_TOO_BIG;
    }
}

void CheckDateTimeValidity(const DateTime& dt) {
    auto iter = CheckDateTimeErrors(const DateTime & dt);
    if (iter != nullopt_t)
    {
        throw domain_error(*iter);
    }
}
    

bool IsLeapYear(const DateTime& dt)
{
    return (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
}