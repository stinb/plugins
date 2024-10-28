class Node
{

private:
  Node *m_left;
  Node *m_right;

public:
  Node(Node *left, Node *right): m_left(left), m_right(right)
  {

  }

  Node *get_left()
  {
    return m_left;
  }

  Node *get_right()
  {
    return m_right;
  }

  void set_left(Node *left)
  {
    m_left = left;
  }

  void set_right(Node *right)
  {
    m_right = right;
  }

  void set_left_left(Node *left_left)
  {
    if (m_left) {
      m_left->set_left(left_left); // UndCC_Valid
    }
  }

  void set_left_right(Node *left_right)
  {
    if (m_left) {
      this->m_left->set_left(left_right); // UndCC_Valid
    }
  }

};

int main()
{
  Node node_y(0, 0);
  Node node_z(0, 0);
  Node node_x(&node_y, &node_z);

  Node *node_x_ptr = &node_x;

  Node *node_y_ptr = node_x_ptr->get_left();    // UndCC_Valid
  node_x_ptr->set_left(node_x_ptr->get_left()); // UndCC_Valid
}
