//---------------------------------------------------------------------------
#include<dir.h>
#include <stdio.h>
#include<windows.h>
#pragma hdrstop
int seach_dir(char *path);
FILE *out;
//---------------------------------------------------------------------------
#pragma argsused
int main(int argc, char* argv[])     //������ � ������� ���� ������, �������� ����� ��������� ������ � ��������(� �������� �� �������� ������ �����)
{
        if(argc==1)
                {printf("Give me a folder");
                return -2;}
        out = fopen("C:\\output.txt","w");
        if(out == NULL)
                {printf("Error. Output.txt not opened");
                return -3; //������ ��� �������� �����
                }
        printf("Search in %s:\n",argv[1]);    //����� �������� �����
        fprintf(out,"Search in %s:\n",argv[1]);   //� ����
        int ret = seach_dir(argv[1]);
        if(ret==-1)  //�� ������� ������
                return -1;      //���� �������� ������, ��������� ��������� � �����  -1
        printf("Found %d hidden files\n",ret);    //����� ����� ��������� ������
        fprintf(out,"Found %d hidden files\n",ret);
        fclose(out);    //��������� �������� �����
        return 0;
}
//---------------------------------------------------------------------------
int seach_dir(char *path)
{
        Sleep(0);               //�������� ���������� ������������ �������, ���� ��������� �� �������� �����
	int done;               //����������� ��� ����� ������ ������ � �������� �����
	struct ffblk file;      //��������� �������� ��������� ������
	int count;            //� ������ ���������� �������� ���������� ��������� ������
        #define dir_deep 256    //������� �������� ��������
        char dir[dir_deep][MAXPATH]; //���������� �������������� �����
        unsigned int dir_cnt; //����� �����

        if((unsigned)path[0]>(unsigned)'Z')
                path[0]-=32;     //��������� ��� ����� �� �������� � ���������, ���� ��� ���� ��������
	setdisk(path[0]-'A');          //��������� �������� �����
	if (chdir(path)){                        //��������� ������� �����
                printf("Error. Folder not found");    //����� ��������� �� ������
                fprintf(out,"Error. Folder not found");
                return -1;
                }                      //���� �� ������� ���������� ������

	//seach file
	done=findfirst("*.*",&file,FA_HIDDEN);  //������ ������
                                                //���������� ����������� � ���������� file
	count=0;                             //�������� �������
	while(!done)
	        {
                if((file.ff_attrib & FA_DIREC)!=FA_DIREC && (file.ff_attrib & FA_HIDDEN)==FA_HIDDEN) //���� ������ ������� �����
                        {char temp[512];                //��������� ���������� ��� �������� ���� ������
                        strcpy(temp,path);                     //��������������� ���������� ����
		        if(temp[strlen(temp)-1]!='\\')
                	        strcat(temp,"\\");
		        strcat(temp,file.ff_name);
                        printf("%s\n", temp); //����� �� ������
                        fprintf(out,"%s\n", temp);
                        count++;                        //����������� �������
                        }
                done=findnext(&file);           //����� ���������� ��������
	        }

        //seach directory
        dir_cnt=0;
	done=findfirst("*",&file,FA_DIREC | FA_SYSTEM | FA_HIDDEN );
	while(!done && dir_cnt<dir_deep)
	{
                if (file.ff_name[0]=='.')   //��������� ����� "�� ������� �����"
		{
			done=findnext(&file);
			continue;
                }
                if((file.ff_attrib & FA_DIREC)==FA_DIREC) //���� ������ �����
		        {//make dir adr
		        strcpy(dir[dir_cnt],path);                     //��������������� ���������� ����
		        if(dir[dir_cnt][strlen(dir[dir_cnt])-1]!='\\')
                	        strcat(dir[dir_cnt],"\\");
		        strcat(dir[dir_cnt],file.ff_name);
                        dir_cnt++;
                        }
               done=findnext(&file);           //����� ���������� ��������
	}
        //����������� ����� �� ���������
        for(unsigned int i=0; i<dir_cnt; i++)
                count+=seach_dir(dir[i]);

	return count; //���������� ����� ��������� ������� ������
}//seach_dir
//*******************************************************************************************************