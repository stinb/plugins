class Good {
public:
  Good() = default;
  Good(const Good &good) = delete;
};


class Bad {
public:
  Bad() {}             // UndCC_Violation

private:
  Bad(const Bad &bad); // UndCC_Violation - unimplemented
};
