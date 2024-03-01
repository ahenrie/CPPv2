#include "Employee.h"

Employee::Employee() {
    this->id = 0;
    this->name = "";
    this->address = "";
    this->city = "";
    this->state = "";
    this->country = "";
    this->phone = "";
    this->salary = 0.0;
}

Employee::Employee(
    int p_id,
    const string &p_name,
    const string &p_addr,
    const string &p_city,
    const string &p_state,
    const string &p_country,
    const string &p_phone,
    double p_salary) : name{p_name}, address{p_addr}, city{p_city}, state{p_state}, country{p_country}, phone{p_phone} {
        this->id = p_id;
        this->salary = p_salary;
    }

Employee *Employee::read(std::istream & is) {
    EmployeeRec emp_struct;
    is.read(reinterpret_cast<char*>(&emp_struct), sizeof(emp_struct));
    Employee* rtn_emp = new Employee();
    if(is) {
        rtn_emp->name = emp_struct.name;
        rtn_emp->id = emp_struct.id;
        rtn_emp->address = emp_struct.address;
        rtn_emp->city = emp_struct.city;
        rtn_emp->state = emp_struct.state;
        rtn_emp->country = emp_struct.country;
        rtn_emp->phone = emp_struct.phone;
        rtn_emp->salary = emp_struct.salary;
        return rtn_emp;
    }
    return nullptr;
}

Employee *Employee::retrieve(std::istream & is, int p_id){
    EmployeeRec emp_struct;
    Employee* rtn_emp = new Employee();
    //is.seekg(0, std::ios::end); start at the end to get the newest data
    while(is) {
        is.read(reinterpret_cast<char*>(&emp_struct), sizeof(emp_struct));
        if(emp_struct.id == p_id) {
            rtn_emp->name = emp_struct.name;
            rtn_emp->id = emp_struct.id;
            rtn_emp->address = emp_struct.address;
            rtn_emp->city = emp_struct.city;
            rtn_emp->state = emp_struct.state;
            rtn_emp->country = emp_struct.country;
            rtn_emp->phone = emp_struct.phone;
            rtn_emp->salary = emp_struct.salary;
            return rtn_emp;
        }
    }
    return nullptr;
}

string Employee::lower(const string& str) {
    string return_str = "";
    for (char c : str) {
        return_str += tolower(c);
    }
    return return_str;
}

bool Employee::str_cmpr(const string& s1, const string& s2) {
    if (s1.length() != s2.length()) {
        return false;
    } else {
        for (size_t i = 0; i < s1.length(); i++) {
            if (s1.at(i) != s2.at(i)){return false;}
        }
    }
    return true;
}

void Employee::display(std::ostream & os) const {
    os << "id: " << this->id << std::endl;
    os << "name: " << this->name << std::endl;
    os << "address: " << this->address << std::endl;
    os << "city: " << this->city << std::endl;
    os << "state: " << this->state << std::endl;
    os << "country: " << this->country << std::endl;
    os << "phone: " << this->phone << std::endl;
    os << "salary: " << this->salary << std::endl;
    os << "\n";
}
/*
void Employee::write(std::ostream &is) const {
    EmployeeRec emp_struct;
    emp_struct.id = this->id;
    std::strncpy(emp_struct.name, this->name.c_str(), 31)[31];
    std::strncpy(emp_struct.address, this->address.c_str(), 26)[26];
    std::strncpy(emp_struct.city, this->city.c_str(), 21)[21];
    std::strncpy(emp_struct.state, this->state.c_str(), 21)[21];
    std::strncpy(emp_struct.country, this->country.c_str(), 21)[21];
    std::strncpy(emp_struct.phone, this->phone.c_str(), 21)[21];
    emp_struct.salary = this->salary;
    is.write(reinterpret_cast<const char*>(&emp_struct), sizeof(emp_struct));
}

*/
//write 2.0
void Employee::write(std::ostream &is) const {
    is.write(reinterpret_cast<const char*>(&id), sizeof(id));
    is.write(name.c_str(), name.size());
    is.write(address.c_str(), address.size());
    is.write(city.c_str(), city.size());
    is.write(state.c_str(), state.size());
    is.write(country.c_str(), country.size());
    is.write(phone.c_str(), phone.size());
    is.write(reinterpret_cast<const char*>(&salary), sizeof(salary));
}


void Employee::store(std::iostream& ios) const {
    EmployeeRec emp_struct;
    emp_struct.id = this->id;
    strncpy(emp_struct.name, this->name.c_str(), 31)[31] = '\0';
    strncpy(emp_struct.address, this->address.c_str(), 26)[26] = '\0'; 
    strncpy(emp_struct.city, this->city.c_str(), 21)[21] = '\0'; 
    strncpy(emp_struct.state, this->state.c_str(), 21)[21] = '\0'; 
    strncpy(emp_struct.country, this->country.c_str(), 21)[21] = '\0'; 
    strncpy(emp_struct.phone, this->phone.c_str(), 21)[21] = '\0'; 
    emp_struct.salary = this->salary;

    ios.seekp(0, std::ios::end); 

    ios.write(reinterpret_cast<const char*>(&emp_struct), sizeof(emp_struct));

    if (!ios.good()) { //check if good
        throw std::runtime_error("Failed to write Employee record");
    }
}

void Employee::toXML(std::ostream & os) const {
    os << "<Employee>" << std::endl;
    os << "\t" << "<id>" << this->id << "</id>" << std::endl;
    os << "\t" << "<name>" << this->name << "</name>" << std::endl;
    if (this->address != "") {
        os << "\t" << "<address>" << this->address << "</address>" << std::endl;
    }
    if (this->city != ""){
        os << "\t" << "<city>" << this->city << "</city>" << std::endl;
    }
    if (this->state != ""){
        os << "\t" << "<state>" << this->state << "</state>" << std::endl;
    }
    if (this->country != "") {
        os << "\t" << "<country>" << this->country << "</country>" << std::endl;
    }
    if (this->phone != "") {
        os << "\t" << "<phone>" << this->phone << "</phone>" << std::endl;
    }
    if (this->salary != 0.0) {
        os << "\t" << "<salary>" << this->salary << "</salary>" << std::endl;
    }
    os << "</Employee>" << std::endl;
}

void Employee::set_salary(double sal) {
    this->salary = sal;
}
 
double Employee::get_salary() {
    return this->salary;
}

Employee* Employee::fromXML(std::istream & ifstr) {
    string chunk{""};//uniform vs director initialization?
    string current_tag = "";
    bool loop_tag = true;
    bool emp_tag = false;
    bool tag_start = false;
    bool id_tag = false;
    bool name_tag = false;
    Employee* swapEmp = new Employee();
    bitset<BITSETSIZE> empbits;//flags flags flags!


    if (ifstr.peek() == EOF) { //null does not work
        return nullptr;
    }
    while (loop_tag && ifstr) {
        std::getline(ifstr, chunk, '<');
        if (tag_start == true) {
            if (current_tag == "id") {
                if (empbits[7] == 1) {
                    throw runtime_error("Duplicate Tags");
                }
                empbits[7] = 1;
                id_tag = true;
                swapEmp->id = stoi(chunk);
            } else if (current_tag == "name") {
                if (empbits[6] == 1) {
                    throw runtime_error("Duplicate Tag");
                }
                empbits[6] = 1;
                name_tag = true;
                swapEmp->name = chunk;
            } else if (current_tag == "address") {
                if (empbits[5] == 1) {
                    throw runtime_error("Duplicate Tags");
                }
                empbits[5] = 1;
                swapEmp->address = chunk;
            } else if (current_tag == "city") {
                if (empbits[4] == 1) {
                    throw runtime_error("Duplicate Tags");
                }
                empbits = 1;
                swapEmp->city = chunk; 
            } else if (current_tag == "state") {
                if (empbits[3] == 1) {
                    throw runtime_error("Duplicate Tags");
                }
                empbits = 1;
                swapEmp->state = chunk;
            } else if (current_tag == "country") {
                if (empbits[2] == 1) {
                    throw runtime_error("Duplicate Tags");
                }
                empbits[2] = 1;
                swapEmp->country = chunk;
            } else if (current_tag == "phone") {
                if (empbits[1] == 1) {
                    throw runtime_error("Duplicate Tags");
                }
                empbits = 1;
                swapEmp->phone = chunk;
            } else if (current_tag == "salary") {
                if (empbits[0] == 1) {
                    throw runtime_error("Duplicate Tags");
                }
                empbits[0] = 1;
                swapEmp->salary = stod(chunk);
            } else {
                throw runtime_error("Invalid Tag: " + current_tag);
            }
        }
        getline(ifstr, chunk, '>');
        chunk = lower(chunk);
        if (str_cmpr(chunk, "employee")) {
            if (emp_tag != true) {
                emp_tag = true;
                continue;
            } else {
                throw runtime_error("No closing tag for employee found.");
            }
        }
        //employee tests
        else if (chunk.find("/") != string::npos && chunk.find("employee") != string::npos) {
            loop_tag = false;
            break;
        }
        else if (chunk.find("/") != string::npos) {
            if (tag_start == false) {
                throw runtime_error("No Opening Tag");
            }
            tag_start = false;
            continue;
        } else {
            if (current_tag != "" && emp_tag == false) {
                throw runtime_error("No Opening Employee Tag");
            }
            if (tag_start == true) {
                throw runtime_error("No Closing Tag For" + current_tag);
            }
            current_tag = chunk;
            tag_start = true;
            }
        }
        if(!ifstr || ifstr.peek() == EOF){
            return nullptr;
        }

        //if (name_tag == false || id_tag == false) {
        //    throw runtime_error("Employee Missing Required Data!");
        //}
    return swapEmp;
}
