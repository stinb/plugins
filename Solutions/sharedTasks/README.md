# Overview

Visualize an architecture of functions with the Shared Tasks graph. See how tasks interact with global objects, and when interrupt control functions are involved.

# Setup: Install the Graph Plugin

1. [Download](https://raw.githubusercontent.com/stinb/plugins/main/Solutions/sharedTasks/sharedTasksGraph.upy) the Shared Tasks graph.

2. Drag and drop it into Understand.

# Setup: Install the CSV Interactive Report Plugin

1. [Download](https://raw.githubusercontent.com/stinb/plugins/main/Solutions/sharedTasks/sharedTasksCSV.upy) the Shared Tasks graph.

2. Drag and drop it into Understand.

# Setup: Use the CSV Script in the Command Line Interface

1. [Download](https://raw.githubusercontent.com/stinb/plugins/main/Solutions/sharedTasks/sharedTasksCSV.upy) the Shared Tasks graph.

2. Run it with upython. Give it the `-h` argument for help.
```sh
upython sharedTasksCSV.upy -h
```

# Setup: Create an Architecture

## Automatic Architecture Creation

**Tasks & Priorities**

1. [Download](https://raw.githubusercontent.com/stinb/plugins/main/Solutions/sharedTasks/archBuilderForSharedTasks.py) the Architecture Builder for Shared Tasks script.

2. Run it with upython.
```sh
upython archBuilderForSharedTasks.py
```

3. Follow the prompts to add functions as tasks.

**Interrupt Control Functions (Optional):**

1. Follow the prompts to add functions as interrupt control.

## Manual Architecture Creation

**Tasks & Priorities:**

1. Create a new architecture in Understand.

2. In this architecture, create a group with "task" or "priority" in the name (capitalization doesn't matter whenever a specific group name is needed.)

3. In this Task Priority group, add a function. Optionally, you can put these functions in an integer group, which indicates the priority of the task.

**Interrupt Control Functions (Optional):**

1. In the architecture, create a group with "interrupt" or "control" in the name.

2. In the Interrupt Control group, make a group with any name. It will be the ancestor of a pair of interrupt control functions.

3. In the group with any name, add 2 groups: one with "disable" in the name and another with "enable" in the name.

4. Finally, add 1 function to the Disable group and a different function to the Enable group.

# Usage

Once an architecture is created, right click it to visualize the shared tasks.
