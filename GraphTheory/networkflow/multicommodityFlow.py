import networkx as nx
from networkx.algorithms.flow import min_cost_flow

# 创建有向图
G = nx.DiGraph()

# 添加节点
G.add_nodes_from(["source", "sink", "a", "b", "c", "d", "e", "f"])

# 添加边
G.add_edge("source", "a", capacity=10, weight=1)
G.add_edge("source", "b", capacity=5, weight=2)
G.add_edge("a", "c", capacity=2, weight=1)
G.add_edge("a", "d", capacity=4, weight=3)
G.add_edge("b", "c", capacity=4, weight=2)
G.add_edge("b", "d", capacity=3, weight=4)
G.add_edge("c", "e", capacity=6, weight=1)
G.add_edge("d", "e", capacity=4, weight=2)
G.add_edge("c", "f", capacity=5, weight=3)
G.add_edge("d", "f", capacity=5, weight=4)
G.add_edge("e", "sink", capacity=10, weight=1)
G.add_edge("f", "sink", capacity=10, weight=1)

# 使用最小费用最大流算法求解最短路径
flow_dict = min_cost_flow(G, "source", "sink")

# 输出最短路径
print("最短路径为：", flow_dict)

        