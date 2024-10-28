int MISRA2012rule156() {

    int flag = 1;
    int flag_1 = 0;
    int flag_2 = 1;
    int data_available = 2;

    while (data_available) // UndCC_Violation
        process_data();

    while (data_available) // UndCC_Violation
        process_data();
    service_watchdog();

    if (flag_1)            // UndCC_Violation
        if (flag_2)        // UndCC_Violation
            action_1();
        else               // UndCC_Violation
            action_2();


    if (flag_1)
    {
        if (flag_2)
        {
            action_1();
        }
        else
        {
            action_2();
        }
    }

    if (flag_1)
    {
        action_1();
    }
    else if (flag_2)       // UndCC_Valid by exception - The exception basically means that 'else if' statements are allowed or specifically you don't need curly braces after the word 'else' and before the word 'if' when using an 'else if'
    {
        action_2();
    }
    else
    {
        ;
    }

    if (flag_1)
    {
        action_1();
    }
    else if (flag_2)       // UndCC_Violation
        action_2();
    else
    {
        ;
    }


    while (flag);          // UndCC_Violation
    {
        flag = fn();
    }

    while (!data_available)
    {
    }

    return 0;
}
