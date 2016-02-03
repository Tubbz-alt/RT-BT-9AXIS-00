
//////////////////////////////////////////////////////////////////////////
//  RT-USB-9AXIS-00(RT-USB-9axisIMU) Serial Test                        //
//                                                                  	//
//  By R.Takahashi RT Corporation,                 			            //
//                                                                  	//
//////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <getopt.h>

/* 9軸センサモジュールに対応するデバイスファイル */
static char serial_port[128] = "/dev/ttyACM0" ;



#define BAUDRATE B115200 //ボーレートの設定

extern void print_hex(char *com, int size);

int main(int argc, char *argv[])
{
	int fd; /* シリアル通信ファイルディスクリプタ */
	char *com;
	struct termios newtio, oldtio;    /* シリアル通信設定 */
	int i,j; 
	char *c,buff[256];	
	if(!(fd = open(serial_port, O_RDWR ))) return -1; /* デバイスをオープンする */
	ioctl(fd, TCGETS, &oldtio);       /* 現在のシリアルポートの設定を待避 */

	bzero(&newtio, sizeof(newtio));
	newtio = oldtio;                  /* ポートの設定をコピー */

	cfmakeraw(&newtio);
     	cfsetispeed(&newtio, BAUDRATE);
	cfsetospeed(&newtio, BAUDRATE);
	ioctl(fd, TCSETS, &newtio);       /* ポートの設定を有効にする */
	
	/*受信データを各バイトごとに表示する*/
	while((i=read(fd,buff,sizeof(buff)-1)) >0  ){
		printf("=====len=%d byte====== %c",i,0x0a);
		for(j=0;j<i;j++){
			printf("|%u|%x ||  %c",j,(0xff&buff[j]),0x0a);
		}
			fflush(stdout);
	}


	ioctl(fd, TCSETS, &oldtio);       /* ポートの設定を元に戻す */
	close(fd);                        /* デバイスのクローズ */

    return 0;
}


