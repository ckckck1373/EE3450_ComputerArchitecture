
// C program for pre-order traversals
#include <stdio.h>
#include <stdlib.h>


struct node{
    int data;
    struct node *left;
    struct node *right;
}


//create the new node for the tree
struct node* newNode(int data){
    struct node* node = (struct node*) malloc(sizeof(struct node));
    node -> data = data;
    node -> left = NULL;
    node -> right = NULL;
    return (node);
}

// print the pre-order traversal of the tree
void printPreorder(struct node* node){
    if(node == NULL)
        return;
    printf("%d ", node->data);
    printPreorder(node->left);
    printPreorder(node->right);
}


// Driver program to test the function
int main(){
    int num_node = 0;
    int i = 1;

    //for input data
    scanf("%d " &num_node);

    int data_base[num_node];
    while(scanf("%d%*c", &node_temp) != EOF){
        data_base[i] = node_temp;
        i += 1;
    }

    //struct node* root = newNode(data_base[0]);
    struct node* node_array = (struct node *)malloc(num_node * sizeof(struct node));

    for(int j = 1;j <= num_node; ++j){
        node_array[j] -> data = data_base[j]
        node_array[j] -> left = (j + j <= num_node)? newNode(data_base[j+j]) : NULL;
        node_array[j] -> right = (j + j + 1 <= num_node)? newNode(data_base[j+j+1]) : NULL;
    }

    printPreorder(node_array[1]);
    return 0:
}

