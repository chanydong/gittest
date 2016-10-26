/* ********************************************
 * led-driver.c 
 * Desc: led on/off, block test driver
 * Author: Youngdeok
 * ********************************************/
#include <linux/module.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <linux/bitops.h>
#include <linux/capability.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/in.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/notifier.h>
#include <linux/miscdevice.h>

#include <linux/irq.h>
#include <asm/io.h>
#include <asm/irq.h>

#include <mach/map.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <mach/regs-gpio.h>
#include <mach/irqs.h>

#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/delay.h>

#include "led-driver.h"

#define IRQ_KEY11	IRQ_EINT(0)


// POINT: define the wait queue
wait_queue_head_t	eventq;
static int flag = 0;
static int key_press_counter = 0;

// ISR (top-half) --------------------------------
static irqreturn_t dev_isr(int irq, void *dev_id)
{
    printk("[ISR:top] interrupt occurred\n");
    key_press_counter++;
	flag = 1;
	// POINT: send the event message to userspace
	// blocked read() systemcall will be wake up!
	wake_up_interruptible(&(eventq)); 

    return IRQ_HANDLED;
}

// open ---------------------------------------------
int dev_open(struct inode *inode, struct file *filp)
{
	int ret = -ENODEV;
 	if( !try_module_get(THIS_MODULE) )
    	goto out;	

    ret = 0;
    
out:	
	return ret;    
}

// close -----------------------------------------------
int dev_release(struct inode *inode, struct file *filp)
{
	module_put(THIS_MODULE);
    return 0;
}

// read --------------------------------------------------------------------
ssize_t dev_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int val = 0;
	flag = 0;	
    if (count < sizeof(int))
        return -EINVAL;

    if (signal_pending(current)) {
        return -ERESTARTSYS;
    }
    
    val = key_press_counter; // read value
    
	// POINT: wait the user process which want to read something 
	// from the driver
	printk("[driver] wait_event\n");
	wait_event_interruptible(eventq, flag!=0 );
    
    if (copy_to_user(buf, (void *)&val, sizeof(int))) {
        return -EFAULT;
    }
    return sizeof(int);
}


// ioctl --------------------------------------------------------------
long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{    
	int ret = 0;
	int size;
	int data;

	// POINT: to check commands 
	if(_IOC_TYPE(cmd) != DEV_MAGIC) {
		printk(" _IOC_TYPE error\n");
		return -EINVAL;
	};
	if(_IOC_NR(cmd) >= DEV_MAXNR) {
		printk("_IOC_NR error\n");
		return -EINVAL;
	};
	size = _IOC_SIZE(cmd);

	// POINT: to interpret commands
	switch (cmd) {

		case DEV_LED_OFF: 
			printk("[driver] cmd = DEV_LED_OFF\n");
			gpio_set_value(S3C2410_GPG(4), 1);
			gpio_set_value(S3C2410_GPG(5), 1);
			gpio_set_value(S3C2410_GPG(6), 1);
			gpio_set_value(S3C2410_GPG(7), 1);
			break;	

		case DEV_LED_ON: 
			printk("[driver] cmd = DEV_LED_OFF\n");
			gpio_set_value(S3C2410_GPG(4), 0);
			gpio_set_value(S3C2410_GPG(5), 0);
			gpio_set_value(S3C2410_GPG(6), 0);
			gpio_set_value(S3C2410_GPG(7), 0);
			break;

		case DEV_GETSTATE: 
			printk("[driver] cmd = DEV_GETSTATE\n"); 
			printk("GPG(4):%d\n", gpio_get_value(S3C2410_GPG(4)));
			printk("GPG(5):%d\n", gpio_get_value(S3C2410_GPG(5)));
			printk("GPG(6):%d\n", gpio_get_value(S3C2410_GPG(6)));
			printk("GPG(7):%d\n", gpio_get_value(S3C2410_GPG(7)));
			data = gpio_get_value(S3C2410_GPG(4));
			//if(copy_to_user((int*)arg, &data, sizeof(int)))
			//	return -EFAULT;
			if(__put_user(data, (int __user *)arg)<0)
				return -EFAULT;
			break;

		default:    	
			printk(KERN_ERR "there is no command\n");
	        ret = -ENOIOCTLCMD;
    }
    
    return ret;
}

// write --------------------------------------------------------------------------
ssize_t dev_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	return 0;
}


// POINT: to initialize the objects
struct file_operations dev_fops = {
  .owner    = THIS_MODULE,
  .open     = dev_open,
  .read		= dev_read,
  .unlocked_ioctl = dev_ioctl,
  .release  = dev_release,
};

struct miscdevice dev_miscdev = {
    MISC_DYNAMIC_MINOR, "led-driver", &dev_fops
};

// init -------------------------
static int __init dev_init(void)
{
	int ret = 0;

	// reset key11 (set to EINT mode)
	gpio_request(S3C2410_GPF(0), "key11_gpio");
	s3c2410_gpio_cfgpin(S3C2410_GPF(0), S3C2410_GPF0_EINT0);
	
	// reset leds (request)
	gpio_request(S3C2410_GPG(4), "led1");
	gpio_request(S3C2410_GPG(5), "led5");
	gpio_request(S3C2410_GPG(6), "led6");
	gpio_request(S3C2410_GPG(7), "led7");
	// reset leds (off)
	gpio_set_value(S3C2410_GPG(4), 1);
	gpio_set_value(S3C2410_GPG(5), 1);
	gpio_set_value(S3C2410_GPG(6), 1);
	gpio_set_value(S3C2410_GPG(7), 1);
	// set the control register to output
	s3c_gpio_cfgpin(S3C2410_GPG(4), S3C2410_GPIO_OUTPUT);
	s3c_gpio_cfgpin(S3C2410_GPG(5), S3C2410_GPIO_OUTPUT);
	s3c_gpio_cfgpin(S3C2410_GPG(6), S3C2410_GPIO_OUTPUT);
	s3c_gpio_cfgpin(S3C2410_GPG(7), S3C2410_GPIO_OUTPUT);

	// to register a ISR
	ret = request_irq(IRQ_KEY11, dev_isr, IRQF_DISABLED | IRQF_TRIGGER_FALLING, "key11", NULL);	
	if (ret) {
        printk("failed to request interrupt.\n");
        ret = -ENOENT;
        goto err;
    }
	
	// to register a misc device
	ret = misc_register(&dev_miscdev);
	if( ret<0 ) {
		printk(KERN_ERR "led-driver mise register error\n");
		goto err;
	}
		
	// init wait queue
	init_waitqueue_head(&eventq);        
    printk("[driver] led-driver init.\n");

	return 0;
err: 
    return ret;
}

// exit --------------------------
static void __exit dev_exit(void)
{
	misc_deregister(&dev_miscdev);
	free_irq(IRQ_KEY11, NULL);	
	gpio_free(S3C2410_GPF(0));
	printk("[driver] led-driver exit.\n");
}

module_init(dev_init);
module_exit(dev_exit);

MODULE_AUTHOR("Youngdeok");
MODULE_LICENSE("GPL");
