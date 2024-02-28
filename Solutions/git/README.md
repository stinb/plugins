This folder contains plugin scripts for Git. All plugin scripts use the common [git_util.py](https://github.com/stinb/plugins/blob/main/Solutions/git/git_util.py) so if a script is installed individually, the common file must be installed as well. Instead of installing individual scripts, it's recommended to copy the entire folder to the plugin directory:

- Windows – C:\Program Files\SciTools\conf\plugin\User\
- Mac – /Users/username/Library/Application Support/SciTools/plugin/
- Linux – /home/username/.config/SciTools/plugin/

# Authors

The architecture plugins are:

- the last author to touch a file [arch_last_author.upy](https://github.com/stinb/plugins/blob/main/Solutions/git/arch_last_author.upy)
- the first author to touch a file [arch_creator.upy](https://github.com/stinb/plugins/blob/main/Solutions/git/arch_creator.upy)
- the author who made the most commits to the file [arch_owner.upy](https://github.com/stinb/plugins/blob/main/Solutions/git/arch_owner.upy) [1]

The metric plugin [metric_authors.upy](https://github.com/stinb/plugins/blob/main/Solutions/git/metric_authors.upy) works for file entities, architectures (with file entities) and the whole project. It defines the following metrics:

- **Commits** The total number of commits [2]
- **Authors** The number of unique authors [2]
- **Ownership** The percentage of commits by the author with the most commits [1,3]
- **Minor Contributors** The number of authors who made less than 5% of the commits [1]
- **Major Contributors** The number of authors who made at least 5% of the commits [1]

For a detailed view, the ireport plugin [ireport_authors.upy](https://github.com/stinb/plugins/blob/main/Solutions/git/ireport_authors.upy) shows the owner(s), other major contributor(s), and the minor contributor(s) with the number and percentage of commits.

# Dates

Calendar architectures can be absolute (Year/Month) or relative (Today, This Week, and so on). There are four calendar architecture plugins:

- Absolute from the last modified date [arch_modified.upy](https://github.com/stinb/plugins/blob/main/Solutions/git/arch_modified.upy)
- Relative from the last modified date [arch_modified_rel.upy](https://github.com/stinb/plugins/blob/main/Solutions/git/arch_modified_rel.upy)
- Absolute from the earliest date [arch_created.upy](https://github.com/stinb/plugins/blob/main/Solutions/git/arch_created.upy)
- Relative from the earliest date [arch_created_rel.upy](https://github.com/stinb/plugins/blob/main/Solutions/git/arch_created_rel.upy)

There is also an architecture plugin based on activity: [arch_stability.upy](https://github.com/stinb/plugins/blob/main/Solutions/git/arch_stability.upy). Files that have been modified at least twice in the past month (=past 30 days) are "Active". If an active file was also active in the previous month, then it is "Reccurently Active". All other files are "Stable". [3]

The metric plugin [metric_dates.upy](https://github.com/stinb/plugins/blob/main/Solutions/git/metric_dates.upy) works for file entities, architectures (with file entities) and the whole project. It defines the following metrics:

- Days since creation
- Days since last modified

# Miscellaneous

## Cohesion

The metric plugin [metric_cohesion.upy](https://github.com/stinb/plugins/blob/main/Solutions/git/metric_cohesion.upy) calculates the percentage of cohesive commits [3] for architectures. A cohesive commit is a commit whose changed files are all in the same architecture. A non-cohesive commit changes files across architectures. The value for a root architecture considers each sub-architecture as a separate group. The value for a non-root architecture forms a group with all descendents and everything else is a different group. So, for this sample architecture:

- Directory Structure
  - Parent Folder
    - Child File
    - Child Folder
      - Grandchild File

The cohesion for "Directory Structure" considers two groups ("Child File") and ("Grandchild File") and a cohesive commit only touches a single group. The cohesion for "Parent Folder" has one group ("Child File", "Grandchild File") and a cohesive commit touches only files within the group.

## Coupling

Code as a Crime Scene [2] mentions coupling. The idea is that if two files appear in the same commit, then they are coupled. The percentage of commits they appear in together determines the coupling value. The interactive report [ireport_coupling.upy](https://github.com/stinb/plugins/blob/main/Solutions/git/ireport_coupling.upy) works on file entities and lists the files the target file is coupled to. The following summary metric values are provided by [metric_coupling.upy](https://github.com/stinb/plugins/blob/main/Solutions/git/metric_coupling.upy):

- **Git Max Coupling** The highest coupling value
- **Git Average Coupling** The average coupling value
- **Git Coupled Files** The total number of coupled files
- **Git Strongly Coupled Files** The number of coupled files whose coupling value is >= 50%

Coupling can also be viewed graphically with [graph_coupling.upy](https://github.com/stinb/plugins/blob/main/Solutions/git/graph_coupling.upy). However, the layout algorithm used by the graph plugin requires build 1164 or later. The graph includes a coupling value cutoff option because too many edges can freeze the user interface on render. Note that unlike most other plugins, graph plugins execute on the main thread so this graph can potentially freeze the user interface while calculating. The graph plugin is based on the Flat Dependencies Graph style, with edges colored by the source node and sized by the number of commits. For OpenSSL, the graph looks like:

<img width="769" alt="image" src="https://github.com/stinb/plugins/assets/7937320/8b639d94-7b38-48e6-971a-3a590df054d3">


# References

1. [“Don’t Touch My Code! Examining the Effects of Ownership on Software Quality”](https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/bird2011dtm.pdf)
2. ["Code as a Crime Scene"](https://www.adamtornhill.com/articles/crimescene/codeascrimescene.htm)
3. ["3 Technical Debt Metrics Every Engineer Should Know"](https://stepsize.com/blog/use-research-from-industry-leaders-to-measure-technical-debt)
