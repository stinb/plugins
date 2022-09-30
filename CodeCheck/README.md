# Overview
CodeCheck scripts are special Perl and Python scripts that let you provide custom checks for verifying your team's coding standards.
They can be used to verify naming guidelines, metric requirements, published best practices, or any other rules or conventions that are important for your team.

These scripts are developed using the Understand Perl and Python API's along with a set of special functions designed to interact with the Understand CodeCheck interface. 

![image](https://user-images.githubusercontent.com/6586272/193142606-7bf859c2-138b-49d5-a0ec-a403a3a6cf7a.png)

# What Type of Checks Can I Write?
With Understand we've taken a different approach to static analysis than a lot of other tools and that is also reflected in CodeCheck. Most other static analysis tools focus on identifying bugs, CodeCheck is also heavily focused on analyzing coding standards. For that reason many of our customers use Understand in addition to other static analysis tools.

Here are some of the things it does well:
* Verify coding style guidelines are met:
* Naming Conventions
* Spacing Conventions
* Required Comment styles
* Specified styles for specific code constructs
* Metric Thresholds
* Line & Statement Counting
* Complexity
* Comment/Code ratio
* Object Coupling
* Readability
* Find required or unnecessary constructs
* Identify required or forbidden calls to libraries or functions

And here are some areas it's not so good at:
* Memory allocation/leaks
* Tracking data/variables between functions
* Anything that requires expression analysis
* Type casting/conversions
* Identifying arithmetic and floating point issues


# Script Locations
CodeCheck has several directories it checks for scripts. The first location gets overwritten with a new install of Understand so don't save any changes to that directory, but it's a good place to look for examples of existing checks.

* C:\Program Files\SciTools\conf\plugin\SciTools\CodeCheck
* C:\Program Files\SciTools\conf\plugin\User\CodeCheck
* C:\Users\USERID\AppData\Roaming\SciTools\plugin\CodeCheck

MacOSX users: ~/Library/Application Support/SciTools/plugin/User/Custom/ 

# Write Your First Custom Check
1. Save the _templatePython.upy file to the Understand installation directory under conf/plugin/User/CodeCheck/myscript.upl and open the file in a text Editor or in Understand

2. Modify the name, checkID unique identifier, description and detailed_description subroutines to reflect our test.

This is what it will look look like in Python:
```
  def name():
      return 'Files start with Letters'

  def id():
      return 'CUST01'

  def description():
      return 'Verify that all files start with a letter.'

  def detailed_description():
      return """\
  Company coding standards specify that 
  file names should start with letters."""
```
3. Now modify the check subroutine to include the regular expression check and to signal a CodeCheck violation reporting the problem. Add this code at the end of the "check" function (make sure you have 'import re' at the top of the file):
```
  if re.search("^[a-zA-Z]", file.name()):
      check.violation(file, file, -1, -1,"File name does not begin with a letter")
```
4. The last step is to verify that the python syntax is correct. The easiest way to do this is to open a command line and run the appropriate python application that ships with Understand: 
```
 upython -c "path/to/mysample.upy"
```
If the CodeCheck tab is open in Understand close it and reopen it, and your new check should appear and be ready to test.
<<<<<<< HEAD

Congratulations! You wrote your first Codecheck script. The next step is to become more familiar with Understand's Perl and Python API's.

=======


Congratulations! You wrote your first Codecheck script. The next step is to become more familiar with Understand's Perl and Python API's.

>>>>>>> main
Browsing the Codecheck scripts that are shipped with Understand can also be very beneficial. They can be found in conf/plugin/Scitools/Codecheck/.

If you have questions, just shoot us an email at support@scitools.com or visit https://support.scitools.com/.

Good Luck!
