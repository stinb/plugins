typedef unsigned long size_t;

// --- Example from MISRA C++ 2023 Dir 15.8.1 ---

class Vector {
public:
    // Non-compliant copy assignment — deletes without guard
    Vector &operator=(Vector const &other) // UndCC_Violation
    {
        delete[] data;
        sz = other.sz;
        data = new double[sz];
        return *this;
    }

    // Non-compliant move assignment — resources released on self-assign
    Vector &operator=(Vector &&other) noexcept // UndCC_Violation
    {
        delete[] data;
        data = other.data;
        sz = other.sz;
        other.data = nullptr;
        other.sz = 0;
        return *this;
    }

private:
    double *data = nullptr;
    size_t sz = 0;
};

// Compliant copy assignment — identity guard
class Vector2 {
public:
    Vector2 &operator=(Vector2 const &other) // UndCC_Valid
    {
        if (this != &other) {
            double *tmp = new double[other.sz];
            delete[] data;
            data = tmp;
            sz = other.sz;
        }
        return *this;
    }

private:
    double *data = nullptr;
    size_t sz = 0;
};

// Compliant copy assignment — copy-and-swap
class Vector3 {
public:
    Vector3 &operator=(Vector3 const &other) // UndCC_Valid
    {
        Vector3 tmp(other);
        swap(tmp);
        return *this;
    }

private:
    double *data = nullptr;
    size_t sz = 0;

    void swap(Vector3 &other) noexcept {
        double *td = data; data = other.data; other.data = td;
        size_t ts = sz; sz = other.sz; other.sz = ts;
    }
};

// Compliant move assignment — move-and-swap
class Vector4 {
public:
    Vector4 &operator=(Vector4 &&other) noexcept // UndCC_Valid
    {
        Vector4 tmp(static_cast<Vector4 &&>(other));
        swap(tmp);
        return *this;
    }

private:
    double *data = nullptr;
    size_t sz = 0;

    void swap(Vector4 &other) noexcept {
        double *td = data; data = other.data; other.data = td;
        size_t ts = sz; sz = other.sz; other.sz = ts;
    }
};

// --- Additional test cases ---

// Copy operator= with no delete, no guard — simple assign, no protection
class D {
public:
    D &operator=(D const &other) // UndCC_Violation
    {
        val = other.val;
        return *this;
    }

private:
    int val = 0;
};

// Copy operator= with this == &other early return
class G {
public:
    G &operator=(G const &other) // UndCC_Valid
    {
        if (this == &other) {
            return *this;
        }
        delete[] data;
        sz = other.sz;
        data = new double[sz];
        return *this;
    }

private:
    double *data = nullptr;
    size_t sz = 0;
};

// Move operator= with this != &other guard
class M {
public:
    M &operator=(M &&other) noexcept // UndCC_Valid
    {
        if (this != &other) {
            delete[] data;
            data = other.data;
            sz = other.sz;
            other.data = nullptr;
            other.sz = 0;
        }
        return *this;
    }

private:
    double *data = nullptr;
    size_t sz = 0;
};

// Defaulted operators — not user-provided
class I {
public:
    I &operator=(I const &) = default; // UndCC_Valid
    I &operator=(I &&) = default;      // UndCC_Valid

private:
    int val = 0;
};
