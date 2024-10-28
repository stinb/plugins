// Client code:
class ExampleClientCode_n {

    private final USCurrency_n currency = new USCurrency_n();
    // ...

    public ExampleClientCode_n() {

        Thread t1 = new Thread(new Runnable() {
            @Override
            public void run() {
                currency.setQuarters(1).setDimes(1); // UndCC_Violation
            }
        });
        t1.start();

        Thread t2 = new Thread(new Runnable() {
            @Override
            public void run() {
                currency.setQuarters(2).setDimes(2); // UndCC_Violation
            }
        });
        t2.start();

        // ...
    }
}
