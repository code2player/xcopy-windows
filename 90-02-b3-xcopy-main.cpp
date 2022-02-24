/*Ф���� ��13 1953072*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>//exit()Ҫ��
#include <sstream>
#include <fstream>
#include <direct.h>//dirent.h������
#include <windows.h>
#include  <io.h>
#include <sys/stat.h>
using namespace std;

/*�·����������MAX_PATH��ʾ*/

void FileName(char* m_pFileDirectory, char* aim, int* file_num)//�������ΪҪ������·��,ע���޸����в��Ϸ�
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
		if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))//�ļ�
		{
			char file1[400] = { 0 };
			strcpy(file1, m_pFileDirectory);
			strcat(file1, findFileData.cFileName);
			//strcat(file1, "/");

			ifstream fin;
			fin.open(file1, ios::in | ios::binary);
			if (!fin.is_open())
			{
				cerr << "�ļ���ʧ��" << endl;
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
				cerr << "�ļ���ʧ��" << endl;
				exit(-1);
			}

			char* ch = new(nothrow) char[1024 * 64 + 2];//��ǰ���Ի�������С��1K
			if (ch == NULL)
			{
				cout << "�ڴ�δ���뵽" << endl;
				exit(-1);
			}
			while (1)
			{

				fin.read(ch, 1024 * 64);
				fin.clear();
				fout.write(ch, fin.gcount());//�����Ʋ���1024����д��ʵ�ʶ����������
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





void PrintFileName(int digui,char* m_pFileDirectory,char* aim, int *file_num)//�������ΪҪ������·��,ע���޸����в��Ϸ�
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
			//Ŀ¼
			if (!strcmp(findFileData.cFileName, ".") || !strcmp(findFileData.cFileName, ".."))
			{
				//.��..
				static int x = 0;//��һ���ļ����е������ļ�����

				if (x == 0)
				{
					x = 1;
					FileName(m_pFileDirectory, aim, file_num);
				}
			}
			else
			{
				//ϵͳĿ¼

				char file1[400] = { 0 };
				strcpy(file1, m_pFileDirectory);
				strcat(file1, findFileData.cFileName);
				strcat(file1, "\\");

				char file2[400] = { 0 };
				strcpy(file2, aim);
				strcat(file2, findFileData.cFileName);
				strcat(file2, "\\");

				

				/*����֮ǰ�ȴ�����ӦĿ¼*/
				_mkdir(file2);//�������ļ���,ֻ��һ�Σ������ж��Ƿ���ڣ���Ϊ�϶������ڣ�������

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
	/*���������������ohhhhhhh��*/
	/*��һ����Դ�ļ���������Ҫ���ƣ�,�������
	�ڶ�����Ŀ���ļ���������Ҫճ���������벻����*/
	//����_mkdir���������ļ���
	/*·����Ϊ�ļ��У���������\\����/ */

	if (argc != 3)
	{
		cout << "��Ч�Ĳ�������" << endl;
		cout << "������ 0 ���ļ�" << endl;
		return -1;
	}

	if (_access(argv[1], 0) == 0)//��һ������
	{
		struct stat s;
		if (stat(argv[1], &s) == 0)
		{
			if (s.st_mode & S_IFDIR)//�ļ���
			{

			}
			else//�ļ�������
			{
				cout << argv[1] << " - �����ļ���" << endl;
				return -1;
			}
		}
	}
	else
	{
		cout << argv[1] << " - ������" << endl;
		return -1;
	}

	if (_access(argv[2], 0) == 0)//�ڶ�������
	{
		cout << argv[2] << " - �Ѵ���" << endl;
		return -1;
	}
	else//�����ڣ������ļ���
	{
		int len = strlen(argv[2]);
		char str[MAX_PATH] = { 0 };
		strcpy(str, argv[2]);
		if (argv[2][len - 1] != '\\' && argv[2][len - 1] != '/')
		{
			str[len] = '\\';
			str[len+1] = 0;
		}

		_mkdir(str);//�������ļ���
	}

	/*�Ե����ļ���д�ö����ƣ�read��write��
	���û�������С����߶�д�ٶȣ�����*/

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

	cout << "������ " << file_number << " ���ļ�" << endl;

}