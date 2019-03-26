#include "javl_tree.h"
#include <stdlib.h>


/* AVL 平衡二叉树的节点 */
struct _JAVLTreeNode {
    JAVLTreeNode*           children[2];
    JAVLTreeNode*           parent;
    JAVLTreeKey             key;
    JAVLTreeValue           value;
    int                     height;
};


/* AVL 平衡二叉树 */
struct _JAVLTree {
    JAVLTreeNode*           rootNode;
    JAVLTreeCompareFunc     compareFunc;
    unsigned int            numNodes;
};


/* 递归删除子树节点(后序遍历) */
static void avl_tree_subtree(JAVLTree* tree, JAVLTreeNode* node) {
    if (JRET_PTR_NULL == node) {
        return;
    }

    avl_tree_subtree(tree, node->children[JAVL_TREE_NODE_LEFT]);
    avl_tree_subtree(tree, node->children[JAVL_TREE_NODE_RIGHT]);
    free(node);
}

/* 更新子节点高度值 */
static void avl_tree_update_height(JAVLTreeNode* node) {

    JAVLTreeNode*            leftSubTree = JRET_PTR_NULL;
    JAVLTreeNode*            rightSubTree = JRET_PTR_NULL;
    int                     leftHeight, rightHeight = 0;

    leftSubTree = node->children[JAVL_TREE_NODE_LEFT];
    rightSubTree = node->children[JAVL_TREE_NODE_RIGHT];

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
static JAVLTreeNodeSide avl_tree_node_parent_sider(JAVLTreeNode* node) {
    if(node == node->parent->children[JAVL_TREE_NODE_LEFT]) {
        return JAVL_TREE_NODE_LEFT;
    } else {
        return JAVL_TREE_NODE_RIGHT;
    }
}

/**
 * 将 node2(node1 下较大的子节点) node1(node1 的左右子树失衡)
 * 旋转前node2 是 node1 的子节点, 旋转后 node2 变为父节点 node1 变为子节点
 */
static void avl_tree_node_replace(JAVLTree* tree, JAVLTreeNode* node1, JAVLTreeNode* node2) {
    JAVLTreeNodeSide             side;

    if(JRET_PTR_NULL != node2) {
        node2->parent = node1->parent;              // 将 node2 变为该子树的根结点
    }

    /* 是否是根节点 */
    if(JRET_PTR_NULL == node1->parent) {            // 检测 node1 之前是不是根节点
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
static JAVLTreeNode *avl_tree_rotate(JAVLTree *tree, JAVLTreeNode *node, JAVLTreeNodeSide direction) {

    JAVLTreeNode                     *newRoot = JRET_PTR_NULL;

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

    if (node->children[1 - direction] != JRET_PTR_NULL) {
        node->children[1 - direction]->parent = node;
    }

    avl_tree_update_height(newRoot);                                                    // 更新 y 及子节点高度
    avl_tree_update_height(node);                                                       // 更新 z 及子节点高度

    return newRoot;
}


/* 返回子树高度 */
int avl_tree_subtree_height(JAVLTreeNode* node) {
    if (JRET_PTR_NULL == node) {
        return 0;
    }

    return node->height;
}

/* avl 树平衡 */
static JAVLTreeNode *avl_tree_node_balance(JAVLTree *tree, JAVLTreeNode *node) {
    JAVLTreeNode *left_subtree = JRET_PTR_NULL;
    JAVLTreeNode *right_subtree = JRET_PTR_NULL;
    JAVLTreeNode *child = JRET_PTR_NULL;
    int diff;

    left_subtree = node->children[JAVL_TREE_NODE_LEFT];
    right_subtree = node->children[JAVL_TREE_NODE_RIGHT];

    diff = avl_tree_subtree_height(right_subtree)
        - avl_tree_subtree_height(left_subtree);

    if (diff >= 2) {                                                                    // 右>左 需要旋转, 确认具体怎么旋转
        child = right_subtree;
        if (avl_tree_subtree_height(child->children[JAVL_TREE_NODE_RIGHT])
                < avl_tree_subtree_height(child->children[JAVL_TREE_NODE_LEFT])) {      // 右<左 属于 右左 --- 先左旋转 再 右旋转
            avl_tree_rotate(tree, right_subtree, JAVL_TREE_NODE_RIGHT);
        }

        node = avl_tree_rotate(tree, node, JAVL_TREE_NODE_LEFT);
    } else if (diff <= -2) {                                                            // 左>右
        child = node->children[JAVL_TREE_NODE_LEFT];
        if (avl_tree_subtree_height(child->children[JAVL_TREE_NODE_LEFT])
          < avl_tree_subtree_height(child->children[JAVL_TREE_NODE_RIGHT])) {           // 左<右 属于 左右 --- 先右旋转 再 左旋转
            avl_tree_rotate(tree, left_subtree, JAVL_TREE_NODE_LEFT);
        }

        node = avl_tree_rotate(tree, node, JAVL_TREE_NODE_RIGHT);
    }

    avl_tree_update_height(node);                                                       // 更新节点高度

    return node;
}

/* 从给定节点开始到跟节点, 针对需要执行旋转的子树进行旋转操作 */
static void avl_tree_balance_to_root(JAVLTree *tree, JAVLTreeNode *node) {
    JAVLTreeNode *rover;
    rover = node;
    while (rover != JRET_PTR_NULL) {
        rover = avl_tree_node_balance(tree, rover);
        rover = rover->parent;
    }
}


/* 创建 */
JAVLTree* avl_tree_new(JAVLTreeCompareFunc compare_func){
    JAVLTree*                newTree = JRET_PTR_NULL;

    newTree = (JAVLTree*)malloc(sizeof (JAVLTree));
    if(JRET_PTR_NULL == newTree) {
        return JRET_PTR_NULL;
    }
    newTree->rootNode = JRET_PTR_NULL;
    newTree->compareFunc = compare_func;
    newTree->numNodes = 0;

    return newTree;
}


/* 销毁 */
void avl_tree_free(JAVLTree* tree) {
    avl_tree_subtree(tree, tree->rootNode);
    free(tree);
}


/**
 *  插入 key value
 *      1. 从根节点向下查找,找到叶子结点再插入
 *
 */
JAVLTreeNode *avl_tree_insert(JAVLTree *tree, JAVLTreeKey key, JAVLTreeValue value) {
    JAVLTreeNode **rover;
    JAVLTreeNode *newNode;
    JAVLTreeNode *previousNode;

    rover = &tree->rootNode;
    previousNode = JRET_PTR_NULL;

    while (*rover != JRET_PTR_NULL) {
        previousNode = *rover;
        if (tree->compareFunc(key, (*rover)->key) < 0) {
            rover = &((*rover)->children[JAVL_TREE_NODE_LEFT]);
        } else {
            rover = &((*rover)->children[JAVL_TREE_NODE_RIGHT]);
        }
    }

    newNode = (JAVLTreeNode *) malloc(sizeof(JAVLTreeNode));         // 找到叶子节点后根据 key value 创建新节点
    if (JRET_PTR_NULL == newNode) {
        return JRET_PTR_NULL;
    }

    newNode->children[JAVL_TREE_NODE_LEFT] = JRET_PTR_NULL;
    newNode->children[JAVL_TREE_NODE_RIGHT] = JRET_PTR_NULL;
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
static JAVLTreeNode *avl_tree_node_get_replacement(JAVLTree *tree, JAVLTreeNode *node) {
    JAVLTreeNode *leftSubtree;
    JAVLTreeNode *rightSubtree;
    JAVLTreeNode *result;
    JAVLTreeNode *child;
    int leftHeight, rightHeight = 0;
    int side;

    leftSubtree = node->children[JAVL_TREE_NODE_LEFT];
    rightSubtree = node->children[JAVL_TREE_NODE_RIGHT];

    // 没有子树则开始返回(迭代结束条件)
    if (JRET_PTR_NULL == leftSubtree && JRET_PTR_NULL == rightSubtree) {
        return JRET_PTR_NULL;
    }

    leftHeight = avl_tree_subtree_height(leftSubtree);
    rightHeight = avl_tree_subtree_height(rightSubtree);

    if (leftHeight < rightHeight) {
        side = JAVL_TREE_NODE_RIGHT;
    } else {
        side = JAVL_TREE_NODE_LEFT;
    }
    result = node->children[side];
    while (result->children[1 - side] != JRET_PTR_NULL) {
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
void avl_tree_remove_node(JAVLTree *tree, JAVLTreeNode *node) {

    JAVLTreeNode *swapNode;
    JAVLTreeNode *balanceStartpoint;
    int i;

    swapNode = avl_tree_node_get_replacement(tree, node);
    if (JRET_PTR_NULL == swapNode) {
        avl_tree_node_replace(tree, node, JRET_PTR_NULL);
        balanceStartpoint = node->parent;

    } else {
        if (swapNode->parent == node) {
            balanceStartpoint = swapNode;
        } else {
            balanceStartpoint = swapNode->parent;
        }
        for (i=0; i<2; ++i) {
            swapNode->children[i] = node->children[i];
            if (swapNode->children[i] != JRET_PTR_NULL) {
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
int avl_tree_remove(JAVLTree *tree, JAVLTreeKey key) {
    JAVLTreeNode                         *node = JRET_PTR_NULL;

    node = avl_tree_lookup_node(tree, key);
    if (node == JRET_PTR_NULL) {
        return 0;
    }
    avl_tree_remove_node(tree, node);

    return 1;
}


/**
 *  查询 key 在子树种的节点
 */
JAVLTreeNode *avl_tree_lookup_node(JAVLTree *tree, JAVLTreeKey key) {
    JAVLTreeNode                         *node = JRET_PTR_NULL;
    int                                 diff;

    node = tree->rootNode;
    while (node != JRET_PTR_NULL) {
        diff = tree->compareFunc(key, node->key);
        if (diff == JRET_EQUAL) {
            return node;
        } else if (diff == JRET_SMALLER) {
            node = node->children[JAVL_TREE_NODE_LEFT];
        } else {
            node = node->children[JAVL_TREE_NODE_RIGHT];
        }
    }

    return JRET_PTR_NULL;
}

JAVLTreeValue avl_tree_lookup(JAVLTree *tree, JAVLTreeKey key) {
    JAVLTreeNode *node = JRET_PTR_NULL;

    node = avl_tree_lookup_node(tree, key);
    if (node == JRET_PTR_NULL) {
        return JAVL_TREE_NULL;
    } else {
        return node->value;
    }
}

JAVLTreeNode* avl_tree_root_node(JAVLTree *tree) {
    return tree->rootNode;
}

JAVLTreeKey avl_tree_node_key(JAVLTreeNode *node) {
    return node->key;
}

JAVLTreeValue avl_tree_node_value(JAVLTreeNode *node) {
    return node->value;
}

/* 返回树的字节点 */
JAVLTreeNode *avl_tree_node_child(JAVLTreeNode *node, JAVLTreeNodeSide side) {
    if (side == JAVL_TREE_NODE_LEFT || side == JAVL_TREE_NODE_RIGHT) {
        return node->children[side];
    } else {
        return JRET_PTR_NULL;
    }
}

JAVLTreeNode *avl_tree_node_parent(JAVLTreeNode *node) {
    return node->parent;
}

/* 树中节点数量 */
unsigned int avl_tree_num_entries(JAVLTree *tree) {
    return tree->numNodes;
}

static void avl_tree_to_array_add_subtree(JAVLTreeNode *subtree, JAVLTreeValue *array, int *index) {
    if (subtree == JRET_PTR_NULL) {
        return;
    }

    avl_tree_to_array_add_subtree(subtree->children[JAVL_TREE_NODE_LEFT], array, index);
    array[*index] = subtree->key;
    ++*index;
    avl_tree_to_array_add_subtree(subtree->children[JAVL_TREE_NODE_RIGHT], array, index);
}

/* 以后序遍历的方式把值copy到数组 */
JAVLTreeValue *avl_tree_to_array(JAVLTree *tree) {
    JAVLTreeValue *array;
    int index = 0;

    array = malloc(sizeof(JAVLTreeValue) * tree->numNodes);
    if (array == JRET_PTR_NULL) {
        return JRET_PTR_NULL;
    }
    avl_tree_to_array_add_subtree(tree->rootNode, array, &index);

    return array;
}


void before_print_tree(JAVLTreeNode* node, tree_print_key print) {
    if (JRET_PTR_NULL == node) {
        return;
    }

    print(node->key);

    before_print_tree(node->children[JAVL_TREE_NODE_LEFT], print);
    before_print_tree(node->children[JAVL_TREE_NODE_RIGHT], print);
}


void middle_print_tree(JAVLTreeNode* node, tree_print_key print) {
    if (JRET_PTR_NULL == node) {
        return;
    }

    middle_print_tree(node->children[JAVL_TREE_NODE_LEFT], print);
    print(node->key);
    middle_print_tree(node->children[JAVL_TREE_NODE_RIGHT], print);
}


void postorder_print_tree(JAVLTreeNode* node, tree_print_key print) {
    if (JRET_PTR_NULL == node) {
        return;
    }

    postorder_print_tree(node->children[JAVL_TREE_NODE_LEFT], print);
    postorder_print_tree(node->children[JAVL_TREE_NODE_RIGHT], print);
    print(node->key);
}
