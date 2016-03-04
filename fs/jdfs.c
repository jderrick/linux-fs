
static void jdfs_put_super(struct super_block *sb)
{
	pr_debug("jdfs super block destroyed\n");
}

static struct super_operations const jdfs_super_ops = {
	.put_super = jdfs_put_super,
}

static int jdfs_fill_sb(struct super_block *sb, void *data, int silent)
{
	struct inode *root = NULL;

	sb->s_magic = JDFS_MAGIC_NUMBER;
	sb->s_op = &jdfs_super_ops;

	root = new_inode(sb);
	if (!root) {
		pr_err("inode allocation failed\n");
		return -ENOMEM;
	}

	root->i_ino = 0;
	root->i_sb = sb;
	root->i_atime = root->i_mtime = root->i_ctime = CURRENT_TIME;
	inode_init_owner(root, NULL, S_IFDIR);

	sb->s_root = d_make_root(root);
	if (!sb->s_root) {
		pr_err("root creation failed\n");
		return -ENOMEM;
	}

	return 0;
}

static struct dentry *jdfs_mount(struct file_system_type *type, int flags,
				 char const *dev, void *data)
{
	struct dentry *const entry = mount_bdev(type, flags, dev, data,
						jdfs_fill_sb);

	if (IS_ERR(entry))
		pr_err("jdfs mounting failed\n");
	else
		pr_debug("jdfs mounted\n");
	return entry;
}

static struct file_system_type jdfs_type = {
	.owner = THIS_MODULE,
	.name = "jdfs",
	.mount = jdfs_mount,
	.kill_sb = kill_block_super,
	.fs_flags = FS_REQUIRES_DEV,
};

MODULE_AUTHOR("Jonathan Derrick <jonathan.derrick@intel.com>");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
//module_init(nvme_init);
//module_exit(nvme_exit);
