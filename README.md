# DTs
decision tree
Input:
Bộ dữ liệu về Balance-Scale gồm 4 thuộc tính và 3 nhãn L (Left), R(Right), B(Balance) với các bản ghi.
File trainTest.txt 
File validTest.txt 

Output:
Print tree: in ra cây quyết định cơ bản

Giới thiệu cấu trúc:

struct Node: gồm Nhãn node (Label), ,Node left, Node right, thuộc tính isLeaf nếu là terminal. Nếu Node chưa phải terminateNode thì sẽ được gán nhãn khác L, R, B.
struct Tree: chứa node root.
vector getData : nhập dữ liệu từ trainTest.txt

float ginni_index(): tính gini trong 1 tập dữ liệu.
test_split() : split dataset base on value and index
get_split() : select best split point
split() : split node thành child hoặc terminal
buildTree: xây cây
printTree() : in ra cây quyết định cơ bản
