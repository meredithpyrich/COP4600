#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "character"
#define BUFFER_SIZE 1024

static int majorDeviceNumber;

static int this_open(struct inode * inode, struct file * file);
static int this_release(struct inode * inode, struct file * file);
static ssize_t this_read(struct file * file, char * buffer, size_t size, loff_t * offset);
static ssize_t this_write(struct file * file, const char * buffer, size_t size, loff_t * offset);

static struct file_operations fops =
{
	.open = this_open,
	.release = this_release,
	.read = this_read,
	.write = this_write,
};

int init_module(void)
{
	majorDeviceNumber = register_chrdev(0, DEVICE_NAME, &fops);

	if (majorDeviceNumber < 0)
	{
		printk(KERN_ALERT "Registering failed. majorDeviceNumber = %d\n", majorDeviceNumber);
		return majorDeviceNumber;
	}

	printk(KERN_INFO "Registering success.\n");
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Removing success\n");
}

static int this_open(struct inode * inode, struct file * file)
{
	printk(KERN_INFO "Device opened\n");
	return 0;
}

static int this_release(struct inode * inode, struct file * file)
{
	printk(KERN_INFO "Device closed");
	return 0;
}

static ssize_t this_read(struct file * file, char * buffer, size_t size, loff_t * offset)
{

	return -EINVAL;
}

static ssize_t this_write(struct file * file, const char * buffer, size_t size, loff_t * offset)
{

	return -EINVAL;
}

