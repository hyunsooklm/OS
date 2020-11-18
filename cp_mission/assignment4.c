#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#define Directory 1
#define regular 2

DIR* start_dir;
struct dirent *dir_entry;
struct stat statBuf;
char *dirName[255],buf[255],permission[11],mtime[20];
char file[255];
int dirYesNo(char fname[])
{
	struct stat buf; 
	lstat(fname,&buf);
	return S_ISDIR(buf.st_mode); 
} //dir or not 여부 확인
void file_to_file_cp(char* src,char* dst){
	int n,in,out;
	char buf[1024];
	in=open(src,O_RDONLY);
	out=open(dst,O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
        while((n=read(in,buf,sizeof(buf)))>0)
	write(out,buf,n);	
	close(out);
	close(in);
}//file - file 복사
void file_to_dir_cp(char* src,char* dst)
{
	strcpy(buf,"../");
	strcat(buf,src); //원본 파일 상대경로
	chdir(dst);
	file_to_file_cp(buf,src);
}
void dir_to_dir_cp(char* src,char* dst)
{
	char file_path[1000];
	char new_file_path[1000];
	int count=0;
	start_dir=opendir(src);
	if(start_dir == NULL)
	{
		perror("opendir error!\n");
		exit(1);
	}
	while((dir_entry = readdir(start_dir))!= NULL)
	{
		if((strcmp(dir_entry->d_name,"."))==0 || strcmp(dir_entry->d_name,"..")==0)
			continue;
		else
		{
			sprintf(file_path, "%s/%s", src, dir_entry->d_name); //src디렉토리/"파일" orsrc디렉토리/"폴더" 
			sprintf(new_file_path, "%s/%s",dst,dir_entry->d_name);
			if(dirYesNo(file_path)){
				mkdir(new_file_path,0766);
				dir_to_dir_cp(file_path,new_file_path);
			}//디렉토리인경우
			else{
				file_to_file_cp(file_path,new_file_path);
			} //파일인경우
		}
	}	
} //dir - dir 복사
void main(int argc,char *argv[])
{	
	//mkdir("test_directory",0766);
	int src,dst;
	struct stat st,ft;
	memset(buf,0,sizeof(buf));
	memset(dirName,0,sizeof(dirName));
	memset(&statBuf,0,sizeof(statBuf));
	memset(&start_dir,0,sizeof(start_dir));
	if(argc==4 && strcmp(argv[3],"-r")==0) //dir-dir copy
	{
		if(dirYesNo(argv[1])!=1){
			printf("dir copy only!");
			exit(1);
		} //argv[1] must be directory.
		else if(dirYesNo(argv[1]==1 && dirYesNo(argv[2])!=1)){
			if(stat(argv[2],&st)==-1){ //dir- not exist_dir
				mkdir(argv[2],0766);
			}
			else{//dir-file copy-> impossible
				printf("dir - file copy is impossible!\n");
				exit(1);
			}
		} 
		dir_to_dir_cp(argv[1],argv[2]);
	}

	else if(argc==3) //file copy
	{	
		if(dirYesNo(argv[1])==1)
		{
			printf("directory copy must include '-r' in sentence\n");
			exit(1);
		} //dir - file 복사 불가
		else
		{
			if(dirYesNo(argv[2])==1)
			{
				printf("file-to-dir copy");
				file_to_dir_cp(argv[1],argv[2]);
				return 0;
			}//FILE - FILE COPY
			else
			{
				printf("file-to-file_copy\n");
				file_to_file_cp(argv[1],argv[2]);	
			}//file - directory copy
		}
	}
	else //argc error
	{
		printf("Usage: copy file1 fil32\n");
		exit(1);
	} 
	return 0;
}
