import kind_util
import und_colors
import understand


# These metric list functions are meant to include as much as possible so that
# chart requests can be made for any metrics. A lot of the listed metrics may
# not be applicable for specific database languages, or may be disabled.
#
# Also note that because understand.Metric.list(kindstring) does not have a
# database argument it won't contain metric plugins (whose test_available
# function provides a database). Even if it did take a database, it would only
# list enabled plugins. So, it's not ideal, but these functions manually list
# metric plugins known to be used by ireports.
def list_function_metrics(db):
  metlist = understand.Metric.list(kind_util.FUNCTION_KIND_STR)
  metlist.extend([
    # HIS Metrics
    "RatioCommentsWithBeforeToCode",
    "CountGoTo",
    "CountCallbyUnique",
    "CountCallsUnique",
    "CountParams",
    "MaxCallDepth",
    "CountEarlyExit",
    "LanguageScope",
    "MinRecursiveDepth",
    "StabilityIndex",
    "HISNOMV",

    # Diff Metrics
    "CountLineChanged",
    "CountLineRemoved",
    "CountLineNew",
  ])
  return metlist

def list_class_metrics(db):
  metlist = understand.Metric.list(kind_util.CLASS_KIND_STR)
  return metlist

def list_file_metrics(db):
  metlist = understand.Metric.list(kind_util.FILE_KIND_STR)
  return metlist

def list_arch_metrics(db):
  # There is no equivalent understand.Metric.list for architecture metrics,
  # however, for built-in metrics, architecture and db metrics are the same
  metlist = db.metrics()
  return metlist

class SizeColorMetricChart:
  """ Create a chart with size and color of nodes each controlled by a metric.
      If drawing hiearchically, clusters are colored by the max value of leaf
      nodes. Callers must set the layout algorithm.
  """
  def define_options(graph, legend="top", labels="Name", max_nodes = "50", layout_target=None):
    """ Define options this chart uses. Pass in None to hide the option.
        max_nodes only impacts flat drawing.
    """
    if labels is not None:
      graph.options().define("Node Label", ["Size Metric Value", "Color Metric Value", "Name", "Long Name", "None"], labels)
    if max_nodes is not None:
      graph.options().define("Maximum Nodes Shown", ["5", "10", "15", "20", "25", "30", "40", "50", "75", "100", "200", "300", "400", "500"], max_nodes)
    if legend is not None:
      graph.options().define("Legend", ["None", "top", "left", "right", "bottom"], legend)
    if layout_target is not None:
      layouts = ["flatbubble", "treemap"]
      if isinstance(layout_target, understand.Arch):
        layouts.extend(["hiearchical_treemap", "sunburst"])
      graph.options().define("Layout", layouts, "flatbubble")

  def __init__(self, graph, size_metric, color_metric):
    self.size_metric = size_metric
    self.color_metric = color_metric if color_metric != "None" else None
    self.color_scale = und_colors.ColorScale() if self.color_metric else und_colors.DiscreteColorScale()

    self.max_nodes = None # Flat draw only
    try:
      self.max_nodes = int(graph.options().lookup("Maximum Nodes Shown"))
    except:
      pass

    self.labels = graph.options().lookup("Node Label")
    self.legend = graph.options().lookup("Legend")

    layout = graph.options().lookup("Layout")
    if layout:
      if layout == "hiearchical_treemap" or layout == "treemap":
        graph.set("layout", "treemap")
        self.color_scale.gradient_fill = True
        self.max_nodes = None
      elif layout == "sunburst":
        graph.set("layout","sunburst")
        graph.set("r1","36")
        graph.set("rstep", "18")
      else:
        graph.set("layout", layout) #flatbubble

  def node_label(self, sval, cval, t):
    """ Return the label for the leaf node with the given size value, color
        value, and target. Override to control labels. """
    if self.labels == "Size Metric Value":
      return str(sval)
    elif self.labels == "Color Metric Value":
      return str(cval)
    elif self.labels == "Name":
      return t.name()
    elif self.labels == "Long Name":
      return t.longname()
    return ""

  def node_tooltip(self, sval, cval, target):
    """ Return the tooltip for the leaf node with the given size value, color
        value, and target. Override to control tooltips. """
    if cval is None:
      return "{}\n{}={}".format(target.longname(), self.size_metric, sval)
    return "{}\n{}={}\n{}={}".format(target.longname(), self.size_metric, sval, self.color_metric, cval)

  def create_node(self, graph, sval, cval, target):
    n = graph.node(self.node_label(sval, cval, target) )
    n.sync(target)
    n.set("area",str(sval))
    self.color_scale.set_colors(n, cval)
    tooltip = self.node_tooltip(sval, cval, target)
    if tooltip:
      n.set("tooltip", tooltip)

  def flat_draw(self, graph, targets):
    """ Draw the graph without clusters. """
    nodes = []
    for t in targets:
      sval = t.metric(self.size_metric)
      if not sval:
        continue
      cval = None
      if self.color_metric:
        cval = t.metric(self.color_metric)
        if cval is None:
          continue
        self.color_scale.update_val_range(cval)
      nodes.append( (sval, cval, t) )

    if not nodes:
      return

    if self.color_metric:
      legend_label = "Color by {} {}; Size by {}".format(self.color_metric, "[{},{}]", self.size_metric)
      self.color_scale.create_legend(graph, self.legend, legend_label)

    cur_node = 0
    for sval, cval, t in sorted(nodes, key=lambda i: (i[0],i[1]), reverse=True):
      self.create_node(graph, sval, cval, t)

      cur_node += 1
      if self.max_nodes and cur_node >= self.max_nodes:
        break

  # Override these if using hierarchical draw with something other
  # than architectures
  def target_parent(self, target):
    return target.parent()

  def target_cluster_children(self, target):
    return target.children()

  def target_node_children(self, target, recursive=False):
    return target.ents(recursive)

  def target_cluster_sync(self, target):
    if isinstance(target, understand.Arch):
      return target
    return None

  def cluster_label(self, target):
    return target.name()

  def cluster_tooltip(self, sval, cval, target):
    if self.color_metric is None:
      return "{}\nSum {}={}".format(target.longname(), self.size_metric, sval)
    return "{}\nSum {}={}\nMax {}={}".format(target.longname(), self.size_metric, sval, self.color_metric, cval)

  def hierarchical_draw(self, graph, root):
    """ Draw the graphs with clusters. If not using understand.Arch as clusters,
        the five methods above may need to be overridden.
    """
    # Init color scale
    if self.color_metric:
      for target in self.target_node_children(root, True):
        self.color_scale.update_val_range(target.metric(self.color_metric))
      legend_label = "Color by {} {}; Size by {}".format(self.color_metric, "[{},{}]", self.size_metric)
      self.color_scale.create_legend(graph, self.legend, legend_label)

    clusters = { self.target_parent(root) : graph }

    def draw_hierarchical_recurse(target):
      # create a cluster for this architecture
      par = clusters.get(self.target_parent(target))
      sync = self.target_cluster_sync(target)
      if sync:
        cluster = par.cluster(self.cluster_label(target),sync)
      else:
        cluster = par.cluster(self.cluster_label(target))
      clusters[target] = cluster

      # Recurse on architecture children. This chart colors clusters by the max
      # color of it's children, so track max color value
      cur_cval = 0
      cur_sval = 0
      for child in self.target_cluster_children(target):
        child_cval, child_sval = draw_hierarchical_recurse(child)
        cur_cval = max(cur_cval, child_cval)
        cur_sval += und_colors.to_float(child_sval)

      # Add the entity nodes
      for node in self.target_node_children(target):
        # Size value
        sval = node.metric(self.size_metric)
        if not sval:
          continue
        cur_sval += und_colors.to_float(sval)

        # Color Value
        cval = node.metric(self.color_metric) if self.color_metric else None
        if cval is not None:
          cval = und_colors.to_float(cval)
          cur_cval = max(cur_cval, cval)
        self.create_node(cluster, sval, cval, node)

      # Set the color for this architecture
      self.color_scale.set_colors(cluster, cur_cval if self.color_metric else None)
      tooltip = self.cluster_tooltip(cur_sval, cur_cval, target)
      if tooltip:
        cluster.set("tooltip", tooltip)

      # Return the max for the parent
      return cur_cval, cur_sval

    draw_hierarchical_recurse(root)
