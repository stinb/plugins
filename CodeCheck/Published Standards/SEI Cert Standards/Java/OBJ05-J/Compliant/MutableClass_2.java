// SEI CERT C++

import java.util.Date;

class MutableClass_2 {
    private Date[] date;

    public MutableClass_2() {
        date = new Date[20];
        for (int i = 0; i < date.length; i++) {
            date[i] = new Date();
        }
    }

    public Date[] getDate() {
        Date[] dates = new Date[date.length];
        for (int i = 0; i < date.length; i++) {
            dates[i] = (Date) date[i].clone();
        }
        return dates;
    }
}
