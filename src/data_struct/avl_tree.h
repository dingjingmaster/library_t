#ifndef AVL_TREE_H
#define AVL_TREE_H

/**
 *  平衡二叉树
 *  AVL 是一种平衡二叉树，
 *  树的每个节点都有一个 key 和与之对应的 value。
 *  节点根据 key 的顺序排列。
 *  即使修改输的节点，树依然保持平衡(树两边节点数量大致相等)
 *
 *  用途：
 *      1. 用作映射 (用于搜索)
 *      2. 总是有序的 key
 *
 *  调用：
 *      avl_tree_new --- 创建
 *      avl_tree_destroy --- 销毁
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/* AVL 平衡二叉树 */
typedef struct _AVLTree AVLTree;

/* AVL 平衡二叉树的 key */
typedef void* AVLTreeKey;

/* AVL 平衡二叉树的 value */
typedef void* AVLTreeValue;

/* AVL 平衡二叉树 节点 */
typedef struct _AVLTreeNode AVLTreeNode;

/* AVL 平衡二叉树值为空 */
#define AVL_TREE_NULL ((void*) 0)

/* 可以有 左孩子 还是 右孩子 */
typedef enum {
  AVL_TREE_NODE_LEFT = 0,
  AVL_TREE_NODE_RIGHT = 1
} AVLTreeNodeSide;


/**
 * 打印树的 key 值 key
 */
typedef void (* tree_print_key)(AVLTreeKey key);


/**
 *  比较 AVL tree 节点 key 值的函数指针
 *
 *  @param value1           第一个key
 *  @param value2           第二个key
 *  @return                 value1 <<< value2  返回负值
 *                          value1 >>> value2  返回正值
 *                          value1 === value2  返回 0
 */
typedef int (*AVLTreeCompareFunc)(AVLTreeKey value1, AVLTreeKey value2);


/**
 *  创建 AVL 树
 *
 *  @param tree             树，以便使用完后销毁
 *  @return                 成功: 返回树
 *                          失败: 返回 NUKK
 */
AVLTree* avl_tree_new(AVLTreeCompareFunc compare_func);


/**
 *  销毁 AVL 树
 *
 *  @param tree             树，要销毁
 */
void avl_tree_free(AVLTree* tree);


/**
 *  插入一个 key-value 对
 *
 *  @param tree             树
 *  @param key              要插入的 key
 *  @value                  要插入的 value
 *  @return                 成功：返回新树的Node
 *                          失败：NULL 注意：就算失败也不会内存泄漏
 */
AVLTreeNode* avl_tree_insert(AVLTree* tree, AVLTreeKey key, AVLTreeValue value);


/**
 *  删除树的一个节点
 *
 *  @param tree             树
 *  @param node             要删除的节点
 */
void avl_tree_remove_node(AVLTree* tree, AVLTreeNode* node);


/**
 *  根据 key 删除一个节点
 *
 *  @param tree             树
 *  @param key              要删除节点的 key
 *  @return                 若删除则为   非零
 *                          没找到则为   零
 */
int avl_tree_remove(AVLTree* tree, AVLTreeKey key);


/**
 * 根据 key 查询 AVL 树，相当于查询一个映射
 *
 * @param tree            树
 * @param key             要查询的key
 * @return                成功：返回找到的节点
 *                        失败：返回NULL
 */
AVLTreeNode *avl_tree_lookup_node(AVLTree *tree, AVLTreeKey key);


/**
 *  根据 key 查询 AVL 树， 返回找到的值
 *
 * @param tree            树
 * @param key             要查询的key
 * @return                成功：返回找到的☞
 *                        失败：返回 AVL_TREE_NULL
 */
AVLTreeValue avl_tree_lookup(AVLTree *tree, AVLTreeKey key);


/**
 * 查找树的根节点
 *
 * @param tree            树
 * @return                成功：返回树的根节点
 *                        失败：NULL
 */
AVLTreeNode* avl_tree_root_node(AVLTree *tree);


/**
 * 检索给定树节点的键
 *
 * @param node            树
 * @return                返回给定树的键
 */
AVLTreeKey avl_tree_node_key(AVLTreeNode *node);


/**
 * 检索给定树节点的值
 *
 * @param node            树
 * @return                返回给定树的值
 */
AVLTreeValue avl_tree_node_value(AVLTreeNode *node);


/**
 * 查询给定树节点的子节点
 *
 * @param node            树节点
 * @param side            左边或右边
 * @return                成功：返回子节点
 *                        失败：返回 NULL
 */
AVLTreeNode *avl_tree_node_child(AVLTreeNode *node, AVLTreeNodeSide side);


/**
 * 返回树节点的 父节点
 *
 * @param node            树节点
 * @return                成功：返回树节点的父节点
 *                        失败：返回NULL
 */
AVLTreeNode *avl_tree_node_parent(AVLTreeNode *node);


/**
 * 求字数的高度
 *
 * @param node            子树的根节点
 * @return                子树的高度
 */
int avl_tree_subtree_height(AVLTreeNode* node);


/**
 * 将数的 key 转换为 C 数组，这将数作为有序集合使用
 *
 * @param tree            树
 * @return                返回由键组成的数组
 */
AVLTreeValue *avl_tree_to_array(AVLTree *tree);


/**
 * 检索树的节点数
 *
 * @param tree            树
 * @return                树的节点数
 */
unsigned int avl_tree_num_entries(AVLTree *tree);

/**
 * 树的前序遍历
 */
void before_print_tree(AVLTreeNode* node, tree_print_key print);


/**
 *  树的中序遍历
 *
 */
void middle_print_tree(AVLTreeNode* node, tree_print_key print);


/**
 *  树的后序遍历
 *
 */
void postorder_print_tree(AVLTreeNode* node, tree_print_key print);
#ifdef __cplusplus
}
#endif
#endif // AVL_TREE_H
