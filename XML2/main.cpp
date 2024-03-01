#include "Employee.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <string>

using std::cin;
using std::fstream;
using std::getline;
using std::ifstream;
using std::make_unique;
using std::runtime_error;
using std::string;
using std::unique_ptr;
using std::vector;
using std::bitset;
using std::cout;

int main(int argc, char* argv[]) {
    ifstream ifile; //store my input file as an ifstream
    fstream empf;
    vector<unique_ptr<Employee> > employees;
    unique_ptr<Employee> found_employee;
    string read_line;

    //get file from argv, find any error
    if (argc < 2) {
        throw runtime_error("Got too few arguments, did you pass a file?");
    } else {
        ifile = ifstream(argv[1], ifstream::in); //first file lives at index 1 of argv array
    }

    //read XML records in file by calling fromXML
    while(ifile){
        unique_ptr<Employee> emp_to_push (Employee::fromXML(ifile));
        if (emp_to_push != nullptr) {
            employees.push_back(std::move(emp_to_push));
        }
    }

    //loop vector and print employee data
    /*
    for (size_t i = 0; i < employees.sizeof(); i++){
        emp->display(cout);
    }
    LOL that doesnt work with unique pointers*/

    for (const auto& emp : employees) {//const ref alias for unique pointer
        emp->display(cout);
    }

    //copy employees to employee.bin, fstream 
    empf.open("employee.bin", std::fstream::out | std::ios::binary | std::ios::trunc);
    for (const auto& emp : employees){
        try {
            emp->write(empf);
        } catch (runtime_error error){
            cout << "Can not write to employee file." << std::endl;
        }
    }
    empf.close();


    //clear vector
    employees.clear();

    //6
    empf.open("employee.bin", std::fstream::in | std::ios::binary);
    while (empf) {
        unique_ptr<Employee> emp_to_push(Employee::read(empf));
        if(emp_to_push != nullptr) {
            employees.push_back(std::move(emp_to_push));
        }
    }
    empf.close();

    //print using toXML()
    for (const auto &emp : employees) {
        emp->toXML(cout);
    }

    //Seach for employee with id of 12345 
    empf.open("employee.bin", std::fstream::in | std::ios::binary);
    found_employee = unique_ptr<Employee>(Employee::retrieve(empf, 12345));
    empf.close();

    //set salary to 1500000.00 of retrieved object 
    found_employee->set_salary(15000.00);

    //write employee object back to file
    empf.open("employee.bin", std::fstream::out | std::ios::binary);
    found_employee->store(empf);

    //ret and print new salary
    found_employee = unique_ptr<Employee>(Employee::retrieve(empf, 12345));
    cout << found_employee->get_salary();

    //make a new employee
    //unique_ptr<Employee> newEmployee{new Employee(0, "I", "Love", "C++", "Sometimes", "Which Means", "Never", 10.00)};
    unique_ptr<Employee> newEmployee{new Employee(0, "I", "Love", "C++", "Sometimes", "Which Means", "Never", 10.00)}; //wtf{}

    //store the new object
    newEmployee->store(empf);

    //retrieve emp with ret and display
    found_employee = unique_ptr<Employee>(Employee::retrieve(empf, 0));
    found_employee->display(cout);

    ifile.close();
    empf.close();

    return 0;
}