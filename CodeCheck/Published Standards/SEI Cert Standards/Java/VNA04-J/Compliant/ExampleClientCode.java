// Client code:
class ExampleClientCode {
    private volatile USCurrency currency;
    // ...

    public ExampleClientCode() {

        Thread t1 = new Thread(new Runnable() {
            @Override
            public void run() {
                currency = USCurrency.Builder.newInstance().setQuarters(1).setDimes(1).build(); // UndCC_Valid
            }
        });
        t1.start();

        Thread t2 = new Thread(new Runnable() {
            @Override
            public void run() {
                currency = USCurrency.Builder.newInstance().setQuarters(2).setDimes(2).build(); // UndCC_Valid
            }
        });
        t2.start();

        // ...
    }
}