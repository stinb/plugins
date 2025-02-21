#include <memory>

// Define the Qt namespace
namespace Qt {
  enum ConnectionType { AutoConnection, DirectConnection, QueuedConnection, BlockingQueuedConnection, UniqueConnection };
}

class QObject;

struct QMetaObject {
  class Connection {};
};

class QMetaMethod {
public:
  QMetaMethod(void (QObject::*)(QObject *));

  template <class F>
  QMetaMethod(F &&lambda) {}
};

class QObject {
public:
  void destroyed(QObject *obj);

  static QMetaObject::Connection connect(
    const QObject *sender,
    const QMetaMethod &signal,
    const QObject *receiver,
    const QMetaMethod &method,
    Qt::ConnectionType type = Qt::AutoConnection);
};


template<typename T> class QSharedPointer {
public:
  T* operator->() const{
    return nullptr;
  }
  void doSomething(){}
  T *data() const;
};

class Class : public QObject {
public:
  void doSomething(){}
};

void f(Class *ctx)
{
  QSharedPointer<Class>  ptr_a;
  std::shared_ptr<Class> ptr_b;

  // No capture
  QObject::connect(ptr_a.data(), &Class::destroyed, ctx, [] {});      // UndCC_Valid
  QObject::connect(&*ptr_b, &Class::destroyed, ctx, [] {});           // UndCC_Valid

  // Captured pointer data is different from the signaling pointer data
  QObject::connect(ptr_a.data(), &Class::destroyed, ctx, [ptr_b] {}); // UndCC_Violation(0)
  QObject::connect(&*ptr_b, &Class::destroyed, ctx, [ptr_a] {});      // UndCC_Violation(0)

  // Captured pointer data is the same as the signaling pointer data
  QObject::connect(ptr_a.data(), &Class::destroyed, ctx, [ptr_a] {}); // UndCC_Violation
  QObject::connect(&*ptr_b, &Class::destroyed, ctx, [ptr_b] {});      // UndCC_Violation
  

  QObject::connect(ptr_a.data(), &Class::destroyed, ctx, [=] {ptr_a->doSomething();}); // UndCC_Violation
  QObject::connect(nullptr, &Class::destroyed, ctx, [] {QSharedPointer<Class> ptr_c; // UndCC_Valid
      });
}
