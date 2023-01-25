#ifndef __KSU_H_FS
#define __KSU_H_FS

#include <linux/fs.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
#define KERNEL_READ_WRITE_NEW_PROTOTYPES
#define FILP_OPEN_WORKS_IN_WORKER
#endif

#ifdef KERNEL_READ_WRITE_NEW_PROTOTYPES
ssize_t kernel_read_compat(struct file *, void *, size_t, loff_t *);
#else
ssize_t kernel_read_compat(struct file *, void *, unsigned long, loff_t *);
#endif

ssize_t kernel_write_compat(struct file *, const void *, size_t, loff_t *);
#endif

struct file *filp_open_compat(const char *filename, int flags, umode_t mode);

void ksu_save_fs_root(void);
