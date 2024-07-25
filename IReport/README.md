# Overview
Interactive Reports are plugins that create a text-based window in Understand to display information of your choosing. They can reference the entire Project, a given Architecture, or a specific entity, and they can be as simple or complex as your imagination. There is an IReport that ships with Understand that shows all the API information available for the selected entity. To see it yourself just right click on an entity and select Interactive Reports->API Info  

# Installing an Interactive Report
To install a Report plugin, simply drag the .upy or .upl file into the Understand GUI, it will ask if you want to install it. Alternatively, you can manually place it in one of the following locations:
* Windows – C:\Program Files\SciTools\conf\plugin\User\IReport
* Mac – /Users/username/Library/Application Support/SciTools/plugin/IReport
* Linux – /home/username/.config/SciTools/plugin/IReport

After installation, project level reports can be run by selecting Project->Interactive Reports -> [Plugin Name]. Entity Specific reports can be run by selecting the entity and then Interactive Reports -> [Plugin Name].

# Making Your Own Custom Reports
You can customize any of these reports, or make your own from scratch. We recommend starting with our [sample IReport plugin](https://documentation.scitools.com/html/python/ireport.html), included for that purpose. The full Interactive Report API documentation is accessible in Understand by selecting Help->Python API Documentation and navigating to the IReports section. There are also several tutorials on building graphs on our [support site](https://support.sciools.com).

# Popular Interactive Reports
Here are some of the more popular reports you'll find here.
* [astyle.upl](https://github.com/stinb/plugins/blob/main/IReport/astyle.upl): Works with the open source project [astyle](https://astyle.sourceforge.net/) to reformat and beautify your code.
* [batch_graphs.upl](https://github.com/stinb/plugins/blob/main/IReport/batch_graphs.upl): Generates all the graphs of the selected type and saves them to the specified folder; also creates an index.html file to view them all with.
* [change_impact.upl](https://github.com/stinb/plugins/blob/main/IReport/change_impact.upl): See what impact changing a particular entity will have on the codebase.
* [cloned_functions.upl](https://github.com/stinb/plugins/blob/main/IReport/cloned_functions.upl): Identify functions that are similar or identical.
* [delta.upl](https://github.com/stinb/plugins/blob/main/IReport/delta.upl): Identify various difference between two versions of a project.
* [duplicate_code.upl](https://github.com/stinb/plugins/blob/main/IReport/duplicate_code.upl): Find lines of code that have been duplicated across your code base.
* [highcomplexity.upl](https://github.com/stinb/plugins/blob/main/IReport/highcomplexity.upl): Gives a list of the most complex functions in your project.
* [macro.upy](https://github.com/stinb/plugins/blob/main/IReport/macro.upy): Show all macros used within a file an the definition of known.
* [overrides.upy](https://github.com/stinb/plugins/blob/main/IReport/overrides.upy): Show all overrides for the specified file.
* [var_init.upl](https://github.com/stinb/plugins/blob/main/IReport/var_init.upl): Reports where variables in a function are initialized.
