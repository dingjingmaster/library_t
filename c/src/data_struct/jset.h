#ifndef JSET_H
#define JSET_H
#include "jret.h"
#ifdef __cplusplus
extern "C" {
#endif

enum {
    JSET_TRUE,
    JSET_FALSE,
    JSET_HAVE,
    JSET_NOT_HAVE
};

typedef struct _JSet JSet;
typedef struct _JSetIterator JSetIterator;
typedef struct _JSetEntry JSetEntry;
typedef void* JSetValue;

struct _JSetIterator {
    JSet* set;
    JSetEntry* nextEntry;
    unsigned int nextChain;
};

/**
 *  hash 函数，为值生成hash键
 */
typedef unsigned int (*JSetHashFunc) (JSetValue value);

/**
 *  比较函数, 比较两个值是否相等
 */
typedef int (*JSetEqualFunc) (JSetValue v1, JSetValue v2);

/**
 *  释放存储在 set 集合中的值
 */
typedef void (JSetFreeFunc) (JSetValue v);


/**
 *  创建一个新集合
 *
 *  @param hashFunc         生成hash值的函数
 *  @param equalFunc        检查值是否在set集合中的函数

 *  @param return           成功: 返回新集合
 *                          失败: 返回NULL
 */
JSet* jset_new(JSetHashFunc hashFunc, JSetEqualFunc equalFunc);

/**
 *  销毁集合
 *
 *  @param set              要销毁的集合
 */
void jset_free(JSet* set);


/**
 *  注册一个函数, 当值移除时调用
 *
 *  @param set              集合
 *  @param freeFunc         当值删除，要执行的操作
 */
void jset_register_free_function(JSet* set, JSetFreeFunc freeFunc);


/**
 *  set集合插入
 *  @param set              集合
 *  @param data             要插入集合的值
 *
 *  @return                 成功: 返回 JSET_TRUE
 *                          失败：返回 JSET_FALSE
 */
int jset_insert(JSet* set, JSetValue data);


/**
 *  移除值
 *
 *  @param set              集合
 *  @param data             要删除的值
 *
 *  @return                 成功：返回 JSET_TRUE
 *                          失败：返回 JSET_FALSE
 */
int jset_remove(JSet* set, JSetValue data);


/**
 *  检查 set 中是否有要查询的值
 *
 *  @param set              集合
 *  @param data             要查询的值
 *
 *  @return                 集合中存在值: JSET_HAVE
 *                          集合中不存在值: JSET_NOT_HAVE
 */
int jset_query(JSet* set, JSetValue data);


/**
 *  检索集合中值的数量
 *
 *  @param set              集合
 *
 *  @return                 集合中值的数量
 */
unsigned int jset_num_entries(JSet* set);


/**
 *  将set中的值都存放到数组中
 *
 *  @param set              集合
 *
 *  @return                 成功： 返回一个包含所有元素的数组
 *                          失败： 返回 NULL
 */
JSetValue* jset_to_array(JSet* set);


/**
 *  计算两个集合的并集
 *
 *  @param s1               集合1
 *  @param s2               集合2
 *
 *  @return                 成功： 返回一个新的，包含集合1和集合2中所有的值
 *                          失败： NULL
 */
JSet* jset_union(JSet* s1, JSet* s2);


/**
 *  计算两个集合的交集
 *
 *  @param s1               集合1
 *  @param s2               集合2
 *
 *  @return                 成功： 返回两个集合的交集
 *                          失败： NULL
 */
JSet* jset_intersection(JSet* s1, JSet* s2);



#ifdef __cplusplus
}
#endif
#endif // JSET_H
