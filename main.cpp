#include "date-time.hpp"
#include <iostream>
#include <fstream>

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

    std::ofstream out_file;
    out_file.open("OUT.txt");
    if(out_file.is_open()){
        out_file << cl2.to_string("///\n$h:$m\n$D.$M.$Y\n///") << std::endl;
    }
    out_file.close();

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
    Date_Time f = Clock::get_from_string("21.10.2022 06:45:11", "$D.$M.$Y $h:$m:$s");
    Date_Time s = Clock::get_from_string("20.10.2022 00:00:01", "$D.$M.$Y $h:$m:$s");
    Clock clcl(*Clock::choose_earlier_date(&f, &s));
    clcl.print();
    std::cout << clcl.secsTo(f) << std::endl;
    return 0;
}