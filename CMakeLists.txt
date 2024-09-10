cmake_minimum_required(VERSION 3.19)
project(algorithm_advanced)

set(CMAKE_CXX_STANDARD 14)

add_executable(algorithm_advanced main.cpp kmp_trie.h dijkstra.h monstack_op.h skiplist.h union_find.h boom_filter.h monqueue_op.h rb_tree.h segment_tree.h kruskal_prim.h huffman_grey_code.h mincut_maxflow.h greed_algorthm.h tu_bao.cpp tu_bao.h)