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
    src/base/jret.h \
    src/data_struct/javl_tree.h \
    src/data_struct/jbinary_heap.h

# source
SOURCES += \
    src/data_struct/javl_tree.c \
    src/data_struct/jbinary_heap.c

#========================== demo ========================
SOURCES += \
#    main.c\
    example/avl_tree_demo.c\
#    example/binary_heap_demo.c\
