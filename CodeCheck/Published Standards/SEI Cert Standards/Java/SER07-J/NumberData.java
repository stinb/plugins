import java.io.NotSerializableException;

public class NumberData extends Number {
    // ... Implement abstract Number methods, like Number.doubleValue()...

    private static final NumberData INSTANCE = new NumberData();

    public static NumberData getInstance() {
        return INSTANCE;
    }

    private NumberData() {
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

    protected final Object readResolve() throws NotSerializableException {
        return INSTANCE;
    }
}