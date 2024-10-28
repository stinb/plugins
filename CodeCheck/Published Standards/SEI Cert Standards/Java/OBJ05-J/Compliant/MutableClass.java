// SEI CERT C++

import java.util.Date;

class MutableClass {
    private Date d;

    public MutableClass() {
        d = new Date();
    }

    public Date getDate() {
        return (Date) d.clone();
    }
}
