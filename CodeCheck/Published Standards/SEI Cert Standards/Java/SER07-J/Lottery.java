import java.io.Serializable;
import java.security.SecureRandom;
import java.io.ObjectInputStream;
import java.io.IOException;
import java.io.InvalidObjectException;

public class Lottery implements Serializable {
    private int ticket = 1;
    private SecureRandom draw = new SecureRandom();

    public Lottery(int ticket) {
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
        Lottery l = new Lottery(2);
        for (int i = 0; i < 10; i++) {
            l.roll();
            System.out.println(l.getTicket());
        }
    }

    private synchronized void readObject(java.io.ObjectInputStream s)
            throws IOException, ClassNotFoundException {
        ObjectInputStream.GetField fields = s.readFields();
        int ticket = fields.get("ticket", 0);
        if (ticket > 20000 || ticket <= 0) {
            throw new InvalidObjectException("Not in range!");
        }
        // Validate draw
        this.ticket = ticket;
    }
}