Creating Your Own Interactive Reports
Copy the file ireport_template.upl.REMOVE to ireport_template.upl and modify as desired using the Understand Perl API: https://scitools.com/documents/manuals/perl/

Overview
Interactive Reports are special Perl scripts that create a text-based window in Understand to display information of your choosing. They can reference the entire Project, a given Architecture, or a specific entity, and they can be as simple or complex as your imagination. There is an IReport that ships with Understand that shows all the API information available for the selected entity. To see it yourself just right click on an entity and select Interactive Reports->API Info
A tutorial for creating your own Interactive Reports is here: https://scitools.com/support/api-tutorial-5-interactive-reports/

To install a plugin, simply drag the .upl file into the Understand GUI, or you can manually place it in one of the following locations:

Windows – e.g. C:\Program Files\SciTools\conf\plugin\User\IReport
Mac – e.g. /Users/username/Library/Application Support/SciTools/plugin/IReport
Linux – e.g. /home/username/.config/SciTools/plugin/IReport