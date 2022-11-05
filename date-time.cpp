#include <iostream>
#include <string>
#include <ctime>
#include <map>
#include <stdexcept>

struct Date_Time{
    struct Date{
        unsigned long long years;
        unsigned short months;
        unsigned short days;
    } dt;
    struct Time{
        unsigned short hours;
        unsigned short minutes;
        unsigned short seconds;
    } tm;
};

class Clock{
    private:
        static const struct max_values{
            /*this num * 365 * 24 * 60 * 60 == max in this type*/
            unsigned long long max_years = 584942417355;
            unsigned short max_months = 12;
            unsigned short max_days[12] = {
                                        31, 28, 31, 30, 31, 30, 
                                        31, 31, 30, 31, 30, 31
                                        };
            unsigned short max_hours = 23;
            unsigned short max_minutes = 59;
            unsigned short max_seconds = 59;
        } max_vals;
        Date_Time save_date_time;
        inline static Date_Time __fill_date_time_struct(
                                            Date_Time dt_tm,
                                            unsigned long long val,
                                            std::string format
                                            );
    public:
        Clock();
        Clock(Date_Time dt_tm);
        Clock(unsigned long long seconds);
        Clock(const Clock &) = default;
        Clock(Clock &&) = default;
        static bool correct_date(
                                unsigned short days,
                                unsigned short months,
                                unsigned long long years
                                );
        static bool correct_time(
                                unsigned short hours,
                                unsigned short minutes,
                                unsigned short seconds
                                );
        static bool correct_date(Date_Time::Date dt);
        static bool correct_time(Date_Time::Time tm);
        static Date_Time* choose_earlier_date(
                                            Date_Time *first_date,
                                            Date_Time *second_date
                                            );
        static Date_Time current_date_time();
        static Date_Time get_from_string(
                                        std::string date,
                                        std::string format
                                        );
        inline void set_date(
                            unsigned short days,
                            unsigned short months,
                            unsigned long long years
                            );
        inline void set_time(
                            unsigned short hours,
                            unsigned short minutes,
                            unsigned short seconds
                            );
        inline void set_date(Date_Time::Date dt);
        inline void set_time(Date_Time::Time tm);
        void addSecs(unsigned long long seconds);
        void addMinutes(unsigned long long minutes);
        void addHours(unsigned long long hours);
        void addDays(unsigned long long days);
        void addMonths(unsigned long long months);
        void addYears(unsigned long long years);
        inline Date_Time::Date get_date() const;
        inline Date_Time::Time get_time() const;
        unsigned long long daysTo(Date_Time date_x) const;  //
        unsigned long long secsTo(Date_Time date_x) const;  //
        std::string to_string(std::string format) const;
        void print() const;
};

const Clock::max_values Clock::max_vals;

Clock::Clock(){
    save_date_time.dt = {1, 1, 1};
    save_date_time.tm = {0, 0, 0};
}

Clock::Clock(unsigned long long second) : Clock::Clock(){
    addSecs(second);
}

Clock::Clock(Date_Time dt_tm){
	/*
    if(correct_date(dt_tm.dt.days, dt_tm.dt.months, dt_tm.dt.years) &&
        correct_time(dt_tm.tm.hours, dt_tm.tm.minutes, dt_tm.tm.seconds))
    */
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

Date_Time Clock::__fill_date_time_struct(
                                        Date_Time dt_tm,
                                        unsigned long long val,
                                        std::string format
                                        )
{
    switch(format[1]){
        case 'Y':
            dt_tm.dt.years = val;
            break;
        case 'M':
            dt_tm.dt.months = (unsigned short)val;
            break;
        case 'D':
            dt_tm.dt.days = (unsigned short)val;
            break;
        case 'h':
            dt_tm.tm.hours = (unsigned short)val;
            break;
        case 'm':
            dt_tm.tm.minutes = (unsigned short)val;
            break;
        case 's':
            dt_tm.tm.seconds = (unsigned short)val;
            break;
    }
    return dt_tm;
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
            //std::cout << copy_date[i];
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
        //std::cout << num << std::endl;
        struc_date = __fill_date_time_struct(
                                            struc_date,
                                            std::stol(num),
                                            copy_format.substr(indx, 2)
                                            );
        copy_date = copy_date.substr(indx + num.length() + added_len);
        copy_format = copy_format.substr(indx + 2);
        indx = copy_format.find("$");
        //std::cout << copy_date << "\n" << copy_format << "\n" << std::endl;
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

void Clock::set_date(unsigned short days,
                    unsigned short months,
                    unsigned long long years)
{
    if(correct_date(days, months, years)){
        save_date_time.dt.days = days;
        save_date_time.dt.months = months;
        save_date_time.dt.years = years;
    }
    else{
        throw std::invalid_argument("Wrong date format");
    }
}

void Clock::set_date(Date_Time::Date dt){
    if(correct_date(dt)){
        save_date_time.dt = dt;
    }
    else{
        throw std::invalid_argument("Wrong date format");
    }
}

void Clock::set_time(unsigned short hours,
                    unsigned short minutes,
                    unsigned short seconds)
{
    if(correct_time(hours, minutes, seconds)){
        save_date_time.tm.hours = hours;
        save_date_time.tm.minutes = minutes;
        save_date_time.tm.seconds = seconds;
    }
    else{
        throw std::invalid_argument("Wrong time format");
    }
}

void Clock::set_time(Date_Time::Time tm){
    if(correct_time(tm)){
        save_date_time.tm = tm;
    }
    else{
        throw std::invalid_argument("Wrong time format");
    }
}

Date_Time::Date Clock::get_date() const{
    return save_date_time.dt;
}

Date_Time::Time Clock::get_time() const{
    return save_date_time.tm;
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

int main(int argc, char const *argv[]){
    std::cout << Clock::correct_date(1, 5, 2022) << std::endl;
    std::cout << Clock::current_date_time().tm.hours << std::endl;
    Clock cl0(13223567432);
    cl0.print();
    Clock cl;
    cl.print();
    Clock cl2(Clock::current_date_time());
    cl2.print();
    Clock cl3 = cl2;
    cl3.print();
    cl3.set_date(cl3.get_date().days, cl3.get_date().months, 1);
    cl3.print();
    std::cout << cl2.to_string("\n$h:$m\n$D.$M.$Y\n") << std::endl;
    Date_Time q = Clock::get_from_string("21.10 03:45:11", "$D.$M $h:$m:$s");
    Clock cl4(q);
    cl4.addYears(2019);
    cl4.print();
    cl4.addMonths(3);
    cl4.print();
    cl4.addDays(345);
    cl4.print();
    cl4.addHours(21);
    cl4.print();
    cl4.addMinutes(15);
    cl4.print();
    cl4.addSecs(12345678);
    cl4.print();
    std::cout << cl4.daysTo(Clock::current_date_time()) << std::endl;
    Date_Time f = Clock::get_from_string("21.10.2022 03:45:11", "$D.$M.$Y $h:$m:$s");
    Date_Time s = Clock::get_from_string("21.10.202 03:45:11", "$D.$M.$Y $h:$m:$s");
    Clock clcl(*Clock::choose_earlier_date(&f, &s));
    clcl.print();
    return 0;
}
