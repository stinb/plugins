import java.io.Serializable;
import java.security.SecureRandom;
import java.io.ObjectInputStream;
import java.io.IOException;

public class Lottery2 implements Serializable {
    private transient int ticket = 1;
    private transient SecureRandom draw = new SecureRandom();

    public Lottery2(int ticket) {
        this.ticket = (int) (Math.abs(ticket % 20000) + 1);
    }

    public final int getTicket() {
        return this.ticket;
    }

    public final int roll() {
        this.ticket = (int) ((Math.abs(draw.nextInt()) % 20000) + 1);
        return this.ticket;
    }

    public static void main(String[] args) {
        Lottery2 l = new Lottery2(2);
        for (int i = 0; i < 10; i++) {
            l.roll();
            System.out.println(l.getTicket());
        }
    }

    private void readObject(ObjectInputStream in)
            throws IOException, ClassNotFoundException {
        in.defaultReadObject();
        this.draw = new SecureRandom();
        roll();
    }
}