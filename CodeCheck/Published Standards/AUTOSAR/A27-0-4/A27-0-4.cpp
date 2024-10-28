// $Id: A27-0-4.cpp 311495 2018-03-13 13:02:54Z michal.szczepankiewicz $
#include <iostream>
#include <cstring>
#include <list>

std::string myFunction(int x);

void F1()
{
  char myString[]{ "string" }; // UndCC_Violation, C-style strings
  char myChars[26]; // UndCC_Valid, only when it is used as a string that it becomes a problem
  char myCheck[10];
  
  std::string sResult;
  std::string string1;
  std::string string2;
  std::cin >> string1 >> string2; // UndCC_Valid - no buffer overflows
  
  strcpy(myCheck,"A27.0.4"); //useby ref // UndCC_Violation
  
  sResult = myFunction(10);
  char myCheck2[sResult.size() + 1];
  strcpy (myCheck2, sResult.c_str());  // useby ref // UndCC_Violation
  
  char myCheck3[] = "Hello"; // UndCC_Violation
  
  char myCheck4[10];
  // myCheck4 = "Hello";  // compile error
  
  char myCheck5[3] = {'H', 'i', '\0'}; // UndCC_Valid
  
  char myCheck6[10];
  strcpy(myCheck6, myFunction(1).c_str());  // useby ref // UndCC_Violation
  
  const char *myCheck9[10] = { "hello" }; // skip pointers?
  
}

std::list<std::string> F2(const std::string& terminator)
{
  std::list<std::string> ret;
  //read a single word until it is different from the given terminator sequence
  for (std::string s; std::cin >> s && s != terminator; )
  {
    ret.push_back(s);
  }
  return ret;
}

std::string myFunction(int x)
{
  if ( x < 5 ) return "YES";
  else return "NO";
}
