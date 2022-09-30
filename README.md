# Understand Plugins

[Understand](www.scitools.com) is a powerful static analysis tool designed for code maintenance and exploration. It creates an extensive database about your code and how everything is interconnected. The Understand IDE shows you a lot of that information directly, but it also has fully extensible plugin systems for Python and Perl to let you gather and display your teams information in the most meaningful way. Understand supports 4 flavors of plugins: Graphs, Interactive Reports, CodeChecks and Scripts.

Checkout the [API tutorials](https://scitools.freshdesk.com/en/support/solutions/articles/70000582855) for more help writing your own plugins.
The most up-to-date documentation is accessible from the Help menu in Understand. Select Help->Python API Documentation. You can also [access it here](https://documentation.scitools.com/html/python/index.html).

## Graph Plugins
With a graph plugin you can leverage the information in the Understand database to create graphs of what is important to you. For example this plugin creates a call tree graph that changes the color based on the complexity of the function. Graphs can be accessed directly in Understand or exported en masse via a script plugin. Check out the [README file](https://github.com/stinb/plugins/blob/main/Graph/README.md) in the Graph folder for more details.  
![image](https://user-images.githubusercontent.com/6586272/193095922-cea89e3d-813d-4492-b90a-ef3a42210105.png)

## Interactive Reports
Interactive report plugins, or IReports, let you run queries or gather information on a specified entity in Understand, from Architectures, or from the project as a whole. For example, the below Change Impact IReport let's you project how much impact changing a function will have, I can see that modifying this function will affect more than 40,000 lines of code.  
![image](https://user-images.githubusercontent.com/6586272/193100232-d82e65a9-dea5-48c1-94fe-0596e66442de.png)

## CodeCheck Plugins
The CodeCheck feature in Understand has hundreds of checks yous can run against your code to validate coding standards and look for issues and vulnerabilities. The Python API makes it easy to [write your own checks](https://github.com/stinb/plugins/blob/main/CodeCheck/README.md) as well, like this one we use to enforce our internal parenthesis standard.  
![image](https://user-images.githubusercontent.com/6586272/193142606-7bf859c2-138b-49d5-a0ec-a403a3a6cf7a.png)

## Scripts
You can also query the Understand DB directly with Python and Perl API scripts. You can run these scripts from inside the GUI or from the command line, which lends itself well to batch processes or DevOps integration. We use these scripts heavily as part of our Jenkins CI Pipeline.
In this trivial example I list all of the enums in my project from the command line.
![image](https://user-images.githubusercontent.com/6586272/193142975-512082f8-b9c6-4fc1-b077-b5c5f450f00c.png)
