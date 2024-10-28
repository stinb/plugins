import java.io.Serializable;
import java.security.SecureRandom;
import java.io.ObjectInputStream;
import java.io.IOException;

public class Lottery_n implements Serializable {        //UndCC_Violation, readObject method invalid
    private int ticket = 1;
    private SecureRandom draw = new SecureRandom();

    public Lottery_n(int ticket) {
        this.ticket = (int) (Math.abs(ticket % 20000) + 1);
    }

    public int getTicket() {
        return this.ticket;
    }

    public int roll() {
        this.ticket = (int) ((Math.abs(draw.nextInt()) % 20000) + 1);
        return this.ticket;
    }

    public static void main(String[] args) {
        Lottery_n l = new Lottery_n(2);
        for (int i = 0; i < 10; i++) {
            l.roll();
            System.out.println(l.getTicket());
        }
    }

    private void readObject(ObjectInputStream in) throws IOException, ClassNotFoundException {
        in.defaultReadObject();
    }
}
