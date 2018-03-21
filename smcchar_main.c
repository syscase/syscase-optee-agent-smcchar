#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/slab.h>

#include "syscase/smcchar.h"
#include "syscase/smcchar/api.h"
#include "syscase/test_run.h"

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Firas Zaidan");
MODULE_DESCRIPTION("Secure monitor call char device for testing from user space");
MODULE_VERSION("0.0.1");

int syscase_verbose = 1;
EXPORT_SYMBOL(syscase_verbose);

static int major_number;
static unsigned long result = 0;
static int trace = 0;
static struct class*  smcchar_class  = NULL;
static struct device* smcchar_device = NULL;

static DEFINE_MUTEX(smcchar_mutex);

static struct file_operations fops =
{
  .open = syscase_smcchar_open,
  .read = syscase_smcchar_read,
  .write = syscase_smcchar_write,
  .release = syscase_smcchar_release,
};

static int __init syscase_smcchar_init(void) {
  printk(KERN_INFO "smcchar: Initializing\n");

  // Try to dynamically allocate a major number for the device
  major_number = register_chrdev(0, DEVICE_NAME, &fops);
  if (major_number<0) {
    printk(KERN_ALERT "smcchar: Failed to register a major number\n");
    return major_number;
  }
  printk(KERN_INFO "smcchar: Registered with major number %d\n", major_number);

  // Register the device class
  smcchar_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(smcchar_class)) {
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_ALERT "smcchar: Failed to register device class\n");
    return PTR_ERR(smcchar_class);
  }
  printk(KERN_INFO "smcchar: Registered device class\n");

  // Register the device driver
  smcchar_device = device_create(smcchar_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
  if (IS_ERR(smcchar_device)) {
    class_destroy(smcchar_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_ALERT "smcchar: Failed to create device\n");
    return PTR_ERR(smcchar_device);
  }
  printk(KERN_INFO "smcchar: Created device class\n");
  mutex_init(&smcchar_mutex);
  return 0;
}

static void __exit syscase_smcchar_exit(void) {
  mutex_destroy(&smcchar_mutex);
  device_destroy(smcchar_class, MKDEV(major_number, 0));
  class_unregister(smcchar_class);
  class_destroy(smcchar_class);
  unregister_chrdev(major_number, DEVICE_NAME);
  printk(KERN_INFO "smcchar: Exit!\n");
}

static int syscase_smcchar_open(struct inode *inodep, struct file *filep){
  if (!mutex_trylock(&smcchar_mutex)) {
    printk(KERN_ALERT "smcchar: Device is busy");
    return -EBUSY;
  }
  printk(KERN_INFO "smcchar: Device opened\n");
  return 0;
}

static ssize_t syscase_smcchar_send(char *buffer, size_t len, void *dest) {
  int error_count = 0;
  error_count = copy_to_user(buffer, dest, len);

  if (error_count==0) {
    printk(KERN_INFO "smcchar: Sent %lu bytes\n", len);
    return 0;
  }

  printk(KERN_INFO "smcchar: Failed to send %d bytes\n", error_count);
  return -EFAULT;
}

static ssize_t syscase_smcchar_read(struct file *file, char *buffer, size_t len, loff_t *offset) {
  unsigned long mode = (unsigned long) *offset;
  
  if(mode != SMC_RESULT) {
    printk(KERN_INFO "smcchar: read with unknown mode %lu! Have you set offset to SMC_RESULT?\n", mode);
    return -EFAULT;
  }

  if(len > SMC_RESULT_SIZE) {
    len = SMC_RESULT_SIZE;
  }
  return syscase_smcchar_send(buffer, len, &result);
}

static ssize_t syscase_smcchar_receive(void *target, const char *buffer, size_t len) {
  int error_count = 0;
  error_count = copy_from_user(target, buffer, len);
  printk(KERN_INFO "smcchar: Write called\n");

  if (error_count==0) {
    printk(KERN_INFO "smcchar: Received %zu bytes\n", len);
    return len;
  }

  printk(KERN_INFO "smcchar: Failed to receive %d bytes\n", error_count);
  return -EFAULT;
}

static ssize_t syscase_smcchar_call(const char *buffer, size_t input_size) {
  char *input;
  ssize_t ret;

  input = kmalloc(SMC_CALL_SIZE, GFP_KERNEL);
  memset(input, 0, SMC_CALL_SIZE);
  ret = syscase_smcchar_receive(input, buffer, input_size);
  result = trace_test_case(
      input,
      input_size,
      // Linux Kernel
      0xffff000000000000L,
      0xffffffffffffffffL,
      // OPTEE core
      0xe100000,
      0xe143fff,
      trace
  );
  kfree(input);
  return ret;
}

static ssize_t syscase_smcchar_write(struct file *file, const char *buffer, size_t len, loff_t *offset) {
  unsigned long mode = (unsigned long) *offset;
  
  switch(mode) {
    case SMC_SET_TRACE:
      if(len > SMC_TRACE_SIZE) {
        len = SMC_TRACE_SIZE;
      }
      memset(&trace, 0, SMC_TRACE_SIZE);
      return syscase_smcchar_receive(&trace, buffer, len);
    case SMC_CALL:
      if(len > SMC_CALL_SIZE) {
        len = SMC_CALL_SIZE;
      }
      return syscase_smcchar_call(buffer, len);
    
    default:
      printk(KERN_INFO "smcchar: write with unknown mode %lu! Have you set offset?\n", mode);
      return -EFAULT;
  }
}

static int syscase_smcchar_release(struct inode *inodep, struct file *filep) {
  mutex_unlock(&smcchar_mutex);
  printk(KERN_INFO "smcchar: Device successfully closed\n");
  return 0;
}

// Register module
module_init(syscase_smcchar_init);
module_exit(syscase_smcchar_exit);
