# Automatic Architectures

Automatic architecture plugins are Python scripts that get run every time you open your Understand project. They allow you to easily create your own architectures that are automatically updated. What can that be used for? Well, anything you want!

Here are a few ideas:
 - Query version control to make an architecture of your most often modified files to identify churn.
 - automatically partition your files based on folder structure or file names
 - make an architecture that quickly separates 3rd party libraries from your teams code


## Installation
To install a Report plugin, simply drag the .upy or .upl file into the Understand GUI, it will ask if you want to install it. Alternatively, you can manually place it in one of the following locations:

- Windows – C:\Program Files\SciTools\conf\plugin\User\IReport
- Mac – /Users/username/Library/Application Support/SciTools/plugin/IReport
 - Linux – /home/username/.config/SciTools/plugin/IReport

To enable your custom architecture after installation, select Architectures->Browse Architectures and in the menu of that dialog select your new architecture

## Performance 
Keep in mind that the project won't open completely until these architecture script finish running. You will definitely want to make scripts that finish quickly so you don't have long waits opening the project.

For example, git_author.upy queries git to find the last person to edit each file, and makes an Architecture from that. Due to the number of git commands being called, it takes several minutes to complete the first run on a large project. During that time, Understand is essentially frozen. So it caches the results of that run to disk and after that only looks up files that have been modified, which only takes a second or two.