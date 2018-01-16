# Katz
# Language: C++
# Input: CSV (network)
# Output: NOA (central nodes and centrality values)

PluMA plugin to compute Katz Prestige Centrality (Katz, 1953), which produces
a list of the most important nodes in an input network.  The plugin takes
the input network in the form of a CSV file with nodes represented as both rows
and columns, and entry (i, j) contains the weight of the edge from node i to node j.

The plugin will produce output as a NOde Attribute (NOA) file of nodes ranked
by centrality value, which can be imported into Cytoscape.  This will then
provide "Centrality" as an attribute for every node, which can be used for further
downstream analysis or visualization.
