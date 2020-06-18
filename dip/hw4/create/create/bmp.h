
#ifndef _BMP_H
#define _BMP_H

//bmp 图片数据获取
//filePath : 路径
//picMaxSize : 返回图片数据字节数, 不接收置NULL
//width : 返回图片宽(像素), 不接收置NULL
//height : 返回图片高(像素), 不接收置NULL
//per : 返回图片每像素的字节数, 不接收置NULL
//返回 : 图片数据指针, 已分配内存, 用完记得释放
unsigned char *bmp_get(char *filePath, int *picMaxSize, int *width, int *height, int *per);

//生成 bmp 图片
//filePath : 路径
//data : 原始数据
//width : 宽(像素)
//height : 高(像素)
//per : 每像素字节数
//返回 : 成功创建的 .bmp 文件大小, 小于0则失败
int bmp_create(char *filePath, unsigned char *data, int width, int height, int per);

#endif


