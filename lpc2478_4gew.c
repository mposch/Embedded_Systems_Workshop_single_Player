/*
link command ln -s /home/es/workspace/Embedded_Systems_Workshop_single_Player/lpc2478_4gew.c

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
#include <linux/syscalls.h>
#include <linux/interrupt.h>
#include <asm-generic/ioctls.h>/*ioctl CTEST_SETPID*/
#include <linux/spinlock.h>

#include<linux/timer.h> //needed for soft-timer from kernel

#include <generated/utsrelease.h> /*defines UTS_RELEASE macro*/
#include <linux/signal.h>
#include <linux/sched.h>
/* Board specific definitions*/
#include "asm/hardware/lpc246x.h"

/******************************************************************************
 * Typedefs and defines
 *****************************************************************************/

/*Identifier used by the interrupt service routine*/
#define DEVICE_NAME "lpc2478_4gew"

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

static long ctest_ioctl (struct file *file,unsigned int cmd,	unsigned long arg);

/******************************************************************************
 * Local variables and function prototypes
 *****************************************************************************/

static spinlock_t lock;
static pid_t user_pid;
/* needed for send_sig() */
static struct task_struct *tsk;
static struct siginfo info;

static int opened = 0;

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

struct timer_list timer;

//uint8_t gpio_values[5] = {0,0,0,0,0};
//uint8_t i2c_values[4] = {0,0,0,0};

struct data {
	uint8_t gpio_values[5];
	uint8_t i2c_values[4];
};

struct data key_data;

uint8_t *gpio_values = key_data.gpio_values;
uint8_t *i2c_values = key_data.i2c_values;

static uint8_t key_state = 0;
static int pid_count = 0;
static int irq_requested = 0;
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
  .unlocked_ioctl	 = ctest_ioctl
};




/******************************************************************************
 * Local functions
 *****************************************************************************/

//get_gpio_values detects falling edges on the joystick
int get_gpio_values(){
	int i = 0;
	if( gpio_values[0] == 0 && FIO2PIN2 & (1<<6)){
			gpio_values[0] = 1;
			i++;
		}else if(gpio_values[0] == 1 &&!(FIO2PIN2 & (1<<6))){
			gpio_values[0] = 0;
			i++;
		}


	if( gpio_values[1] == 0 && FIO2PIN2 & (1<<7)){
			gpio_values[1] = 1;
			i++;
		}else if(gpio_values[1] == 1 &&!(FIO2PIN2 & (1<<7))){
			gpio_values[1] = 0;
			i++;
		}

	if(gpio_values[2] == 0 && FIO2PIN3 & (1<<1)){
		gpio_values[2] = 1;
		i++;
	}else if(gpio_values[2] == 1 && !(FIO2PIN3 & (1<<1))){
		gpio_values[2] = 0;
		i++;
	}

	if(gpio_values[3] == 0 && FIO2PIN3 & (1 << 2)){
		gpio_values[3] = 1;
		i++;
	}else if(gpio_values[3] == 1 && !(FIO2PIN3 & (1<<2))){
		gpio_values[3] = 0;
		i++;
	}


	if(gpio_values[4] == 0 && FIO2PIN3 & (1 << 3)){
		gpio_values[4] = 1;
		i++;
	}else if(gpio_values[4] == 1 && !(FIO2PIN3 & (1<<3))){
		gpio_values[4] = 0;
		i++;
	}

	return i;
}

void i2c_start(){
	I20CONSET = (1 << 5);
}

void timer_callback (unsigned long data){
	static int i = 0;
	if(get_gpio_values() && pid_count){
		//printk("joystick pressed\n");
		//printk("signal: %d\n",SIGUSR1);
		send_sig_info(SIGUSR1,&info,tsk);

	}
	i2c_start();
	timer.expires += 10;
	add_timer(&timer);
}

static irqreturn_t i2c_interrupt(int irq, void *dev_id){
	int i = 0;
	switch(I20STAT){
		case 0x08:		//start condition
		case 0x10:		//repeated start condition
			I20DAT = 0xC1;
			//I20DAT = (i2c_address << 1) | 1;
			I20CONCLR = (1<<3) | (1 << 5);
			break;
		case 0x40:		//SLA+W transmitted and ACK received
			//printk("Adresse %x liefert ACK\n",I20DAT);
			I20CONCLR = (1<<3);
			break;
		case 0x58:		//data received, NACK send
			if(key_state != I20DAT){
				if(i2c_values[0] == 0 && I20DAT & 1){
					i2c_values[0] = 1;
				}else if(i2c_values[0] == 1 && !(I20DAT & 1)){
					i2c_values[0] = 0;
					//printk("key pressed\n");
					i++;
				}
				if(i2c_values[1] == 0 && I20DAT & (1 << 1)){
					i2c_values[1] = 1;
				}else if(i2c_values[1] == 1 && !(I20DAT & (1 << 1))){
					i2c_values[1] = 0;
					//printk("key pressed\n");
					i++;
				}
				if(i2c_values[2] == 0 && I20DAT & (1 << 2)){
					i2c_values[2] = 1;
				}else if(i2c_values[2] == 1 && !(I20DAT & (1 << 2))){
					i2c_values[2] = 0;
					//printk("key pressed\n");
					i++;
				}
				if(i2c_values[3] == 0 && I20DAT & (1 << 3)){
					i2c_values[3] = 1;
				}else if(i2c_values[3] == 1 && !(I20DAT & (1 << 3))){
					i2c_values[3] = 0;
					//printk("key pressed\n");
					i++;
				}
				if(i && pid_count){
					send_sig_info(SIGUSR1,&info,tsk);
				}
				//printk("Key Pressed 0x%x\n",I20DAT);
				//key_state = I20DAT;
			}
			I20CONCLR = (1<<3);
			I20CONSET = (1 << 4);
			break;
		default:
			printk("Status: %x\n",I20STAT);
	}
}

int init_i2c(){
	int ret;

	if(!irq_requested){
		ret = request_irq(9, i2c_interrupt, 0, DEVICE_NAME, NULL);
		if (ret < 0){
			printk("%s: Could not bind interrupt. Error: %i\n",DEVICE_NAME,ret);
		return -EIO;
		}
		irq_requested = 1;
	}

	PCONP |= (1<<7); //activate I2C0
	PINSEL1 &= ~((1 << 23) | (1 << 25));
	PINSEL1 |= (1<<22) | (1 << 24);
	PINMODE1 = (10 << 22);

	I20DAT = 0x60;
	I20CONSET = 0x60; //activate master only mode

	return 0;
}

//Function to initialize GPIO pins P2.22,P2.23 and P2.25 - P2.27 (Joystick)
void init_gpio(){

	//Setze GPIO pins auf Funktion GPIO
	//m_reg_bfc(PINSEL5,((3<<12) | (3<<14) | (3<<18) | (3 << 20) | (3 << 22))); //Bit 12,13 = P2.22 Bit 14,15 = P2.23 Bit 18,19 = P2.25 Bit 20,21 = P2.26 Bit 22,23 = P2.27
	PINSEL5 &= ~((3<<12) | (3<<14) | (3<<18) | (3 << 20) | (3 << 22));//Durch BFC werden damit die Bits 12 bis 15 und 18 bis 23 auf 0 gesetzt.

	//m_reg_bfc(FIO2DIR2,(3 << 6)); //Set P2.22 and P2.23 to input
	//m_reg_bfc(FIO2DIR3,(7 << 1)); //Set P2.25 to P2.27 to input
	FIO2DIR2 &= ~(3<<6);
	FIO2DIR3 &= ~(7 << 1);

	//printk("FIO2DIR: %x\n",m_reg_read(PINMODE5));

}

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



	if(!opened){
		init_timer(&timer); //timer initialisieren

		timer.expires = jiffies;			//expires = nach wie vielen Jiffies die callback function aufgerufen werden soll
		timer.function = (*timer_callback);		//callback Function die aufgerufen werden soll
		timer.data = 10;		//daten für die Callback function. Kann auch ein auf unsinged long gecasteter pointer auf eine struct sein

		init_gpio();
		init_i2c();
		timer_callback(0); //first callback call um timer zu starten. Für entwicklungszwecke in setup gesetzt, soll danach nach open verschoben werden.
		int i = 0;
		for(i = 0;i<4;i++){
			i2c_values[i] = 0;
		}
		i = 0;
		for( i = 0; i < 5;i++){
			gpio_values[i] = 0;
		}
	}

	opened++; //count how many times opened

    return 0;
  
}

static long ctest_ioctl (struct file *file,unsigned int cmd,	unsigned long arg)
{
#ifdef DEBUG
	//printk("in ioctl\n");
#endif
	switch(cmd)
	{
		case CTEST_SETPID:
			memset(&info, 0, sizeof(struct siginfo));
			info.si_signo = SIGUSR1;
			info.si_code = SI_KERNEL;
			info.si_int = 1234;
			/*without interrupt blocking*/
			spin_lock_bh(&lock);
			user_pid = arg;
			spin_unlock_bh(&lock);

			tsk = find_task_by_vpid(user_pid);	// get the task, which called this driver (arg = pid
			if(tsk == NULL)
			{
				printk("no such pid \n");
				return -ENODEV;
			}
			pid_count = 1;
			break;
		default:
			printk("no valid arg for ioctl\n");
			break;
	}
	//printk("ioctl: user_pid: %lu \n", arg);
	return 0;

}


static int ctest_close(struct inode* inode, 
                     struct file* file) {

	
	/*disable fast gpio ports - only applicable if THIS module
	is the only one using gpio ports*/
	//m_reg_bfc(SCS,(1<<0));
	if(opened == 1){ //delete timer only when last time closed

		del_timer(&timer);
		SCS &= ~(1<<0);
	}
	opened--;
	return 0;

}

ssize_t ctest_read(struct file *p_file, 
		      char __user *p_buf, 
		      size_t count, 
		      loff_t *p_pos) {
	if(count < sizeof(struct data)){
		printk("Buffer im user space zu klein!!!\n");
		return 0;
	}
	
	/* use copy_to_user(dst, src, size)) to copy data from kernel
	to user space*/;
	copy_to_user(p_buf,(char *)&(key_data),sizeof(struct data));

	return (sizeof(struct data));

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
static void __exit esw_4gew_exit(void)
{
	free_irq(9,NULL);
	struct cdev *cdev = &ctest_dev.cdev;
	dev_t devno = ctest_dev.devno;
  	cdev_del(cdev);
	unregister_chrdev_region(MAJOR(devno), 1);

}

static int __init esw_4gew_init(void){
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
MODULE_AUTHOR("Thomas Bittner & Matthias Posch");
MODULE_DESCRIPTION("4 Gewinnt Treiber");
// Mandatory, anounce init and exit functions
module_init(esw_4gew_init);
module_exit(esw_4gew_exit);


