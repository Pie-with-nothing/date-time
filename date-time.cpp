#include "date-time.hpp"
#include <stdexcept>
#include <ctime>
#include <map>
#include <cmath>
#include <iostream>

Clock::Clock(){
    save_date_time.dt = {1, 1, 1};
    save_date_time.tm = {0, 0, 0};
}

Clock::Clock(unsigned long long second) : Clock::Clock(){
    addSecs(second);
}

Clock::Clock(Date_Time dt_tm){
    if(correct_date(dt_tm.dt) && correct_time(dt_tm.tm)){
        save_date_time = dt_tm;
    }
    else{
    	throw std::invalid_argument("Wrong date_time format");
    }
}

bool Clock::correct_date(unsigned short days,
                        unsigned short months,
                        unsigned long long years)
{
    if(max_vals.max_years < years || max_vals.max_months < months){
        return false;
    }
    return max_vals.max_days[months - 1] < days || !days || !years ? false : true;
}

bool Clock::correct_date(Date_Time::Date dt){
    if(max_vals.max_years < dt.years || max_vals.max_months < dt.months){
        return false;
    }
    return max_vals.max_days[dt.months - 1] < dt.days || !dt.days ? false : true;
}

bool Clock::correct_time(unsigned short hours,
                        unsigned short minutes,
                        unsigned short seconds)
{
    if(max_vals.max_hours < hours ||
        max_vals.max_minutes < minutes ||
        max_vals.max_seconds < seconds)
    {
        return false;
    }
    return true;
}

bool Clock::correct_time(Date_Time::Time tm){
    if(max_vals.max_hours < tm.hours ||
        max_vals.max_minutes < tm.minutes ||
        max_vals.max_seconds < tm.seconds)
    {
        return false;
    }
    return true;
}

Date_Time* Clock::choose_earlier_date(Date_Time *first_date, 
                                    Date_Time *second_date)
{
    unsigned long long first_date2arr[6] = {
                                    first_date->dt.years,
                                    first_date->dt.months,
                                    first_date->dt.days,
                                    first_date->tm.hours,
                                    first_date->tm.minutes,
                                    first_date->tm.seconds,
                                    };
    unsigned long long second_date2arr[6] = {
                                    second_date->dt.years,
                                    second_date->dt.months,
                                    second_date->dt.days,
                                    second_date->tm.hours,
                                    second_date->tm.minutes,
                                    second_date->tm.seconds,
                                    };
    if(
        !correct_date(first_date->dt) || !correct_date(second_date->dt) ||
        !correct_time(first_date->tm) || !correct_time(second_date->tm)
    )
    {
        throw std::invalid_argument("Wrong time format");
    }
    for(int i = 0; i < 6; i++){
        if(first_date2arr[i] < second_date2arr[i]){
            return first_date;
        }
        else if(first_date2arr[i] > second_date2arr[i]){
            return second_date;
        }
    }
    return first_date;
}

Date_Time Clock::current_date_time(){
    Date_Time curr_dt_tm;
    std::time_t t = std::time(0);
    std::tm *now = std::localtime(&t);

    curr_dt_tm.dt.years = now->tm_year + 1900;
    curr_dt_tm.dt.months = now->tm_mon + 1;
    curr_dt_tm.dt.days = now->tm_mday;
    curr_dt_tm.tm.hours = now->tm_hour;
    curr_dt_tm.tm.minutes = now->tm_min;
    curr_dt_tm.tm.seconds = now->tm_sec;
    return curr_dt_tm;
}

Date_Time Clock::get_from_string(std::string date, std::string format){
    Date_Time struc_date = {{1, 1, 1}, {0, 0, 0}};
    std::string copy_date = date;
    std::string copy_format = format;
    std::string num;
    int added_len;
    int indx = copy_format.find("$");
    while(indx != std::string::npos && copy_format.length() > indx + 1){
        if(std::string("YMDhms").find(copy_format[indx + 1]) == std::string::npos){
            copy_date = copy_date.substr(indx + 1);
            copy_format = copy_format.substr(indx + 1);
            indx = copy_format.find("$");
            continue;
        }
        num = "";
        added_len = 0;
        for(int i = indx; i < date.length() - 1; i++){
            if(isdigit(copy_date[i])){
                num.push_back(copy_date[i]);
            }
            else{
                break;
            }
        }
        if(num[0] == '0' && num != "0"){
            num = num.substr(1);
            added_len = 1;
        }
        struc_date = __fill_date_time_struct(
                                            struc_date,
                                            std::stol(num),
                                            copy_format.substr(indx, 2)
                                            );
        copy_date = copy_date.substr(indx + num.length() + added_len);
        copy_format = copy_format.substr(indx + 2);
        indx = copy_format.find("$");
    }
    return struc_date;
}

void Clock::addYears(unsigned long long years){
	Date_Time::Date dt = save_date_time.dt;
	dt.years += years;
	if(correct_date(dt)){
    	set_date(dt);
	}
	else{
    	throw std::invalid_argument("Wrong date_time format");
    }
}

void Clock::addMonths(unsigned long long months){
    Date_Time::Date dt = save_date_time.dt;
    unsigned long long all_months = dt.months + months;
    dt.months = all_months % max_vals.max_months;
    dt.years += all_months / max_vals.max_months;
	if(correct_date(dt)){
    	set_date(dt);
	}
	else{
    	throw std::invalid_argument("Wrong date_time format");
    }
}

void Clock::addDays(unsigned long long days){
    Date_Time::Date dt = save_date_time.dt;
    unsigned long long all_days = dt.days + days;
    unsigned short days_in_year = 0;
    for(auto &i : max_vals.max_days){
        days_in_year += i;
    }
    if(all_days > days_in_year){
        dt.years += all_days / days_in_year;
        all_days %= days_in_year;
    }
    while(all_days > max_vals.max_days[dt.months - 1]){
        all_days -= max_vals.max_days[dt.months - 1];
        if(dt.months == max_vals.max_months){
            dt.years++;
            dt.months = 1;
        }
        else{
            dt.months++;
        }
    }
    dt.days = all_days;
	if(correct_date(dt)){
    	set_date(dt);
	}
	else{
    	throw std::invalid_argument("Wrong date_time format");
    }
}

void Clock::addHours(unsigned long long hours){
    Date_Time::Time tm = save_date_time.tm;
    unsigned long long all_hours = tm.hours + hours;
    tm.hours = all_hours % (max_vals.max_hours + 1);
    if(correct_time(tm)){
    	set_time(tm);
	}
	else{
    	throw std::invalid_argument("Wrong date_time format");
    }
    if(all_hours > max_vals.max_hours){
        addDays(all_hours / max_vals.max_hours);
    }
}

void Clock::addMinutes(unsigned long long minutes){
    Date_Time::Time tm = save_date_time.tm;
    unsigned long long all_minutes = tm.minutes + minutes;
    tm.minutes = all_minutes % (max_vals.max_minutes + 1);
    if(correct_time(tm)){
    	set_time(tm);
	}
	else{
    	throw std::invalid_argument("Wrong date_time format");
    }
    if(all_minutes > max_vals.max_minutes){
        addHours(all_minutes / max_vals.max_minutes);
    }
}

void Clock::addSecs(unsigned long long seconds){
    Date_Time::Time tm = save_date_time.tm;
    unsigned long long all_seconds = tm.seconds + seconds;
    tm.seconds = all_seconds % (max_vals.max_seconds + 1);
    if(correct_time(tm)){
    	set_time(tm);
	}
	else{
    	throw std::invalid_argument("Wrong date_time format");
    }
    if(all_seconds > max_vals.max_seconds){
        addMinutes(all_seconds / max_vals.max_seconds);
    }
}



unsigned long long Clock::daysTo(Date_Time date_x) const{
    Date_Time copy_save_date_time = save_date_time;
    Date_Time *earlier;
    unsigned short days_in_year = 0;
    unsigned long long days_counter = 0;
    short months_counter;
    short diff_month_koef = 1;

    earlier = choose_earlier_date(&copy_save_date_time, &date_x);
    if(earlier == &date_x){
        throw std::invalid_argument("Argument date can't be less than self date");
    }
    for(auto &i : max_vals.max_days){
        days_in_year += i;
    }
    days_counter += days_in_year * (date_x.dt.years - save_date_time.dt.years);
    months_counter = 0;
    for(
        int i = std::min(date_x.dt.months, save_date_time.dt.months) - 1;
        i < std::max(date_x.dt.months, save_date_time.dt.months) - 1;
        i++
    )
    {   
        months_counter += max_vals.max_days[i];
    }
    diff_month_koef = date_x.dt.months < save_date_time.dt.months ? -1 : 1;
    days_counter += diff_month_koef * months_counter;
    days_counter -= diff_month_koef * save_date_time.dt.days;
    days_counter += diff_month_koef * date_x.dt.days;

    return days_counter;
}

unsigned long long Clock::secsTo(Date_Time date_x) const{
    unsigned long long days2secs = daysTo(date_x) * 24 * 60 * 60;
    short self_time2arr[3] = {
                            (short)save_date_time.tm.hours,
                            (short)save_date_time.tm.minutes,
                            (short)save_date_time.tm.seconds
                            };
    short date_x_time2arr[3] = {
                                (short)date_x.tm.hours,
                                (short)date_x.tm.minutes,
                                (short)date_x.tm.seconds
                                };
    for(int i = 0; i < 3; i++){
        days2secs += (date_x_time2arr[i] - self_time2arr[i]) * pow(60, 2 - i);
    }
    return days2secs;
}

std::string Clock::to_string(std::string format) const{
    std::map <std::string, unsigned long long> frmt_pair = {
        {"$Y", save_date_time.dt.years},
        {"$M", (unsigned long long)save_date_time.dt.months},
        {"$D", (unsigned long long)save_date_time.dt.days},
        {"$h", (unsigned long long)save_date_time.tm.hours},
        {"$m", (unsigned long long)save_date_time.tm.minutes},
        {"$s", (unsigned long long)save_date_time.tm.seconds},
    };
    int indx;
    std::string val;
    for(auto &i : frmt_pair){
        indx = format.find(i.first);
        if(indx != std::string::npos){
            val = i.second < 10 ? "0" + std::to_string(i.second) : std::to_string(i.second);
            format = format.replace(indx, 2, val);
        }
    }
    return format;
}

void Clock::print() const{
    std::cout << to_string("$D.$M.$Y $h:$m:$s") << std::endl;
}

const Clock::max_values Clock::max_vals;