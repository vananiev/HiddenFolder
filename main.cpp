//---------------------------------------------------------------------------
#include<dir.h>
#include <stdio.h>
#include<windows.h>
#pragma hdrstop
int seach_dir(char *path);
FILE *out;
//---------------------------------------------------------------------------
#pragma argsused
int main(int argc, char* argv[])     //строка в которой надо искать, задается через командрую строку в кавычках(в кавычках не задается корень диска)
{
        if(argc==1)
                {printf("Give me a folder");
                return -2;}
        out = fopen("C:\\output.txt","w");
        if(out == NULL)
                {printf("Error. Output.txt not opened");
                return -3; //ошибка при открытии файла
                }
        printf("Search in %s:\n",argv[1]);    //вывод заданной папки
        fprintf(out,"Search in %s:\n",argv[1]);   //в файл
        int ret = seach_dir(argv[1]);
        if(ret==-1)  //на функцию поиска
                return -1;      //Если возникла ошибка, завершаем программу с кодом  -1
        printf("Found %d hidden files\n",ret);    //вывод числа найденных файлов
        fprintf(out,"Found %d hidden files\n",ret);
        fclose(out);    //закрываем файловый поток
        return 0;
}
//---------------------------------------------------------------------------
int seach_dir(char *path)
{
        Sleep(0);               //передаем управление операционной системе, чтоб программа не томозила винду
	int done;               //испльзуется для цикла поиска файлов в заданной папке
	struct ffblk file;      //структура хранящая найденная объект
	int count;            //в данной переменной хранится количество найденных файлов
        #define dir_deep 256    //глубина вложения подпапок
        char dir[dir_deep][MAXPATH]; //рекурсивно обрабатываемые папки
        unsigned int dir_cnt; //число папок

        if((unsigned)path[0]>(unsigned)'Z')
                path[0]-=32;     //переводим имя диска из строчной в прописную, если она была строчной
	setdisk(path[0]-'A');          //установка текущего диска
	if (chdir(path)){                        //установка текущей папки
                printf("Error. Folder not found");    //вывод сообщения об ошибке
                fprintf(out,"Error. Folder not found");
                return -1;
                }                      //если не удалось возвращаем ошибку

	//seach file
	done=findfirst("*.*",&file,FA_HIDDEN);  //начало поиска
                                                //результаты схораняются в переменную file
	count=0;                             //обнуляем счетчик
	while(!done)
	        {
                if((file.ff_attrib & FA_DIREC)!=FA_DIREC && (file.ff_attrib & FA_HIDDEN)==FA_HIDDEN) //ищем только скрытые файлы
                        {char temp[512];                //временнаф переменная для хранения имен файлов
                        strcpy(temp,path);                     //восстанавливаем абсолютный путь
		        if(temp[strlen(temp)-1]!='\\')
                	        strcat(temp,"\\");
		        strcat(temp,file.ff_name);
                        printf("%s\n", temp); //вывод на печать
                        fprintf(out,"%s\n", temp);
                        count++;                        //увеличиваем счетчик
                        }
                done=findnext(&file);           //поиск следующего элемента
	        }

        //seach directory
        dir_cnt=0;
	done=findfirst("*",&file,FA_DIREC | FA_SYSTEM | FA_HIDDEN );
	while(!done && dir_cnt<dir_deep)
	{
                if (file.ff_name[0]=='.')   //отсеиваем папку "на уровень вверх"
		{
			done=findnext(&file);
			continue;
                }
                if((file.ff_attrib & FA_DIREC)==FA_DIREC) //ищем только папки
		        {//make dir adr
		        strcpy(dir[dir_cnt],path);                     //восстанавливаем абсолютный путь
		        if(dir[dir_cnt][strlen(dir[dir_cnt])-1]!='\\')
                	        strcat(dir[dir_cnt],"\\");
		        strcat(dir[dir_cnt],file.ff_name);
                        dir_cnt++;
                        }
               done=findnext(&file);           //поиск следующего элемента
	}
        //Рекурсивный поиск по подпапкам
        for(unsigned int i=0; i<dir_cnt; i++)
                count+=seach_dir(dir[i]);

	return count; //возвращаем число найденных скрытых файлов
}//seach_dir
//*******************************************************************************************************