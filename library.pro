TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# flags
QMAKE_CXXFLAGS += -Wall

# head path
INCLUDEPATH += \
    src/base/\
    src/data_struct/\

# head
HEADERS += \
    src/data_struct/binary_heap.h \
    src/data_struct/avl_tree.h\
    src/base/ret.h

# source
SOURCES += \
    src/data_struct/binary_heap.c \
    src/data_struct/avl_tree.c

#========================== demo ========================
SOURCES += \
#    main.c\
#    example/avl_tree_demo.c\
    example/binary_heap_demo.c\
