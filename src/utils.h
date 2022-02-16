#ifndef __STUDY_SRC_UTILS_H__
#define __STUDY_SRC_UTILS_H__
namespace aris {

#include <string>



class StringGenerator {
public:
    template<typename... Args>
    static const std::string format(const std::string & fmt, Args... args) {
        char buf[128];
        snprintf(buf, 128, fmt.c_str(), args...);
        return std::string(buf);
    }
};



#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)


// #define ARIS_ASSERT(x) \
//     if(likely(x)) { \
//         A
//     }


}


#endif