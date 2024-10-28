public class NumberData_n extends Number {      //UndCC_Violation, no readResolve method
    // ... Implement abstract Number methods, like Number.doubleValue()...

    private static final NumberData_n INSTANCE = new NumberData_n();

    public static NumberData_n getInstance() {
        return INSTANCE;
    }

    private NumberData_n() {
        // Perform security checks and parameter validation
    }

    protected int printData() {
        int data = 1000;
        // Print data
        return data;
    }

    @Override
    public int intValue() {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public long longValue() {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public float floatValue() {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public double doubleValue() {
        // TODO Auto-generated method stub
        return 0;
    }
}
