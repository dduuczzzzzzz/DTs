#include <bits/stdc++.h>

using namespace std;

//get dataset from file
vector<vector<int>> getData(string path){
    ifstream inFile(path);
    vector<vector<int>> dataSet;
    vector<int> row;
    string line;
    if(inFile.is_open()) {
        while(getline(inFile,line)) {
            row.clear();
            stringstream ss(line);
            string temp;
            while(getline(ss,temp,',')) {
                row.push_back(temp.at(0)-'0');
            }
            dataSet.push_back(row);
        }
    }
    inFile.close();
    return dataSet;
}

struct Node{
    Node* left;
    Node* right;
    vector<vector<int>> group;
    int Index =-1;
    int Value =-1;
    bool isLeaf = false;
    char label;

    Node() {
    }

    Node(vector<vector<int>> dataset){
        group = dataset;
    }

    //decide to terminal node
    void to_terminal() {
        left = nullptr;
        right = nullptr;
        isLeaf = true;
        int l_ = 0, r_ = 0, b_ = 0;
        for (int row = 0;row < group.size();row++){
            if (group[row][0]==('L'-'0')) l_++;
            else if (group[row][0]==('B'-'0')) b_++;
            else r_++;
        }
        int maxi = max(max(l_, b_), r_);
        if(maxi == l_) label = 'L';
        else if(maxi == b_) label = 'B';
        else label = 'R';
    }
};

struct Tree{
    Node* root;

    Tree(Node* node){
        root = node;
    }
};

// get gini index for split 
float gini_index(vector<vector<vector<int>>> groups, vector<int> classes) {
    int n_instances = 0;
    for(vector<vector<int>> group : groups) {
        n_instances += group.size();
    }
    float gini = 0.0;
    for(vector<vector<int>> group : groups) {
        float size = float(group.size());
        if(size == 0){
            continue;
        }
        float score = 0;
        for(int class_val : classes) {
            int count = 0;
            for(int i = 0;i<group.size();i++){
                if (group[i][0] == class_val) count++;
            }
            float p = (float) count/(float)size;
            score+= p*p;
        }
        gini += (1.0 - score) * (size / n_instances);
    }
    return gini;
}

//split dataset base on value and index
vector<vector<vector<int>>> test_split (int index, int value, vector<vector<int>> dataset){
    vector<vector<vector<int>>> groups;
    vector<vector<int>> left;
    vector<vector<int>> right;
    for(int i=0; i<dataset.size(); i++){
        if(dataset[i][index] < value){
            left.push_back(dataset[i]);
        }
        else{
            right.push_back(dataset[i]);
        }
    }
    groups.push_back(left);
    groups.push_back(right);
    return groups;
}

// select best split point
Node* get_split(vector<vector<int>> dataset){
    vector<int> classes = {'L'-'0','R'-'0','B'-'0'};
    float b_score = 10.0;
    Node* newNode = new Node(dataset);
    Node* left = new Node();
    Node* right = new Node();
    newNode -> left = left;
    newNode -> right = right;
    vector<vector<vector<int>>> groups;
    for(int index = 1; index < dataset[0].size(); index++){
        for(int value = 1; value <=5; value++){
            groups = test_split(index,value,dataset);
            float gini = gini_index(groups,classes);
            if (gini < b_score){
                b_score = gini;
                newNode->Index = index;
                newNode->Value = value;
                left->group = groups[0];
                right->group = groups[1];
            }
        }
    }
    return newNode;
}

// split a node to child or make terminal
void split(Node* node, int maxDepth, int minSize, int depth){
    if(node->left == nullptr || node->right == nullptr
       || node->left->group.size() == 0 || node->right->group.size() == 0){
        node->to_terminal();
        return;
    };
    if (depth >= maxDepth){
        node->left->to_terminal();
        node->right->to_terminal();
        return;
    };
    if (node->left->group.size() <= minSize){
        node->left->to_terminal();
    }
    else{
        node->left = get_split(node->left->group);
        split(node->left, maxDepth, minSize, depth+1);
    };
    if (node->right->group.size() <= minSize){
        node->right->to_terminal();
    }
    else{
        node->right = get_split(node->right->group);
        split(node->right, maxDepth, minSize, depth+1);
    }
}

Tree* buildTree(vector<vector<int>> dataset, int maxDepth, int minSize){
    Node* root = get_split(dataset);
    split(root, maxDepth, minSize,1);
    Tree* newTree = new Tree(root);
    return newTree;
}

float accuracy_metric(vector<int> actual, vector<int> predicted) {
    int correct = 0;
    for(int i=0; i<actual.size(); i++) {
        if(actual[i] == predicted[i]) {
            correct += 1;
        }
    }
    return correct / (actual.size()) * 100.0;
}

void printTree(Node* node, int depth=0){
    if (node->isLeaf == false){
        for(int i=0;i<depth;i++){
            cout<<"  ";
        }
        cout<<"X"<<node->Index<<" < "<<  node->Value<<endl;
        printTree(node->left, depth+1);
        printTree(node->right, depth+1);
    }
    else{
        for(int i=0;i<depth;i++){
            cout<<"  ";
        }
        cout<< node->label <<endl;
    }
}

char predict(Node* node, vector<int> row) {
    if(row[node->Index] < node->Value){
        if(node->left->isLeaf == false){
            return predict(node->left, row);
        }
        else return node->left->label;
    }
    else{
        if(node->right->isLeaf == false) {
            return predict(node->right, row);
        }
        else return node->right->label;
    }
}

int main() {
    string trainPath = "train.txt";
    string testPath = "valid.txt";
    vector<vector<int>> trainSet = getData(trainPath);
    Tree* testTree = buildTree(trainSet, 10, 1);
    printTree(testTree->root);
    //cout << accuracy_metric(trainSet[0], ) 

    return 0;
}

