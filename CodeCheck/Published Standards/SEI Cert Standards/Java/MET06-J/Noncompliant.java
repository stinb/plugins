import java.net.HttpCookie;
import java.lang.Cloneable;

class CloneExample_1 implements Cloneable {

    public Object clone() throws CloneNotSupportedException {
        final CloneExample_1 clone = (CloneExample_1) super.clone();
        clone.doSomething(); // UNDCC_Violation overridable method called in clone()
        clone.cookies = clone.deepCopy();
        return clone;
    }

    void doSomething() { // Overridable
        for (int i = 0; i < cookies.length; i++) {
            cookies[i].setValue("" + i);
        }
    }
}

class Sub_1 extends CloneExample {
    Sub_1(HttpCookie[] c) {
        super(c);
    }

    public Object clone() throws CloneNotSupportedException {
        final Sub_1 clone = (Sub) super.clone();
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
        CloneExample_1 bc = new Sub(hc);
        bc.clone();
    }
}
