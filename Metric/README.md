# Metrics

Metric plugins provide custom metrics to Understand. Once you've installed your metric plugin, you can use your custom metric anywhere metrics are available in Understand. For example, you can:
 - View values in the Metrics Browser
 - Plot them in a Metrics Treemap
 - Use them for a custom color scale on a graph

Check out some of the sample plugins:
 - Find code churn by reporting the number of commits and number of unique authors for your files and folders. [git.upy](https://github.com/stinb/plugins/blob/Metric/git.upy)
 - Include test coverage information from lcov info files. [coverage.upy](https://github.com/stinb/plugins/blob/6.4/Metric/coverage.upy)
 - Calculate halstead metrics. [halstead.upy](https://github.com/stinb/plugins/blob/6.4/Metric/halstead.upy)
 
Some of the metric ids changed with Understand 6.4. If your scripts rely on the old metric ids, no problem. Just install [compatability.upy](https://github.com/stinb/plugins/blob/6.4/Metric/compatability6-3.upy). That script is also a great starting point as a template for your own plugins.

Metrics Plugins are only available through the Python API.


## Installation
To install a Metric plugin, simply drag the .upy file into the Understand GUI, it will ask if you want to install it. Alternatively, you can manually place it in one of the following locations:

- Windows – C:\Program Files\SciTools\conf\plugin\User\Metric
- Mac – /Users/username/Library/Application Support/SciTools/plugin/Metric
- Linux – /home/username/.config/SciTools/plugin/Metric

[This article](https://blog.scitools.com/wp-admin/post.php?post=2470&action=edit) about the Coverage plugin provides a walkthough on installing and using a metrics plugin.

## Performance 
The Understand GUI has been updated to calculate metrics on a background thread. So long running metrics like the git churn metrics won't slow down the UI. But they can still take a long time to generate so be careful using them. Note that the Metrics Browser calculates all metrics for the selected entity or architecture before displaying any of them so installing long running metrics can increase the wait time.
