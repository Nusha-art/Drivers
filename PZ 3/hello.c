#include <linux/module.h> /* Needed by all modules */ 
#include <linux/printk.h> /* Needed for pr_info() */ 
#include <linux/fs.h>
#include <linux/init.h>


static int device_open(struct inode *inode, struct file *file){
   return 0;
}
static int device_release(struct inode *inode, struct file *file){
  return 0;
}


const struct file_operations fops = {
//   .read = device_read,
//   .write = device_write,
   .open = device_open,
   .release = device_release
};

unsigned int Major;

static int __init hello_init(void) {

   pr_info("MatveevaElova.\n");

   Major = register_chrdev(0, "foo", &fops);
   if ((int)Major < 0) {
       printk(KERN_ALERT "Registering char device failed with %d\n", Major);
       return (int)Major;
   }

    /* A non 0 return means init_module failed; module can't be loaded. */
    return 0;
}

static void __exit hello_exit(void) { 
    pr_info("Goodbye.\n"); 
   unregister_chrdev(Major, "foo");

} 

module_init(hello_init);
module_exit(hello_exit);
 
MODULE_LICENSE("GPL");
