This script exports dependency and metric data from Understand for use in the Open Source tool Gephi. It was written to go along [with this article](https://blog.scitools.com/create-beautiful-overview-graphs-of-your-code/).

![image](.doc/gephi.png)

To run the script, use our custom python, upython, that ships with Understand. The command from your terminal should look like this: 

`upython gephi_dependency_export.py src/gitahead/gitahead.und`

It will create two new files - nodes.csv and edges.csv chock full of information about your project that you can import into Gephi.