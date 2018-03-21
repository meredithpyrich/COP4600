#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

static int majorDeviceNumber;

static int open();
static int release();
static ssize_t read();
static ssize_t write();

int init_module(void)
{
	printk(KERN_INFO "Installing module.\n");
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Removing module\n");
}

static int open()
{
	printk(KERN_INFO "Device opened\n");
	return 0;
}

static int release()
{
	printk(KERN_INFO "Device closed");
	return 0;
}

static ssize_t read()
{

}

static ssize_t write()
{

}

