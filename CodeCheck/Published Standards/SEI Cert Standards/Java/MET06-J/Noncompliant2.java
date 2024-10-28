import java.net.HttpCookie;
import java.lang.Cloneable;

class CloneExample implements Cloneable {
    HttpCookie[] cookies;

    CloneExample(HttpCookie[] c) {
        cookies = c;
    }

    public Object clone() throws CloneNotSupportedException {
        final CloneExample clone = (CloneExample) super.clone();
        clone.doNothing(); // UNDCC_Violation overridable method called in clone()
        clone.cookies = clone.deepCopy();
        return clone;
    }

    void doNothing() { // Overridable
        for (int i = 0; i < cookies.length; i++) {
            cookies[i].setValue(0);
        }
    }

    HttpCookie[] deepCopy() {
        if (cookies == null) {
            throw new NullPointerException();
        }

        // Deep copy
        HttpCookie[] cookiesCopy = new HttpCookie[cookies.length];

        for (int i = 0; i < cookies.length; i++) {
            // Manually create a copy of each element in array
            cookiesCopy[i] = (HttpCookie) cookies[i].clone();
        }
        return cookiesCopy;
    }
}

class Sub extends CloneExample {
    Sub(HttpCookie[] c) {
        super(c);
    }

    public Object clone() throws CloneNotSupportedException {
        final Sub clone = (Sub) super.clone();
        clone.doSomething();
        return clone;
    }

    void doSomething() { // Erroneously executed
        for (int i = 0; i < cookies.length; i++) {
            cookies[i].setDomain(i + ".foo.com");
        }
    }

    public static void main(String[] args)
            throws CloneNotSupportedException {
        HttpCookie[] hc = new HttpCookie[20];
        for (int i = 0; i < hc.length; i++) {
            hc[i] = new HttpCookie("cookie" + i, "" + i);
        }
        CloneExample bc = new Sub(hc);
        bc.clone();
    }
}
