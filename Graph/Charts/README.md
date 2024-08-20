By default, graph layout is done with [Graphviz](https://graphviz.org)'s [Dot](https://graphviz.org/docs/layouts/dot/) algorithm. However, this can be changed by setting the [layout](https://graphviz.org/docs/attrs/layout/) attribute on the root graph object:

```Python
def draw(graph, target):
  graph.set("layout","sfdp")
```

Note that support for Graphviz "sfdp" layout is only available in builds 1165 or later. 

In builds 1188 and later, the "layout" attribute can be used to access Understand's layout algorithms. Supported chart layout algorithms are documented below.

# Sunburst

The sunburst algorithm is used for pie charts, donut charts, half donut charts, and sunburst charts. It does not support node or cluster labels. It uses the "area" attribute on nodes to determine the weight of each segment. 

## Pie Chart
For a simple pie chart:

```Python
def draw(graph, target):
  graph.set("layout","sunburst")
  data = [(100,"blue"),(200,"green"),(400,"red")]
  for area, color in data:
    n = graph.node()
    n.set("area", str(area))
    n.set("color", color)
    n.set("fillcolor", color)
```

<img width="171" alt="image" src="https://github.com/user-attachments/assets/8216b431-58d4-4cd7-99d9-b8e5306e2ea9">

## Donut Chart
To change this chart into a donut chart, use the "r1" attribute. The "r0" and "r1" attributes are the inner and outer radius of the graph object. Setting "r1" on the root graph determines where the root ends and the child nodes begin.

```Python
def draw(graph, target):
  graph.set("layout","sunburst")
  graph.set("r1","30")
  data = [(100,"blue"),(200,"green"),(400,"red")]
  for area, color in data:
    n = graph.node()
    n.set("area", str(area))
    n.set("color", color)
    n.set("fillcolor", color)
```

<img width="228" alt="image" src="https://github.com/user-attachments/assets/622cec85-ad63-4139-b615-cb0ee19c412e">

## Half Donut
To change this chart into a half donut chart, use the "sweep" attribute. This is the angle, in degrees, covered by the chart. By default, this is 360, the full circle. To make it a half circle, pass in 180 degrees:

```Python
def draw(graph, db):
  graph.set("layout","sunburst")
  graph.set("r1","30")
  graph.set("sweep","180")
  data = [(100,"blue"),(200,"green"),(400,"red")]
  for area, color in data:
    n = graph.node()
    n.set("area", str(area))
    n.set("color", color)
    n.set("fillcolor", color)
```

<img width="231" alt="image" src="https://github.com/user-attachments/assets/cb9d9896-c99a-43ae-bc23-834328b9b739">

## Sunburst
A sunburst chart occurs when there are clusters. Each nesting level is a layer. The width of each layer is determined by "rstep" the radius step/change. In a sunburst chart, only nodes are expected to have the "area" attribute. The area of a cluster will be calculated based on the nodes inside it. 

The complete draw function for an architecture sunburst chart by lines of code is:

```Python
colors = [ "#4e8cb9", "#569f38", "#e45721", "#90c4e4","#a1d870","#fcad56"]
def draw(graph, arch):
  graph.set("layout","sunburst")
  graph.set("r1","36")
  graph.set("rstep", "18")

  cIdx = 0
  clusters = dict()
  archList = [arch]
  while archList:
    a = archList.pop(0)

    # create a cluster for this architecture
    par = clusters.get(a.parent(), graph)
    cluster = par.cluster(a.name(),a)
    clusters[a] = cluster

    # Random color
    cluster.set("color", colors[cIdx % 6])
    cluster.set("fillcolor", colors[cIdx % 6])
    cIdx = cIdx + 1

    # Add children architectures
    archList.extend(a.children())

    # Add children entities
    for ent in a.ents(False):
      n = cluster.node(ent.name(),ent)
      n.set("area", str(ent.metric("CountLineCode")))
      n.set("color", colors[cIdx % 6])
      n.set("fillcolor", colors[cIdx % 6])
      cIdx = cIdx + 1
```

And, the result for a custom GitAhead's Directory Structure architecture is:
<img width="416" alt="image" src="https://github.com/user-attachments/assets/a30c248c-8409-44e3-b5f0-625854ab70d8">

# Treemap
The treemap algorithm is essentially equivalent to Graphviz's [patchwork](https://graphviz.org/docs/layouts/patchwork/) algorithm, but gives space to clusters. Unlike the sunburst chart, the treemap chart supports node and cluster labels. However, the label is only shown if it fits. Treemaps require the "area" attribute to be set on all nodes. The desired diagram size is set with "width" and "height" which default to 640 and 480 respectively. 

Changing the sunburst code above to set "layout" to "treemap" gives this result for the same architecture.

<img width="651" alt="image" src="https://github.com/user-attachments/assets/03c2f402-542c-4674-8fb4-ae22050e3192">

The treemap layout supports the same [gradient color format](https://graphviz.org/docs/attr-types/colorList/) supported by Graphviz. Note that gradient color output is not supported by Understand's Visio exports or printing.

# Flat Bubble
The flat bubble chart layout is adapted from [D3's pack algorithm](https://github.com/d3/d3-hierarchy/blob/main/src/pack/siblings.js) and the [packCircles](https://github.com/pmenzel/packCircles) project. Like sunburst charts and treemaps, it requires nodes to have an "area" attribute and supports clusters. However, cluster labels are not supported well (may end up behind nodes). Unlike the treemap, the overall size of the graph will be scaled up to fit all labels. A "width" and "height" attribute can be given, but they only set the minimum size of the chart. The chart will be bigger if needed to fit the labels. 

```Python
def draw(graph, db):
  graph.set("layout","flatbubble")
  data = [(100,"blue"),(200,"green"),(400,"red")]
  for area, color in data:
    n = graph.node(color)
    n.set("area", str(area))
    n.set("color", color)
    n.set("fillcolor", color)
```

<img width="96" alt="image" src="https://github.com/user-attachments/assets/096facf5-18dd-421f-adf2-5df4043a4605">

# Legends
The "sunburst", "flatbubble" and "bar" (below) layouts support legends that are drawn as part of the graph. This is different then the legend Understand displays in the GUI which floats above the graph. Legends are created by creating a top level cluster with the "legend" attribute set to one of the following positions "left", "right", "top", or "bottom". Each node in the legend cluster is drawn with the color as a rectangle and the label to the right of it, in rows and columns determined by the location and chart size. 

```Python
def draw(graph, arch):
  graph.set("layout","sunburst")
  data = [(100,"blue"),(200,"green"),(400,"red")]
  legendCluster = graph.cluster()
  legendCluster.set("legend", "left")
  for area, color in data:
    n = graph.node(color)
    n.set("area", str(area))
    n.set("color", color)
    n.set("fillcolor", color)

    n2 = legendCluster.node(color)
    n2.set("color", color)
    n2.set("fillcolor", color)
```

<img width="254" alt="image" src="https://github.com/user-attachments/assets/bb2a027d-aa69-4d90-9268-6bdaee612094">


# Beta Layouts
There are two additional chart layouts, but they're not as intuitive as the layouts above. Because Understand communicates with layout algorithms through the Graphviz [dot file format](https://graphviz.org/doc/info/lang.html), all graphs have to be composed of nodes, edges, and clusters. That makes it hard to represent table-like data, as the two layouts below attempt to do. 

## Bar Charts
The "bar" layout creates a barchart. It uses the "area" attribute like the other charts, and supports the Graphviz "rankdir" attribute to determine the direction of the chart. For a single dimension of data, the bar chart is interchangeable with the flat bubble, pie, and treemap charts. It supports the Graphviz "rankdir" attribute to determine the direction of the chart.

```Python
def draw(graph, db):
  graph.set("layout","bar")
  data = [(100,"blue"),(200,"green"),(400,"red")]
  for area, color in data:
    n = graph.node(color)
    n.set("area", str(area))
    n.set("color", color)
    n.set("fillcolor", color)
```

<img width="130" alt="image" src="https://github.com/user-attachments/assets/1747c847-70b8-432d-97e2-a22d99fb0718">

A stacked bar chart is possible using clusters. Each cluster is a bar, with the cluster label appearing as the axis label for the bar. Each node is stacked in that clusters bar. Check out [bar_demo.upy](https://github.com/stinb/plugins/7.0/Graph/Charts/bar_demo.upy) for an example. Note that the scale is always linear, so the bar_demo.upy plugin suffers from huge chart areas when there's a mix of small and large values.

## Matrix layout
The "matrix" layout is intended to display a graph as a matrix. So, nodes are displayed as the axis and edges are squares inside the matrix. Clusters are supported and form frames around the edge squares. The x and y axis can be different by setting an "axis" attribute on the cluster equal to x or y.

Limitations:
- Because this graph is using dot language, it tends to take a lot more time and memory then creating the image directly would.
- Because there has to be room to draw cluster frames, there is space between edge squares that tends to make the diagram look too big and space filling
- Because of the way edges are rendered in the Understand GUI, edges will only be clickable and have tooltips if the corresponding nodes are drawn, which means nodes have to show labels. The node labels set the size of the matrix grid, which can make the graph large.

Example Dot file and output
```
digraph "" {
  graph [rankdir="LR", label="", fontname=".AppleSystemUIFont", bgcolor="none", fontsize="13", fontcolor="#000000", style="rounded", ranksep="1.0", compound="true"];
  node [label="", fontname=".AppleSystemUIFont", height="0.0", color="#90c4e4", fixedsize="false", fontsize="13", fontcolor="#000000", style="filled", shape="box", width="0.0", fillcolor="#ffffff"];
  edge [color="#4e8cb9", style="solid"];

  subgraph "cluster_1" {
    graph [label="MISRA", axis ="x"];
    __N2 [ label="1" ];
    __N3 [ label="2" ];
    __N4 [ label="3" ];
    __N5 [ label="4" ];
    __N6 [ label="5" ];
  }
  subgraph "cluster_7" {
    graph [label="AUTOSARR", axis="x"];
    __N8;
    __N9;
    __N10;
    __N11;
    __N12;
  }

  subgraph "cluster_13" {
    graph [label="Files", axis="y"];
    __N14;
    __N15;
    __N16;
    __N17;
    __N18;
  }

  subgraph "cluster_19" {
    graph [label="Outer"];
    __N20 [label="1st"];
    subgraph "cluster_21" {
      graph [label="Middle"];
      __N22 [label="2nd"];
      subgraph "cluster_23" {
        graph [label="Inner"];
        __N24 [label="3rd"];
      }
    }
    __N20 -> __N22;
    __N20 -> __N24;
    __N22 -> __N20;
    __N22 -> __N24;
    __N24 -> __N20;
    __N24 -> __N22;
  }

  __N14 -> __N2;
  __N15 -> __N3;
  __N16 -> __N4;
  __N17 -> __N5;
  __N18 -> __N6;

  __N18 -> __N8;
  __N17 -> __N9;
  __N16 -> __N10;
  __N15 -> __N11 [label="5,000"];
  __N14 -> __N12 [label="50"];


}
```

![matrix2](https://github.com/user-attachments/assets/78bcdfd2-14a3-4778-8043-b3610ce1fdf6)

