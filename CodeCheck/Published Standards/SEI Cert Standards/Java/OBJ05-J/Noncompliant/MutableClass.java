// SEI CERT C++

import java.util.Date;

class MutableClass_3 {
    private Date d;

    public MutableClass_3() {
        d = new Date();
    }

    public Date getDate() {
        return d; // UNDCC_Violation, returns the sole instance of the private Date object
    }
}
