#include <iostream>
#include <stdio.h>
#include <queue>
#include <algorithm>
using namespace std;

struct Node{
    int data;
    int max;
    int min;
    bool state;
    Node* left;
    Node* right;
};

//create the new node for the tree
inline Node* createnode(int value){
    Node* newnode = new Node;
    newnode -> data = value;
    newnode -> max = 0;
    newnode -> min = 1000;
    newnode -> state = true;
    newnode -> left = NULL;
    newnode -> right  = NULL;
    return newnode;
};

// print the pre-order traversal of the tree
inline void printPreOrder(Node *node){
    if(node == NULL)
        return;

    cout << node->data << " ";
    printPreOrder(node -> left);
    printPreOrder(node -> right);
}


// find largst number in tree
inline int largest(Node* node){
    if(node == NULL)
        return 0;

    int left = largest(node -> left);
    int right = largest(node -> right);
    if ((node -> data > left) && (node -> data > right)){
        node -> max = node -> data;
        return node -> max;
    }
    else{
        node -> max = max(left, right);
        return node -> max;
    }

    return node -> max;
}


//find smallest number in tree
inline int smallest(Node* node){
    if(node == NULL)
        return 1000;

    int left = smallest(node -> left);
    int right = smallest(node -> right);
    if((node -> data < left) && (node -> data < right)){
        node -> min = node -> data;
        return node -> min;

    }
    else{
        node -> min = min(left, right);
        return node -> min;
    }

    return node -> min;
}

inline bool checkBSP(Node* node){
    if(node == NULL)
        return true;

    Node* node_left = new Node;
    Node* node_right = new Node;
    node_left = node -> left;
    node_right = node -> right;


    if(node_left -> max > node -> data  && node_left != NULL){
       // node -> state = false;
        return false;
    };
    if(node_right -> min < node -> data && node_right != NULL){
       // node -> state = false;
        return false;
    };

    if(checkBSP(node-> left) == false && node_left != NULL){
        return false;
    };

    if(checkBSP(node-> right) == false && node_right != NULL){
        return false
    }

    //cout << node_left-> state << endl << node_right-> state;
    return true;

}
int main()
{
    int num_node = 0;
    int node_temp = 0;
    bool property = true;

    // for input data
    scanf("%d" ,&num_node);
    int data_base[1000];
    queue <int*> data_baseq;


    for(int k = 0;k < num_node;k++){
        scanf("%d%*c", &node_temp);
        data_base[k] = node_temp;
        data_baseq.push(&data_base[k]);
    }


    Node* root = createnode(*data_baseq.front());

    data_baseq.pop();
    queue <Node*> nodes;
    nodes.push(root);

    while(data_baseq.empty() == false){
        Node *newnode = nodes.front();
        int temp_left;
        int temp_right = -1;
        if(newnode -> left == NULL){
            newnode -> left = createnode(*data_baseq.front());
            temp_left = *data_baseq.front();
            data_baseq.pop();
            nodes.push(newnode -> left);
        }
        if((newnode -> right == NULL) &&(data_baseq.empty() == false)){
            newnode -> right = createnode(*data_baseq.front());

            temp_right = *data_baseq.front();
            data_baseq.pop();
            nodes.push(newnode -> right);
        }
        property = ((temp_left <= nodes.front()->data) && ((nodes.front() -> data <= temp_right) || (temp_right == -1)))? property : false;
        nodes.pop();
    }

    int temp = largest(root);
    int temp2 = smallest(root);
    bool check = checkBSP(root)&checkBSP(root -> left);//**check不應該這樣寫

    printPreOrder(root);


    if(check&&property){
        printf("\ntrue\n");
    }else{
        printf("\nfalse\n");
    }
    return 0;

}

