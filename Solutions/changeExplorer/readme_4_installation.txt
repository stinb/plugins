Instructions:

Copy the files to an installation folder of your choice. 
You must not delete this folder after you complete the installation, as the graphs and checkers file will refer to and use it.

Execute install.bat on windows or install on Linux.

The installation will:

-	Mark the installation folder as the location where you installed the “python” dependencies packages.
      (example: comparison.py, pather.py, architectureparser.py).
-	Copy the custom graphs’ python implementation to SciTools’ custom Graph folder.
-	Copy the custom checkers’ python implementation to SciTools’ custom Codecheck folder.

To validate your installation:

-	Open Understand
-	Open any project
-	Make sure the top-bar menu has a Graphs >> Project Graphs >> CRITICAL PATHS Graph
-	Make sure any entity has a context menu Graphical Views >> COMPARISON Graph 
-	Make sure any function has a context menu Graphical Views >> CRITICAL PATHS Graph
-	Make sure Codecheck now shows a CRITICAL_PATH_CMP – Critical Path Comparison
