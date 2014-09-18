#ifndef __PILF_MINION__UTLIST_UTILS_H__
#define __PILF_MINION__UTLIST_UTILS_H__


#define DL_NEW_NODE(ctx, node, type) do { \
    ASSERT_NNULL(ctx); \
    node = (type *) minion_secure_malloc((ctx), sizeof(type)); \
    CHECK_NNULL_R((node), __LINE__); \
} while(0);

#define DL_FINALIZE_NODE(ctx, node, type) do { \
    ASSERT_NNULL(ctx); \
    minion_secure_free((ctx), (node), sizeof(type)); \
} while(0);

#define DL_IS_EMPTY(head) (head == NULL)


#endif /* __PILF_MINION__UTLIST_UTILS_H__ */