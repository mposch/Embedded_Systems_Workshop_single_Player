/*
link command ln -s /home/es/DevEnv/Embsys_4gewinnt/lpc2478_4gew.c

    This module implements a driver for LPC2468 ctest peripheral.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <linux/types.h>
#include <linux/init.h>
#include <linux/fs.h> /*Needed by char devices*/
#include <linux/module.h> 
#include <linux/cdev.h>/*Char device structures and functions*/
#include <linux/kdev_t.h>	/* needed for the dynamic major and minor number creation*/
#include <asm/uaccess.h> /*Interaction with user space*/


#include <generated/utsrelease.h> /*defines UTS_RELEASE macro*/

/* Board specific definitions*/
#include "asm/hardware/lpc24xx.h"

/******************************************************************************
 * Typedefs and defines
 *****************************************************************************/

/*Identifier used by the interrupt service routine*/
#define DEVICE_NAME "lpc2478_chardrv_min"

/*
Character devices amongst others are represented by device files usually located in /dev.
Files for char drivers are represented with a leading c in the listing seen below.
Other device drivers like block devices are represented in a similar way.


#ls -l /dev
 crw-rw-rw-    1 root     root       1,   3 Apr 11  2002 null
 crw-------    1 root     root      10,   1 Apr 11  2002 psaux
 crw-------    1 root     root       4,   1 Oct 28 03:04 tty1
 crw-rw-rw-    1 root     tty        4,  64 Apr 11  2002 ttys0
 crw-rw----    1 root     uucp       4,  65 Apr 11  2002 ttyS1
 crw--w----    1 vcsa     tty        7,   1 Apr 11  2002 vcs1
 crw--w----    1 vcsa     tty        7, 129 Apr 11  2002 vcsa1
 crw-rw-rw-    1 root     root       1,   5 Apr 11  2002 zero

Traditionally the major number identifies the driver associated with the device. For example, /dev/null and /dev/zero are both managed by driver 1, whereas virtual consoles and serial terminals are managed by driver 4; similarly, both vcs1 and vcsa1 devices are managed by driver 7. Modern Linux kernels allow multiple drivers to share major numbers, but most devices that you will see are still organized on the one-major-one-driver principle.

The minor number is used by the kernel to determine exactly which device is being referred to. Depending on how your driver is written (as we will see below), you can either get a direct pointer to your device from the kernel, or you can use the minor number yourself as an index into a local array of devices. Either way, the kernel itself knows almost nothing about minor numbers beyond the fact that they refer to devices implemented by your driver.

Bibliography:Linux Device Driver Kit Vol.3 Chapter 3
*/



/*
Register bit set, bit clear read and write macros
*/

#define m_reg_read(reg) (*(volatile unsigned long *)(reg))
#define m_reg_write(reg, data) ((*(volatile unsigned long *)(reg)) = (volatile unsigned long)(data))

#define m_reg_bfs(reg, data) (*(volatile unsigned long *)(reg)) |= (data)

#define m_reg_bfc(reg, data) (*(volatile unsigned long *)(reg)) &= ~(data)



/******************************************************************************
 * Driver specific function prototypes
 *****************************************************************************/


static int ctest_open(struct inode* inode, 
                    struct file* file);
static int ctest_close(struct inode* inode, 
                     struct file* file);

ssize_t ctest_read(struct file *p_file, 
		      char __user *p_buf, 
		      size_t count, 
		      loff_t *p_pos);

ssize_t ctest_write(struct file *p_file, 
		      const char __user *p_buf, 
		      size_t count, 
		      loff_t *p_pos);



/******************************************************************************
 * Local variables and function prototypes
 *****************************************************************************/


/*structure to keep everything in one place*/
struct ctest {
	/* dev_t is a 32bit variable with 12 bits for the major number and 20 for the minor number 
	   use MAJOR(dev_t dev); and 
	   MINOR(dev_t dev); to get the major and minor number 
	   see http://www.makelinux.net/ldd3 chapter 3.2.2.
	*/
	dev_t devno;

	/*
	character device type administrative data structure
	*/
	struct cdev cdev;
};

static struct ctest ctest_dev;

/*
Attach driver specific functions to Linux Kernel file operation structure.
These functions are used by the Kernel to access the device driver.

For further Information see http://www.makelinux.net/ldd3/chp-3-sect-3.shtml

*/
static struct file_operations ctest_fops = {
  .owner   = THIS_MODULE,
  .read    = ctest_read,
  .write   = ctest_write,
  .open    = ctest_open,
  .release = ctest_close,
};




/******************************************************************************
 * Local functions
 *****************************************************************************/
int ctest_setup(struct ctest *dev) {

	struct cdev *cdev = &(dev->cdev);
    
	cdev_init(cdev, &ctest_fops);
	cdev->owner = THIS_MODULE;
	cdev->ops = &ctest_fops;

return 0;
}




/******************************************************************************
 * Device functions
 *****************************************************************************/

/*
Open the device, only one process can open it at a time.

*/
static int ctest_open(struct inode* inode, 
                    struct file* file) {


	/*enable fast gpio ports - only applicable if THIS module
	is the only one using gpio ports*/
	
	// Setup registers
	// Read current state
	
	//unsigned long tmp = m_reg_read(GPIO2);
	//tmp=0;
	
	
	// Let led P2_10 blink!
	
	
	
	// Setup registers
	
	// Setup Register DIrection
	
	// Clear Led



    return 0;
  
}

static int ctest_close(struct inode* inode, 
                     struct file* file) {

	
	/*disable fast gpio ports - only applicable if THIS module
	is the only one using gpio ports*/
	m_reg_bfc(SCS,(1<<0));

	return 0;

}

ssize_t ctest_read(struct file *p_file, 
		      char __user *p_buf, 
		      size_t count, 
		      loff_t *p_pos) {
	
	
	/* use copy_to_user(dst, src, size)) to copy data from kernel
	to user space*/;


	return (0);

}



/*
It is not neccessary to implement a dummy function if the device does not support it the corresponding field in the file operation structure can also be left blank.
In that case EINVAL is returned to the calling function.
*/
ssize_t ctest_write(struct file *p_file, 
		      const char __user *p_buf, 
		      size_t count, 
		      loff_t *p_pos) {

return -EINVAL;

}


/* Free any dynamically allocated data here.*/
static void __exit ctest_mod_exit(void)
{

	struct cdev *cdev = &ctest_dev.cdev;
	dev_t devno = ctest_dev.devno;

  	cdev_del(cdev);
	unregister_chrdev_region(MAJOR(devno), 1);

}

static int __init ctest_mod_init(void){
	int ret;
	dev_t devno = 0;

	/* 
	alloc_chrdev_region() dynamically allocates the first free major device number. 
	see http://www.makelinux.net/ldd3 chapter 3.2.2.gpio_dev 
	*/
	//(dev_t *dev, unsigned int firstminor, unsigned int count, char *name);
	ret = alloc_chrdev_region(&(ctest_dev.devno), 0, 1, DEVICE_NAME); 
	if(ret < 0) {
		printk("%s: Failed to register char device\n",DEVICE_NAME);
   		return ret;
  	}
	
	ret = ctest_setup(&ctest_dev);
  	if (ret < 0) {
    	printk("%s: Error adding module.\n",DEVICE_NAME);
		return ret;
	}
	
	devno = ctest_dev.devno;

	ret = cdev_add(&(ctest_dev.cdev),devno,1);
    	if (ret != 0) {
		printk("cdev_add(): Error adding device %s", DEVICE_NAME);
		return ret;
	}
	/*device is ready for calls to fops functions now!*/

     printk("#### Driver Module for 4 Gewinnt");
     printk("In case the module is not already loaded:\n");
	 printk("Load the driver with: 'insmod /lib/modules/%s/kernel/drivers/char/%s.ko'\n",UTS_RELEASE,DEVICE_NAME);
	 printk("Call 'mknod /dev/%s c %i 0' to create a device file\n",DEVICE_NAME, MAJOR(devno));
	 printk("Remove the module with 'rmmod %s' when you are done. \n",DEVICE_NAME);
	 printk("Remove the device file with 'rm /dev/%s'. \n",DEVICE_NAME);

  return ret;
}




MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matthias Wenzl");
MODULE_DESCRIPTION("Example driver skeleton");
// Mandatory, anounce init and exit functions
module_init(ctest_mod_init);
module_exit(ctest_mod_exit);


