final public class Test {
    private Position pos;
    private Position max;

    public Test() {
        this.pos = new Position(0, 0);
        this.max = new Position(1920, 1080);
    }

    final public void checkAndTeleport0(Position pos) {
        if (pos.x < 0)
            this.pos.x = 0;
        else if (pos.x > max.x)
            this.pos.x = max.x;
        else
            this.pos.x = pos.x;

        if (pos.y < 0) {
            this.pos.y = 0;
        }
        else if (pos.y > max.y) {
            this.pos.y = max.y;
        }
        else {
            this.pos.y = pos.y;
        }
    }

    final public void noCheckTeleport1(Position pos) {
        this.pos.x = pos.x; // UndCC_Violation
        this.pos.y = pos.y;
    }

    final public void noCheckTeleport2(Position pos) {
        this.pos = pos; // UndCC_Violation
    }

    final public void check3(Position pos) {
        if (pos.x < 0)
            pos.x = 0;
        else if (pos.x > max.x)
            pos.x = max.x;

        if (pos.y < 0)
            pos.y = 0;
        else if (pos.y > max.y)
            pos.y = max.y;

        teleport3(pos);
    }
    final private void teleport3(Position pos) {
        this.pos.x = pos.x;
        this.pos.y = pos.y;
    }

    final public void fakeCheck4(Position pos) {
        if (3 == 3);

        teleport4(pos); // UndCC_Violation
    }
    final private void teleport4(Position pos) {
        this.pos.x = pos.x; // UndCC_Violation
        this.pos.y = pos.y;
    }

    final public void check5(Position pos) {
        if (pos.x < 0)
            pos.x = 0;
        else if (pos.x > max.x)
            pos.x = max.x;

        if (pos.y < 0)
            pos.y = 0;
        else if (pos.y > max.y)
            pos.y = max.y;

        passAlong5(pos);
    }
    final private void passAlong5(Position pos) {
        passAlongAgain5(pos);
    }
    final private void passAlongAgain5(Position pos) {
        teleport5(pos);
    }
    final private void teleport5(Position pos) {
        this.pos.x = pos.x;
        this.pos.y = pos.y;
    }

    public static void main(String[] args) {

    }
}

final public class Position {
    public int x;
    public int y;

    public Position(int nx, int ny) {
        x = nx;
        y = ny;
    }

    @Override
    public String toString() {
        return this.x + ", " + this.y;
    }
}
