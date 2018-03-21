#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>

static int majorDeviceNumber;

static int open(struct inode * iNode, struct file * File);
static int release(struct inode * iNode, struct file * File);
static ssize_t read(struct file * File, char __user * User, size_t Size, loff_t * Loff);
static ssize_t write(struct file * File, const char __user * User, size_t Size, loff_t * Loff);

//static struct class* Class = NULL;
//static struct device* Device = NULL;

/*static struct file_operations fileOps =
{
	.open = open,
	.release = release,
	.read = read,
	.write = write,
};*/

int init_module(void)
{
	printk(KERN_INFO "Installing module.\n");
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Removing module\n");
}

static int open(struct inode * iNode, struct file * File)
{
	printk(KERN_INFO "Device opened\n");
	return 0;
}

static int release(struct inode * iNode, struct file * File)
{
	printk(KERN_INFO "Device closed");
	return 0;
}

static ssize_t read(struct file * File, char __user * User, size_t Size, loff_t * Loff)
{

}

static ssize_t write(struct file * File, const char __user * User, size_t Size, loff_t * Loff)
{

}

