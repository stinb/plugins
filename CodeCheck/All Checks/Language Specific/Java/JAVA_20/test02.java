import java.lang.*;

class E {

    public void finalize()

    {

        System.out.println("Finalize A");

    }

}


class F extends E {

    public void finalize()

    {

        System.out.println("Finalize B");

        super.finalize();    /* UNDCC_Violation - more than one call to E.finalize */

        super.finalize();

    }

}
