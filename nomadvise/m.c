#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Zdenek Salvet");
MODULE_DESCRIPTION("block madvise");

#define __NR_compat_madvise 219

static asmlinkage long (*o_ptr)(unsigned long start, size_t len, int behavior);
static asmlinkage long (*o_ptr32)(unsigned long start, size_t len, int behavior);

asmlinkage long dummymadvise(unsigned long start, size_t len, int behavior) { return 0; }

static void sys_call_table_make_rw(void **addr);
static void sys_call_table_make_ro(void **addr);

static int __init init_dummymadvise(void) {
	void **sysctab = sys_call_table_addr;
	void **ia32_sysctab = ia32_sys_call_table_addr;

	sys_call_table_make_rw(sysctab);
	o_ptr = sysctab[__NR_madvise];
	sysctab[__NR_madvise] = dummymadvise;
	sys_call_table_make_ro(sysctab);

	sys_call_table_make_rw(ia32_sysctab);
	o_ptr32 = ia32_sysctab[__NR_compat_madvise];
	ia32_sysctab[__NR_compat_madvise] = dummymadvise;
	sys_call_table_make_ro(ia32_sysctab);

	return 0;
}

static void __exit exit_dummymadvise(void) {
	void **sysctab = sys_call_table_addr;
	void **ia32_sysctab = ia32_sys_call_table_addr;

	sys_call_table_make_rw(sysctab);
	sysctab[__NR_madvise] = o_ptr;
	sys_call_table_make_ro(sysctab);

	sys_call_table_make_rw(ia32_sysctab);
	ia32_sysctab[__NR_compat_madvise] = o_ptr32;
	sys_call_table_make_ro(ia32_sysctab);

}

module_init(init_dummymadvise);
module_exit(exit_dummymadvise);

static void sys_call_table_make_rw(void **addr) {
	unsigned int lvl;

	pte_t *pte = lookup_address((unsigned long) addr, &lvl);

	if (pte -> pte &~ _PAGE_RW)
		pte -> pte |= _PAGE_RW;

	write_cr0(read_cr0() & (~ 0x10000));
}

static void sys_call_table_make_ro(void **addr) {
	unsigned int lvl;

	pte_t *pte = lookup_address((unsigned long) addr, &lvl);
	pte -> pte = pte -> pte &~_PAGE_RW;

	write_cr0(read_cr0() | 0x10000);
}
