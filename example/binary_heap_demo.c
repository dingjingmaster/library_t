#include "jbinary_heap.h"
#include <stdio.h>

int compare_func(JBinaryHeapValue v1, JBinaryHeapValue v2) {
    if ((*(int*)v1) > (*(int*)v2)) {
        return JRET_BIGGER;
    } else if ((*(int*)v1) < (*(int*)v2)) {
        return JRET_SMALLER;
    }

    return JRET_EQUAL;
}

int main(void) {

    int a = 1;
    int b = 200;
    int c = 3;
    int d = 44;
    int e = -56;
    int f = 677;
    int g = 7;
    int h = 12;
    int i = 8;
    int j = 9;
    int k = 14;
    int l = 18;

    unsigned int ii = 0;

    JBinaryHeap* minheap = JRET_PTR_NULL;
    JBinaryHeap* maxheap = JRET_PTR_NULL;

    // 最小堆
    minheap = binary_heap_new(JBINARY_HEAP_TYPE_MIN, compare_func);

    // 最大堆
    maxheap = binary_heap_new(JBINARY_HEAP_TYPE_MAX, compare_func);

    // 最小堆插入
    binary_heap_insert(minheap, &a);
    binary_heap_insert(minheap, &b);
    binary_heap_insert(minheap, &c);
    binary_heap_insert(minheap, &d);
    binary_heap_insert(minheap, &e);
    binary_heap_insert(minheap, &f);
    binary_heap_insert(minheap, &g);
    binary_heap_insert(minheap, &h);
    binary_heap_insert(minheap, &i);
    binary_heap_insert(minheap, &j);
    binary_heap_insert(minheap, &k);
    binary_heap_insert(minheap, &l);

    // 最大堆插入
    binary_heap_insert(maxheap, &a);
    binary_heap_insert(maxheap, &b);
    binary_heap_insert(maxheap, &c);
    binary_heap_insert(maxheap, &d);
    binary_heap_insert(maxheap, &e);
    binary_heap_insert(maxheap, &f);
    binary_heap_insert(maxheap, &g);
    binary_heap_insert(maxheap, &h);
    binary_heap_insert(maxheap, &i);
    binary_heap_insert(maxheap, &j);
    binary_heap_insert(maxheap, &k);
    binary_heap_insert(maxheap, &l);


    // 最小堆输出
    printf("min heap size: %d\n", binary_heap_num(minheap));
    for(ii = binary_heap_num(minheap); i > 0; --i) {
        JBinaryHeapValue v = binary_heap_pop(minheap);
        printf("%d\t", *((int*)v));
    }
    puts("\n");

    // 最大堆输出
    printf("\nmax heap size: %d\n", binary_heap_num(maxheap));
    for(ii = binary_heap_num(maxheap); i > 0; --i) {
        printf("%d\t", *((int*)binary_heap_pop(maxheap)));
    }
    puts("\n");
}
