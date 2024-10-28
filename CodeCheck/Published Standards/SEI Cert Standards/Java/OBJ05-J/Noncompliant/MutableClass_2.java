// SEI CERT C++

import java.util.Date;

class MutableClass_4 {
    private Date[] date;

    public MutableClass_4() {
        date = new Date[20];
        for (int i = 0; i < date.length; i++) {
            date[i] = new Date();
        }
    }

    public Date[] getDate() {
        return date; // Or return date.clone(), UNDCC_Violation, method fails to make a defensive
                     // copy of the array before returning it
    }
}
