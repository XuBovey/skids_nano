/**
 * @file lv_port_fs_templ.c
 *
 */

 /*Copy this file as "lv_port_fs.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_fs_template.h"

#include "stdio.h"
#include "ff.h"
#include "usr_sd_card.h"
#include "esp_log.h"
#include "dirent.h"

/*********************
 *      DEFINES
 *********************/

#define TAG "lv_port_fs"

void test(void);

/**********************
 *      TYPEDEFS
 **********************/

/* Create a type to store the required data about your file.
 * If you are using a File System library
 * it already should have a File type.
 * For example FatFS has `FIL`. In this case use `typedef FIL file_t`*/
typedef struct {
    /*Add the data you need to store about a file*/
    uint32_t dummy1;
    uint32_t dummy2;
}file_t;

/*Similarly to `file_t` create a type for directory reading too */
typedef struct {
    /*Add the data you need to store about directory reading*/
    uint32_t dummy1;
    uint32_t dummy2;
}dir_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void fs_init(void);

static lv_fs_res_t fs_open (lv_fs_drv_t * drv, void * file_p, const char * path, lv_fs_mode_t mode);
static lv_fs_res_t fs_close (lv_fs_drv_t * drv, void * file_p);
static lv_fs_res_t fs_read (lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br);
static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw);
static lv_fs_res_t fs_seek (lv_fs_drv_t * drv, void * file_p, uint32_t pos);
static lv_fs_res_t fs_size (lv_fs_drv_t * drv, void * file_p, uint32_t * size_p);
static lv_fs_res_t fs_tell (lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p);
static lv_fs_res_t fs_remove (lv_fs_drv_t * drv, const char *path);
static lv_fs_res_t fs_trunc (lv_fs_drv_t * drv, void * file_p);
static lv_fs_res_t fs_rename (lv_fs_drv_t * drv, const char * oldname, const char * newname);
static lv_fs_res_t fs_free (lv_fs_drv_t * drv, uint32_t * total_p, uint32_t * free_p);
static lv_fs_res_t fs_dir_open (lv_fs_drv_t * drv, void * rddir_p, const char *path);
static lv_fs_res_t fs_dir_read (lv_fs_drv_t * drv, void * rddir_p, char *fn);
static lv_fs_res_t fs_dir_close (lv_fs_drv_t * drv, void * rddir_p);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_fs_init(void)
{
    /*----------------------------------------------------
     * Initialize your storage device and File System
     * -------------------------------------------------*/
    fs_init();

    /*---------------------------------------------------
     * Register the file system interface in LVGL
     *--------------------------------------------------*/

    /* Add a simple drive to open images */
    lv_fs_drv_t fs_drv;
    lv_fs_drv_init(&fs_drv);

    /*Set up fields...*/
    fs_drv.file_size = sizeof(file_t);
    fs_drv.letter = 'S';
    fs_drv.open_cb = fs_open;
    fs_drv.close_cb = fs_close;
    fs_drv.read_cb = fs_read;
    fs_drv.write_cb = fs_write;
    fs_drv.seek_cb = fs_seek;
    fs_drv.tell_cb = fs_tell;
    fs_drv.free_space_cb = fs_free;
    fs_drv.size_cb = fs_size;
    fs_drv.remove_cb = fs_remove;
    fs_drv.rename_cb = fs_rename;
    fs_drv.trunc_cb = fs_trunc;

    fs_drv.rddir_size = sizeof(dir_t);
    fs_drv.dir_close_cb = fs_dir_close;
    fs_drv.dir_open_cb = fs_dir_open;
    fs_drv.dir_read_cb = fs_dir_read;

    lv_fs_drv_register(&fs_drv);

    // test();
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/* Initialize your Storage device and File system. */
static void fs_init(void)
{
    /*E.g. for FatFS initialize the SD card and FatFS itself*/

    /*You code here*/
    sd_card_init();
}

/**
 * Open a file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param path path to the file beginning with the driver letter (e.g. S:/folder/file.txt)
 * @param mode read: FS_MODE_RD, write: FS_MODE_WR, both: FS_MODE_RD | FS_MODE_WR
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_open (lv_fs_drv_t * drv, void * file_p, const char * path, lv_fs_mode_t mode)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    char * f_path = lv_mem_alloc(128);
    FILE *f = (FILE *) file_p ;

    sprintf(f_path, "/sdcard/%s", path);

    if(mode == LV_FS_MODE_WR)
    {
        /*Open a file for write*/

        /* Add your code here*/
        f = fopen(f_path, "wb");
    }
    else if(mode == LV_FS_MODE_RD)
    {
        /*Open a file for read*/

        /* Add your code here*/
        f = fopen(f_path, "rb");
    }
    else if(mode == (LV_FS_MODE_WR | LV_FS_MODE_RD))
    {
        /*Open a file for read and write*/

        /* Add your code here*/
        f = fopen(f_path, "rb+");
    }

	if (NULL == f) {
		res = LV_FS_RES_NOT_EX;
	}
	else {
        fseek(f, 0, SEEK_SET);
        res = LV_FS_RES_OK;
    }

    lv_mem_free(f_path);
    ESP_LOGI(TAG, "fs_open %s res=%d", f_path, res);

    return res;
}

/**
 * Close an opened file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable. (opened with lv_ufs_open)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_close (lv_fs_drv_t * drv, void * file_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/
   if (NULL == file_p) {
		ESP_LOGE(TAG, "NULL file pointer");
		res = LV_FS_RES_NOT_EX;
	}

	if (EOF == fclose((FILE *) file_p)) {
		ESP_LOGE(TAG, "Failed close file pointer");
		res = LV_FS_RES_NOT_EX;
	}else {
	    res = LV_FS_RES_OK; 
    }

    return res;
}

/**
 * Read data from an opened file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable.
 * @param buf pointer to a memory block where to store the read data
 * @param btr number of Bytes To Read
 * @param br the real number of read bytes (Byte Read)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_read (lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/
	*br = fread(buf, 1, btr, (FILE *)file_p);
	res = LV_FS_RES_OK;

    return res;
}

/**
 * Write into a file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param buf pointer to a buffer with the bytes to write
 * @param btr Bytes To Write
 * @param br the number of real written bytes (Bytes Written). NULL if unused.
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/
    *bw = fwrite(buf, 1, btw, (FILE *)file_p);
    res = LV_FS_RES_OK;

    return res;
}

/**
 * Set the read write pointer. Also expand the file size if necessary.
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable. (opened with lv_ufs_open )
 * @param pos the new position of read write pointer
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_seek (lv_fs_drv_t * drv, void * file_p, uint32_t pos)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/
    fseek((FILE *)file_p, pos, SEEK_SET);
    res = LV_FS_RES_OK;

    return res;
}

/**
 * Give the size of a file bytes
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param size pointer to a variable to store the size
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_size (lv_fs_drv_t * drv, void * file_p, uint32_t * size_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/
    uint32_t cur = ftell((FILE *)file_p);
	fseek((FILE *)file_p, 0L, SEEK_END);
	*size_p = ftell((FILE *)file_p);
	fseek((FILE *)file_p, cur, SEEK_SET);
    res = LV_FS_RES_OK;

    return res;
}
/**
 * Give the position of the read write pointer
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable.
 * @param pos_p pointer to to store the result
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_tell (lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/
    *pos_p = ftell((FILE *)file_p);
    res = LV_FS_RES_OK;

    return res;
}

/**
 * Delete a file
 * @param drv pointer to a driver where this function belongs
 * @param path path of the file to delete
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_remove (lv_fs_drv_t * drv, const char *path)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/
    char * pathbuf = lv_mem_alloc(128);
	sprintf(pathbuf,"/sdcard/%s",path);
	if(remove(pathbuf)==0){
		res = LV_FS_RES_OK;
	}else{
		res = LV_FS_RES_UNKNOWN;
	}
    lv_mem_free(pathbuf);

    return res;
}

/**
 * Truncate the file size to the current position of the read write pointer
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to an 'ufs_file_t' variable. (opened with lv_fs_open )
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_trunc (lv_fs_drv_t * drv, void * file_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * Rename a file
 * @param drv pointer to a driver where this function belongs
 * @param oldname path to the file
 * @param newname path with the new name
 * @return LV_FS_RES_OK or any error from 'fs_res_t'
 */
static lv_fs_res_t fs_rename (lv_fs_drv_t * drv, const char * oldname, const char * newname)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/
    char * oldnamebuf = lv_mem_alloc(128);
	char * newnamebuf = lv_mem_alloc(128);
	sprintf(oldnamebuf,"/sdcard/%s",oldname);
	sprintf(newnamebuf,"/sdcard/%s",newname);	

	if(rename(oldnamebuf,newnamebuf)==0){
		lv_mem_free(oldnamebuf);
		lv_mem_free(newnamebuf);
		res = LV_FS_RES_OK;
	}else{
		res = LV_FS_RES_UNKNOWN;
	}
    lv_mem_free(oldnamebuf);
    lv_mem_free(newnamebuf);

    return res;
}

/**
 * Get the free and total size of a driver in kB
 * @param drv pointer to a driver where this function belongs
 * @param letter the driver letter
 * @param total_p pointer to store the total size [kB]
 * @param free_p pointer to store the free size [kB]
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_free (lv_fs_drv_t * drv, uint32_t * total_p, uint32_t * free_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/
    FATFS *fs;
    size_t free_clusters;
    // int res = f_getfree("0:", &free_clusters, &fs);
    assert(f_getfree("0:", &free_clusters, &fs) == FR_OK);
    size_t total_sectors = (fs->n_fatent - 2) * fs->csize;
    size_t free_sectors = free_clusters * fs->csize;

    // assuming the total size is < 4GiB, should be true for SPI Flash
    if (total_p != NULL) {
        *total_p = total_sectors * fs->ssize;
    }
    if (free_p != NULL) {
        *free_p = free_sectors * fs->ssize;
    }
    return res;
}

/**
 * Initialize a 'lv_fs_dir_t' variable for directory reading
 * @param drv pointer to a driver where this function belongs
 * @param rddir_p pointer to a 'lv_fs_dir_t' variable
 * @param path path to a directory
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_open (lv_fs_drv_t * drv, void * rddir_p, const char *path)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * Read the next filename form a directory.
 * The name of the directories will begin with '/'
 * @param drv pointer to a driver where this function belongs
 * @param rddir_p pointer to an initialized 'lv_fs_dir_t' variable
 * @param fn pointer to a buffer to store the filename
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_read (lv_fs_drv_t * drv, void * rddir_p, char *fn)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/
	do {
		struct dirent *entry = readdir((DIR *)rddir_p);
		if(entry) {
			if(entry->d_type == DT_DIR) sprintf(fn, "/%s", entry->d_name);
			else strcpy(fn, entry->d_name);
		} else {
			strcpy(fn, "");
		}
	} while(strcmp(fn, "/.") == 0 || strcmp(fn, "/..") == 0);

    return res;
}

/**
 * Close the directory reading
 * @param drv pointer to a driver where this function belongs
 * @param rddir_p pointer to an initialized 'lv_fs_dir_t' variable
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_close (lv_fs_drv_t * drv, void * rddir_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/
    closedir(rddir_p);
    res = LV_FS_RES_OK;

    return res;
}


void test(void)
{
    lv_fs_file_t f;
	lv_fs_dir_t d;
	lv_fs_res_t res;
	uint32_t resCount;
	uint8_t buf[64];

	// 用只读方式打开一个文件，来判断文件是否存在
	res = lv_fs_open(&f,"S:/hello.txt", LV_FS_MODE_RD);
	if (res == LV_FS_RES_OK) {	// 文件存在
		lv_fs_close(&f);		// 关闭文件
		ESP_LOGI(TAG,"'hello.txt' file is existsres : %d",res);
	}else if(res == LV_FS_RES_NOT_EX){// 文件不存在
		ESP_LOGI(TAG,"'hello.txt' file is not exists res : %d",res);
	}
 
	// /创建一个文件并写入内容
	res = lv_fs_open(&f,"S:/hello.txt", LV_FS_MODE_WR);// 创建一个文件
	if (res == LV_FS_RES_OK) {
		ESP_LOGI(TAG,"Create file 'hello.txt' Success res : %d",res);
		res = lv_fs_write(&f,"hellobug esp32", sizeof("hellobug esp32"), &resCount);// 创建一个文件
		if (res == LV_FS_RES_OK) {
			ESP_LOGI(TAG,"Write file 'hello.txt' Success res : %d WriteByte: %d",res,resCount);
		}		
	}
	lv_fs_close(&f);
 
	// /读取刚才创建的文件并打印内容
	res = lv_fs_open(&f,"S:/hello.txt", LV_FS_MODE_RD);// 只读方式打开一个文件
	if(res == LV_FS_RES_OK) {
		ESP_LOGI(TAG,"lv_fs_open->Success %d",res);
		memset(buf,0,64);
		res = lv_fs_read(&f, buf, 64, &resCount);
		if(res != LV_FS_RES_OK) {
			ESP_LOGI(TAG,"lv_fs_read->Failed %d",res);
		}else{
			ESP_LOGI(TAG,"lv_fs_read->Success %d",res);
		}
		ESP_LOGI(TAG,"lv_fs_read ReadByte : %d Content : %s",resCount,buf);
		lv_fs_close(&f);
	}else{
		ESP_LOGI(TAG,"lv_fs_open->Failed %d",res);
	}
	// 删除文件
	res = lv_fs_remove("S:/bug.txt");
	ESP_LOGI(TAG,"lv_fs_remove-> bug.txt   res:%d",res);
	// 重命名文件
	res = lv_fs_rename("S:/hello.txt","S:/bug.txt");
	ESP_LOGI(TAG,"lv_fs_rename-> oldname:hello.txt     newname:bug.txt   res:%d",res);
	// /读取刚才改名的文件并打印内容
	res = lv_fs_open(&f,"S:/bug.txt", LV_FS_MODE_RD);// 只读方式打开一个文件
	if(res == LV_FS_RES_OK) {
		// 获取文件大小
		res = lv_fs_size(&f,&resCount);
		ESP_LOGI(TAG,"lv_fs_size-> bug.txt   file size : %d Byte",resCount);
 
		ESP_LOGI(TAG,"lv_fs_open-> S:/bug.txt Success %d",res);
		memset(buf,0,64);
		res = lv_fs_read(&f, buf, 64, &resCount);
		if(res != LV_FS_RES_OK) {
			ESP_LOGI(TAG,"lv_fs_read-> S:/bug.txt Failed %d",res);
		}else{
			ESP_LOGI(TAG,"lv_fs_read-> S:/bug.txt Success %d",res);
		}
		ESP_LOGI(TAG,"lv_fs_read S:/bug.txt ReadByte : %d Content : %s",resCount,buf);
 
		// 定位文件操作指针示例
		res = lv_fs_seek(&f, 7);
		ESP_LOGI(TAG,"lv_fs_seek-> S:/bug.txt res %d",res);
		// 获取文件操作指针示例
		res = lv_fs_tell(&f, &resCount);
		ESP_LOGI(TAG,"lv_fs_tell-> S:/bug.txt res %d   index: %d",res,resCount);
 
		memset(buf,0,64);
		res = lv_fs_read(&f, buf, 64, &resCount);
		ESP_LOGI(TAG,"lv_fs_read seek 7Byte S:/bug.txt ReadByte : %d Content : %s",resCount,buf);

		lv_fs_close(&f);
	}else{
		ESP_LOGI(TAG,"lv_fs_open-> S:/bug.txt Failed %d",res);
	}
 
	res = lv_fs_dir_open(&d,"S:/");
	ESP_LOGI(TAG,"lv_fs_dir_open-> res %d",res);
 
	while(1){
		memset(buf,0,64);
		res = lv_fs_dir_read(&d,(char *)buf);
		if(buf[0] == '\0'){
			break;
		}
		ESP_LOGI(TAG,"lv_fs_dir_read   res : %d Content : %s",res,buf);
	}
	res = lv_fs_dir_close(&d);
	ESP_LOGI(TAG,"lv_fs_dir_close   res : %d ",res);
}


#else /* Enable this file at the top */

/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;
#endif
