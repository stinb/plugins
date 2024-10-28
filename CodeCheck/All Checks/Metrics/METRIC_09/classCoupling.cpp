#include <iostream>
#include <string>

class Person
{
    private:


    public:
        std::string name;

        void setName(std::string n)
        {
            name = n;
        }
};

class Dog
{
private:


public:
    std::string name;

    void setName(std::string n)
    {
        name = n;
    }
};

class Cat
{
private:


public:
    std::string name;

    void setName(std::string n)
    {
        name = n;
    }
};

class Ewok
{
private:


public:
    std::string name;

    void setName(std::string n)
    {
        name = n;
    }
};

class Avatar
{
private:


public:
    std::string name;

    void setName(std::string n)
    {
        name = n;
    }
};

class Bird
{
private:


public:
    std::string name;

    void setName(std::string n)
    {
        name = n;
    }
};

class ZooSetup  /* UndCC_Violation when max coupling set to 5, coupled with 6 other classes */
{
    void setNames()
    {
        Person myPerson;
        myPerson.name = "Todd";

        Dog myDog;
        myDog.name = "Spot";

        Cat myCat;
        myCat.name = "Garfield";

        Ewok myEwok;
        myEwok.name = "Ewok";

        Avatar myAvatar;
        myAvatar.name = "Pandora";

        Bird myBird;
        myBird.name = "Foghorn";

    }
};

