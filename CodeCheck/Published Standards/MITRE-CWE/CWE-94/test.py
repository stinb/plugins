def main():
    sum = 0
    numbers = eval(input("Enter a space-separated list of numbers: ")) # UndCC_Violation
    for num in numbers:
        sum = sum + num
    print(f"Sum of {numbers} = {sum}")
main()
