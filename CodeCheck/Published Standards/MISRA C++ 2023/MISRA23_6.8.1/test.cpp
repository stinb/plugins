struct X {
    int val;
    void doSomething() {}
};

// Pattern 1: Use after delete
void f_use_after_delete() {
    int *pi = new int{42};
    delete pi;
    int x = *pi;        // UndCC_Violation - use after delete
}

void f_valid_use_before_delete() {
    int *pi = new int{42};
    int x = *pi;        // UndCC_Valid - use before delete
    delete pi;
}

void f_array_delete() {
    int *arr = new int[10];
    delete[] arr;
    int x = arr[0];     // UndCC_Violation - use after delete[]
}

// Pattern 2: Use after explicit destructor call
void h_use_after_dtor(X *px) {
    px->~X();
    px->doSomething();  // UndCC_Violation - use after destructor
}

void h_valid_use_before_dtor(X *px) {
    px->doSomething();  // UndCC_Valid - use before destructor
    px->~X();
}

// Pattern 3: Non-active union member
union u {
    int a;
    short b[2];
};

short u_nonactive_read() {
    u o;
    o.a = 42;
    return o.b[0];      // UndCC_Violation - b is not active member
}

short u_valid_same_member() {
    u o;
    o.b[0] = 42;
    return o.b[0];      // UndCC_Valid - b is the active member
}

// Branching: delete in one branch, use unconditionally
void branch_test(bool cond) {
    int *p = new int{5};
    if (cond) {
        delete p;
    }
    int x = *p;         // UndCC_Violation - reachable from delete path
}

void branch_safe(bool cond) {
    int *p = new int{5};
    if (cond) {
        delete p;
        return;
    }
    int x = *p;         // UndCC_Valid - delete path returns early
}

// Different union variables of the same type should be independent
short u_different_variables() {
    u x, y;
    x.a = 42;
    return y.b[0];      // UndCC_Valid - x and y are different objects
}

// Destructor via dot syntax on stack object
void h_dot_dtor() {
    X obj;
    obj.~X();
    obj.doSomething();  // UndCC_Violation - use after destructor via dot
}

// Union: reactivation of member by writing to it
short u_reactivated() {
    u o;
    o.a = 42;
    o.b[0] = 10;
    return o.b[0];      // UndCC_Valid - b was reactivated by writing to it
}

// Pointer reassignment after delete should not be flagged
void f_reassign_after_delete() {
    int *p = new int{42};
    delete p;
    p = 0;              // UndCC_Valid - reassigning pointer, not accessing freed memory
}

// Pointer reassigned then used should not be flagged
void f_reassign_then_use() {
    int *p = new int{42};
    delete p;
    p = new int{99};
    int x = *p;         // UndCC_Valid - pointer was reassigned to valid memory
    delete p;
}

// Delete of member access return value should not flag the object
struct Menu {};
struct Button {
    Menu *m;
    Menu *menu() { return m; }
    void setMenu(Menu *) {}
};

void f_delete_method_return(Button *btn, Menu *newMenu) {
    delete btn->menu();
    btn->setMenu(newMenu); // UndCC_Valid - btn was not deleted, its menu() was
}
