// Mutable class Employee
class Employee {
    private String name;
    private double salary;
    private final long employeeID; // Unique for each Employee

    Employee(String name, double salary, long empID) {
        this.name = name;
        this.salary = salary;
        this.employeeID = empID;
    }

    // ...

    @Override
    public boolean equals(Object o) {
        if (!(o instanceof Employee)) {
            return false;
        }

        Employee emp = (Employee) o;
        return emp.employeeID == employeeID;    //UndCC_Valid
    }
}
