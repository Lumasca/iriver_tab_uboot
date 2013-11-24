/*
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *              http://www.samsung.com/
 *
 * Added fat format.
 * Original license code is as follows:
 *
 * fat.c
 *
 * R/O (V)FAT 12/16/32 filesystem implementation by Marcus Sundberg
 *
 * 2002-07-28 - rjones@nexus-tech.net - ported to ppcboot v1.1.6
 * 2003-03-10 - kharris@nexus-tech.net - ported to uboot
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <config.h>
#include <fat.h>
#include <asm/byteorder.h>
#include <part.h>
#include <fastboot.h>

#if defined(CONFIG_CMD_FAT)

/*
 * Convert a string to lowercase.
 */
static void
downcase(char *str)
{
	while (*str != '\0') {
		TOLOWER(*str);
		str++;
	}
}

static  block_dev_desc_t *cur_dev = NULL;
static unsigned long part_offset = 0;
static int cur_part = 1;

#define BYTE_PER_SEC	512
#define RESERVED_CNT	32
#define DOS_PART_TBL_OFFSET	0x1be
#define DOS_PART_MAGIC_OFFSET	0x1fe
#define DOS_FS_TYPE_OFFSET	0x36

int disk_read (__u32 startblock, __u32 getsize, __u8 * bufptr)
{
	startblock += part_offset;
	if (cur_dev == NULL)
		return -1;
	if (cur_dev->block_read) {
		return cur_dev->block_read (cur_dev->dev
			, startblock, getsize, (unsigned long *)bufptr);
	}
	return -1;
}


int
fat_register_device(block_dev_desc_t *dev_desc, int part_no)
{
	unsigned char buffer[SECTOR_SIZE];
	disk_partition_t info;

	if (!dev_desc->block_read)
		return -1;
	cur_dev = dev_desc;
	/* check if we have a MBR (on floppies we have only a PBR) */
	if (dev_desc->block_read (dev_desc->dev, 0, 1, (ulong *) buffer) != 1) {
		FAT_DPRINT ("** Can't read from device %d **\n", dev_desc->dev);
		return -1;
	}
	if (buffer[DOS_PART_MAGIC_OFFSET] != 0x55 ||
		buffer[DOS_PART_MAGIC_OFFSET + 1] != 0xaa) {
		/* no signature found */
		return -1;
	}
#if (defined(CONFIG_CMD_IDE) || \
     defined(CONFIG_CMD_SCSI) || \
     defined(CONFIG_CMD_USB) || \
     defined(CONFIG_MMC) || \
     defined(CONFIG_SYSTEMACE) )
	/* First we assume, there is a MBR */
	if (!get_partition_info (dev_desc, part_no, &info)) {
		part_offset = info.start;
		cur_part = part_no;
	} else if (!strncmp((char *)&buffer[DOS_FS_TYPE_OFFSET], "FAT", 3)) {
		/* ok, we assume we are on a PBR only */
		cur_part = 1;
		part_offset = 0;
	} else {
		FAT_DPRINT ("** Partition %d not valid on device %d **\n",
				part_no, dev_desc->dev);
		return -1;
	}

#else
	if (!strncmp((char *)&buffer[DOS_FS_TYPE_OFFSET],"FAT",3)) {
		/* ok, we assume we are on a PBR only */
		cur_part = 1;
		part_offset = 0;
		info.start = part_offset;
	} else {
		/* FIXME we need to determine the start block of the
		 * partition where the DOS FS resides. This can be done
		 * by using the get_partition_info routine. For this
		 * purpose the libpart must be included.
		 */
		part_offset = 32;
		cur_part = 1;
	}
#endif
	FAT_DPRINT("Partition%d: Start Address(0x%x), Size(0x%x)\n", part_no, info.start, info.size);
	return 0;
}

/*
 * Copy string, padding with spaces.
 */
static void
setstr(u_int8_t *dest, const char *src, size_t len)
{
    while (len--)
	*dest++ = *src ? *src++ : ' ';
}

static int write_pbr(block_dev_desc_t *dev_desc, disk_partition_t *info)
{
	struct bs *bs;
	struct bsbpb *bsbpb;
	struct bsxbpb *bsxbpb;
	struct bsx *bsx;
	__u8 *img;
	int img_offset = 0;
	int reserved_cnt= 0;
	int i;
	int fat_size = 0;

	img = malloc(sizeof(__u8)*512);
	if(img == NULL) {
		FAT_DPRINT("Can't make img buffer~~!!\n");
		return -1;
	}
	memset(img, 0x0, sizeof(__u8)*512);


	/* Erase Reserved Sector(PBR) */
	for (i = 0;i < RESERVED_CNT; i++) {
		if (dev_desc->block_write(dev_desc->dev, info->start + i, 1, (ulong *)img) != 1) {
			FAT_DPRINT ("Can't erase reserved sector~~~!!!\n");
			return -1;
		}
	}

	/* Set bs */
	bs = (struct bs *)img;
	img_offset += sizeof(struct bs) - 1;

	mk1(bs->jmp[0], 0xeb);
	mk1(bs->jmp[1], 0x58);
	mk1(bs->jmp[2], 0x90); /* Jump Boot Code */
	setstr(bs->oem, "SAMSUNG", sizeof(bs->oem)); /* OEM Name */

	uint spc;
	/* Set bsbpb */
	bsbpb = (struct bsbpb *)(img + img_offset);
	img_offset += sizeof(struct bsbpb) - 3;

	mk2(bsbpb->bps, 512); /* Byte Per Sector */
	/* Sector Per Cluster */
	if (info->size < 0x10000) { /* partition size >= 32Mb */
		FAT_DPRINT("Can't format less than 32Mb partition!!\n");
		return -1;
	}
	if (info->size <= 0x20000) { /* under 64M -> 512 bytes */
		FAT_DPRINT("Under 64M\n");
		mk1(bsbpb->spc, 1);
		spc = 1;
	}
	else if (info->size <= 0x40000) { /* under 128M -> 1K */
		FAT_DPRINT("Under 128M\n");
		mk1(bsbpb->spc, 2);
		spc = 2;
	}
	else if (info->size <= 0x80000) { /* under 256M -> 2K */
		FAT_DPRINT("Under 256M\n");
		mk1(bsbpb->spc, 4);
		spc = 4;
	}
	else if (info->size <= 0xFA0000) { /* under 8G -> 4K */
		FAT_DPRINT("Under 8G\n");
		mk1(bsbpb->spc, 8);
		spc = 8;
	}
/* ktj increase spc to 32K */
#if defined(CONFIG_MX100) 
	else if (info->size <= 0x1F40000) { /* under 16G -> 32K */
		FAT_DPRINT("Under 16G\n");
		mk1(bsbpb->spc, 64);
		spc = 64;
	}
#else
	else if (info->size <= 0x1F40000) { /* under 16G -> 8K */
		FAT_DPRINT("Under 16G\n");
		mk1(bsbpb->spc, 16);
		spc = 16;
	}
#endif
	else {
		FAT_DPRINT("16G~\n");
		mk1(bsbpb->spc, 32);
		spc = 32;
	}

	mk2(bsbpb->res, RESERVED_CNT); /* Reserved Sector Count */
	mk1(bsbpb->nft, 2); /* Number of FATs */
	mk2(bsbpb->rde, 0); /* Root Directory Entry Count : It's no use in FAT32 */
	mk2(bsbpb->sec, 0); /* Total Sector : It's no use in FAT32 */
	mk1(bsbpb->mid, 0xF8); /* Media */
	mk2(bsbpb->spf, 0); /* FAT Size 16 : It's no use in FAT32 */
	mk2(bsbpb->spt, 0); /* Sector Per Track */
	mk2(bsbpb->hds, 0); /* Number Of Heads */
	mk4(bsbpb->hid, 0); /* Hidden Sector */
	mk4(bsbpb->bsec, info->size); /* Total Sector For FAT32 */

	/* Set bsxbpb */
	bsxbpb = (struct bsxbpb *)(img + img_offset);
	img_offset += sizeof(struct bsxbpb);

	mk4(bsxbpb->bspf, (info->size / (spc * 128))); /* FAT Size 32 */
	fat_size = info->size / (spc * 128);
	FAT_DPRINT("Fat size : 0x%x\n", info->size / (spc * 128));
	mk2(bsxbpb->xflg, 0); /* Ext Flags */
	mk2(bsxbpb->vers, 0); /* File System Version */
	mk4(bsxbpb->rdcl, 2); /* Root Directory Cluster */
	mk2(bsxbpb->infs, 1); /* File System Information */
	mk2(bsxbpb->bkbs, 0); /* Boot Record Backup Sector */

	/* Set bsx */
	bsx = (struct bsx *)(img + img_offset);
	mk1(bsx->drv, 0); /* Drive Number */
	mk1(bsx->sig, 0x29); /* Boot Signature */
	mk4(bsx->volid, 0x3333); /* Volume ID : 0x3333 means nothing */
	setstr(bsx->label, "NO NAME ", sizeof(bsx->label)); /* Volume Label */
	setstr(bsx->type, "FAT32", sizeof(bsx->type)); /* File System Type */

	/* Set Magic Number */
	mk2(img + BYTE_PER_SEC - 2, 0xaa55); /* Signature */

	/*
	FAT_DPRINT("Print Boot Recode\n");
	for(i = 0;i<512;i++) {
		if(img[i] == 0)
			FAT_DPRINT("00 ");
		else
			FAT_DPRINT("%2x ", img[i]);
		if (!((i+1) % 16))
			FAT_DPRINT("\n");
	}
	*/

	if (dev_desc->block_write(dev_desc->dev, info->start, 1, (ulong *)img) != 1) {
		FAT_DPRINT ("Can't write PBR~~~!!!\n");
		return -1;
	}

	return fat_size;
}
static int write_reserved(block_dev_desc_t *dev_desc, disk_partition_t *info)
{
	/* Set Reserved Region */
	__u8 *img;
	int i;
	img = malloc(sizeof(__u8)*512);
	if(img == NULL) {
		FAT_DPRINT("Can't make img buffer~~(reserved)!!\n");
		return -1;
	}

	memset(img, 0x0, sizeof(__u8)*512);

	mk4(img, 0x41615252); /* Lead Signature */
	mk4(img + BYTE_PER_SEC - 28, 0x61417272); /* Struct Signature */
	mk4(img + BYTE_PER_SEC - 24, 0xffffffff); /* Free Cluster Count */
	mk4(img + BYTE_PER_SEC - 20, 0x3); /* Next Free Cluster */
	mk2(img + BYTE_PER_SEC - 2, 0xaa55); /* Trail Signature */

	/*
	FAT_DPRINT("Print Reserved Region\n");
	for(i = 0;i<512;i++) {
		if(img[i] == 0)
			FAT_DPRINT("00 ");
		else
			FAT_DPRINT("%2x ", img[i]);
		if (!((i+1) % 16))
			FAT_DPRINT("\n");
	}
	*/

	/* Write Reserved region */
	if (dev_desc->block_write(dev_desc->dev, info->start+1, 1, (ulong *)img) != 1) {
		FAT_DPRINT ("Can't write reserved region~~~!!!\n");
		return -1;
	}

	return 1;
}
static int write_fat(block_dev_desc_t *dev_desc, disk_partition_t *info, int
fat_size)
{
	__u8 *dummy;
	__u8 *img;
	int i;

	/* Create buffer for FAT */
	img = malloc(sizeof(__u8)*512);
	if(img == NULL) {
		FAT_DPRINT("Can't make img buffer~~!!\n");
		return -1;
	}
	memset(img, 0x0, sizeof(__u8) * 512);

	/* Create buffer for erase */
	dummy = malloc(sizeof(__u8) * 8192);
	if(dummy == NULL) {
		FAT_DPRINT("Can't make dummy buffer~~!!\n");
		return -1;
	}
	memset(dummy, 0x0, sizeof(__u8) * 8192);

	/* Erase FAT Region */
	int erase_block_cnt = (fat_size * 2);
	int temp = 0;
	/*Shanghai ewada add root dir to erased*/
	erase_block_cnt +=64 *4;
	printf("Erase FAT region erase_block_cnt%d=", erase_block_cnt);
	/*Shanghai ewada for uboot format error*/
	for (i = 0; i + 16  <= erase_block_cnt /*+ 10*/; i+=16) {
		if (dev_desc->block_write(dev_desc->dev, info->start +
			RESERVED_CNT + i, 16, (ulong *)dummy) != 16) {
			FAT_DPRINT ("Can't erase FAT region~~!!!\n");
		}
		if((i % 160) == 0)
			FAT_DPRINT(".");

	}

	/*Shanghai ewada for uboot format error*/
	if (erase_block_cnt%16){
		temp = erase_block_cnt - i -1;
		printf("erase left some i=%d temp=%d \n", i, temp);
		if (dev_desc->block_write(dev_desc->dev, info->start +
			RESERVED_CNT + i, temp, (ulong *)dummy) != temp) {
			printf ("Can't erase FAT region~~!!!\n");
		}
		
	}
	printf("\n");

	mk4(img, 0x0ffffff8);
	mk4(img+4, 0x0fffffff);
	mk4(img+8, 0x0fffffff); /* Root Directory */

	/*
	FAT_DPRINT("Print FAT Region\n");
	for(i = 0;i<512;i++) {
		if(img[i] == 0)
			FAT_DPRINT("00 ");
		else
			FAT_DPRINT("%2x ", img[i]);
		if (!((i+1) % 16))
			FAT_DPRINT("\n");
	}
	*/
	/* Write FAT Region */
	if (dev_desc->block_write(dev_desc->dev, info->start + RESERVED_CNT, 1, (ulong *)img) != 1) {
		FAT_DPRINT ("Can't write FAT~~~!!!\n");
		return -1;
	}

	return 1;
}

/*
 * Make a volume label.
 */

static void
mklabel(u_int8_t *dest, const char *src)
{
    int c, i;

    for (i = 0; i < 11; i++) {
	c = *src ? __toupper(*src++) : ' ';
	*dest++ = !i && c == '\xe5' ? 5 : c;
    }
}

/*
 * Format device
 */
int
fat_format_device(block_dev_desc_t *dev_desc, int part_no)
{
	unsigned char buffer[SECTOR_SIZE];
	disk_partition_t info;

	if (!dev_desc->block_read)
		return -1;
	cur_dev = dev_desc;
	/* check if we have a MBR (on floppies we have only a PBR) */
	if (dev_desc->block_read (dev_desc->dev, 0, 1, (ulong *) buffer) != 1) {
		FAT_DPRINT ("** Can't read from device %d **\n", dev_desc->dev);
		return -1;
	}
	if (buffer[DOS_PART_MAGIC_OFFSET] != 0x55 ||
		buffer[DOS_PART_MAGIC_OFFSET + 1] != 0xaa) {
		FAT_DPRINT("** MBR is broken **\n");
		/* no signature found */
		return -1;
	}
#if (defined(CONFIG_CMD_IDE) || \
     defined(CONFIG_CMD_SCSI) || \
     defined(CONFIG_CMD_USB) || \
     defined(CONFIG_MMC) || \
     defined(CONFIG_SYSTEMACE) )
	/* First we assume, there is a MBR */
	if (!get_partition_info (dev_desc, part_no, &info)) {
		part_offset = info.start;
		cur_part = part_no;
	} else if (!strncmp((char *)&buffer[DOS_FS_TYPE_OFFSET], "FAT", 3)) {
		/* ok, we assume we are on a PBR only */
		cur_part = 1;
		part_offset = 0;
	} else {
		FAT_DPRINT ("** Partition %d not valid on device %d **\n",
				part_no, dev_desc->dev);
		return -1;
	}
#endif

	FAT_DPRINT("Partition%d: Start Address(0x%x), Size(0x%x)\n", part_no, info.start, info.size);

	int fat_size;
	fat_size = write_pbr(dev_desc, &info);
	if(fat_size < 0)
		return -1;
	if(write_reserved(dev_desc, &info) < 0)
		return -1;
	if(write_fat(dev_desc, &info, fat_size) < 0)
		return -1;
	FAT_DPRINT("Partition%d format complete.\n", part_no);

	return 0;
}

/*
 * Get the first occurence of a directory delimiter ('/' or '\') in a string.
 * Return index into string if found, -1 otherwise.
 */
static int
dirdelim(char *str)
{
	char *start = str;

	while (*str != '\0') {
		if (ISDIRDELIM(*str)) return str - start;
		str++;
	}
	return -1;
}


/*
 * Match volume_info fs_type strings.
 * Return 0 on match, -1 otherwise.
 */
static int
compare_sign(char *str1, char *str2)
{
	char *end = str1+SIGNLEN;

	while (str1 != end) {
		if (*str1 != *str2) {
			return -1;
		}
		str1++;
		str2++;
	}

	return 0;
}


/*
 * Extract zero terminated short name from a directory entry.
 */
static void get_name (dir_entry *dirent, char *s_name)
{
	char *ptr;

	memcpy (s_name, dirent->name, 8);
	s_name[8] = '\0';
	ptr = s_name;
	while (*ptr && *ptr != ' ')
		ptr++;
	if (dirent->ext[0] && dirent->ext[0] != ' ') {
		*ptr = '.';
		ptr++;
		memcpy (ptr, dirent->ext, 3);
		ptr[3] = '\0';
		while (*ptr && *ptr != ' ')
			ptr++;
	}
	*ptr = '\0';
	if (*s_name == DELETED_FLAG)
		*s_name = '\0';
	else if (*s_name == aRING)
		*s_name = '�';
	downcase (s_name);
}

/*
 * Get the entry at index 'entry' in a FAT (12/16/32) table.
 * On failure 0x00 is returned.
 */
static __u32
get_fatent(fsdata *mydata, __u32 entry)
{
	__u32 bufnum;
	__u32 offset;
	__u32 ret = 0x00;

	switch (mydata->fatsize) {
	case 32:
		bufnum = entry / FAT32BUFSIZE;
		offset = entry - bufnum * FAT32BUFSIZE;
		break;
	case 16:
		bufnum = entry / FAT16BUFSIZE;
		offset = entry - bufnum * FAT16BUFSIZE;
		break;
	case 12:
		bufnum = entry / FAT12BUFSIZE;
		offset = entry - bufnum * FAT12BUFSIZE;
		break;

	default:
		/* Unsupported FAT size */
		return ret;
	}

	/* Read a new block of FAT entries into the cache. */
	if (bufnum != mydata->fatbufnum) {
		int getsize = FATBUFSIZE/FS_BLOCK_SIZE;
		__u8 *bufptr = mydata->fatbuf;
		__u32 fatlength = mydata->fatlength;
		__u32 startblock = bufnum * FATBUFBLOCKS;

		fatlength *= SECTOR_SIZE;	/* We want it in bytes now */
		startblock += mydata->fat_sect;	/* Offset from start of disk */

		if (getsize > fatlength) getsize = fatlength;
		if (disk_read(startblock, getsize, bufptr) < 0) {
			FAT_DPRINT("Error reading FAT blocks\n");
			return ret;
		}
		mydata->fatbufnum = bufnum;
	}

	/* Get the actual entry from the table */
	switch (mydata->fatsize) {
	case 32:
		ret = FAT2CPU32(((__u32*)mydata->fatbuf)[offset]);
		break;
	case 16:
		ret = FAT2CPU16(((__u16*)mydata->fatbuf)[offset]);
		break;
	case 12: {
		__u32 off16 = (offset*3)/4;
		__u16 val1, val2;

		switch (offset & 0x3) {
		case 0:
			ret = FAT2CPU16(((__u16*)mydata->fatbuf)[off16]);
			ret &= 0xfff;
			break;
		case 1:
			val1 = FAT2CPU16(((__u16*)mydata->fatbuf)[off16]);
			val1 &= 0xf000;
			val2 = FAT2CPU16(((__u16*)mydata->fatbuf)[off16+1]);
			val2 &= 0x00ff;
			ret = (val2 << 4) | (val1 >> 12);
			break;
		case 2:
			val1 = FAT2CPU16(((__u16*)mydata->fatbuf)[off16]);
			val1 &= 0xff00;
			val2 = FAT2CPU16(((__u16*)mydata->fatbuf)[off16+1]);
			val2 &= 0x000f;
			ret = (val2 << 8) | (val1 >> 8);
			break;
		case 3:
			ret = FAT2CPU16(((__u16*)mydata->fatbuf)[off16]);;
			ret = (ret & 0xfff0) >> 4;
			break;
		default:
			break;
		}
	}
	break;
	}
	FAT_DPRINT("ret: %d, offset: %d\n", ret, offset);

	return ret;
}


#ifdef SD_DATA_COPY_SLICE

static int
get_cluster_slice(fsdata *mydata, __u32 clustnum, __u8 *buffer, unsigned long size)
{
	int idx = 0;
	__u32 startsect;

	if (clustnum > 0) {
		startsect = mydata->data_begin + clustnum*mydata->clust_size;
	} else {
		startsect = mydata->rootdir_sect;
	}

	if (disk_read(startsect, size/FS_BLOCK_SIZE , buffer) < 0) {
		FAT_DPRINT("Error reading data\n");
		return -1;
	}
	if(size % FS_BLOCK_SIZE) {
		__u8 tmpbuf[FS_BLOCK_SIZE];
		idx= size/FS_BLOCK_SIZE;
		if (disk_read(startsect + idx, 1, tmpbuf) < 0) {
			FAT_DPRINT("Error reading data\n");
			return -1;
		}
		buffer += idx*FS_BLOCK_SIZE;

		memcpy(buffer, tmpbuf, size % FS_BLOCK_SIZE);
		return 0;
	}

	return 0;
}


#endif

/*
 * Read at most 'size' bytes from the specified cluster into 'buffer'.
 * Return 0 on success, -1 otherwise.
 */
static int
get_cluster(fsdata *mydata, __u32 clustnum, __u8 *buffer, unsigned long size)
{
	int idx = 0;
	__u32 startsect;
	#ifdef SD_DATA_COPY_SLICE
	if(mydata->data_slice_copy.slice_read_mode==1) {
		unsigned long ret,i;
		unsigned long read_size;
		unsigned long once_read_size = DATA_COPY_SLICE_SIZE;
		unsigned long read_count;
		unsigned long read_rest;
		unsigned long read_cluster_num;
		unsigned long readed_size = 0;

		read_count = size / once_read_size;

		read_rest = size % once_read_size;

		if(read_rest >0) {
			read_count ++;
		}

		MX100_DPRINT("size:%d clustnum: %d read_count : %d read_rest:%d clust_size:%d\n",size,clustnum,read_count,read_rest,mydata->clust_size);

		for(i=0;i<read_count ;i++) {
			if(i==(read_count -1)) {
				if(read_rest > 0) {
					read_size = read_rest;
				} else {
					read_size = once_read_size;
				}
			} else {
				read_size = once_read_size;
			}
			read_cluster_num = clustnum + readed_size / (mydata->clust_size *FS_BLOCK_SIZE);

			MX100_DPRINT("data readed :%ld bytes read_cluster_num:%d read_size:%d \n",mydata->data_slice_copy.data_readed,read_cluster_num,read_size);

			ret = get_cluster_slice(mydata,read_cluster_num, buffer,read_size);
			mydata->callback_write_slice(&mydata->data_slice_copy,buffer,read_size);


			readed_size += read_size;			
		}
		return 0;
	}
	else 
	#endif
	{
		if (clustnum > 0) {
			startsect = mydata->data_begin + clustnum*mydata->clust_size;
		} else {
			startsect = mydata->rootdir_sect;
		}

		if (disk_read(startsect, size/FS_BLOCK_SIZE , buffer) < 0) {
			FAT_DPRINT("Error reading data\n");
			return -1;
		}
		if(size % FS_BLOCK_SIZE) {
			__u8 tmpbuf[FS_BLOCK_SIZE];
			idx= size/FS_BLOCK_SIZE;
			if (disk_read(startsect + idx, 1, tmpbuf) < 0) {
				FAT_DPRINT("Error reading data\n");
				return -1;
			}
			buffer += idx*FS_BLOCK_SIZE;

			memcpy(buffer, tmpbuf, size % FS_BLOCK_SIZE);
			return 0;
		}
	}
	return 0;
}


/*
 * Read at most 'maxsize' bytes from the file associated with 'dentptr'
 * into 'buffer'.
 * Return the number of bytes read or -1 on fatal errors.
 */
static long
get_contents(fsdata *mydata, dir_entry *dentptr, __u8 *buffer,
	     unsigned long maxsize)
{
	unsigned long filesize = FAT2CPU32(dentptr->size), gotsize = 0;
	unsigned int bytesperclust = mydata->clust_size * SECTOR_SIZE;
	__u32 curclust = START(dentptr);
	__u32 endclust, newclust;
	unsigned long actsize;

	FAT_DPRINT("Filesize: %ld bytes\n", filesize);

	if (maxsize > 0 && filesize > maxsize) filesize = maxsize;

	FAT_DPRINT("Reading: %ld bytes\n", filesize);

	actsize=bytesperclust;
	endclust=curclust;
	do {
		/* search for consecutive clusters */
		while(actsize < filesize) {
			newclust = get_fatent(mydata, endclust);
			if((newclust -1)!=endclust)
				goto getit;
			if (CHECK_CLUST(newclust, mydata->fatsize)) {
				FAT_DPRINT("curclust: 0x%x\n", newclust);
				FAT_DPRINT("Invalid FAT entry\n");
				return gotsize;
			}
			endclust=newclust;
			actsize+= bytesperclust;
		}
		/* actsize >= file size */
		actsize -= bytesperclust;

		/* get remaining clusters */
		if (get_cluster(mydata, curclust, buffer, (int)actsize) != 0) {
			FAT_ERROR("Error reading cluster\n");
			return -1;
		}
		/* get remaining bytes */
		gotsize += (int)actsize;
		filesize -= actsize;
		#ifdef SD_DATA_COPY_SLICE
		if(mydata->data_slice_copy.slice_read_mode==1) {  // not increase buffer address.

		} else
		#endif
		{
			buffer += actsize;
		}
		actsize= filesize;

		#ifdef SD_DATA_COPY_SLICE
		MX100_DPRINT("endclust:%d actsize:%d\n",endclust,actsize);
		#endif

		if (get_cluster(mydata, endclust, buffer, (int)actsize) != 0) {
			FAT_ERROR("Error reading cluster\n");
			return -1;
		}
		gotsize+=actsize;

		FAT_DPRINT("filesize:%ld bytes\n",filesize);

		return gotsize;
getit:
		if (get_cluster(mydata, curclust, buffer, (int)actsize) != 0) {
			FAT_ERROR("Error reading cluster\n");
			return -1;
		}
		gotsize += (int)actsize;
		filesize -= actsize;
		#ifdef SD_DATA_COPY_SLICE
		if(mydata->data_slice_copy.slice_read_mode==1) {  // not increase buffer address.

		} else
		#endif
		{
			buffer += actsize;
		}
		curclust = get_fatent(mydata, endclust);
		if (CHECK_CLUST(curclust, mydata->fatsize)) {
			FAT_DPRINT("curclust: 0x%x\n", curclust);
			FAT_ERROR("Invalid FAT entry\n");
			return gotsize;
		}
		actsize=bytesperclust;
		endclust=curclust;
	} while (1);
}


#ifdef CONFIG_SUPPORT_VFAT
/*
 * Extract the file name information from 'slotptr' into 'l_name',
 * starting at l_name[*idx].
 * Return 1 if terminator (zero byte) is found, 0 otherwise.
 */
static int
slot2str(dir_slot *slotptr, char *l_name, int *idx)
{
	int j;

	for (j = 0; j <= 8; j += 2) {
		l_name[*idx] = slotptr->name0_4[j];
		if (l_name[*idx] == 0x00) return 1;
		(*idx)++;
	}
	for (j = 0; j <= 10; j += 2) {
		l_name[*idx] = slotptr->name5_10[j];
		if (l_name[*idx] == 0x00) return 1;
		(*idx)++;
	}
	for (j = 0; j <= 2; j += 2) {
		l_name[*idx] = slotptr->name11_12[j];
		if (l_name[*idx] == 0x00) return 1;
		(*idx)++;
	}

	return 0;
}


/*
 * Extract the full long filename starting at 'retdent' (which is really
 * a slot) into 'l_name'. If successful also copy the real directory entry
 * into 'retdent'
 * Return 0 on success, -1 otherwise.
 */
__u8	 get_vfatname_block[MAX_CLUSTSIZE];
static int
get_vfatname(fsdata *mydata, int curclust, __u8 *cluster,
	     dir_entry *retdent, char *l_name)
{
	dir_entry *realdent;
	dir_slot  *slotptr = (dir_slot*) retdent;
	__u8	  *nextclust = cluster + mydata->clust_size * SECTOR_SIZE;
	__u8	   counter = (slotptr->id & ~LAST_LONG_ENTRY_MASK) & 0xff;
	int idx = 0;

	while ((__u8*)slotptr < nextclust) {
		if (counter == 0) break;
		if (((slotptr->id & ~LAST_LONG_ENTRY_MASK) & 0xff) != counter)
			return -1;
		slotptr++;
		counter--;
	}

	if ((__u8*)slotptr >= nextclust) {
		dir_slot *slotptr2;

		slotptr--;
		curclust = get_fatent(mydata, curclust);
		if (CHECK_CLUST(curclust, mydata->fatsize)) {
			FAT_DPRINT("curclust: 0x%x\n", curclust);
			FAT_ERROR("Invalid FAT entry\n");
			return -1;
		}
		if (get_cluster(mydata, curclust, get_vfatname_block,
				mydata->clust_size * SECTOR_SIZE) != 0) {
			FAT_DPRINT("Error: reading directory block\n");
			return -1;
		}
		slotptr2 = (dir_slot*) get_vfatname_block;
		while (slotptr2->id > 0x01) {
			slotptr2++;
		}
		/* Save the real directory entry */
		realdent = (dir_entry*)slotptr2 + 1;
		while ((__u8*)slotptr2 >= get_vfatname_block) {
			slot2str(slotptr2, l_name, &idx);
			slotptr2--;
		}
	} else {
		/* Save the real directory entry */
		realdent = (dir_entry*)slotptr;
	}

	do {
		slotptr--;
		if (slot2str(slotptr, l_name, &idx)) break;
	} while (!(slotptr->id & LAST_LONG_ENTRY_MASK));

	l_name[idx] = '\0';
	if (*l_name == DELETED_FLAG) *l_name = '\0';
	else if (*l_name == aRING) *l_name = '�';
	downcase(l_name);

	/* Return the real directory entry */
	memcpy(retdent, realdent, sizeof(dir_entry));

	return 0;
}


/* Calculate short name checksum */
static __u8
mkcksum(const char *str)
{
	int i;
	__u8 ret = 0;

	for (i = 0; i < 11; i++) {
		ret = (((ret&1)<<7)|((ret&0xfe)>>1)) + str[i];
	}

	return ret;
}
#endif


/*
 * Get the directory entry associated with 'filename' from the directory
 * starting at 'startsect'
 */
__u8 get_dentfromdir_block[MAX_CLUSTSIZE];
static dir_entry *get_dentfromdir (fsdata * mydata, int startsect,
				   char *filename, dir_entry * retdent,
				   int dols)
{
    __u16 prevcksum = 0xffff;
    __u32 curclust = START (retdent);
    int files = 0, dirs = 0;

    FAT_DPRINT ("get_dentfromdir: %s\n", filename);
    while (1) {
	dir_entry *dentptr;
	int i;

	if (get_cluster (mydata, curclust, get_dentfromdir_block,
		 mydata->clust_size * SECTOR_SIZE) != 0) {
	    FAT_DPRINT ("Error: reading directory block\n");
	    return NULL;
	}
	dentptr = (dir_entry *) get_dentfromdir_block;
	for (i = 0; i < DIRENTSPERCLUST; i++) {
	    char s_name[14], l_name[256];

	    l_name[0] = '\0';
	    if (dentptr->name[0] == DELETED_FLAG) {
		    dentptr++;
		    continue;
	    }
	    if ((dentptr->attr & ATTR_VOLUME)) {
#ifdef CONFIG_SUPPORT_VFAT
		if ((dentptr->attr & ATTR_VFAT) &&
		    (dentptr->name[0] & LAST_LONG_ENTRY_MASK)) {
		    prevcksum = ((dir_slot *) dentptr)
			    ->alias_checksum;
		    get_vfatname (mydata, curclust, get_dentfromdir_block,
				  dentptr, l_name);
		    if (dols) {
			int isdir = (dentptr->attr & ATTR_DIR);
			char dirc;
			int doit = 0;

			if (isdir) {
			    dirs++;
			    dirc = '/';
			    doit = 1;
			} else {
			    dirc = ' ';
			    if (l_name[0] != 0) {
				files++;
				doit = 1;
			    }
			}
			if (doit) {
			    if (dirc == ' ') {
				FAT_DPRINT (" %8ld   %s%c\n",
					(long) FAT2CPU32 (dentptr->size),
					l_name, dirc);
			    } else {
				FAT_DPRINT ("            %s%c\n", l_name, dirc);
			    }
			}
			dentptr++;
			continue;
		    }
		    FAT_DPRINT ("vfatname: |%s|\n", l_name);
		} else
#endif
		{
		    /* Volume label or VFAT entry */
		    dentptr++;
		    continue;
		}
	    }
	    if (dentptr->name[0] == 0) {
		if (dols) {
		    FAT_DPRINT ("\n%d file(s), %d dir(s)\n\n", files, dirs);
		}
		FAT_DPRINT ("Dentname == NULL - %d\n", i);
		return NULL;
	    }
#ifdef CONFIG_SUPPORT_VFAT
	    if (dols && mkcksum (dentptr->name) == prevcksum) {
		dentptr++;
		continue;
	    }
#endif
	    get_name (dentptr, s_name);
	    if (dols) {
		int isdir = (dentptr->attr & ATTR_DIR);
		char dirc;
		int doit = 0;

		if (isdir) {
		    dirs++;
		    dirc = '/';
		    doit = 1;
		} else {
		    dirc = ' ';
		    if (s_name[0] != 0) {
			files++;
			doit = 1;
		    }
		}
		if (doit) {
		    if (dirc == ' ') {
			FAT_DPRINT (" %8ld   %s%c\n",
				(long) FAT2CPU32 (dentptr->size), s_name,
				dirc);
		    } else {
			FAT_DPRINT ("            %s%c\n", s_name, dirc);
		    }
		}
		dentptr++;
		continue;
	    }
	    if (strcmp (filename, s_name) && strcmp (filename, l_name)) {
		FAT_DPRINT ("Mismatch: |%s|%s|\n", s_name, l_name);
		dentptr++;
		continue;
	    }
	    memcpy (retdent, dentptr, sizeof (dir_entry));

	    FAT_DPRINT ("DentName: %s", s_name);
	    FAT_DPRINT (", start: 0x%x", START (dentptr));
	    FAT_DPRINT (", size:  0x%x %s\n",
			FAT2CPU32 (dentptr->size),
			(dentptr->attr & ATTR_DIR) ? "(DIR)" : "");

	    return retdent;
	}
	curclust = get_fatent (mydata, curclust);
	if (CHECK_CLUST(curclust, mydata->fatsize)) {
	    FAT_DPRINT ("curclust: 0x%x\n", curclust);
	    FAT_ERROR ("Invalid FAT entry\n");
	    return NULL;
	}
    }

    return NULL;
}


/*
 * Read boot sector and volume info from a FAT filesystem
 */
static int
read_bootsectandvi(boot_sector *bs, volume_info *volinfo, int *fatsize)
{
	__u8 block[FS_BLOCK_SIZE];
	volume_info *vistart;

	if (disk_read(0, 1, block) < 0) {
		FAT_DPRINT("Error: reading block\n");
		return -1;
	}

	memcpy(bs, block, sizeof(boot_sector));
	bs->reserved	= FAT2CPU16(bs->reserved);
	bs->fat_length	= FAT2CPU16(bs->fat_length);
	bs->secs_track	= FAT2CPU16(bs->secs_track);
	bs->heads	= FAT2CPU16(bs->heads);
#if 0 /* UNUSED */
	bs->hidden	= FAT2CPU32(bs->hidden);
#endif
	bs->total_sect	= FAT2CPU32(bs->total_sect);

	/* FAT32 entries */
	if (bs->fat_length == 0) {
		/* Assume FAT32 */
		bs->fat32_length = FAT2CPU32(bs->fat32_length);
		bs->flags	 = FAT2CPU16(bs->flags);
		bs->root_cluster = FAT2CPU32(bs->root_cluster);
		bs->info_sector  = FAT2CPU16(bs->info_sector);
		bs->backup_boot  = FAT2CPU16(bs->backup_boot);
		vistart = (volume_info*) (block + sizeof(boot_sector));
		*fatsize = 32;
	} else {
		vistart = (volume_info*) &(bs->fat32_length);
		*fatsize = 0;
	}
	memcpy(volinfo, vistart, sizeof(volume_info));

	/* Terminate fs_type string. Writing past the end of vistart
	   is ok - it's just the buffer. */
	vistart->fs_type[8] = '\0';

	if (*fatsize == 32) {
		if (compare_sign(FAT32_SIGN, vistart->fs_type) == 0) {
			return 0;
		}
	} else {
		if (compare_sign(FAT12_SIGN, vistart->fs_type) == 0) {
			*fatsize = 12;
			return 0;
		}
		if (compare_sign(FAT16_SIGN, vistart->fs_type) == 0) {
			*fatsize = 16;
			return 0;
		}
	}

	FAT_DPRINT("Error: broken fs_type sign\n");
	return -1;
}

#ifdef SD_DATA_COPY_SLICE
int  callback_write_slice(struct ts_data_slice_copy* ps_dsc,unsigned char *buffer_addr,int size)
{
int ret;
struct fastboot_ptentry* target_ptn = ps_dsc->target_ptn;

//		if (mydata->target_ptn->flags & FASTBOOT_PTENTRY_FLAGS_USE_MMC_CMD)
{
	char device[32], part[32];
	char start[32], length[32], buffer[32];
	char *argv[6]  = { NULL, "write", NULL, NULL, NULL, NULL, };
	int argc = 0;
	int offset_blk,size_blk;

	argv[2] = device;
	argv[3] = buffer;
	argv[4] = start;
	argv[5] = length;
	offset_blk =   ((target_ptn->start + ps_dsc->data_readed) / CFG_FASTBOOT_SDMMC_BLOCKSIZE);
	size_blk = (size / CFG_FASTBOOT_SDMMC_BLOCKSIZE)	;
	sprintf(device, "mmc %d", 1);
	sprintf(buffer, "0x%x", buffer_addr);
	sprintf(start, "0x%x",offset_blk);
	sprintf(length, "0x%x", size_blk);

//				MX100_DPRINT ("callback offset: %d, size: %d\n",offset,size);

//				MX100_DPRINT("%s %s\n",device,mydata->target_ptn->name);

	MX100_DPRINT ("callback %s offset:(%d)%s size: %d\n",target_ptn->name,offset_blk,start,size);

	ret = do_mmcops(NULL, 0, 6, argv);

	mx100_set_progress(ps_dsc->data_readed / (ps_dsc->data_size/100));
	
	}
}
#endif

__u8 do_fat_read_block[MAX_CLUSTSIZE];  /* Block buffer */
long
do_fat_read (const char *filename, void *buffer, unsigned long maxsize,
	     int dols)
{
#if CONFIG_NIOS /* NIOS CPU cannot access big automatic arrays */
    static
#endif
    char fnamecopy[2048];
    boot_sector bs;
    volume_info volinfo;
    fsdata datablock;
    fsdata *mydata = &datablock;
    dir_entry *dentptr;
    __u16 prevcksum = 0xffff;
    char *subname = "";
    int rootdir_size, cursect;
    int idx, isdir = 0;
    int files = 0, dirs = 0;
    long ret = 0;
    int firsttime;

    if (read_bootsectandvi (&bs, &volinfo, &mydata->fatsize)) {
	FAT_DPRINT ("Error: reading boot sector\n");
	return -1;
    }
    if (mydata->fatsize == 32) {
	mydata->fatlength = bs.fat32_length;
    } else {
	mydata->fatlength = bs.fat_length;
    }
    mydata->fat_sect = bs.reserved;
    cursect = mydata->rootdir_sect
	    = mydata->fat_sect + mydata->fatlength * bs.fats;
    mydata->clust_size = bs.cluster_size;
    if (mydata->fatsize == 32) {
	rootdir_size = mydata->clust_size;
	mydata->data_begin = mydata->rootdir_sect   /* + rootdir_size */
		- (mydata->clust_size * 2);
    } else {
	rootdir_size = ((bs.dir_entries[1] * (int) 256 + bs.dir_entries[0])
			* sizeof (dir_entry)) / SECTOR_SIZE;
	mydata->data_begin = mydata->rootdir_sect + rootdir_size
		- (mydata->clust_size * 2);
    }
    mydata->fatbufnum = -1;

    FAT_DPRINT ("FAT%d, fatlength: %d\n", mydata->fatsize,
		mydata->fatlength);
    FAT_DPRINT ("Rootdir begins at sector: %d, offset: %x, size: %d\n"
		"Data begins at: %d\n",
		mydata->rootdir_sect, mydata->rootdir_sect * SECTOR_SIZE,
		rootdir_size, mydata->data_begin);
    FAT_DPRINT ("Cluster size: %d\n", mydata->clust_size);

    /* "cwd" is always the root... */
    while (ISDIRDELIM (*filename))
	filename++;
    /* Make a copy of the filename and convert it to lowercase */
    strcpy (fnamecopy, filename);
    downcase (fnamecopy);
    if (*fnamecopy == '\0') {
	if (!dols)
	    return -1;
	dols = LS_ROOT;
    } else if ((idx = dirdelim (fnamecopy)) >= 0) {
	isdir = 1;
	fnamecopy[idx] = '\0';
	subname = fnamecopy + idx + 1;
	/* Handle multiple delimiters */
	while (ISDIRDELIM (*subname))
	    subname++;
    } else if (dols) {
	isdir = 1;
    }

    while (1) {
	int i;

	if (disk_read (cursect, mydata->clust_size, do_fat_read_block) < 0) {
	    FAT_DPRINT ("Error: reading rootdir block\n");
	    return -1;
	}
	dentptr = (dir_entry *) do_fat_read_block;
	for (i = 0; i < DIRENTSPERBLOCK; i++) {
	    char s_name[14], l_name[256];

	    l_name[0] = '\0';
	    if ((dentptr->attr & ATTR_VOLUME)) {
#ifdef CONFIG_SUPPORT_VFAT
		if ((dentptr->attr & ATTR_VFAT) &&
		    (dentptr->name[0] & LAST_LONG_ENTRY_MASK)) {
		    prevcksum = ((dir_slot *) dentptr)->alias_checksum;
		    get_vfatname (mydata, 0, do_fat_read_block, dentptr, l_name);
		    if (dols == LS_ROOT) {
			int isdir = (dentptr->attr & ATTR_DIR);
			char dirc;
			int doit = 0;

			if (isdir) {
			    dirs++;
			    dirc = '/';
			    doit = 1;
			} else {
			    dirc = ' ';
			    if (l_name[0] != 0) {
				files++;
				doit = 1;
			    }
			}
			if (doit) {
			    if (dirc == ' ') {
				FAT_DPRINT (" %8ld   %s%c\n",
					(long) FAT2CPU32 (dentptr->size),
					l_name, dirc);
			    } else {
				FAT_DPRINT ("            %s%c\n", l_name, dirc);
			    }
			}
			dentptr++;
			continue;
		    }
		    FAT_DPRINT ("Rootvfatname: |%s|\n", l_name);
		} else
#endif
		{
		    /* Volume label or VFAT entry */
		    dentptr++;
		    continue;
		}
	    } else if (dentptr->name[0] == 0) {
		FAT_DPRINT ("RootDentname == NULL - %d\n", i);
		if (dols == LS_ROOT) {
		    FAT_DPRINT ("\n%d file(s), %d dir(s)\n\n", files, dirs);
		    return 0;
		}
		return -1;
	    }
#ifdef CONFIG_SUPPORT_VFAT
	    else if (dols == LS_ROOT
		     && mkcksum (dentptr->name) == prevcksum) {
		dentptr++;
		continue;
	    }
#endif
	    get_name (dentptr, s_name);
	    if (dols == LS_ROOT) {
		int isdir = (dentptr->attr & ATTR_DIR);
		char dirc;
		int doit = 0;

		if (isdir) {
		    dirc = '/';
		    if (s_name[0] != 0) {
			dirs++;
			doit = 1;
		    }
		} else {
		    dirc = ' ';
		    if (s_name[0] != 0) {
			files++;
			doit = 1;
		    }
		}
		if (doit) {
		    if (dirc == ' ') {
			FAT_DPRINT (" %8ld   %s%c\n",
				(long) FAT2CPU32 (dentptr->size), s_name,
				dirc);
		    } else {
			FAT_DPRINT ("            %s%c\n", s_name, dirc);
		    }
		}
		dentptr++;
		continue;
	    }
	    if (strcmp (fnamecopy, s_name) && strcmp (fnamecopy, l_name)) {
		FAT_DPRINT ("RootMismatch: |%s|%s|\n", s_name, l_name);
		dentptr++;
		continue;
	    }
	    if (isdir && !(dentptr->attr & ATTR_DIR))
		return -1;

	    FAT_DPRINT ("RootName: %s", s_name);
	    FAT_DPRINT (", start: 0x%x", START (dentptr));
	    FAT_DPRINT (", size:  0x%x %s\n",
			FAT2CPU32 (dentptr->size), isdir ? "(DIR)" : "");

	    goto rootdir_done;  /* We got a match */
	}
	cursect++;
    }
  rootdir_done:

    firsttime = 1;
    while (isdir) {
	int startsect = mydata->data_begin
		+ START (dentptr) * mydata->clust_size;
	dir_entry dent;
	char *nextname = NULL;

	dent = *dentptr;
	dentptr = &dent;

	idx = dirdelim (subname);
	if (idx >= 0) {
	    subname[idx] = '\0';
	    nextname = subname + idx + 1;
	    /* Handle multiple delimiters */
	    while (ISDIRDELIM (*nextname))
		nextname++;
	    if (dols && *nextname == '\0')
		firsttime = 0;
	} else {
	    if (dols && firsttime) {
		firsttime = 0;
	    } else {
		isdir = 0;
	    }
	}

	if (get_dentfromdir (mydata, startsect, subname, dentptr,
			     isdir ? 0 : dols) == NULL) {
	    if (dols && !isdir)
		return 0;
	    return -1;
	}

	if (idx >= 0) {
	    if (!(dentptr->attr & ATTR_DIR))
		return -1;
	    subname = nextname;
	}
    }
	#ifdef SD_DATA_COPY_SLICE
   //	 FAT_DPRINT ("Size: %d\n", FAT2CPU32 (dentptr->size));

	if(dentptr->size > DATA_COPY_SLICE_SIZE) {
		mydata->data_slice_copy.slice_read_mode = 1;
		mydata->data_slice_copy.data_readed = 0;	
		mydata->data_slice_copy.data_size = dentptr->size;
		mydata->callback_write_slice = callback_write_slice;
		if(strstr(filename,"system")) {
			printf("system.img\n");
			mydata->data_slice_copy.target_ptn = fastboot_flash_find_ptn("system");
		} else if(strstr(filename,"bundle")) {
			printf("bundle.img\n");
			mydata->data_slice_copy.target_ptn = fastboot_flash_find_ptn("fat");
		}
	}
	#endif

    ret = get_contents (mydata, dentptr, buffer, maxsize);
    FAT_DPRINT ("Size: %d, got: %ld\n", FAT2CPU32 (dentptr->size), ret);
    #ifdef SD_DATA_COPY_SLICE
    mydata->data_slice_copy.slice_read_mode = 0;
    #endif

    return ret;
}

int
file_fat_detectfs()
{
	boot_sector	bs;
	volume_info	volinfo;
	int		fatsize;
	char	vol_label[12];

	if(cur_dev==NULL) {
		FAT_DPRINT("No current device\n");
		return 1;
	}
#if defined(CONFIG_CMD_IDE) || \
    defined(CONFIG_CMD_SCSI) || \
    defined(CONFIG_CMD_USB) || \
    defined(CONFIG_MMC)
	FAT_DPRINT("Interface:  ");
	switch(cur_dev->if_type) {
		case IF_TYPE_IDE :	FAT_DPRINT("IDE"); break;
		case IF_TYPE_SCSI :	FAT_DPRINT("SCSI"); break;
		case IF_TYPE_ATAPI :	FAT_DPRINT("ATAPI"); break;
		case IF_TYPE_USB :	FAT_DPRINT("USB"); break;
		case IF_TYPE_DOC :	FAT_DPRINT("DOC"); break;
		case IF_TYPE_MMC :	FAT_DPRINT("SD/MMC"); break;
		default :		FAT_DPRINT("Unknown");
	}
	FAT_DPRINT("\n  Device %d: ",cur_dev->dev);
	dev_print(cur_dev);
#endif
	if(read_bootsectandvi(&bs, &volinfo, &fatsize)) {
		FAT_DPRINT("\nNo valid FAT fs found\n");
		return 1;
	}
	memcpy (vol_label, volinfo.volume_label, 11);
	vol_label[11] = '\0';
	volinfo.fs_type[5]='\0';
	FAT_DPRINT("Partition %d: Filesystem: %s \"%s\"\n"
			,cur_part,volinfo.fs_type,vol_label);
	return 0;
}


int
file_fat_ls(const char *dir)
{
	return do_fat_read(dir, NULL, 0, LS_YES);
}


long
file_fat_read(const char *filename, void *buffer, unsigned long maxsize)
{
	FAT_DPRINT("reading %s\n",filename);
	return do_fat_read(filename, buffer, maxsize, LS_NO);
}

#endif