typedef enum {
    CMP_STR_UNMATCH = -1,
    CMP_STR_MATCH = 0,
    CMP_DST_END = 1,
} str_find_e;

typedef struct {
    char *str;
    int beginPos;
    int len;
} strFind_t;

#define strFindINIT(X) { \
.str = X, \
.beginPos = 0, \
.len = sizeof(X) - 1, \
}

#define ACK "!OK"
#define NACK "~NG"

static str_find_e string_match(const char *dst_cmp, int dst_len, strFind_t *cmpstr) {
    int index = 0;
    char *cmp = cmpstr->str;
    printf("%s - %s\n", dst_cmp, &cmp[cmpstr->beginPos]);
    if(dst_cmp == NULL || cmp == NULL || (cmpstr->beginPos >= cmpstr->len)){
        printf("debug 1\n");
        return CMP_STR_UNMATCH;
    }
    
    while(index < dst_len) {
        if (dst_cmp[index] != cmp[cmpstr->beginPos]) 
            cmpstr->beginPos = 0;

        if(dst_cmp[index] == cmp[cmpstr->beginPos])
            cmpstr->beginPos++;

        if (cmpstr->beginPos >= cmpstr->len) {
            return CMP_STR_MATCH;
        }

        index++;
    }
    
    if (cmpstr->beginPos == 0) {
        return CMP_STR_UNMATCH;
    }
    else {
        return CMP_DST_END;
    }
}
