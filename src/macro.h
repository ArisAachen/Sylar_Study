
namespace aris {

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)


#define ARIS_ASSERT(x) \
    if(unlikely(x)) { \
        assert(x);  \
    }

}