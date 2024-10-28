import javax.swing.JFrame;
import java.util.Date;

class MyFrame_n extends JFrame {
    private byte[] buffer = new byte[16 * 1024 * 1024];
    // Persists for at least two GC cycles
}

class BaseClass {
    protected void finalize() throws Throwable {
        System.out.println("Superclass finalize!");
        doLogic();
    }

    public void doLogic() throws Throwable {
        System.out.println("This is super-class!");
    }
}

class SubClass extends BaseClass {
    private Date d; // Mutable instance field

    protected SubClass() {
        d = new Date();
    }

    protected void finalize() throws Throwable {
        System.out.println("Subclass finalize!");
        try {
            // Cleanup resources
            d = null;
        } finally {
            super.finalize(); // Call BaseClass's finalizer
        }
    }

    public void doLogic() throws Throwable {
        // Any resource allocations made here will persist

        // Inconsistent object state
        System.out.println(
                "This is sub-class! The date object is: " + d);
        // 'd' is already null
    }
}
