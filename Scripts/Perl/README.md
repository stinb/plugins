# Getting Started with the Perl API
Understand includes a PERL API which allows you to directly query the Understand database. If you want to create your own report, or gather information in a manner that we didn’t foresee in Understand, you can access the information yourself via the API. The API can be accessed in several different ways: most of the scripts are designed to be run from Understand’s GUI on the currently opened database. They can also be run from the command line, and some plugins are designed to interact directly with Understand.

You can access the latest version of the Perl API documentation from in Understand. Select Help -> Perl API Documentation

## Run Scripts in Understand
To run a script from inside of Understand, use Tools->Run a Command, configure any necessary parameters, and hit run. The resulting output can then be double-clicked to visit the referenced source.
![image](https://user-images.githubusercontent.com/6586272/193364777-98a6a56f-1182-4645-9dfc-7869d58d7b89.png)

## Run Scripts from the Command Line

Scripts can be run from the command line using uperl, a special PERL executable included in Understand’s bin directory. To run a script from the command line, specify the script, the database and any options:
```
uperl scriptName.pl -db myproject.udb [Options]
```
So to run this same script from the command line, you could use the following command:
![image](https://user-images.githubusercontent.com/6586272/193364835-b82df100-cd70-436c-9605-dd21f14e449a.png)

## Using your own Perl Installation

If you would rather access the Understand API from your own customized Perl 5.34.0 installation, just add /bin/pc-win32/Perl/STI to your Perl include path, on OSX the default path to uperl is /Applications/Understand.app/Contents/MacOS/uperl. On non-Windows systems, you will also need to create an STIHOME environment variable that points to the directory. It may also be necessary to set the LD_LIBRARY_PATH environment variable to /bin/linux64,. Keep in mind if using your own installation, that it will need to have thread support since our Understand Perl module is built with thread support. For example, when using Perlbrew, install it with this command: perlbrew install perl-5.34.0 -Duseithreads to build it with thread support, making it compatible with our perl module.
