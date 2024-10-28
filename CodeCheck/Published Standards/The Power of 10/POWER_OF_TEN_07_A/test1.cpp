// C++ operator call expressions and construct expressions

class VirtualPoint
{
   protected:
      int m_x;
      int m_y;

   public:
      VirtualPoint(): m_x(0), m_y(0)
      {

      }

      VirtualPoint(int x, int y): m_x(x), m_y(y)
      {

      }

      int get_x() const
      {
        return m_x;
      }

      int get_y() const
      {
        return m_y;
      }
};

class PointDefaultOp: public VirtualPoint
{
  public:
    PointDefaultOp(): VirtualPoint()
    {

    }

    PointDefaultOp(int x, int y): VirtualPoint(x, y)
    {

    }
};

class PointVoidOp: public VirtualPoint
{
  public:
    PointVoidOp(): VirtualPoint()
    {

    }

    PointVoidOp(int x, int y): VirtualPoint(x, y)
    {

    }

    void operator= (const PointVoidOp &other )
    {
      m_x = other.get_x();
      m_y = other.get_y();
    }
};

class PointNonVoidOp: public VirtualPoint
{
  public:
    PointNonVoidOp(): VirtualPoint()
    {

    }

    PointNonVoidOp(int x, int y): VirtualPoint(x, y)
    {

    }

    PointNonVoidOp & operator= (const PointNonVoidOp &other )
    {
      m_x = other.get_x();
      m_y = other.get_y();
      return *this;
    }
};

static void operator_calls()
{
   PointDefaultOp point_default_op_0(0, 1), point_default_op_1(2, 3);
   point_default_op_0 = point_default_op_1;

   PointVoidOp point_void_op_0(0, 1), point_void_op_1(2, 3);
   point_void_op_0 = point_void_op_1;

   PointNonVoidOp point_non_void_op_0(0, 1), point_non_void_op_1(2, 3);
   point_non_void_op_0 = point_non_void_op_1;
}

static void constructor_calls()
{
   PointDefaultOp(0, 1);     // UndCC_Violation - result ignored
   PointDefaultOp point_0(0, 1);

   new PointDefaultOp(0, 1); // UndCC_Violation - result ignored
   PointDefaultOp *point_1 = new PointDefaultOp(0, 1);
}
