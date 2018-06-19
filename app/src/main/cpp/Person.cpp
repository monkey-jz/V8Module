//
// Created by JerryZhu on 2018/5/31.
//

#include <class.hpp>
#include "Person.h"

int Person::account = 100001;

Person::Person(const std::string &name, int age) : name(name), age(age) {}
void Person::setName(std::string name)
{
    this -> name = name;
}
std::string Person::getName()
{
    return this -> name;
}
void Person::setAge(int age)
{
    this ->age = age;
}
int Person::getAge()
{
    return this -> age;
}

Person::~Person() {

}
