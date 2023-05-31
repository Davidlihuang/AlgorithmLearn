"""
1. 100 x 100中 10个net的布线
2. 使用多商品流模型
"""
import numpy as np
import gurobipy as gp

import random
import networkx as nx
import matplotlib.pyplot as plt

import random
import networkx as nx
import matplotlib.pyplot as plt

# Define the floorplane dimensions
floorplan_width = 100
floorplan_height = 100

# Define the number of nets and pins
num_nets = 10
num_pins_per_net = 2

# Create a graph for the floorplan
floorplan = nx.grid_2d_graph(floorplan_width, floorplan_height)

# Add the nets and pins to the floorplan graph
nets = []
for i in range(num_nets):
    # Choose two random locations for the pins of the net
    pin1 = (random.randint(0, floorplan_width-1), random.randint(0, floorplan_height-1))
    pin2 = (random.randint(0, floorplan_width-1), random.randint(0, floorplan_height-1))
    while pin2 == pin1:
        pin2 = (random.randint(0, floorplan_width-1), random.randint(0, floorplan_height-1))
    
    # Add the pins to the floorplan graph and connect them with an edge
    floorplan.add_edge(pin1, pin2)
    
    # Add the net to the list of nets
    nets.append((pin1, pin2))

# Create a graph for the multi-commodity flow problem
mcf_graph = nx.DiGraph()

# Add the nodes for the pins of each net to the multi-commodity flow graph
for net in nets:
    mcf_graph.add_node(net[0])
    mcf_graph.add_node(net[1])
    
    # Add the edge between the pins of the net and set its capacity to 1
    mcf_graph.add_edge(net[0], net[1], capacity=1)

# Add a source node and an edge from it to each pin with capacity 1
source_node = (-1, -1)
for node in list(mcf_graph.nodes()):
    if node not in source_node:
        mcf_graph.add_edge(source_node, node, capacity=1)

# Add a sink node and an edge from each pin to it with capacity 1
sink_node = (-2, -2)
for node in list(mcf_graph.nodes()):
    if node not in [source_node, sink_node]:
        mcf_graph.add_edge(node, sink_node, capacity=1)

# Solve the multi-commodity flow problem and get the routing solution
routing_solution = nx.multi_commodity_flow(mcf_graph, source_node, sink_node)

# Plot the floorplan and the routing solution
plt.figure(figsize=(10, 10))
nx.draw_networkx(floorplan, pos=nx.spring_layout(floorplan), node_color='lightgray', with_labels=False)
for net in nets:
    path = routing_solution[net[0]][net[1]][0]['path']
    path_edges = [(path[i], path[i+1]) for i in range(len(path)-1)]
    nx.draw_networkx_edges(floorplan, pos=nx.spring_layout(floorplan), edgelist=path_edges, edge_color='red')
plt.axis('equal')
plt.show()

