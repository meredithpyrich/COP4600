#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

static int     dev_open();
static int     dev_release();
static ssize_t dev_read();
static ssize_t dev_write();

int init_module(void)
{
	printk(KERN_INFO "Installing module.\n");
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Removing module\n");
}

static int dev_open()
{
	return 0;
}

static int dev_release()
{
	printf(KERN_INFO "Device closed");
	return 0;
}

static ssize_t dev_read()
{

}

static ssize_t dev_write()
{

}

