#include <memory>

// Define the QT namespace
namespace QT {
enum ConnectionType { AutoConnection, DirectConnection, QueuedConnection, BlockingQueuedConnection, UniqueConnection };
}

// Define the Qt namespace
namespace Qt {
using ConnectionType = QT::ConnectionType;
}

class QObject;


class QObject {
public:
  const char * destroyed(QObject *obj);
  static void connect(const QObject *sender, const char *signal, const QObject *receiver, Qt::ConnectionType type);
};

template<typename T> class QSharedPointer {
public:
  T *data() const;
};

class Class : public QObject {};

void f(Class *ctx)
{
  QSharedPointer<Class>  ptr_a;
  std::shared_ptr<Class> ptr_b;

  // No capture
  QObject::connect(ptr_a.data(), &Class::destroyed, ctx, [] {});      // UndCC_Valid
  QObject::connect(&*ptr_b, &Class::destroyed, ctx, [] {});           // UndCC_Valid

  // Captured pointer data is different from the signaling pointer data
  QObject::connect(ptr_a.data(), &Class::destroyed, ctx, [ptr_b] {}); // UndCC_Valid
  QObject::connect(&*ptr_b, &Class::destroyed, ctx, [ptr_a] {});      // UndCC_Valid

  // Captured pointer data is the same as the signaling pointer data
  QObject::connect(ptr_a.data(), &Class::destroyed, ctx, [ptr_a] {}); // UndCC_Violation
  QObject::connect(&*ptr_b, &Class::destroyed, ctx, [ptr_b] {});      // UndCC_Violation
}

