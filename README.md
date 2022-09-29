# Understand Plugins

[Understand](www.scitools.com) is a powerful static analysis tool designed for code maintenance and exploration. It has fully extensible plugin systems for Python and Perl. Understand supports 4 flavors of plugins: Graphs, Interactive Reports, CodeChecks and Scripts.

Checkout the [API tutorials](https://scitools.freshdesk.com/en/support/solutions/articles/70000582855) for more help writing your own plugins.
The most up-to-date documentation is accessible from the Help menu in Understand. Select Help->Python API Documentation. You can also [access it here](https://support.scitools.com/en/support/solutions/articles/70000582856).

## Graph Plugins
With a graph plugin you can leverage the information in the Understand database to create graphs of what is important to you. For example this plugin creates a call tree graph that changes the color based on the complexity of the function. Graphs can be accessed directly in Understand or exported en masse via a script plugin. Check out the README file in the Graph folder for more details.  
![image](https://user-images.githubusercontent.com/6586272/193095922-cea89e3d-813d-4492-b90a-ef3a42210105.png)

## Interactive Reports
Interactive report plugins, or IReports, let you run queries or gather information on a specificed entity in Understand, from Architectures, or from the project as a whole. For example, the below Change Impact IReport let's you project how much impact changing a function will have, I can see that modifing this function will affect more than 40,000 lines of code.  
![image](https://user-images.githubusercontent.com/6586272/193100232-d82e65a9-dea5-48c1-94fe-0596e66442de.png)

## CodeCheck Plugins
The CodeCheck feature in Understand has hundreds of checks yous can run against your code to validate coding standards and look for issues and vunerabilities. The Python API makes it easy to write your own checks as well. 

