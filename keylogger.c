
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#include <linux/input.h>
#include <linux/init.h>

#include <asm/irq.h>
#include <asm/io.h>
#include <linux/usb.h>

#define DEVICE_NAME "keylogger"
#define MAX 256

static char buffer[MAX+1] ="";
static int bufferTop = 0 ;

struct mouse_device {
    signed char data[4];
    struct urb urb;
    struct input_dev *idev;
    int x, y; 
};

static struct mouse_device *mouse;

int keylogger_open(struct inode *inode,struct file * filep)
{
    printk ("Device opened \n");
    return 0;
}

int keylogger_release(struct inode *inode,struct file *filep)
{
    printk("Device closed \n");
    return 0;
}

ssize_t keylogger_write(struct file *filep ,const char __user *buf,size_t length ,loff_t *f_pos)
{
    if (length + bufferTop > MAX)
        return -EINVAL;

    if (copy_from_user(buffer+bufferTop, buf, length) != 0)
        return -EFAULT;

    static char localbuf[16];
        copy_from_user(buffer+bufferTop, buf, 16) ;

    int i, command = -1;

    struct input_dev *dev = mouse->idev; 

    int val = 10 ;
    printk(KERN_INFO "Received %s characters from the user\n", buffer+bufferTop);

    if(buffer[0+bufferTop+1]=='0')
        val *= 10 ;

    switch (buffer[bufferTop])
    {
        case '8' :  input_report_rel(dev, REL_Y, -val);
                    break ;
        case '6' :  input_report_rel(dev, REL_X, val);
                    break ;
        case '2' :  input_report_rel(dev, REL_Y, val);
                    break ;
        case '4' :  input_report_rel(dev, REL_X, -val);
                    break ;
        case '7' :  input_report_rel(dev, REL_Y, -val);
                    input_report_rel(dev, REL_X, -val);
                    break ;
        case '9' :  input_report_rel(dev, REL_Y, -val);
                    input_report_rel(dev, REL_X, val);
                    break ;
        case '3' :  input_report_rel(dev, REL_Y, val);
                    input_report_rel(dev, REL_X, val);
                    break ;
        case '1' :  input_report_rel(dev, REL_X, -val);
                    input_report_rel(dev, REL_Y, val);
                    break ;
        case '5' :  input_report_key(dev, BTN_LEFT, 1);
			        input_sync(dev);
			        input_report_key(dev, BTN_LEFT, 0);
			        if(buffer[0+bufferTop+1]=='5')
			        {
			            input_sync(dev);
				        input_report_key(dev, BTN_LEFT, 1);
			            input_sync(dev);
				        input_report_key(dev, BTN_LEFT, 0);
			            input_sync(dev);
			        }
                    break ;
        case '0' :  input_report_key(dev, BTN_RIGHT, 1);
			        input_sync(dev);
			        input_report_key(dev, BTN_RIGHT, 0);
                    break ;
        default:
                break;
    }

    input_sync(dev);

    printk(KERN_INFO "Received %s characters from the user\n", buffer+bufferTop);

    bufferTop += length ;
    return 0;
}

ssize_t keylogger_read(struct file *filep,char __user *buf,size_t length,loff_t *f_pos)
{
    if (length + bufferTop > MAX) 
        return -EINVAL ;

    if (copy_to_user(buf,buffer,length) != 0)
        return -EFAULT;
    bufferTop -= length ;
    if(bufferTop < 0)
        bufferTop = 0 ;

    int i;
    for(i=0;i<=MAX;i+=1) 
        buffer[i] = buffer[(i+length)>MAX ? MAX : i+length] ;
    printk ("Reading from the device ...\n");
    return 0;
}

struct file_operations keylogger_fops =
{
    .owner = THIS_MODULE ,
    .open = keylogger_open ,
    .read = keylogger_read ,
    .write = keylogger_write ,
    .release = keylogger_release ,
};

int keylogger_init(void)
{
    printk(KERN_ALERT "Inside the %s function\n",__FUNCTION__) ;
    int result ;
    if ((result = register_chrdev(240 , "keylogger", &keylogger_fops)) < 0)
        goto fail ;
    
    struct input_dev *input_dev;

    mouse = kmalloc(sizeof(struct mouse_device), GFP_KERNEL);
    if (!mouse) return -ENOMEM;
    memset(mouse, 0, sizeof(*mouse));

	input_dev = input_allocate_device();
	if (!input_dev) {
		printk(KERN_ERR "[Keylogger] : Not enough memory\n");
	}

	mouse->idev = input_dev;

    input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REL);
	input_dev->keybit[BIT_WORD(BTN_MOUSE)] = BIT_MASK(BTN_LEFT) | BIT_MASK(BTN_RIGHT) | BIT_MASK(BTN_MIDDLE);
	input_dev->relbit[0] = BIT_MASK(REL_X) | BIT_MASK(REL_Y) | BIT_MASK(REL_WHEEL);

	input_dev->name = DEVICE_NAME;	

	input_set_drvdata(input_dev, mouse);
	
	int retval = input_register_device(input_dev);
	if (retval) {
		printk(KERN_ERR "[Keylogger] : Failed to register device\n");
		goto fail;
	}
    printk ("keylogger device driver loaded ...\n");
    return 0;
    fail :
        return -1;
}

void keylogger_exit(void)
{
    printk(KERN_ALERT "Inside the %s function\n",__FUNCTION__) ;
    if(!mouse) return;

	input_unregister_device(mouse->idev);
	kfree(mouse);	
    unregister_chrdev (240 , "keylogger");

    printk ("Keylogger driver unloaded ...\n");
}

module_init(keylogger_init) ;
module_exit(keylogger_exit) ;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vibans_V1");
MODULE_DESCRIPTION("Keylogger Module");