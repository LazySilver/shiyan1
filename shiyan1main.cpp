#include <bits/stdc++.h>
using namespace std;
struct point
{
	string text;
	struct point *l,*r;//假设是从左(头)到右(尾)的双向链表
};
typedef struct point p;
int length = 0;

// 快照前缀与索引
string snapPrefix = "snapshot_";
int snapIndex = 0;    // 当前快照索引（0 初始）
int maxSnapIndex = 0; // 最大快照索引（用于限制 redo）

// 清空链表
void clearAll(p*& head)
{
	while (head)
	{
		p* t = head->r;
		delete head;
		head = t;
	}
	head = NULL;
	length = 0;
}

// 从文件按行追加到链表尾（不清空）
void appendLine(p*& head, const string& s)
{
	p* node = new p;
	node->text = s;
	node->l = node->r = NULL;
	if (head == NULL) {
		head = node;
	} else {
		p* t = head;
		while (t->r) t = t->r;
		t->r = node;
		node->l = t;
	}
	++length;
}

// 将当前内存文本写到指定文件（每行一条）
bool saveToFilename(p* head, const string& fname)
{
	ofstream ofs(fname);
	if (!ofs.is_open()) return false;
	p* t = head;
	while (t) {
		ofs << t->text << '\n';
		t = t->r;
	}
	ofs.close();
	return true;
}

// 从指定文件加载文本到链表（覆盖当前内容）
bool loadFromFilename(p*& head, const string& fname)
{
	ifstream ifs(fname);
	if (!ifs.is_open()) return false;
	clearAll(head);
	string line;
	while (std::getline(ifs, line)) {
		appendLine(head, line);
	}
	ifs.close();
	return true;
}

// 把当前状态保存为下一个快照文件 snapshot_N.txt
bool saveSnapshot(p* head)
{
	++snapIndex;
	string fname = snapPrefix + to_string(snapIndex) + ".txt";
	if (!saveToFilename(head, fname)) {
		--snapIndex;
		return false;
	}
	// 新的操作使 redo 历史无效
	maxSnapIndex = snapIndex;
	return true;
}

// 加载指定索引的快照（0..max），索引为 0 时为初始空文档（文件 snapshot_0.txt）
bool loadSnapshot(p*& head, int index)
{
	if (index < 0) return false;
	string fname = snapPrefix + to_string(index) + ".txt";
	return loadFromFilename(head, fname);
}

// 在位置 line (0..length) 插入一行
void insert(p*& head)
{
	system("cls");
	cout << "整行插入操作:在<行号>处插入新文本" << endl;
	cout << "请输入行号:" << endl;
	int line;
	cin >> line;
	if(line < 0 || line > length)
	{
		cout << "行号输入有误！" << endl;
		return;
	}
	cout << "请输入文本内容:" << endl;
	// 读取整行文本（吞掉残留换行）
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	string str;
	getline(cin, str);

	p* newnode = new p;
	newnode->text = str;
	newnode->l = newnode->r = NULL;

	if (head == NULL)//空链表
	{
		head = newnode;
	}
	else if (line == 0)//在头部插入
	{
		newnode->r = head;
		head->l = newnode;
		head = newnode;
	}
	else
	{
		p* temp = head;
		for (int i = 0; i < line - 1 && temp->r != NULL; i++)
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
	// 保存快照
	if (!saveSnapshot(head)) {
		cout << "警告：保存快照失败（撤销/重做不可用）。" << endl;
	}
	cout << "插入成功！" << endl;
}

// 删除行（0-based）
bool removeAtPos(p*& head, int pos, string& out)
{
	if (!head || pos < 0 || pos >= length) return false;
	p* target = head;
	for (int i = 0; i < pos; ++i) target = target->r;
	out = target->text;
	if (target->l) target->l->r = target->r;
	else head = target->r; // 删除头节点
	if (target->r) target->r->l = target->l;
	delete target;
	--length;
	return true;
}

// 修改行（0-based）
bool setLineAt(p* head, int pos, const string& newText, string& outOld)
{
	if (!head || pos < 0 || pos >= length) return false;
	p* node = head;
	for (int i = 0; i < pos; ++i) node = node->r;
	outOld = node->text;
	node->text = newText;
	return true;
}

void printall(p* head)
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

// 撤销：加载上一个快照
void undo(p*& head)
{
	system("cls");
	cout << "撤销操作:" << endl;
	if (snapIndex <= 0) {
		cout << "没有可撤销的操作。" << endl;
		return;
	}
	int target = snapIndex - 1;
	if (!loadSnapshot(head, target)) {
		cout << "撤销失败：无法加载快照文件 " << snapPrefix << target << ".txt" << endl;
		return;
	}
	snapIndex = target;
	cout << "撤销完成（已恢复到快照 " << snapIndex << "）。" << endl;
}

// 重做：加载下一个快照（仅当存在更后快照时）
void redo(p*& head)
{
	system("cls");
	cout << "重做操作:" << endl;
	if (snapIndex >= maxSnapIndex) {
		cout << "没有可重做的操作。" << endl;
		return;
	}
	int target = snapIndex + 1;
	if (!loadSnapshot(head, target)) {
		cout << "重做失败：无法加载快照文件 " << snapPrefix << target << ".txt" << endl;
		return;
	}
	snapIndex = target;
	cout << "重做完成（已恢复到快照 " << snapIndex << "）。" << endl;
}

// 读文件（用户指定文件），读入后作为一次新的操作 -> 保存快照
void readFileOp(p*& head)
{
	system("cls");
	cout << "读文件: 请输入要读取的文件名:" << endl;
	string fname;
	cin >> fname;
	if (loadFromFilename(head, fname)) {
		if (!saveSnapshot(head)) {
			cout << "警告：保存快照失败（撤销/重做不可用）。" << endl;
		} else {
			cout << "读取文件成功，已保存快照。" << endl;
		}
	} else {
		cout << "读取文件失败！" << endl;
	}
}

// 写文件（用户指定文件名）
void writeFileOp(p* head)
{
	system("cls");
	cout << "写文件: 请输入要写入的文件名:" << endl;
	string fname;
	cin >> fname;
	if (saveToFilename(head, fname)) {
		cout << "写入文件成功：" << fname << endl;
	} else {
		cout << "写入文件失败！" << endl;
	}
}

int main() {
	p* head = NULL;
	length = 0;

	// 创建初始快照文件 snapshot_0.txt（当前可能为空）
	{
		string fname0 = snapPrefix + to_string(0) + ".txt";
		saveToFilename(head, fname0); // 忽略返回值
		snapIndex = 0;
		maxSnapIndex = 0;
	}

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
			system("cls");
			cout << "整行删除: 请输入要删除的行号:" << endl;
			{
				int line;
				cin >> line;
				if (line < 0 || line >= length) {
					cout << "行号输入有误！" << endl;
				} else {
					string deleted;
					if (removeAtPos(head, line, deleted)) {
						if (!saveSnapshot(head)) {
							cout << "警告：保存快照失败（撤销/重做不可用）。" << endl;
						} else {
							cout << "删除成功，已保存快照以便撤销。" << endl;
						}
					} else {
						cout << "删除失败。" << endl;
					}
				}
			}
			system("pause");
			system("cls");
			goto here;
			break;
		case 3:
			system("cls");
			cout << "整行修改: 请输入要修改的行号:" << endl;
			{
				int line;
				cin >> line;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				if (line < 0 || line >= length) {
					cout << "行号输入有误！" << endl;
				} else {
					cout << "输入新文本:" << endl;
					string newText;
					getline(cin, newText);
					string oldText;
					if (setLineAt(head, line, newText, oldText)) {
						if (!saveSnapshot(head)) {
							cout << "警告：保存快照失败（撤销/重做不可用）。" << endl;
						} else {
							cout << "修改成功，已保存快照以便撤销。" << endl;
						}
					} else {
						cout << "修改失败。" << endl;
					}
				}
			}
			system("pause");
			system("cls");
			goto here;
			break;
		case 4:
			undo(head);
			system("pause");
			system("cls");
			goto here;
			break;
		case 5:
			redo(head);
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
			readFileOp(head);
			system("pause");
			system("cls");
			goto here;
			break;
		case 8:
			writeFileOp(head);
			system("pause");
			system("cls");
			goto here;
			break;
		// 其余功能占位
		case 9: case 10: case 11: case 12: case 13: case 14: case 15:
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