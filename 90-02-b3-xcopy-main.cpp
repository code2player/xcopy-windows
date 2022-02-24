/*肖鹏飞 信13 1953072*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>//exit()要用
#include <sstream>
#include <fstream>
#include <direct.h>//dirent.h不包含
#include <windows.h>
#include  <io.h>
#include <sys/stat.h>
using namespace std;

/*最长路径：可以用MAX_PATH表示*/

void FileName(char* m_pFileDirectory, char* aim, int* file_num)//传入参数为要遍历的路径,注意修改其中不合法
{
	char pLogFileDirectory[MAX_PATH] = { 0 };
	strcpy_s(pLogFileDirectory, MAX_PATH, m_pFileDirectory);
	int iLen = strlen(pLogFileDirectory);
	pLogFileDirectory[iLen] = '*';
	pLogFileDirectory[iLen + 1] = 0;

	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA(pLogFileDirectory, &findFileData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		int iErrno = GetLastError();
		return;
	}

	BOOL bRet = FALSE;
	for (;;)
	{
		//cout << m_pFileDirectory << endl;
		if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))//文件
		{
			char file1[400] = { 0 };
			strcpy(file1, m_pFileDirectory);
			strcat(file1, findFileData.cFileName);
			//strcat(file1, "/");

			ifstream fin;
			fin.open(file1, ios::in | ios::binary);
			if (!fin.is_open())
			{
				cerr << "文件打开失败" << endl;
				exit(-1);
			}

			char file2[400] = { 0 };
			strcpy(file2, aim);
			strcat(file2, findFileData.cFileName);

			ofstream fout;
			fout.open(file2, ios::out | ios::binary);
			if (!fout.is_open())
			{
				fin.close();
				cerr << "文件打开失败" << endl;
				exit(-1);
			}

			char* ch = new(nothrow) char[1024 * 64 + 2];//当前测试缓冲区大小：1K
			if (ch == NULL)
			{
				cout << "内存未申请到" << endl;
				exit(-1);
			}
			while (1)
			{

				fin.read(ch, 1024 * 64);
				fin.clear();
				fout.write(ch, fin.gcount());//若复制不足1024，则写入实际读入的数据量
				fout.clear();
				if (fin.peek() == EOF)
					break;

			}
			fin.close();
			fout.close();
			delete[]ch;
			(*file_num)++;
			cout << file1 << '\n';
		}
		bRet = FindNextFileA(hFind, &findFileData);
		if (!bRet)
			break;
	}
	FindClose(hFind);
}





void PrintFileName(int digui,char* m_pFileDirectory,char* aim, int *file_num)//传入参数为要遍历的路径,注意修改其中不合法
{
	char pLogFileDirectory[MAX_PATH] = { 0 };
	strcpy_s(pLogFileDirectory, MAX_PATH, m_pFileDirectory);
	int iLen = strlen(pLogFileDirectory);
	pLogFileDirectory[iLen] = '*';
	pLogFileDirectory[iLen + 1] = 0;

	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA(pLogFileDirectory, &findFileData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		int iErrno = GetLastError();
		return;
	}

	BOOL bRet = FALSE;
	for (;;)
	{
		//cout << m_pFileDirectory << endl;
		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//目录
			if (!strcmp(findFileData.cFileName, ".") || !strcmp(findFileData.cFileName, ".."))
			{
				//.或..
				static int x = 0;//第一个文件夹中的所有文件处理

				if (x == 0)
				{
					x = 1;
					FileName(m_pFileDirectory, aim, file_num);
				}
			}
			else
			{
				//系统目录

				char file1[400] = { 0 };
				strcpy(file1, m_pFileDirectory);
				strcat(file1, findFileData.cFileName);
				strcat(file1, "\\");

				char file2[400] = { 0 };
				strcpy(file2, aim);
				strcat(file2, findFileData.cFileName);
				strcat(file2, "\\");

				

				/*进入之前先创建对应目录*/
				_mkdir(file2);//创建新文件夹,只读一次，不用判断是否存在，因为肯定不存在？？？？

				FileName(file1, file2, file_num);

				PrintFileName(digui + 1, file1, file2, file_num);

			}
		}
		
		bRet = FindNextFileA(hFind, &findFileData);
		if (!bRet)
			break;
	}

	FindClose(hFind);
}



int main(int argc, char* argv[])
{
	/*不存在乱序参数（ohhhhhhh）*/
	/*第一个：源文件夹名（需要复制）,必须存在
	第二个：目标文件夹名（需要粘贴），必须不存在*/
	//函数_mkdir，创建新文件夹
	/*路径若为文件夹，后面必须加\\或者/ */

	if (argc != 3)
	{
		cout << "无效的参数数量" << endl;
		cout << "复制了 0 个文件" << endl;
		return -1;
	}

	if (_access(argv[1], 0) == 0)//第一个存在
	{
		struct stat s;
		if (stat(argv[1], &s) == 0)
		{
			if (s.st_mode & S_IFDIR)//文件夹
			{

			}
			else//文件或其他
			{
				cout << argv[1] << " - 不是文件夹" << endl;
				return -1;
			}
		}
	}
	else
	{
		cout << argv[1] << " - 不存在" << endl;
		return -1;
	}

	if (_access(argv[2], 0) == 0)//第二个存在
	{
		cout << argv[2] << " - 已存在" << endl;
		return -1;
	}
	else//不存在，创建文件夹
	{
		int len = strlen(argv[2]);
		char str[MAX_PATH] = { 0 };
		strcpy(str, argv[2]);
		if (argv[2][len - 1] != '\\' && argv[2][len - 1] != '/')
		{
			str[len] = '\\';
			str[len+1] = 0;
		}

		_mkdir(str);//创建新文件夹
	}

	/*对单个文件读写用二进制（read，write）
	设置缓冲区大小来提高读写速度？？？*/

	int len1 = strlen(argv[1]);
	char str1[MAX_PATH] = { 0 };
	strcpy(str1, argv[1]);
	if (argv[1][len1 - 1] != '\\' && argv[1][len1 - 1] != '/')
	{
		str1[len1] = '\\';
		str1[len1 + 1] = 0;
	}
	int len = strlen(argv[2]);
	char str[MAX_PATH] = { 0 };
	strcpy(str, argv[2]);
	if (argv[2][len - 1] != '\\' && argv[2][len - 1] != '/')
	{
		str[len] = '\\';
		str[len + 1] = 0;
	}

	int file_number = 0;
	PrintFileName(1,str1, str,&file_number);

	cout << "复制了 " << file_number << " 个文件" << endl;

}