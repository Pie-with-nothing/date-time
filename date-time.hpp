#pragma once
#include <string>
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
        unsigned long long daysTo(Date_Time date_x) const;
        unsigned long long secsTo(Date_Time date_x) const; 
        std::string to_string(std::string format) const;
        void print() const;
};

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