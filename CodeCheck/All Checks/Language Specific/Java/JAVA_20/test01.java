import java.lang.*;

class A {

    public void finalize()

    {

        System.out.println("Finalize A");

    }

}


class B extends A {

    public void finalize()                  /* UndCC_Valid - calls A.finalize last */

    {

        System.out.println("Finalize B");

        super.finalize();

    }

}


class C extends A {

    public void finalize()                 /* UndCC_Violation - no call to C.finalize */

    {

        System.out.println("Finalize C");

    }

}


class D extends A {

    public void finalize()

    {

        super.finalize();                 /* UndCC_Violation - D.finalize call should come last */

        System.out.println("Finalize D");

    }

}


public class override_finalize {

    public static void main(String args[]) {

        A A_obj_A = new A();

        A A_obj_B = new B();

        A A_obj_C = new C();

        A A_obj_D = new D();

        System.out.println("In main");

    }

}
