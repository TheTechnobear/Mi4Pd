#include "m_pd.h"  

#include <string>

static t_class *tbtest_class;  

namespace test {
    class testmsg {
        public:
        std::string hello() {
            return "TBtest";
        }
    };
}


// Pure data interface
typedef struct _tbtest {  
    t_object  x_obj;
    test::testmsg tm_;  
} t_tbtest;  

extern "C"  {
    void *tbtest_bang(t_tbtest*);  
    void *tbtest_new(void);
    void *tbtest_setup(void);
}

// Pure data implmentation  
void *tbtest_bang(t_tbtest* x)  
{
    const char* msg = x->tm_.hello().c_str();
    post(msg);
    return 0;  
}  
 
void *tbtest_new(void)  
{  
    t_tbtest *x = (t_tbtest *)pd_new(tbtest_class);  

    return (void *)x;  
}  
 
void *tbtest_setup(void) {  
    tbtest_class = ::class_new(gensym("tbtest"),  
        (t_newmethod)tbtest_new,  
        0, sizeof(t_tbtest),  
        CLASS_DEFAULT, A_NULL);  
    ::class_addbang(tbtest_class, tbtest_bang);  
    return 0;
}
// end of pure data implementation
