#include "jbinary_heap.h"

#include <stdlib.h>
#include <string.h>

#define BINARY_HEAP_CAPACITY   (1024)

struct _JBinaryHeap {
    JBinaryHeapType          heapType;
    JBinaryHeapValue*        values;
    unsigned int            size;
    unsigned int            capacity;
    binary_heap_compare_cb  compareFunc;
};

static unsigned int left(unsigned int i) { return 2 * i + 1;}

static unsigned int right(unsigned int i) { return 2 * i + 2;}

static unsigned int parent(unsigned int i) { return (i - 1) / 2;}

static void swap(JBinaryHeapValue* x, JBinaryHeapValue* y) { JBinaryHeapValue tmp = *x; *x = *y; *y = tmp;}

static int value_compare(JBinaryHeap* heap, JBinaryHeapValue v1, JBinaryHeapValue v2) {
    if (heap->heapType == JBINARY_HEAP_TYPE_MIN) {
        return heap->compareFunc(v1, v2) == JRET_BIGGER ? JRET_BIGGER : JRET_SMALLER;
    }

    return heap->compareFunc(v1, v2) == JRET_BIGGER ? JRET_SMALLER : JRET_BIGGER;
}

static void heap_adjust(JBinaryHeap* heap, unsigned int i) {
    unsigned int l = left(i);
    unsigned int r = right(i);
    unsigned int st = i;

    if(l < heap->size &&\
            (JRET_SMALLER == value_compare(heap, *(heap->values + l), *(heap->values + st)))) {          // 节点和左孩子比较找最值
        st = l;
    }

    if(r < heap->size &&\
            (JRET_SMALLER == value_compare(heap, *(heap->values + r), *(heap->values + st)))) {          // 节点根右孩子比较找最值
        st = r;
    }

    if (i != st) {                                                                                      // 需要调整
        swap(heap->values + i, heap->values + st);
        heap_adjust(heap, st);
    }
}


JBinaryHeap *binary_heap_new(JBinaryHeapType type, binary_heap_compare_cb compareFunction) {
    JBinaryHeap*             heap = JRET_PTR_NULL;

    heap = malloc(sizeof (JBinaryHeap));
    if(JRET_PTR_NULL == heap) {
        return JRET_PTR_NULL;
    }

    heap->heapType = type;
    heap->compareFunc = compareFunction;
    heap->size = 0;
    /* 初始化 128 个堆空间 */
    heap->capacity = BINARY_HEAP_CAPACITY;
    heap->values = malloc(sizeof(JBinaryHeapValue) * heap->capacity);
    if (JRET_PTR_NULL == heap->values) {
        free(heap);
        return JRET_PTR_NULL;
    }
    memset(heap->values, 0, heap->capacity);

    return heap;
}

int binary_heap_insert(JBinaryHeap *heap, JBinaryHeapValue value) {
    JBinaryHeapValue*    newValue = JRET_PTR_NULL;
    unsigned int        index;
    unsigned int        newSize;
    static unsigned int heapTms;                // 最大 10 倍
    heapTms = heapTms > 10 ? 10 : heapTms + 1;

    /* 检查是否需要重新分配内存 */
    if(heap->size >= heap->capacity) {
        newSize = heap->capacity + BINARY_HEAP_CAPACITY * heapTms;
        newValue = realloc(heap->values, sizeof (JBinaryHeapValue) * newSize);
        if(JRET_PTR_NULL == newValue) {
            return JRET_ERROR;
        }
        heap->capacity = newSize;
        heap->values = newValue;
    }

    /* 添加新值 */
    index = heap->size;
    *(heap->values + index) = value;
    ++ heap->size;
    while (index > 0 && (JRET_BIGGER == value_compare(heap, *(heap->values + parent(index)), *(heap->values + index)))) {
        swap(heap->values + index, heap->values + parent(index));
        index = parent(index);
    }

    return JRET_OK;
}

JBinaryHeapValue binary_heap_pop(JBinaryHeap *heap) {
    JBinaryHeapValue     popValue;

    /* 是否为空堆 */
    if(0 == heap->size) {
        return JBINARY_HEAP_NULL;
    }

    if(1 == heap->size) {
        -- heap->size;
        return *heap->values;
    }

    /* 从堆顶取元素 */
    popValue = *heap->values;

    /* 删除堆顶元素 */
    -- heap->size;
    *heap->values = *(heap->values + heap->size);

    heap_adjust(heap, 0);

    return popValue;
}

unsigned int binary_heap_num(JBinaryHeap *heap) {
    return heap->size;
}

void binary_heap_free(JBinaryHeap *heap) {
    free(heap->values);
    free(heap);
}
