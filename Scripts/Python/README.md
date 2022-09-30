# Getting Started with the Python API

The Python 3 API allows you to write your own scripts that can create custom reports, generate your own metrics and in general make Understand deliver exactly what you want it to.

Understand bundles its own simplified version of Python (upython.exe) so you can easily get started with the API. Simply import understand into your script and run the script with the upython executable:
```
c:\Program Files\SciTools\pc-win64>upython
>>> import understand
>>> understand.version()
1107
>>>
```
## Documentation
The most up-to-date documentation is accessible from the Help menu in Understand. Select Help->Python API Documentation. You can also [access it here](https://documentation.scitools.com/html/python/index.html).

## Sample Scripts
In addition to these scripts, the documentation also includes some samples. More detailed example scripts are shipped with Understand in the SciTools/scripts/python folder.
[Let us know](mailto:support@scitools.com) if you need help, and Happy Scripting!

# Custom Python Installation
If you want to use custom Python libraries you will need to add Understand to your own Python installation following these steps:

1. The Python API requires a 3.x installation of Python. It needs to be the same bitness as Understand, so if you have a 64 bit version of Understand you’ll need a 64 bit version of Python 3.
1. Modify the PYTHONPATH environment variable to include the module location, which is c:\Program Files\SciTools\bin\pc-win64\Python.
1. Edit the PATH environment variable to c:\Program Files\SciTools\bin\pc-win64
1. Linux Only: Set LD_LIBRARY_PATH to scitools/bin/linux64
1. Test the API Setup by running the test script included with Understand:
1. python “c:\Program Files\SciTools\python\api_install_test.py”
1. In your script add import understand
1. Python 3.8 and newer requires specifying the path to the Understand installation inside the script itself using the os.add_dll_directory command, as in the example below:

```
c:\python39>python
>>> import os
>>> os.add_dll_directory("C:\Program Files\Scitools\bin\pc-win64")
>>> import understand
>>> understand.version()
1055
>>>
```

### Cygwin
Unfortunately we are not able to support Cygwin’s python due to the unique way it links everything together from the Linux and Windows worlds.

##Troubleshooting
The official python distribution from python.org includes a python3.dll which implements the stable ABI That’s what allows us to target python 3.2+ with the same module. Essentially it’s just a wrapper that forwards to the python3x.dll. Some other distributions of Python such as Anaconda may not have the python3.dll, you may be able to copy in python3.dll from the official distribution.
