// C program for pre-order traversals
#include <stdio.h>
#include <stdlib.h>

 int flag = 0;

struct node{
    int data = -1;
    struct node *left;
    struct node *right;
};
struct node* node_array[100];
// crate the new node for tree
struct node* newNode(int data)//check
{
    struct node* node = (struct node*) malloc(sizeof(struct node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return(node); 
}

// direct the right place for nodes
struct node* insert(struct node* node, int info, int k){
   
    if(node == NULL){
        node_array[k] = newNode(info);
    }else if((flag == 0)&&(node_array[k-1]->right != NULL)) {
        node_array[k] -> left = insert(node->left, info);
        flag = 1;
    }else if((flag == 1)&&(node_array[k-1]->left != NULL)){
        node_array[k] -> right = insert(node->right, info);
        flag = 0;
    }
    return node;
}

// print the pre-order set for the tree
void printPreorder(struct node* node){

    if(node == NULL)
        return;
    printf("%d ", node->data);

    printPreorder(node->left); 
    printPreorder(node->right);
}

// Driver program to test the function.
int main(int argc,char** argv){
    FILE* fp;
    int num_node = 0;
    int node_temp = 0;
    int data_base[100];
    int i = 0;
   
    

    //讀檔案的格式，之後可能會用到
    /*
    fp = fopen(argv[1],"r");
    if(fp == NULL){
        printf("open failure");
        return 1;
    }else{
        num_node = fscanf(fp, "%d", &num_node); //先讀一個有多少個node
        while(fscanf(fp, "%d%*c", &node_temp) != EOF){ //有c但是略過c(好玩噎))
            //printf("%d ", node_temp); for testing I/O
            data_base[i] = node_temp;
            i += 1;
        }
        
    }
    fclose(fp);
    */

    //for input data
    scanf("%d ", &num_node);
    while(scanf("%d%*c", &node_temp) != EOF){
        data_base[i] = node_temp;
        i += 1;
    }

    struct node* root = newNode(data_base[0]); 

    for(int k = 1; k < num_node; ++k){
        insert(root, data_base[k], node_array[k], k);
    }

    
/*
    root -> left      = newNode(data_base[1]);
    root -> right     = newNode(data_base[2]);
    root -> left -> left = newNode(data_base[3]);
    root -> left -> right = newNode(data_base[4]);
    root -> right -> left = newNode(data_base[5]);
    root -> right -> right = newNode(data_base[6]);
    //要general
*/
    printPreorder(root);
    return 0;



}