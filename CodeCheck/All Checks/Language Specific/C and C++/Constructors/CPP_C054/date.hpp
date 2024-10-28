#ifndef DATE_HPP
#define DATE_HPP

/* This class is will be flagged */
class Date // UndCC_Violation
{
private:
  int m_year;
  int m_month;
  int m_day;
  
public:
  Date(int year, int month, int day);
  
  void SetDate(int year, int month, int day);
  
  int getYear() { return m_year; }
  int getMonth() { return m_month; }
  int getDay()  { return m_day; }
};

#endif
