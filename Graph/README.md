# Overview
The ability to draw custom graphs is a core feature of the Understand PERL API. Using the Understand Plugin system, advanced users can develop their own custom graphs addressing specific information needs.

# Installing  Graph Plugins
To install a graph plugin, simply drag the .upy or .upl file into the Understand GUI, it will ask if you want to install it. Alternatively, you can manually place it in one of the following locations:
* Windows – C:\Program Files\SciTools\conf\plugin\User\Graph
* Mac – /Users/username/Library/Application Support/SciTools/plugin/Graph
* Linux – /home/username/.config/SciTools/plugin/Graph

After installation, project level graphs can be run by selecting Graphs->Project Graphs -> [Plugin Name]. Entity Specific graphs can be run by selecting the entity and then Graphs -> Graphs for [Entity Name] -> [Plugin Name].

# Making Your Own Custom Graphs
You can customize any of these graphs, or make your own from scratch. We recommend starting with our [graph plugin example](https://documentation.scitools.com/html/python/graph.html), designed for that purpose. The full Graph API documentation is accessible in Understand by selecting Help->Python API Documentation and navigating to the Graphs section. There are also several tutorials on building graphs on our [support site](https://support.sciools.com).

# Popular Graphs
Here are some of the more popular graphs you'll find here.
* [invocation.upl](https://github.com/stinb/plugins/blob/main/Graph/invocation.upl): This graph creates the call trees for all functions/methods in the project. It is very large, looks like spaghetti, and can be very slow. Use at your own risk.
* [flowchart.upl](https://github.com/stinb/plugins/blob/main/Graph/flowchart.upl): This is a version of the Function/Method Control flow graph that uses the Perl API
* [assignments_and_assigned_by.upl](https://github.com/stinb/plugins/blob/main/Graph/assignments_and_assigned_by.upl): This graph shows where each object is assigned, and where it assigns others.

![image](https://user-images.githubusercontent.com/6586272/193376858-3e304851-dcb3-4280-9d03-e9ba75185bde.png)

# Bonus
Check out the [Gephi Extract](https://blog.scitools.com/create-beautiful-overview-graphs-of-your-code/) for an an amazing overview graph option.

![image](https://blog.scitools.com/wp-content/uploads/2023/08/gitahead-overview_labeled.png)