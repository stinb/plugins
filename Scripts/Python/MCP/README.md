# MCP Servers for Understand

This directory contains MCP (Model Context Protocol) servers that provide access to Understand databases for LLM use.

## Setup Instructions

For detailed setup instructions, see the [official documentation](https://scitools.freshdesk.com/support/solutions/articles/70000680031). The [directions](https://scitools.freshdesk.com/support/solutions/articles/70000582855-api-tutorial-1-getting-started-with-the-python-api) for setting up a custom python installation with the Understand API may also be useful because the `upython` executable shipped with Understand does not include FastMCP.

### Quick Setup

1. **Install FastMCP**:
   ```bash
   pip install fastmcp
   ```

2. **Configure Environment Variables**:
   - **macOS/Linux**: Set `PYTHONPATH` to the directory containing Understand's Python bindings
   - **Windows**: Set `PYTHONPATH` and `UNDERSTAND_DLL_DIR` environment variables

3. **Configure in Cursor**:
   Create `.cursor/mcp.json` in your project with:
   ```json
   {
     "mcpServers": {
       "understand": {
         "command": "/path/to/python",
         "args": [
           "/path/to/mcp_server_for_understand_comprehensive.py",
           "path/to/project.und"
         ],
         "env": {
           "PYTHONPATH": "/path/to/Understand/bin/macosx/Python"
         }
       }
     }
   }
   ```

## Choosing a Server

This directory contains two MCP servers:

### `mcp_server_for_understand_comprehensive.py`

- Tools provide direct access to Understand API methods
- Low-level, comprehensive API coverage
- Best for: Full control, exploring all API features, working with architectures/lexemes/control flow graphs

### `mcp_server_for_understand_llm_optimized.py`

- Tools designed for efficient LLM interaction
- Higher-level, task-oriented abstractions with built-in result limiting
- Best for: LLM use cases, smaller context windows, common workflows (finding entities, getting source, analyzing references)

**Recommendation**: Start with `mcp_server_for_understand_llm_optimized.py` for most LLM use cases. Use `mcp_server_for_understand_comprehensive.py` when you need specific API features not available in the optimized version.

## Recommended plugins

If the `mcp_server_for_understand_llm_optimized.py` server is used with older Understand builds, the following plugins may need to be installed for full functionality. The table below shows the earliest build that ships with the required plugin version.

| Plugin (path) | Build | Description |
|---------------|-------|-------------|
| [Metric/arch_metrics.upy](https://github.com/stinb/plugins/blob/main/Metric/arch_metrics.upy) | 1246 | Entity/children counts (direct and recursive), depth; used by `get_architecture_details` key metrics. |
| [Metric/arch_depends.upy](https://github.com/stinb/plugins/blob/main/Metric/arch_depends.upy) | 1245 | Dependency counts and refs (in/out, direct/major); used by `get_architecture_details` key metrics. |
| [Metric/arch_percent.upy](https://github.com/stinb/plugins/blob/main/Metric/arch_percent.upy) | 1246 | Percent of root/parent (entities and code lines); used by `get_architecture_details` key metrics. |
