# Automatic Architectures

Automatic architecture plugins are Python scripts that get run every time you open your Understand project. They allow you to easily create your own architectures that are automatically updated. What can that be used for? Well, anything you want!

Here are a few ideas:
 - Query version control to make an architecture of your most often modified files to identify churn.
 - automatically partition your files based on folder structure or file names
 - make an architecture that quickly separates 3rd party libraries from your teams code

For example, [git_author.upy](https://github.com/stinb/plugins/blob/main/Arch/git_author.upy) queries git to find the last person to edit each file, and makes an Architecture from that. I can that use that architecture to see who owns the file I'm looking at, explore dependecies based on author, or even see who's code is the most complex!
![image](https://user-images.githubusercontent.com/6586272/206825252-1d7c2f5f-038c-4196-874d-e32475e798d0.png)


## Installation
To install an Architecture plugin, simply drag the .upy or .upl file into the Understand GUI, it will ask if you want to install it. Alternatively, you can manually place it in one of the following locations:

- Windows – C:\Program Files\SciTools\conf\plugin\User\Arch
- Mac – /Users/username/Library/Application Support/SciTools/plugin/Arch
 - Linux – /home/username/.config/SciTools/plugin/Arch

To enable your custom architecture after installation, select Architectures->Browse Architectures and in the menu of that dialog select your new architecture
![image](https://user-images.githubusercontent.com/6586272/206825152-d6911452-549b-433c-9b51-7cddcb14680d.png)

Some plugins may import common files. For example, all git_ architectures use git_util.py. To install those, it's best to manually place both files in the correct location so the import is found.

## Performance
Keep in mind that the project won't open completely until these architecture scripts finish running. You will definitely want to make scripts that finish quickly so you don't have long waits opening the project.

Due to the number of git commands being called, it takes several minutes to complete the first run on a large project. During that time, Understand is essentially frozen. So it caches the results of that run to disk and after that only looks up files that have been modified, which only takes a second or two.
