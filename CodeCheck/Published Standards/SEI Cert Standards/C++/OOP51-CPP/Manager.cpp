#include "Manager.h"

const Employee &Manager::get_assistant() const { return assistant; }
Manager::Manager(const std::string &name, const Employee &assistant) : Employee(name), assistant(assistant) {}

void Manager::print(std::ostream &os) const
{
    os << "Manager: " << get_name() << std::endl;
    os << "Assistant: " << std::endl
       << "\t" << get_assistant() << std::endl;
}
