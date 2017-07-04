#include "m_pd.h"  
 
static t_class *tbtest_class;  
 
typedef struct _tbtest {  
  t_object  x_obj;  
} t_tbtest;  
 
void *tbtest_bang(t_tbtest *x)  
{  
  post("TB test!");  
}  
 
void *tbtest_new(void)  
{  
  t_tbtest *x = (t_tbtest *)pd_new(tbtest_class);  
  
  return (void *)x;  
}  
 
void *tbtest_setup(void) {  
  tbtest_class = class_new(gensym("tbtest"),  
        (t_newmethod)tbtest_new,  
        0, sizeof(t_tbtest),  
        CLASS_DEFAULT, 0);  
  class_addbang(tbtest_class, tbtest_bang);  
}
