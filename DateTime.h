#ifndef DATETIME_H
#define DATETIME_H

#include <string>
#include <vector>

class DateTime
{
    int day;
    int month;
    int year;
    int minute;
    int hour;
    int utc;
    std::string dateTimeStr;
    std::vector<std::string> string_split(std::string str, char delim, int count = 3);
    void setDate(std::string date_string);
    void setTime(std::string time, std::string utc_string);
public:
    DateTime(std::string dt_string);
    std::string getDateTimeString() const;
    int getDay() const;
    int getMonth() const;
    int getYear() const;
    int getMinute() const;
    int getHour() const;
    int getUTC() const;
    bool operator<(const DateTime& other) const;
    bool operator>(const DateTime& other) const;
    bool operator==(const DateTime& other) const;
    bool operator!=(const DateTime& other) const;
    bool operator<=(const DateTime& other) const;
    bool operator>=(const DateTime& other) const;
};

#endif
