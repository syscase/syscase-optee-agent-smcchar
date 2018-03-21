#ifndef SYSCASE_SMCCHAR_H
#define SYSCASE_SMCCHAR_H

static int syscase_smcchar_open(struct inode *, struct file *);
static int syscase_smcchar_release(struct inode *, struct file *);
static ssize_t syscase_smcchar_read(struct file *, char *, size_t, loff_t *);
static ssize_t syscase_smcchar_write(struct file *, const char *, size_t, loff_t *);

#endif /*SYSCASE_SMCCHAR_H*/

