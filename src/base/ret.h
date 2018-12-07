#ifndef RET_H
#define RET_H

#ifdef __cplusplus
extern "C"
#endif

/**
 *  程序返回宏定义及解释, 我所有程序返回都依赖此文件
 *  宏长度默认 32bit
 *
 *  正常：返回值 >= 0
 *  异常：返回值 <  0  表示比较严重的错误，需要 return 或 exit
 */

#define         RET_OK                                  (int)(0X00000000)   /* 正常返回 0 为征程 */
#define         RET_BIGGER                              (int)(0X00000001)   /* 比较结果————较大 */
#define         RET_EQUAL                               (int)(0X00000002)   /* 比较结果————相等 */
#define         RET_SMALLER                             (int)(0X00000003)   /* 比较结果————较小 */
#define         RET_NOTFOUND                            (int)(0X00000004)   /* 查找 ———— 未找到 */

#define         RET_ERROR                               (int)(0XFFFFFFFF)   /* 错误返回 <0 为异常 */
#define         RET_NULL                                (int)(0XFFFFFFFE)   /* 返回————空值 */

#define         RET_PTR_NULL                            (NULL)              /* 返回————指针 NULL */


#ifdef __cplusplus
}
#endif
#endif
