class Test {

  public:
    Test( int x, int y, int level);
    virtual ~Test( );

    int getX( ) const;
    int getY( ) const;
    float getLevel( ) const;
    void setPosition( int x, int y );

    int a;    // UndCC_Violation(0)
    float b;  // UndCC_Violation(0)
    bool no;  // UndCC_Violation(0)

  protected:
    int mX;        // UndCC_Violation(0)
    int mY;        // UndCC_Violation(0)
    float mLevel;  // UndCC_Violation(0)
    bool yes;      // UndCC_Violation(0)

  private:
    int x;
    int y;
    float yLevel;
    bool good;
};
