#include "avl_tree.h"
#include <stdlib.h>


/* AVL 平衡二叉树的节点 */
struct _AVLTreeNode {
    AVLTreeNode*            children[2];
    AVLTreeNode*            parent;
    AVLTreeKey              key;
    AVLTreeValue            value;
    int                     height;
};


/* AVL 平衡二叉树 */
struct _AVLTree {
    AVLTreeNode*            rootNode;
    AVLTreeCompareFunc      compareFunc;
    unsigned int            numNodes;
};


/* 递归删除子树节点(后序遍历) */
static void avl_tree_subtree(AVLTree* tree, AVLTreeNode* node) {
    if (NULL == node) {
        return;
    }

    avl_tree_subtree(tree, node->children[AVL_TREE_NODE_LEFT]);
    avl_tree_subtree(tree, node->children[AVL_TREE_NODE_RIGHT]);
    free(node);
}

/* 更新子节点高度值 */
static void avl_tree_update_height(AVLTreeNode* node) {

    AVLTreeNode*            leftSubTree = NULL;
    AVLTreeNode*            rightSubTree = NULL;
    int                     leftHeight, rightHeight = 0;

    leftSubTree = node->children[AVL_TREE_NODE_LEFT];
    rightSubTree = node->children[AVL_TREE_NODE_RIGHT];

    leftHeight = avl_tree_subtree_height(leftSubTree);
    rightHeight = avl_tree_subtree_height(rightSubTree);

    /* 二者大的计算高度, 叶子节点高度为 1 */
    if(leftHeight > rightHeight) {
        node->height = leftHeight + 1;
    } else {
        node->height = rightHeight + 1;
    }
}

/* 节点相对于父节点的哪一边 */
static AVLTreeNodeSide avl_tree_node_parent_sider(AVLTreeNode* node) {
    if(node == node->parent->children[AVL_TREE_NODE_LEFT]) {
        return AVL_TREE_NODE_LEFT;
    } else {
        return AVL_TREE_NODE_RIGHT;
    }
}

/**
 * 将 node2(node1 下较大的子节点) node1(node1 的左右子树失衡)
 * 旋转前node2 是 node1 的子节点, 旋转后 node2 变为父节点 node1 变为子节点
 */
static void avl_tree_node_replace(AVLTree* tree, AVLTreeNode* node1, AVLTreeNode* node2) {
    AVLTreeNodeSide             side;

    if(NULL != node2) {
        node2->parent = node1->parent;              // 将 node2 变为该子树的根结点
    }

    /* 是否是根节点 */
    if(NULL == node1->parent) {                     // 检测 node1 之前是不是根节点
        tree->rootNode = node2;
    } else {
        side = avl_tree_node_parent_sider(node1);   // node1 是左子树还是右子树
        node1->parent->children[side] = node2;      // node2 加入到 node1 父节点的子节点中
        avl_tree_update_height(node1->parent);      // 更新子节点高度
    }
}


/**
 * 旋转树的一部分。'node' 表示待旋转部分的根结点; 'direction' 表示旋转方向: 左旋、右旋
 *    左旋转:                        右旋转
 *
 *      B                             D
 *     / \                           / \
 *    A   D                         B   E
 *       / \                       / \
 *      C   E                     A   C
 *
 *    旋转为:                      旋转为:
 *        D                           B
 *       / \                         / \
 *      B   E                       A   D
 *     / \                             / \
 *    A   C                           C   E
 *
 *  右边孩子左旋转、左边孩子右旋转
 */
static AVLTreeNode *avl_tree_rotate(AVLTree *tree, AVLTreeNode *node, AVLTreeNodeSide direction) {

    AVLTreeNode                     *newRoot;

    /**
     * 针对如下失衡子树(括号里表示高度)
     *                  z (4)                               (3)  y
     *                /  \                                    /    \
     *           (3) y   T4 (1)         ------>          (2)x      z
     *             /  \                                   /  \   /  \
     *         (2)x   T3 (1)                          (1)T1  T2 T3  T4
     *          /  \
     *        T1  T2(1)
     *
     *  显然 z 节点左右子节点失衡,且左子树高度大于右子树高度,需要右旋转
     *  第一步:
     *      找到失衡的左孩子: y (newRoot)
     *      并将 y 变为旋转后子树的根节点，更改 y 的父指针
     *  第二步:
     *      更改失衡子树的 root 节点 ()
     *
     */
    newRoot = node->children[1 - direction];
    avl_tree_node_replace(tree, node, newRoot);                                         // 将 newRoot 位置上移 --- y 提到根部
    node->children[1 - direction] = newRoot->children[direction];                       // z 的做节点指向 y调整之前指向的右子树 T3
    newRoot->children[direction] = node;                                                // y 的右节点指向 z
    node->parent = newRoot;                                                             // 更新 z 指向的父节点

    if (node->children[1 - direction] != NULL) {
        node->children[1 - direction]->parent = node;
    }

    avl_tree_update_height(newRoot);                                                    // 更新 y 及子节点高度
    avl_tree_update_height(node);                                                       // 更新 z 及子节点高度

    return newRoot;
}


/* 返回子树高度 */
int avl_tree_subtree_height(AVLTreeNode* node) {
    if (NULL == node) {
        return 0;
    }

    return node->height;
}

/* avl 树平衡 */
static AVLTreeNode *avl_tree_node_balance(AVLTree *tree, AVLTreeNode *node) {
    AVLTreeNode *left_subtree;
    AVLTreeNode *right_subtree;
    AVLTreeNode *child;
    int diff;

    left_subtree = node->children[AVL_TREE_NODE_LEFT];
    right_subtree = node->children[AVL_TREE_NODE_RIGHT];

    diff = avl_tree_subtree_height(right_subtree)
        - avl_tree_subtree_height(left_subtree);

    if (diff >= 2) {                                                                    // 右>左 需要旋转, 确认具体怎么旋转
        child = right_subtree;
        if (avl_tree_subtree_height(child->children[AVL_TREE_NODE_RIGHT]) 
                < avl_tree_subtree_height(child->children[AVL_TREE_NODE_LEFT])) {       // 右<左 属于 右左 --- 先左旋转 再 右旋转
            avl_tree_rotate(tree, right_subtree, AVL_TREE_NODE_RIGHT);
        }

        node = avl_tree_rotate(tree, node, AVL_TREE_NODE_LEFT);
    } else if (diff <= -2) {                                                            // 左>右
        child = node->children[AVL_TREE_NODE_LEFT];
        if (avl_tree_subtree_height(child->children[AVL_TREE_NODE_LEFT])
          < avl_tree_subtree_height(child->children[AVL_TREE_NODE_RIGHT])) {            // 左<右 属于 左右 --- 先右旋转 再 左旋转
            avl_tree_rotate(tree, left_subtree, AVL_TREE_NODE_LEFT);
        }

        node = avl_tree_rotate(tree, node, AVL_TREE_NODE_RIGHT);
    }

    avl_tree_update_height(node);                                                       // 更新节点高度

    return node;
}

/* 从给定节点开始到跟节点, 针对需要执行旋转的子树进行旋转操作 */
static void avl_tree_balance_to_root(AVLTree *tree, AVLTreeNode *node) {
    AVLTreeNode *rover;
    rover = node;
    while (rover != NULL) {
        rover = avl_tree_node_balance(tree, rover);
        rover = rover->parent;
    }
}


/* 创建 */
AVLTree* avl_tree_new(AVLTreeCompareFunc compare_func){
    AVLTree*                newTree;

    newTree = (AVLTree*)malloc(sizeof (AVLTree));
    if(NULL == newTree) {
        return NULL;
    }
    newTree->rootNode = NULL;
    newTree->compareFunc = compare_func;
    newTree->numNodes = 0;

    return newTree;
}


/* 销毁 */
void avl_tree_free(AVLTree* tree) {
    avl_tree_subtree(tree, tree->rootNode);
    free(tree);
}


/**
 *  插入 key value
 *      1. 从根节点向下查找,找到叶子结点再插入
 *
 */
AVLTreeNode *avl_tree_insert(AVLTree *tree, AVLTreeKey key, AVLTreeValue value) {
    AVLTreeNode **rover;
    AVLTreeNode *newNode;
    AVLTreeNode *previousNode;

    rover = &tree->rootNode;
    previousNode = NULL;

    while (*rover != NULL) {
        previousNode = *rover;
        if (tree->compareFunc(key, (*rover)->key) < 0) {
            rover = &((*rover)->children[AVL_TREE_NODE_LEFT]);
        } else {
            rover = &((*rover)->children[AVL_TREE_NODE_RIGHT]);
        }
    }

    newNode = (AVLTreeNode *) malloc(sizeof(AVLTreeNode));          // 找到叶子节点后根据 key value 创建新节点
    if (NULL == newNode) {
        return NULL;
    }

    newNode->children[AVL_TREE_NODE_LEFT] = NULL;
    newNode->children[AVL_TREE_NODE_RIGHT] = NULL;
    newNode->parent = previousNode;                                 // 将新节点加入树中
    newNode->key = key;
    newNode->value = value;
    newNode->height = 1;                                            // 此时新节点变为了树的叶子节点
    *rover = newNode;                                               // 更新目前节点的指针 --- 为了向上更新高度
    avl_tree_balance_to_root(tree, previousNode);                   // 重新平衡二叉树
    ++ tree->numNodes;                                              // 树的节点加1

    return newNode;
}

/**
 *  根据给定 node 查找树中最近的 node 节点并替代(旋转过程中的子树替换)
 *  没找到返回 NULL
 *  找到则返回节点
 */
static AVLTreeNode *avl_tree_node_get_replacement(AVLTree *tree, AVLTreeNode *node) {
    AVLTreeNode *leftSubtree;
    AVLTreeNode *rightSubtree;
    AVLTreeNode *result;
    AVLTreeNode *child;
    int leftHeight, rightHeight = 0;
    int side;

    leftSubtree = node->children[AVL_TREE_NODE_LEFT];
    rightSubtree = node->children[AVL_TREE_NODE_RIGHT];

    // 没有子树则开始返回(迭代结束条件)
    if (NULL == leftSubtree && NULL == rightSubtree) {
        return NULL;
    }

    leftHeight = avl_tree_subtree_height(leftSubtree);
    rightHeight = avl_tree_subtree_height(rightSubtree);

    if (leftHeight < rightHeight) {
        side = AVL_TREE_NODE_RIGHT;
    } else {
        side = AVL_TREE_NODE_LEFT;
    }
    result = node->children[side];
    while (result->children[1 - side] != NULL) {
        result = result->children[1 - side];
    }
    child = result->children[side];
    avl_tree_node_replace(tree, result, child);
    avl_tree_update_height(result->parent);

    return result;
}

/**
 *  从指定树删除一个节点
 *
 */
void avl_tree_remove_node(AVLTree *tree, AVLTreeNode *node) {

    AVLTreeNode *swapNode;
    AVLTreeNode *balanceStartpoint;
    int i;

    swapNode = avl_tree_node_get_replacement(tree, node);
    if (NULL == swapNode) {
        avl_tree_node_replace(tree, node, NULL);
        balanceStartpoint = node->parent;

    } else {
        if (swapNode->parent == node) {
            balanceStartpoint = swapNode;
        } else {
            balanceStartpoint = swapNode->parent;
        }
        for (i=0; i<2; ++i) {
            swapNode->children[i] = node->children[i];
            if (swapNode->children[i] != NULL) {
                swapNode->children[i]->parent = swapNode;
            }
        }
        swapNode->height = node->height;
        avl_tree_node_replace(tree, node, swapNode);
    }

    free(node);
    --tree->numNodes;
    avl_tree_balance_to_root(tree, balanceStartpoint);
}

/* 通过比较 key 的值进行删除 */
int avl_tree_remove(AVLTree *tree, AVLTreeKey key) {
    AVLTreeNode                         *node;

    node = avl_tree_lookup_node(tree, key);
    if (node == NULL) {
        return 0;
    }
    avl_tree_remove_node(tree, node);

    return 1;
}


/**
 *  查询 key 在子树种的节点
 */
AVLTreeNode *avl_tree_lookup_node(AVLTree *tree, AVLTreeKey key) {
    AVLTreeNode                         *node;
    int                                 diff;

    node = tree->rootNode;
    while (node != NULL) {
        diff = tree->compareFunc(key, node->key);
        if (diff == 0) {
            return node;
        } else if (diff < 0) {
            node = node->children[AVL_TREE_NODE_LEFT];
        } else {
            node = node->children[AVL_TREE_NODE_RIGHT];
        }
    }

    return NULL;
}

AVLTreeValue avl_tree_lookup(AVLTree *tree, AVLTreeKey key) {
    AVLTreeNode *node;

    node = avl_tree_lookup_node(tree, key);
    if (node == NULL) {
        return AVL_TREE_NULL;
    } else {
        return node->value;
    }
}

AVLTreeNode* avl_tree_root_node(AVLTree *tree) {
    return tree->rootNode;
}

AVLTreeKey avl_tree_node_key(AVLTreeNode *node) {
    return node->key;
}

AVLTreeValue avl_tree_node_value(AVLTreeNode *node) {
    return node->value;
}

/* 返回树的字节点 */
AVLTreeNode *avl_tree_node_child(AVLTreeNode *node, AVLTreeNodeSide side) {
    if (side == AVL_TREE_NODE_LEFT || side == AVL_TREE_NODE_RIGHT) {
        return node->children[side];
    } else {
        return NULL;
    }
}

AVLTreeNode *avl_tree_node_parent(AVLTreeNode *node) {
    return node->parent;
}

/* 树中节点数量 */
unsigned int avl_tree_num_entries(AVLTree *tree) {
    return tree->numNodes;
}

static void avl_tree_to_array_add_subtree(AVLTreeNode *subtree, AVLTreeValue *array, int *index) {
    if (subtree == NULL) {
        return;
    }

    avl_tree_to_array_add_subtree(subtree->children[AVL_TREE_NODE_LEFT], array, index);
    array[*index] = subtree->key;
    ++*index;
    avl_tree_to_array_add_subtree(subtree->children[AVL_TREE_NODE_RIGHT], array, index);
}

/* 以后序遍历的方式把值copy到数组 */
AVLTreeValue *avl_tree_to_array(AVLTree *tree) {
    AVLTreeValue *array;
    int index = 0;

    array = malloc(sizeof(AVLTreeValue) * tree->numNodes);
    if (array == NULL) {
        return NULL;
    }
    avl_tree_to_array_add_subtree(tree->rootNode, array, &index);

    return array;
}


void before_print_tree(AVLTreeNode* node, tree_print_key print) {
    if (NULL == node) {
        return;
    }

    print(node->key);

    before_print_tree(node->children[AVL_TREE_NODE_LEFT], print);
    before_print_tree(node->children[AVL_TREE_NODE_RIGHT], print);
}


void middle_print_tree(AVLTreeNode* node, tree_print_key print) {

    if (NULL == node) {
        return;
    }

    middle_print_tree(node->children[AVL_TREE_NODE_LEFT], print);
    print(node->key);
    middle_print_tree(node->children[AVL_TREE_NODE_RIGHT], print);
}


void postorder_print_tree(AVLTreeNode* node, tree_print_key print) {
    if (NULL == node) {
        return;
    }

    postorder_print_tree(node->children[AVL_TREE_NODE_LEFT], print);
    postorder_print_tree(node->children[AVL_TREE_NODE_RIGHT], print);
    print(node->key);
}
