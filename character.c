#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "character"
#define CLASS_NAME "chara"
#define BUFFER_SIZE 1024

MODULE_LICENSE("GPL");

static int majorDeviceNumber = -1;
static char ourInternalBuffer[BUFFER_SIZE];
static int messageLength;
static struct class* deviceClass = NULL;
static struct device* devicePointer = NULL;

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
	
	
	// Register device class
	deviceClass = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(deviceClass))
	{
		unregister_chrdev(majorDeviceNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to register class\n");
		return PTR_ERR(deviceClass);
	}

	// Register driver
	devicePointer = device_create(deviceClass, NULL, MKDEV(majorDeviceNumber, 0), NULL, DEVICE_NAME);
	if (IS_ERR(devicePointer))
	{
		class_destroy(deviceClass);
		unregister_chrdev(majorDeviceNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to create device\n");
	}		
	printk(KERN_INFO "Registering success.\n");
	return 0;
}

void cleanup_module(void)
{
	
	device_destroy(deviceClass, MKDEV(majorDeviceNumber, 0));
	class_unregister(deviceClass);
	class_destroy(deviceClass);
	unregister_chrdev(majorDeviceNumber, DEVICE_NAME); 
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

	return messageLength;
}

static ssize_t this_write(struct file * file, const char * buffer, size_t size, loff_t * offset)
{
	// check to make sure amount of space going to buffer isn't too large
	if (size >= (BUFFER_SIZE-messageLength))
		size = BUFFER_SIZE-messageLength;
	
	copy_from_user(ourInternalBuffer,buffer,size);
	messageLength += size;
	
	return messageLength;
}

static void shiftBuffer(int shift)
{
	int counter;
	for(counter = 0; counter < messageLength; counter++)
	{
		ourInternalBuffer[counter] = ourInternalBuffer[counter+shift];
	}
}
