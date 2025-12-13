#include <bits/stdc++.h>
using namespace std;
struct point
{
	string text;
	struct point *l,*r;//假设是从左(头)到右(尾)的双向链表
};
typedef struct point p;
int length;
void insert(p *&head)//在第n行插入字符串str
{
	system("cls");
	cout << "整行插入操作:在<行号>处插入新文本" << endl;
	cout << "请输入行号:" << endl;
	int line;
	cin >> line;
	if(line<0||line>length)
	{
		cout << "行号输入有误！" << endl;
		return;
	}
	cout << "请输入文本内容:" << endl;
	string str;
	cin >> str;
	p* newnode = new p;
	newnode->text = str;
	if (head == NULL)//空链表
	{
		newnode->l = NULL;
		newnode->r = NULL;
		head = newnode;
	}
	else if (line == 0)//在头部插入
	{
		newnode->l = NULL;
		newnode->r = head;
		head->l = newnode;
		head = newnode;
	}
	else
	{
		p* temp = head;
		for (int i = 0; i < line - 1; i++)
		{
			temp = temp->r;
		}
		newnode->l = temp;
		newnode->r = temp->r;
		if (temp->r != NULL)
			temp->r->l = newnode;
		temp->r = newnode;
	}
	length++;
	cout << "插入成功！" << endl;
}
void printall(p *head)
{
	system("cls");
	cout << "打印所有文本操作:" << endl;
	p* temp = head;
	int line = 0;
	while (temp != NULL)
	{
		cout << line << ": " << temp->text << endl;
		temp = temp->r;
		line++;
	}
}
int main() {
	p *head = NULL;
	here:
	cout << "选择你的操作类型:" << endl;
	cout << "1.整行插入" << endl;
	cout << "2.整行删除" << endl;
	cout << "3.整行修改" << endl;
	cout << "4.撤销" << endl;
	cout << "5.重做" << endl;
	cout << "6.打印所有文本" << endl;
	cout << "7.读文件" << endl;
	cout << "8.写文件" << endl;
	cout << "9.查找" << endl;
	cout << "10.替换" << endl;
	cout << "11.复制" << endl;
	cout << "12.查找所有符合要求的字符串" << endl;
	cout << "13.依次替换字符串" << endl;
	cout << "14.插入函数" << endl;
	cout << "15.剪切" << endl;
	cout << endl<<"输入相应序号并回车来进行操作" << endl;
	int choice=0;
	cin >> choice;
	switch (choice)
	{
		case 1:
			insert(head);
			system("pause");
			system("cls");
			goto here;
			break;
		case 2:
			system("pause");
			system("cls");
			goto here;
			break;
		case 3:
			system("pause");
			system("cls");
			goto here;
			break;
		case 4:
			system("pause");
			system("cls");
			goto here;
			break;
		case 5:
			system("pause");
			system("cls");
			goto here;
			break;
		case 6:
			printall(head);
			system("pause");
			system("cls");
			goto here;
			break;
		case 7:
			system("pause");
			system("cls");
			goto here;
			break;
		case 8:
			system("pause");
			system("cls");
			goto here;
			break;
		case 9:
			system("pause");
			system("cls");
			goto here;
			break;
		case 10:
			system("pause");
			system("cls");
			goto here;
			break;
		case 11:
			system("pause");
			system("cls");
			goto here;
			break;
		case 12:
			system("pause");
			system("cls");
			goto here;
			break;
		case 13:
			system("pause");
			system("cls");
			goto here;
			break;
		case 14:
			system("pause");
			system("cls");
			goto here;
			break;
		case 15:
			system("pause");
			system("cls");
			goto here;
			break;
		default:
			cout << "输入有误！" << endl;
			system("pause");
			system("cls");
			goto here;
			break;
	}
	return 0;
}