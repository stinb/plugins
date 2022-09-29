Writing CodeCheck Scripts
Copy the file codecheck_template.upl.REMOVE to codecheck_template.upl and modify using the Understand Perl API: https://scitools.com/documents/manuals/perl/

Overview
CodeCheck scripts are special Perl and Python scripts that let you provide custom checks for verifying your team’s coding standards.
They can be used to verify naming guidelines, metric requirements, published best practices, or any other rules or conventions that are important for your team.

These scripts are developed using the Understand Perl and Python API's along with a set of special functions designed to interact with the Understand CodeCheck interface.

What Type of Checks Can I Write?
With Understand we’ve taken a different approach to static analysis than a lot of other tools and that is also reflected in CodeCheck. Most other static analysis tools focus on identifying bugs, CodeCheck is more focused on analyzing coding standards. For that reason many of our customers use Understand in addition to other static analysis tools.

Here are some of the things it does well:
Verify coding style guidelines are met:
Naming Conventions
Spacing Conventions
Required Comment styles
Specified styles for specific code constructs
Metric Thresholds
Line & Statement Counting
Complexity
Comment/Code ratio
Object Coupling
Readability
Find required or unnecessary constructs
Identify required or forbidden calls to libraries or functions

And here are some areas it’s not so good at:
Memory allocation/leaks
Tracking data/variables between functions
Anything that requires expression analysis
Type casting/conversions
Identifying arithmetic and floating point issues


Script Locations
Codecheck has several directories it checks for scripts. The first location gets overwritten with a new install of Understand so don’t save any changes to that directory, but it’s a good place to look for examples of existing checks.
C:\Program Files\SciTools\conf\plugin\SciTools\Codecheck
C:\Program Files\SciTools\conf\plugin\User\CodeCheck
C:\Users\USERID\AppData\Roaming\SciTools\plugin\Codecheck

MacOSX users: ~/Library/Application Support/SciTools/plugin/User/Custom/   Note that the /plugin/User/Custom/ directories will need to be created.


Write Your First Custom Check
The first part of writing your own Codecheck script is becoming familiar with the API by going through the API Tutorials. Next, to write your own check, start with this template file
codecheck_template.txt,  

1. Save the template file to the Understand installation directory under conf/plugin/User/codecheck/myscript.upl and open the file in a text Editor or in Understand

2. Modify the name, checkID unique identifier, description and detailed_description subroutines to reflect our test.

This is what it will look like in Perl:

  sub name { return "Files start with Letters";}
  sub checkID { return "CUST01";}
  sub description { return "Verify that all files start with a letter.";}

  sub detailed_description { return "Company coding standards specify that ". 
                                  "file names should start with letters.";}

And this is what it will look like in Python:

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

3. Now modify the check subroutine to include the regular expression check and to signal a codecheck violation reporting the problem. Add this code at the end of the subroutine for a Perl script:</>

  if ($file->name =~ /^[^a-zA-Z]/){
    $check->violation(0,$file,-1,-1,"File name does not begin with a letter");  
  }

or this code for a Python script (make sure you have ‘import re’ at the top of the file):

  if re.search("^[a-zA-Z]", file.name()):
      check.violation(file, file, -1, -1,"File name does not begin with a letter")

4. The last step is to verify that the perl/python syntax is correct. The easist way to do this is to open a command line and run the appropriate perl or python application that ships with Understand: 

a) uperl -c "path/to/mysample.upl"

or

b) upython -c "path/to/mysample.upy"

If the CodeCheck tab is open in Understand close it and reopen it, and your new check should appear and be ready to test.

Congratulations! You wrote your first Codecheck script. The next step is to become more familiar with Understand’s Perl and Python API's.
Browsing the Codecheck scripts that are shipped with Understand can also be very beneficial. They can be found in conf/plugin/Scitools/Codecheck/.

If you have questions, just shoot us an email at support@scitools.com or visit https://support.scitools.com/.

Good Luck!