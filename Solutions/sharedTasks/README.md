# Overview

Calls graphs with global objects are useful for finding all global objects accessed by a call tree. But, what if you want to find which global objects are shared between multiple call trees? The shared tasks plugins find global objects accessed from multiple call trees.

The plugin [sharedTasksGraph.upy](https://raw.githubusercontent.com/stinb/plugins/main/Solutions/sharedTasks/sharedTasksGraph.upy) displays all the call trees and objects in a single graph, highlighting global objects that are accessed from multiple root functions. The plugin [sharedTasksObjectGraph.upy](https://raw.githubusercontent.com/stinb/plugins/main/Solutions/sharedTasks/sharedTasksObjectGraph.upy) does is the reverse - it starts from objects and goes to root functions. The plugin [sharedTasksCSV.upy](https://raw.githubusercontent.com/stinb/plugins/main/Solutions/sharedTasks/sharedTasksCSV.upy) displays each reference in a table in an interactive report. It can also export a CSV file at the same time as the interactive report or when run directly from the command line.

<img width="886" alt="image" src="https://github.com/stinb/plugins/assets/7937320/ff1f01b2-4970-4c34-ad12-ada42ee13c22">

All plugin scripts in this folder use the common [sharedTasks.py](https://github.com/stinb/plugins/blob/main/Solutions/sharedTasks/sharedTAsks.py) so if a script is installed individually, the common file must be installed as well. Instead of installing individual scripts, it's recommended to copy the entire folder to a plugin directory:

- Windows – `%AppData%\SciTools\conf\plugin\User\`
- Mac – `~/Library/Application Support/SciTools/plugin/User/`
- Linux – `~/.config/SciTools/plugin/User/`

# Architectures

Working with multiple call trees means working with multiple root functions. To pass multiple entities to a plugin, the entities must be grouped together in an architecture. A simple architecture can have the format:

- Name of root architecture can be anything
  - Any name containing the word `tasks` (case insensitive)
    - function1()
    - function2()

The shared tasks plugins also support additional fields for the tasks. These fields are displayed on the graph under the task name, and in the table as columns. Currently supported fields are 'core' and 'priority'. A field can be used instead of an architecture name containing the word tasks.

- Name of root architecture can be anything
  - Any name containing the the word `core` or `priority` (case insensitive)
    - Field value like 1
      - function1()
    - Another field value like 2
      - function2()

Finally, the architecture can be used to identify functions that enable and disable interrupts. References protected by these functions (preceeded by a call to the disable function and followed by a call to the enable function) are identified by the plugins. The naming pattern interrupt control is (ommitting the tasks):

- Name of root architecture can be anything
  - name containing the word `interrupt` or the word `control` (case insensitive)
    - any name here. This layer allows multiple pairs of interrupt functions
      - enable
        - enableFunction()
      - disable
        - disableFunction()

There are a lot of options for building an architecture:

- Use the Architecture Designer in the GUI
- Right click on entities and select "Add to Architecture". The architecture name can be typed and does not have to already exist
- Create architecture nodes in the Architecture Browser and drag and drop entities into the nodes
- Run the helper [archBuilderForSharedTasks.py](https://raw.githubusercontent.com/stinb/plugins/main/Solutions/sharedTasks/archBuilderForSharedTasks.py) script in this folder from the command line.

# Exports

The interactive report plugin can be used to generate a CSV file while running the report or from the command line.

```sh
upython sharedTasksCSV.upy -h
```

To have a dynamic file name, you can add {arch}, {date}, or {time} (case-insensitive) to the "CSV file name" option (IReport) or `-csvFileName` argument (command line)
