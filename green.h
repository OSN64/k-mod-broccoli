#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/syscalls.h>
#include <linux/fs.h>
#include <linux/namei.h>
#include <linux/delay.h>
#include <asm/paravirt.h>

#include <linux/audit.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OSN");
MODULE_DESCRIPTION("A Kernel Mod to hook syscalls ");

#define ENABLE_PAGE_PROTECTION write_cr0(original_cr0)
#define DISABLE_PAGE_PROTECTION write_cr0(original_cr0 & ~0x00010000)

unsigned long original_cr0;
unsigned long **sys_call_table;

static int __init hello_init(void);
static void __exit hello_cleanup(void);

// reference to old syscalls
asmlinkage long (*ref_sys_unlink)(const char __user *pathname);
asmlinkage long (*ref_sys_unlinkat)(int dfd, const char __user * pathname, int flag);
asmlinkage long (*ref_sys_read)(unsigned int fd, char __user *buf, size_t count);

// new syscalls
asmlinkage long new_sys_unlink(const char __user *pathname);
asmlinkage long new_sys_unlinkat(int dfd, const char __user * pathname, int flag);

static unsigned long **aquire_sys_call_table(void);

module_init(hello_init);
module_exit(hello_cleanup);
