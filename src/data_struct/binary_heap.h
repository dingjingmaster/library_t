#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 堆空值 */
#define BINARY_HEAP_NULL ((void *) 0)

/* 堆类型——最大堆/最小堆 */
typedef enum {
    BINARY_HEAP_TYPE_MIN,
    BINARY_HEAP_TYPE_MAX
} BinaryHeapType;

/* 堆结构 */
typedef struct _BinaryHeap BinaryHeap;

/* 堆中存储的值 */
typedef void* BinaryHeapValue;

/**
 * 堆中用来比较大小的函数指针类型
 *
 * @return: value1等于value2返回 0
 *          value1小于value2返回 -1
 *          value1大于value2返回 1
 */
typedef int (* binary_heap_compare_cb) (BinaryHeapValue value1, BinaryHeapValue value2);

/**
 * 创建新的堆
 * @param type:                     堆类型
 * @param compareFunction:          值比较函数
 *
 * @return 返回新的堆, 如果是 NULL 则返回堆失败
 */
BinaryHeap* binary_heap_new(BinaryHeapType type, binary_heap_compare_cb compareFunction);


/**
 * 释放堆
 * @param heap:                     要是放的堆指针
 * 注意: 只释放了本堆自己申请的内存空间，至于用户自己申请的堆 value 需要用户自己去释放
 */
void binary_heap_free(BinaryHeap* heap);


/**
 * 插入值
 * @param heap:                     堆
 * @param value:                    要插入的值
 *
 * @return                          0 成功, 否则失败
 */
int binary_heap_insert(BinaryHeap* heap, BinaryHeapValue value);


/**
 * 弹出堆顶元素
 * @param heap:                     堆
 *
 * @return                          成功:返回堆顶元素, 失败: 返回 BINARY_HEAP_NULL
 */
BinaryHeapValue binary_heap_pop(BinaryHeap* heap);


/**
 * 堆中值得数量
 * @param heap:                     堆
 *
 * @return                          堆中值得数量
 */
unsigned int binary_heap_num(BinaryHeap* heap);

#ifdef __cplusplus
}
#endif
#endif // BINARY_HEAP_H
