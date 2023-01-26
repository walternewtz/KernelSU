#ifndef __KSU_H_KSHOOK
#define __KSU_H_KSHOOK

#include "linux/fs.h"
#include "linux/types.h"
#include "linux/version.h"

// For sucompat

int ksu_handle_faccessat(int *dfd, const char __user **filename_user, int *mode,
			 int *flags);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
int ksu_handle_stat(int *dfd, const char __user **filename_user, int *flags);
#else
int ksu_handle_stat(int *dfd, const char __user **filename_user);
#endif

// For ksud

int ksu_handle_vfs_read(struct file **file_ptr, char __user **buf_ptr,
			size_t *count_ptr, loff_t **pos);

// For ksud and sucompat

int ksu_handle_execveat(int *fd, struct filename **filename_ptr, void *argv,
			void *envp, int *flags);

#endif
