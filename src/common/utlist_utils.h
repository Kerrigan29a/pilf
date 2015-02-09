#ifndef __PILF_COMMON__UTLIST_UTILS_H__
#define __PILF_COMMON__UTLIST_UTILS_H__


#define DL_NEW_NODE(node, type) do { \
    node = (type *) pilf_secure_malloc(sizeof(type)); \
    CHECK_NNULL_R((node), __LINE__); \
} while(0);

#define DL_FINALIZE_NODE(node, type) do { \
    pilf_secure_free((node), sizeof(type)); \
} while(0);

#define DL_IS_EMPTY(head) (head == NULL)


#endif /* __PILF_COMMON__UTLIST_UTILS_H__ */
