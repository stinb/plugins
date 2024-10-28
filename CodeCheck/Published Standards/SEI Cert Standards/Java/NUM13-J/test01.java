public class Test01 {
    public static int subFloatFromInt(int op1, float op2) {
        return op1 - (int)op2;
    }

    public void f01(int op3) {
        op3 += 1111111111;
        float z = (float) op3; // UndCC_Violation
    }

    public static void main(String[] args) {
        int result = subFloatFromInt(1234567890, 1234567890); // UndCC_Violation

        int a = 111;
        a = 1111111111;
        float z = (float) a; // UndCC_Violation

        int b = 111;
        float x = (float) b; // UndCC_FalsePos - b has multiple assignments, so it's flagged
        x += 1111111111;
        b = (int) x;
        float y = (float) b; // UndCC_Violation

        int c;
        c = 1111111111;

        float w = (float) c; // UndCC_Violation

        System.out.print(w);
    }
}
