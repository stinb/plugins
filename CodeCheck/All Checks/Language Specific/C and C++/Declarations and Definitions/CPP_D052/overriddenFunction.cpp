class Canvas
{
};
class Shape
{
    virtual void draw(Canvas &destination) = 0;
    virtual void fill(Canvas &destination) = 0;
};
class Rectangle : public Shape
{
    void draw(Canvas &canvas) override;      // UndCC_Violation
    void fill(Canvas &destination) override; // UndCC_Valid
};