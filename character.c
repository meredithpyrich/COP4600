#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "character"
#define BUFFER_SIZE 1024

static int majorDeviceNumber;
static char ourInternalBuffer[BUFFER_SIZE];
static int messageLength;

static void shiftBuffer(int shift);

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
	unregister_chrdev(majorDeviceNumber, DEVICE_NAME);
	/*if(error < 0)
	{
		printk(KERN_ALERT "Error in unregister_chrdev: %d\n", error);
	}
	else
	{
		printk(KERN_INFO "Removing success\n");
	}*/
	printk(KERN_INFO "Removing success\n");
}


static int this_open(struct inode * inode, struct file * file)
{
	//try_module_get(THIS_MODULE);
	printk(KERN_INFO "Device opened\n");
	return 0;
}

static int this_release(struct inode * inode, struct file * file)
{
	//module_put(THIS_MODULE);
	printk(KERN_INFO "Device closed");
	return 0;
}

static ssize_t this_read(struct file * file, char * buffer, size_t size, loff_t * offset)
{
	if (size > messageLength)
		size = messageLength;

	copy_to_user(buffer, ourInternalBuffer, size);
	
	size -= messageLength;
	shiftBuffer(size);

	return 0;
}

static ssize_t this_write(struct file * file, const char * buffer, size_t size, loff_t * offset)
{
	// check to make sure amount of space going to buffer isn't too large
	if (size >= (BUFFER_SIZE-messageLength))
		size = BUFFER_SIZE-messageLength;
	
	copy_from_user(ourInternalBuffer,buffer,size);
	messageLength += size;
	
	return 0;
}

static void shiftBuffer(int shift)
{
	int counter;
	for(counter = 0; counter < messageLength; counter++)
	{
		ourInternalBuffer[counter] = ourInternalBuffer[counter+shift];
	}
}
