#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int keylogger_open(struct inode *inode,struct file * filep)
{
    printk ("Device opened \n");
    return 0;
}

static int keylogger_release(struct inode *inode,struct file *filep)
{
    printk("Device closed \n");
    return 0;
}

static ssize_t keylogger_write(struct file *filep ,const char *buf,size_t count ,loff_t *f_pos)
{
    printk (" Writing to the device ...\n");
    return 1;
}

static ssize_t keylogger_read(struct file *filep,char *buf,size_t count,loff_t *f_pos)
{
    printk (" Reading from the device ...\n");
    return 1;
}

struct file_operations keylogger_fops =
{
    owner : THIS_MODULE ,
    open : keylogger_open ,
    release : keylogger_release ,
    read : keylogger_read ,
    write : keylogger_write ,
};

int keylogger_init(void)
{
    printk(KERN_ALERT "Inside the %s function\n",__FUNCTION__) ;
    int result ;
    if (( result = register_chrdev (240 , "keylogger", &keylogger_fops)) < 0)
        goto fail ;
    printk ("keylogger device driver loaded ...\n");
    return 0;
    fail :
        return -1;
}

void keylogger_exit(void)
{
    printk(KERN_ALERT "Inside the %s function\n",__FUNCTION__) ;
    unregister_chrdev (240 , "keylogger");
    printk ("Keylogger driver unloaded ...\n");
}

module_init(keylogger_init) ;
module_exit(keylogger_exit) ;

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("Vibans_V1");
MODULE_DESCRIPTION ("Keylogger Module");