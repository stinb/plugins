public class Test00 {
    void a() {              // UNDCC_Valid

    }
    void b() {              // UNDCC_Violation

    }
    void c() {              // UNDCC_Violation

    }

    void d() {              // UNDCC_Valid

    }
    void e() {              // UNDCC_Violation

    }

    void f() {              // UNDCC_Valid

    }

    void g() {}             // UNDCC_Valid
    void h() {}             // UNDCC_Violation
    void i() {}             // UNDCC_Violation

    void j() {}             // UNDCC_Valid
    void k() {}             // UNDCC_Violation

    void l() {}             // UNDCC_Valid

    void k() {} void m() {} // UNDCC_Violation

    public static void main(String[] args) {

    }
}
