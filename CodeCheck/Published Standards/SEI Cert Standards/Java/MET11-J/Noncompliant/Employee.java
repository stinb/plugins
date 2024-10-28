// Mutable class Employee
class Employee_2 {
    private String name;
    private double salary;

    Employee_2(String empName, double empSalary) {
        this.name = empName;
        this.salary = empSalary;
    }

    public void setEmployeeName(String empName) {
        this.name = empName;
    }

    public void setSalary(double empSalary) {
        this.salary = empSalary;
    }

    @Override
    public boolean equals(Object o) {
        if (!(o instanceof Employee_2)) {
            return false;
        }

        Employee_2 emp = (Employee_2) o;
        return emp.name.equals(name);   //UndCC_Violation
    }

}
