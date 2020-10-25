#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>


int temp_init(void)
{
    printk(KERN_ALERT "Inside the %s function\n",__FUNCTION__) ;
    return 0 ;
}

void temp_exit(void)
{
    printk(KERN_ALERT "Inside the %s function\n",__FUNCTION__) ;
}

module_init(temp_init) ;
module_exit(temp_exit) ;