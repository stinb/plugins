# Metrics

Understand comes with a lot of built-in metrics, like the number of code lines or the McCabe cyclomatic complexity of a function. With the Python API metrics plugins you can add your own metrics as well! 
Not only can you view and export these values, but you can also use them in other Understand views. For example, you can shade the nodes in a call tree by cyclomatic complexity, find outliers with treemaps, 
or sort by metrics in the locator.  
![image](.doc/metrics.png)  
*A Metric’s Treemap for OpenSSL with size by lines of code and coloring by maximum cyclomatic complexity. A Call Tree with nodes colored by cyclomatic complexity. The Entity Locator showing the Comment to Code Ratio.*

# Sample Plugins
 - Find Git related metrics such as the code churn from the number of commits and number of unique authors for your files and folders. [Git Plugins](https://github.com/stinb/plugins/blob/main/Solutions/git/README.md)
 - Include test coverage information from lcov info files using the Coverage Metrics.
 - Calculate Halstead Maintainability metrics.

Some of the metric ids changed with Understand 6.4. If your scripts rely on the old metric ids, no problem. Just rename compatability6-3.upy.rename to make all the legacy metric ids available in your project. 
That script is also a great starting point as a template for your own plugins.

## Performance
The Understand GUI has been updated to calculate metrics on a background thread. So long running metrics like the git churn metrics won't slow down the UI. But they can still take a long time to generate so be careful using them.
Note that the Metrics Browser calculates all metrics for the selected entity or architecture before displaying any of them so installing long running metrics can increase the wait time.
