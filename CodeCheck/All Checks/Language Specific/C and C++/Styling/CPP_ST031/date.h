#ifndef DATE_H
#define DATE_H

class Date
{
private:
  int m_year;
  int m_month;
  int m_day, m_hour; // UndCC_Violatin - each member declaration should be on own line // UndCC_Violation

public:
  Date(int year, int month, int day);

  void SetDate(int year, int month, int day);

  int getYear() { return m_year; }
  int getMonth() { return m_month; }
  int getDay()  { return m_day; }
};

#endif
