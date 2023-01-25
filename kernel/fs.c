#include <linux/sched.h>
#include <linux/fs_struct.h>

#include "fs.h"

#ifndef FILP_OPEN_WORKS_IN_WORKER
struct path root;
bool root_is_set = false;
#endif

#ifdef KERNEL_READ_WRITE_NEW_PROTOTYPES
ssize_t kernel_read_compat(struct file *p, void *buf, size_t count, loff_t *pos) {
	return kernel_read(p, buf, count, pos);
#else
ssize_t kernel_read_compat(struct file *p, void *buf, unsigned long count, loff_t *pos) {
	loff_t offset = pos ? *pos : 0;
	int result = kernel_read(p, *pos, (char *) buf, count);
	if (pos && result > 0)
		*pos = offset + result;
	return result;
#endif
}

ssize_t kernel_write_compat(struct file *p, const void *buf, size_t count, loff_t *pos) {
#ifdef KERNEL_READ_WRITE_NEW_PROTOTYPES
	return kernel_write(p, buf, count, pos);
#else
	loff_t offset = pos ? *pos : 0;
	ssize_t result = kernel_write(p, buf, count, offset);
	if (pos && result > 0)
		*pos = offset + result;
	return result;
#endif
}

// https://stackoverflow.com/questions/54181615/how-to-open-and-read-file-from-struct-inode-in-linux-kernel
struct file *filp_open_compat(const char *filename, int flags, umode_t mode) {
#ifndef FILP_OPEN_WORKS_IN_WORKER
	if (root_is_set)
		return file_open_root(root.dentry, root.mnt, filename, flags, mode);
	else {
		pr_warn("filp_open_compat('%s', ...): fs root is not saved yet, can we defer this access?\n", filename);
#endif
		return filp_open(filename, flags, mode);
#ifndef FILP_OPEN_WORKS_IN_WORKER
	}
#endif
}

#ifndef FILP_OPEN_WORKS_IN_WORKER
void ksu_save_fs_root(void) {
	if (root_is_set) {
		pr_warn("ksu_save_fs_root: cannot set fs root again.\n");
	} else {
		task_lock(current);
		get_fs_root(current->fs, &root);
		task_unlock(current);
		root_is_set = true;
	}
}
#endif
