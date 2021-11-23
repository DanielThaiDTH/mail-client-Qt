#include "DateTime.h"
#include <cstdlib>

DateTime::DateTime(std::string dt_string)
{
    //Strip white space
    size_t end_pos = (dt_string.find_last_not_of(' ') == std::string::npos) ? dt_string.size() : dt_string.find_last_not_of(' ')+1;
    size_t start_pos = dt_string.find_first_not_of(' '); 
    dt_string = dt_string.substr(start_pos, end_pos - start_pos);

    dateTimeStr = dt_string;
    
    std::vector<std::string> dt_vec = string_split(dt_string, ' ');
    setDate(dt_vec[0]);
    setTime(dt_vec[1], dt_vec[2]);
}


std::vector<std::string> DateTime::string_split(std::string str, char delim, int count)
{
    std::vector<size_t> delims_pos;
    for (int i = 0; i < count - 1; i++) {
        if (i == 0)
            delims_pos.push_back(str.find_first_of(' '));
        else
            delims_pos.push_back(str.find_first_of(' ', delims_pos[i-1] + 1));
    }

    std::vector<std::string> split_vec;

    for (int i = 0; i < count; i++) {
        if (i == 0)
            split_vec.push_back(str.substr(0, delims_pos[i]));
        else if (i == count - 1)
            split_vec.push_back(str.substr(delims_pos[i-1] + 1));
        else
            split_vec.push_back(str.substr(delims_pos[i-1] + 1, delims_pos[i] - delims_pos[i-1] - 1));        
    }

    return split_vec;
}


void DateTime::setDate(std::string date_string)
{
    std::vector<std::string> date_split = string_split(date_string, '/');
    day = std::atoi(date_split[0].c_str());
    month = std::atoi(date_split[1].c_str());
    year = std::atoi(date_split[2].c_str());
}


void DateTime::setTime(std::string time_string, std::string utc_string)
{
    std::vector<std::string> time = string_split(time_string, ':', 2);
    hour = std::atoi(time[0].c_str());
    minute = std::atoi(time[1].c_str());

    //Assumes starts with characters UTC
    utc = std::atoi(utc_string.substr(3).c_str());
}


std::string DateTime::getDateTimeString() const
{
    return dateTimeStr;
}


int DateTime::getYear() const
{
    return year;
}


int DateTime::getMonth() const
{
    return month;
}


int DateTime::getHour() const
{
    return hour;
}


int DateTime::getDay() const
{
    return day;
}


int DateTime::getUTC() const
{
    return utc;
}


int DateTime::getMinute() const
{
    return minute;
}
