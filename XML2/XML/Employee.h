#ifndef __EMPLOYEE__
#define __EMPLOYEE__

#include <iostream>
#include <fstream>
#include <ostream>
#include <memory>
#include <string>
#include <istream>
#include <exception>
#include <bitset>
#include <cctype>
#include <cstring>

using std::bitset;
using std::cin;
using std::cout;
using std::fstream;
using std::getline;
using std::ifstream;
using std::iostream;
using std::istream;
using std::make_unique;
using std::ostream;
using std::runtime_error;
using std::string;
using std::unique_ptr;
using std::tolower;

class Employee
{
private:

    static const size_t BITSETSIZE = 8;
    static const char LEFTANGLE = '<';
    static const char RIGHTANGLE = '>';

    int id;
    string name;
    string address;
    string city;
    string state;
    string country;
    string phone;
    double salary;

    struct EmployeeRec
    {
        int id;
        char name[31];
        char address[26];
        char city[21];
        char state[21];
        char country[21];
        char phone[21];
        double salary;
    };

    //helpers
    static string lower(const string& str);
    static bool str_cmpr(const string& s1, const string& s2);

public:
    Employee();

    Employee(int p_id, const string &p_name, const string &p_address, const string &p_city, const string &p_state, const string &p_country, const string &p_phone, double p_salary);

    void display(std::ostream&) const;  // Write a readable Employee representation to a stream 
    void write(std::ostream&) const;    // Write a fixed-length record to current file position 
    void store(std::iostream&) const;   // Overwrite (or append) record in (to) file 
    void toXML(std::ostream&) const;    // Write XML record for Employee 
    static Employee* read(std::istream&);         // Read record from current file position 
    static Employee* retrieve(std::istream&,int); // Search file for record by id 
    static Employee* fromXML(std::istream&);      // Read the XML record from a stream

    //getter and setter for salaries
    void set_salary(double d);
    double get_salary();
    

};




#endif