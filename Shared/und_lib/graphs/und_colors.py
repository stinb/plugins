import colorsys

UND_EXTENDED_COLORS = ["#90c4e4", "#afd870", "#fcad56", "#4e8cb9", "#569f38", "#e45721", "#133a52", "#315015", "#71290d", "#acd3eb", "#c6e39a", "#fcc281", "#5aa7d7", "#84c968", "#fa8504", "#2774a4", "#47641b", "#b54216"]
UND_COLORS = UND_EXTENDED_COLORS[:6]
UND_FONT_COLOR= "#FFFFFF"

class DiscreteColorScale:
  def __init__(self, extended=False):
    self.colors = UND_EXTENDED_COLORS if extended else UND_COLORS
    self.idx = 0
    self.font_color = UND_FONT_COLOR
    self.set_color = True
    self.set_fill = True

  def set_colors(self, node_or_cluster, idx = None):
    if idx is None:
      idx = self.idx
      self.idx += 1

    color = self.colors[idx % len(self.colors)]
    if self.set_color:
      node_or_cluster.set("color", color)
    if self.set_fill:
      node_or_cluster.set("fillcolor", color)
    if self.font_color:
      node_or_cluster.set("fontcolor", self.font_color)

def to_float(val):
  if val is not None and isinstance(val,str):
    if val.endswith('%'):
      val = val[:-1]
    val = float(val)
  return val

class ColorScale:
  def __init__(self, gradient_fill=False):
    self.min_val = None
    self.max_val = None
    self.min_color = (158, 202, 225)  # RGB for #9ECAE1
    self.max_color = (8, 69, 148)     # RGB for #084594
    self.font_color = UND_FONT_COLOR
    self.set_color = True
    self.set_fill = True
    self.gradient_fill = gradient_fill

    # See https://graphviz.org/docs/attr-types/colorList/ for more details
    self.gradient_fill_style = "radial" # Set to None for linear gradient
    self.gradient_fill_angle = None # defaults to 0, set to change angle

  def update_val_range(self, val):
    """ Find the color scale range by calling this function with each value"""
    val = to_float(val)
    if val is None:
      return
    if self.min_val is None or val < self.min_val:
      self.min_val = val
    if self.max_val is None or val > self.max_val:
      self.max_val = val

  def val_rgb(self, val):
    val = to_float(val)
    if val is None:
      raise Exception("Invalid Value")
    if self.min_val is None or self.max_val is None:
      raise Exception("Color Scale Range not known")

    r,g,b = self.max_color if val >= self.max_val else self.min_color
    if self.max_val - self.min_val > 0:
      percent = (val - self.min_val) / (self.max_val - self.min_val)
      r = self.min_color[0] + percent * (self.max_color[0] - self.min_color[0])
      g = self.min_color[1] + percent * (self.max_color[1] - self.min_color[1])
      b = self.min_color[2] + percent * (self.max_color[2] - self.min_color[2])
    return r,g,b

  def set_colors(self, node_or_cluster, val):
    """ Set the colors on an understand.Node or understand.Graph (cluster)
        to a color scaled between self.min_color and self.max_color based
        on val.
    """
    try:
      r,g,b = self.val_rgb(val)
    except:
      return # Leave colors unset

    if self.font_color:
      node_or_cluster.set("fontcolor", self.font_color)
    if self.set_color:
      node_or_cluster.set("color", '#%02x%02x%02x' % (int(r), int(g), int(b)))
    if self.set_fill:
      if self.gradient_fill:
        node_or_cluster.set("fillcolor", graph_gradient_color_string(r,g,b))
        if self.gradient_fill_style:
          node_or_cluster.set("style", self.gradient_fill_style)
        if self.gradient_fill_angle:
          node_or_cluster.set("gradientangle", self.gradient_fill_angle)
      else:
        node_or_cluster.set("fillcolor", '#%02x%02x%02x' % (int(r), int(g), int(b)))

  def create_legend(self, graph, legend, lbl="Color {} - {}"):
    if self.min_val is None or \
       self.max_val is None or \
       not legend or legend == "None":
      return

    legendCluster = graph.cluster()
    legendCluster.set("legend", legend)

    color_node = legendCluster.node(lbl.format(self.min_val, self.max_val))
    color_node.set("color", "none")
    color_node.set("fillcolor", '#%02x%02x%02x;#%02x%02x%02x' % tuple(self.min_color + self.max_color))

    return legendCluster

def graph_gradient_color_string(r, g, b):
  # Gradient fill, find colors lighter and darker, requires converting to hsv
  h,s,v = colorsys.rgb_to_hsv(r/255.0,g/255.0,b/255.0)

  vLight = v * 1.2 # 20% lighter
  sLight = s
  if vLight > 1.0:
    sLight = sLight - vLight + 1.0
    if sLight < 0:
      sLight = 0
    vLight = 1.0

  light = [ int(255*x) for x in colorsys.hsv_to_rgb(h, sLight, vLight) ]
  dark =  [ int(255*x) for x in colorsys.hsv_to_rgb(h, s, v*0.714) ] # 40% (140/100) darker

  return '#%02x%02x%02x;#%02x%02x%02x' % tuple(light + dark)
