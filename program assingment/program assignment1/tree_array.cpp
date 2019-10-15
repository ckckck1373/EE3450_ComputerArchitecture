#include <iostream>
#include <stdio.h>
#include <queue>


using namespace std;


//variable declaration

inline void printPreOrder(int *data_base, int j){
    if(data_base[j]== NULL){
        return;
    }

    cout << data_base[j] << " ";
    printPreOrder(data_base, 2*j);
    printPreOrder(data_base, 2*j+1);
}



inline int largest(int *data_base,int *data_base_max, int j){
    if(data_base[j] == NULL)
        return 0;

    int left0 = largest(data_base, data_base_max, 2*j);
    int right0 = largest(data_base, data_base_max, 2*j+1);
    if((data_base[j] > left0) && (data_base[j] > right0)){
        data_base_max[j] = data_base[j];
        return data_base_max[j];
    }else{
        if(left0 >right0){
            data_base_max[j] = left0;
        }else{
            data_base_max[j] = right0;
        }
    }

    return data_base_max[j];
}





inline int smallest(int *data_base, int *data_base_min, int j){
    if(data_base[j] == NULL)
        return 1000;

    int left1 = smallest(data_base, data_base_min, 2*j);
    int right1 = smallest(data_base, data_base_min, 2*j+1);
    if((data_base[j] < left1)&&(data_base[j] < right1)){
        data_base_min[j] = data_base[j];
    }else{
        if(left1 <right1){
            data_base_min[j] = left1;
        }else{
            data_base_min[j] = right1;
        }
    }
    return data_base_min[j];
}



inline bool checkBSP(int *data_base, int *data_base_max, int *data_base_min, int j, int num_node){
    cout << endl << "j:"<<  j;
    if(data_base[j] == NULL){
        return true;
    }

    if((num_node >= 2*j+1)&&((data_base_max[2*j] > data_base[j])||(data_base_min[2*j+1] < data_base[j]))){
        return false;
    }else if(num_node == 2*j && data_base_max[2*j] > data_base[j]){
        return false;
    }else{

         return checkBSP(data_base, data_base_max, data_base_min, 2*j, num_node)&&checkBSP(data_base, data_base_max, data_base_min, 2*j+1, num_node);
    }



}




int main(){
    int num_node = 0;
    int node_temp;
    int j=1;
    bool property = true;

    scanf("%d", &num_node);
//之後記得盡量不要用這種array宣告，因為當你存值進去的時候，後面會變成0
    int data_base[100]={NULL};
    int data_base_max[100] = {NULL};
    int data_base_min[100] = {NULL};

    for(int k=1;k<=num_node;k++){
        scanf("%d%*c", &node_temp);
        data_base[k]=node_temp;
    }

    printPreOrder(data_base, 1);
    int temp1=largest(data_base, data_base_max, 1);
    int temp2=smallest(data_base, data_base_min, 1);


    for(int j=1; j< 10;j++){
        cout << endl;
        cout << "data_base: " << data_base[j] << endl;
        cout << "data_base_max: " << data_base_max[j] << endl;
        cout << "data_base_min:" << data_base_min[j] << endl;
    }



    if(!checkBSP(data_base, data_base_max, data_base_min, 1, num_node)){
        cout << endl << "false" << endl;
    }else{
        cout << endl << "true" << endl;
    }
    return 0;
}
