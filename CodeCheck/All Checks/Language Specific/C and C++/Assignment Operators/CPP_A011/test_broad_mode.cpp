// Test cases for broad option — flags all user-provided copy/move
// operator= without self-assignment handling, not just delete-then-use.

// No delete-then-use — only flagged when broad is enabled
class D {
public:
    D &operator=(D const &oth) // UndCC_Violation(broad)
    {
        val = oth.val;
        return *this;
    }

private:
    int val = 0;
};

class E {
public:
    E &operator=(E &&oth) noexcept // UndCC_Violation(broad)
    {
        val = oth.val;
        return *this;
    }

private:
    int val = 0;
};
