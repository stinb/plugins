// Custom

using int_t = int;

// The following will not compile and are violations of the rule:
// void inconsistent() throw(const char *);
// void inconsistent() throw(int) {}

void consistent() throw(const int);
void consistent() throw(int) {}
