#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "avl_tree.h"

int my_compare(AVLTreeKey value1, AVLTreeKey value2) {

    if (*(int*)value1 > *(int*)value2) {
        return RET_BIGGER;
    } else if (*(int*)value1 < *(int*)value2) {
        return RET_SMALLER;
    }

    return RET_EQUAL;
}

void my_print(AVLTreeKey key) {
    printf("%d\t", *((int*)key));
}


int main(void) {
    AVLTree* tree = avl_tree_new(my_compare);

    int values[] = { 0, 1, 2, 3, 4, 5, 6 };
    for (unsigned int i = 0; i < sizeof (values) / sizeof (int); ++ i) {
        printf("%d|%d\t", i, values[i]);
        avl_tree_insert(tree, &values[i], &values[i]);
    }

    int key = values[1];
    int key1 = values[2];
    printf("delete: %d\n", avl_tree_remove(tree, &key1));                   // 能否正常使用,取决于比较函数是否正确(比较的是值,还是地址)

    void* res = avl_tree_lookup(tree, &key);
    if (RET_PTR_NULL == res) {
        printf("\nNOT FOUND!\n");
    } else {
        printf("\nvalue is %d\n", *(int*)res);
    }

    printf("\ntree's node number is %d\n", avl_tree_num_entries(tree));


    /* 不好打印树, 所以用前序遍历 和 后序遍历的输出来推断树 */
    printf("\nbefore order traversal binary tree. result as follow:\n");
    before_print_tree(avl_tree_root_node(tree), my_print);                  // 前序遍历

    printf("\nMiddle order traversal binary tree. result as follow:\n");
    middle_print_tree(avl_tree_root_node(tree), my_print);                  // 中序遍历

    printf("\npost order traversal binary tree. result as follow:\n");
    postorder_print_tree(avl_tree_root_node(tree), my_print);               // 后续遍历

    avl_tree_free(tree);
    printf("\n\n");

    return 0;
}

