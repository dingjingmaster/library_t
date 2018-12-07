#include "binary_heap.h"

#define BINARY_HEAP_CAPACITY   (1024)

struct _BinaryHeap {
    BinaryHeapType          heapType;
    BinaryHeapValue*        values;
    unsigned int            size;
    unsigned int            capacity;
    binary_heap_compare_cb  compareFunc;
};

static unsigned int left(unsigned int i) { return 2 * i + 1;}

static unsigned int right(unsigned int i) { return 2 * i + 2;}

static unsigned int parent(unsigned int i) { return (i - 1) / 2;}

static void swap(BinaryHeapValue* x, BinaryHeapValue* y) { BinaryHeapValue tmp = *x; *x = *y; *y = tmp;}

static int value_compare(BinaryHeap* heap, BinaryHeapValue v1, BinaryHeapValue v2) {
    if (heap->heapType == BINARY_HEAP_TYPE_MIN) {
        return heap->compareFunc(v1, v2);
    } else {
        return -heap->compareFunc(v1, v2);
    }
}

static void heap_adjust(BinaryHeap* heap, unsigned int i) {
    unsigned int l = left(i);
    unsigned int r = right(i);
    unsigned int st = i;

    if(l < heap->size &&\
            value_compare(heap, *(heap->values + l), *(heap->values + st)) < 0) {      // 节点和左孩子比较找最值
        st = l;
    }

    if(r < heap->size &&\
            value_compare(heap, *(heap->values + r), *(heap->values + st)) < 0) {       // 节点根右孩子比较找最值
        st = r;
    }

    if (i != st) {                                                                      // 需要调整
        swap(heap->values + i, heap->values + st);
        heap_adjust(heap, st);
    }
}



BinaryHeap *binary_heap_new(BinaryHeapType type, binary_heap_compare_cb compareFunction) {
    BinaryHeap*             heap;

    heap = malloc(sizeof (BinaryHeap));
    if(NULL == heap) {
        return NULL;
    }

    heap->heapType = type;
    heap->compareFunc = compareFunction;
    heap->size = 0;
    /* 初始化 128 个堆空间 */
    heap->capacity = BINARY_HEAP_CAPACITY;
    heap->values = malloc(sizeof(BinaryHeapValue) * heap->capacity);
    if (NULL == heap->values) {
        free(heap);
        return NULL;
    }
    memset(heap->values, 0, heap->capacity);

    return heap;
}

int binary_heap_insert(BinaryHeap *heap, BinaryHeapValue value) {
    BinaryHeapValue*    newValue;
    unsigned int        index;
    unsigned int        newSize;
    static unsigned int heapTms;                // 最大 10 倍
    heapTms = heapTms > 10 ? 10 : heapTms + 1;

    /* 检查是否需要重新分配内存 */
    if(heap->size >= heap->capacity) {
        newSize = heap->capacity + BINARY_HEAP_CAPACITY * heapTms;
        newValue = realloc(heap->values, sizeof (BinaryHeapValue) * newSize);
        if(NULL == newValue) {
            return -1;
        }
        heap->capacity = newSize;
        heap->values = newValue;
    }

    /* 添加新值 */
    index = heap->size;
    *(heap->values + index) = value;
    ++ heap->size;
    while (index > 0 && value_compare(heap, *(heap->values + parent(index)), *(heap->values + index)) > 0) {
        swap(heap->values + index, heap->values + parent(index));
        index = parent(index);
    }

    return 0;
}

BinaryHeapValue binary_heap_pop(BinaryHeap *heap) {
    BinaryHeapValue     popValue;

    /* 是否为空堆 */
    if(0 == heap->size) {
        return BINARY_HEAP_NULL;
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

unsigned int binary_heap_num(BinaryHeap *heap) {
    return heap->size;
}

void binary_heap_free(BinaryHeap *heap) {
    free(heap->values);
    free(heap);
}
