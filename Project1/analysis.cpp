//
/////****************************************************************************
///// @file    : analysis.cpp
///// @version : v1.0
///// @author  : mmkk                                                        
///// @date    :                                                                                                                   
///// @brief   :                                                                                                                   
/////****************************************************************************
//
//
//#include<iostream>
//#include<stack>
//#include <iomanip>
//#include "analysis.h"
//
//using namespace std;
//
//
//// 初始化根结点
//void default_Node(Node* root) {
//	root->lchild = NULL;
//	root->rchild = NULL;
//}
//
//// 创建树叶
//Node* default_L_Node(char c) {
//	Node* N = new Node;
//	default_Node(N);
//	N->Char = c;
//	N->type = '1';
//	return N;
//}
//
//// 创建根节点
//Node* default_R_Node(char type, Node* lchild, Node* rchild)
//{
//
//	Node* N = new Node;
//	default_Node(N);
//	N->type = type;
//	N->lchild = lchild;
//	N->rchild = rchild;
//	return N;
//}
//
//// 根据运算符连接树节点
//Node* connect_Nodes(char op, stack<Node*>& S)
//{
//	switch (op)
//	{
//		Node* item1, * item2;
//	case '|':
//		item1 = S.top(); S.pop();
//		item2 = S.top(); S.pop();
//		//cout << item1->Char << " " << item2->Char << endl;
//		return default_R_Node(op, item2, item1);
//
//	case '+':
//		item1 = S.top(); S.pop();
//		item2 = S.top(); S.pop();
//		//cout << item1->Char << " " << item2->Char << endl;
//		return default_R_Node(op, item2, item1);
//	case '*':
//		item1 = S.top(); S.pop();
//		//cout << item1->Char << endl;
//		return default_R_Node(op, item1, NULL);
//	case '?':
//		item1 = S.top(); S.pop();
//		return default_R_Node(op, item1, NULL);
//	}
//}
//
//// 函数声明
//bool if_cat(char l, char r);
//bool if_high(char a, char b);
//int o_astype(char o);
//
//void printStack(const std::stack<char>& stk) {
//	std::stack<char> tempStack = stk;
//
//	while (!tempStack.empty()) {
//		cout << tempStack.top() << " ";
//		tempStack.pop();
//	}
//	cout << std::endl;
//}
//
//// 构建语法树
//Tree Analysis::Syntax_Tree(string regex)
//{
//	// 符号栈
//	stack<char> opStack;
//	// 语法栈 存储树节点
//	stack<Node*> synStack;
//	// 将#压入op栈中
//	opStack.push(regex[0]);
//
//	// 扫描#中间的内容
//	for (int i = 1; i < regex.size() - 1; i++)
//	{
//		char c = regex[i];
//		//printStack(opStack);
//
//		// 语法内容
//		if (!o_astype(c))
//			synStack.push(default_L_Node(c));
//		else // 符号
//		{
//			switch (c)
//			{
//			case '(': opStack.push(c); break;
//			case ')': // 关括号，需不断出栈直到匹配开括号
//				while (opStack.top() != '(')
//				{
//					char op = opStack.top();
//					opStack.pop();
//					Node* tmp = connect_Nodes(op, synStack); // 为当前运算符进行运算
//					synStack.push(tmp); // 运算结果再次存入语法栈
//				}
//				opStack.pop(); // 将左括号(弹出
//				break;
//			case '*': // 运算优先级最高
//				synStack.push(connect_Nodes(c, synStack));
//				break;
//			case '?': // 运算优先级最高
//				synStack.push(connect_Nodes(c, synStack));
//				break;
//			default: // | + # 
//				// 依次比较当前运算符与栈顶元素
//				// 直到 当前 > 栈顶
//				while (!if_high(opStack.top(), c))
//				{
//					char op = opStack.top(); // 取栈顶元素
//					opStack.pop();
//					synStack.push(connect_Nodes(op, synStack));// 得到新的根结点
//				}
//				opStack.push(c); // 最后将c压入op栈
//			}
//
//		}
//	}
//
//	// 扫描全部结束 将栈内剩余部分连接
//	while (opStack.top() != '#')
//	{
//		char op = opStack.top();
//		opStack.pop();
//		synStack.push(connect_Nodes(op, synStack));
//	}
//	return synStack.top();
//}
//
//
//// 运算符op转换为对应优先级 (从低->高）
//// 输入：运算符  输出：优先级数
//int o_astype(char o)
//{
//	switch (o)
//	{
//	case '#': return 1;
//	case '(': return 2;
//	case '|': return 3;
//	case '+': return 4;
//	case ')': return 5;
//	case '*': return 6;
//	case '?': return 6;
//	default: return 0; // 不是op
//	}
//}
//
//// 比较运算符优先级
//// 左边是否大于右边
//bool if_high(char a, char b)
//{
//	int odd_ = o_astype(a);
//	int new_ = o_astype(b);
//	return (new_ > odd_) ? 1 : 0;
//}
//
//// 判断是否加入连接运算符
//bool if_cat(char l, char r)
//{
//	bool res = 0;
//	if (!o_astype(l) && r == '(') // char+( 
//		res = 1;
//	else if (!o_astype(l) && !o_astype(r)) // char+char
//		res = 1;
//	else if (l == ')' && !o_astype(r)) // )+char
//		res = 1;
//	else if (l == '*' && !o_astype(r)) // *+char
//		res = 1;
//	else if (l == ')' && r == '(')  // )+(
//		res = 1;
//	else if (l == '*' && r == '(')  // *+(
//		res = 1;
//	else if (l == '?' && !o_astype(r)) // ?+char
//		res = 1;
//	else if (l == '?' && r == '(')  // ?+(
//		res = 1;
//	else res = 0;
//
//	return res;
//}
//
//
//// 获取正则表达式
//string Analysis::getRegex(string inputstr)
//{
//	//cout << "请输入正则表达式:";
//	std::string regex = inputstr;
//	regex.insert(0, "#"); // 头插入开始符
//	regex.append("#"); // 尾插入结束符
//
//	// 将regex加入cat连接符
//	int left = 0, right = 1;
//	while (regex[right] != '#')
//	{
//		if (if_cat(regex[left], regex[right]))
//			regex.insert(left + 1, "+");
//		left++;  right++;
//	}
//	return regex;
//}
//
//// 后续遍历树
//void PostTravelse(Node* root)
//{
//	if (root == NULL) {
//		return;
//	}
//	else {
//		PostTravelse(root->lchild);
//		PostTravelse(root->rchild);
//		if (root->lchild == NULL && root->rchild == NULL)
//			cout << root->Char << " ";
//		else cout << root->type << " ";
//	}
//}
//
///*-----------------------------------------------------------
//-------------------------------------------------------------*/
//
//
//
//
//// 初始化状态转移矩阵
//void default_graph(Skips_graph& NFA, char* ab_char, int ab_char_size)
//{
//	NFA.states_size = 0;
//	NFA.ab_char_size = ab_char_size;
//	// 初始化吸收字符数组
//	for (int i = 0; i < ab_char_size; i++)
//		NFA.ab_char[i] = ab_char[i];
//	// 初始化状态跳转表
//	for (int i = 0; i < MaxNum; i++)
//	{
//		for (int j = 0; j < MaxNum; j++)
//		{
//			NFA.Skips[i][j] = MaxNum; // 表示无跳转
//		}
//	}
//	// 初始化空跳转表
//	for (int i = 0; i < MaxNum; i++)
//		NFA.Skips_empty[i][0] = 0; // 空跳转个数为0
//}
//
//
//// 在字符数组中加入新元素，若已存在则返回数组编号
//int add_find_char(char* arr, char c, int& size)
//{
//	if (size == 0) {
//		arr[0] = c;
//		size++;
//		return MaxNum;
//	}
//	else {
//		int i;
//		for (i = 0; i < size; i++)
//		{
//			// 已存在 不添加
//			if (*(arr + i) == c) return i;
//		}
//		arr[i] = c; size++; // 不存在 添加
//		return MaxNum;
//	}
//}
//
//// 得到正则表达式的吸收符号
//int get_absorb_char(string regex, char* ab_chars)
//{
//	int size = 0;
//	for (int i = 0; i < regex.size(); i++)
//	{
//		if (!o_astype(regex[i]))
//		{
//			add_find_char(ab_chars, regex[i], size);
//		}
//	}
//	return size;
//}
//
//// 添加新的状态跳转 图 起点 终点 吸收字符
//void add_Skips(Skips_graph& NFA, int begin, int end, char c) {
//	int ab_char = add_find_char(NFA.ab_char, c, NFA.ab_char_size);
//	NFA.Skips[begin][ab_char] = end;
//}
//// 添加新的空状态跳转 图 起点 终点 吸收字符
//void add_empty_Skips(Skips_graph& NFA, int begin, int end) {
//	NFA.Skips_empty[begin][0] = NFA.Skips_empty[begin][0] + 1; //空跳转数加一
//	int size_ = NFA.Skips_empty[begin][0];
//	NFA.Skips_empty[begin][size_] = end;
//}
//
//// 构造叶子节点状态转移
//void Leaf_Skip(Skips_graph& NFA, Node& N) {
//	N.start_S = NFA.states_size; NFA.states_size++;
//	N.end_S = NFA.states_size; NFA.states_size++;
//	add_Skips(NFA, N.start_S, N.end_S, N.Char); // 添加始末状态转移
//}
//// 构造根结点状态转移
//void Root_Skip(Skips_graph& NFA, Node& N)
//{
//	// 或根构造
//	if (N.type == '|')
//	{
//		N.start_S = NFA.states_size; NFA.states_size++;
//		N.end_S = NFA.states_size; NFA.states_size++;
//		add_empty_Skips(NFA, N.start_S, N.lchild->start_S);
//		add_empty_Skips(NFA, N.start_S, N.rchild->start_S);
//		add_empty_Skips(NFA, N.lchild->end_S, N.end_S);
//		add_empty_Skips(NFA, N.rchild->end_S, N.end_S);
//	}
//	// 闭包构造
//	else if (N.type == '*')
//	{
//		N.start_S = NFA.states_size; NFA.states_size++;
//		N.end_S = NFA.states_size; NFA.states_size++;
//		add_empty_Skips(NFA, N.start_S, N.lchild->start_S);
//		add_empty_Skips(NFA, N.start_S, N.end_S);
//		add_empty_Skips(NFA, N.lchild->end_S, N.lchild->start_S);
//		add_empty_Skips(NFA, N.lchild->end_S, N.end_S);
//	}
//	// 可选构造
//	else if (N.type == '?')
//	{
//		// 不加状态版
//		//N.start_S = N.lchild->start_S;
//		//N.end_S = N.lchild->end_S;
//		//add_empty_Skips(NFA, N.lchild->end_S, N.lchild->start_S);
//		N.start_S = NFA.states_size; NFA.states_size++;
//		N.end_S = NFA.states_size; NFA.states_size++;
//		add_empty_Skips(NFA, N.start_S, N.lchild->start_S);
//		add_empty_Skips(NFA, N.lchild->end_S, N.end_S);
//		//add_empty_Skips(NFA, N.lchild->end_S, N.end_S);
//		add_empty_Skips(NFA, N.start_S, N.end_S);
//	}
//	// 连接构造
//	else if (N.type == '+')
//	{
//		N.start_S = N.lchild->start_S;
//		N.end_S = N.rchild->end_S;
//		add_empty_Skips(NFA, N.lchild->end_S, N.rchild->start_S);
//	}
//	else cout << "error..." << endl;
//}
//
//
//// 递归构造NFA
//void Analysis::createNFA(Tree tree, Skips_graph& NFA, string regex)
//{
//	// 构造状态转移矩阵
//	if (tree->type == 'l') // 叶子状态转移
//		Leaf_Skip(NFA, *tree);
//	else
//	{
//		createNFA(tree->lchild, NFA, regex); // 构造左子树状态转移
//		if (tree->rchild != NULL) createNFA(tree->rchild, NFA, regex); //构造右子树状态转移
//		Root_Skip(NFA, *tree); // 构造根状态转移
//	}
//}
//
//void PostCreate(Tree tree, Skips_graph& NFA, string regex)
//{
//	if (tree == NULL) {
//		return;
//	}
//	else {
//		PostCreate(tree->lchild, NFA, regex);
//		PostCreate(tree->rchild, NFA, regex);
//		if (tree->lchild == NULL && tree->rchild == NULL) {
//			cout << "create: " << tree->Char << endl;
//			Leaf_Skip(NFA, *tree);
//		}
//
//		else {
//			cout << "create: " << tree->type << endl;
//			Root_Skip(NFA, *tree); // 构造根状态转移
//		}
//	}
//}
//
//// Tompson算法得到NFA
//void Analysis::getNFA(Tree tree, Skips_graph& NFA, string regex)
//{
//	// 得到吸收符号
//	char ab_chars[MaxNum]; // 正则表达式中的吸收符号
//	int ab_chars_size = get_absorb_char(regex, ab_chars);
//	cout << "ab_chars[]:";
//	for (int i = 0; i < ab_chars_size; i++) cout << ab_chars[i] << " ";
//	cout << endl;
//	// 初始化状态转移表NFA
//	default_graph(NFA, ab_chars, ab_chars_size);
//	// 构造NFA 将信息存入synTree中
//	PostCreate(tree, NFA, regex);
//	NFA.start_s = tree->start_S;
//	NFA.end_s = tree->end_S;
//	cout << "NFA start_state: " << tree->start_S << endl;
//	cout << "NFA end_state: " << tree->end_S << endl;
//	cout << endl;
//}
//
////读NFA
//void Analysis::display_Graph(Skips_graph& S, string type)
//{
//	cout << "skip matrix:" << endl;
//	// 打印属性行
//	cout << "state";
//	for (int i = 0; i < S.ab_char_size; i++)
//		cout << setw(6) << S.ab_char[i];
//	cout << setw(10) << "null" << endl;
//	// 打印NFA
//	for (int i = 0; i < S.states_size; i++)
//	{
//		cout << setw(3) << i;
//		for (int j = 0; j < S.ab_char_size; j++)
//			if (S.Skips[i][j] == MaxNum) cout << setw(6) << "-";
//			else cout << setw(6) << S.Skips[i][j];
//		//输出空跳转
//		cout << setw(6) << " ";
//		for (int j = 0; j < S.Skips_empty[i][0]; j++)
//			cout << S.Skips_empty[i][j + 1] << " ";
//		cout << endl;
//	}
//	// 打印起点终点
//	cout << "start_state:" << S.start_s << " ";
//	// NFA
//	if (type == "nfa") cout << "end_state:" << S.end_s << " ";
//	// DFA
//	else {
//		cout << "end_state:";
//		for (int i = 0; i < S.end_set.length; i++)
//			cout << S.end_set.st_set[i] << " ";
//	}
//	cout << endl;
//}
//
///*------------------------------------------------------
//--------------------------------------------------------*/
//// 向状态集中添加元素
//// 输入：状态集数组、状态数值
//bool add_s_to_set(states_set& set, int s)
//{
//	for (int i = 0; i < set.length; i++)
//	{
//		if (set.st_set[i] == s) return 0;
//	}
//	set.st_set[set.length] = s;
//	set.length++;
//	return 1;
//}
//
//// 求当前状态集合的空闭包
//void empty_closure(states_set& set, int Skips_empty[][MaxNum])
//{
//	for (int i = 0; i < set.length; i++)
//	{
//		int p = set.st_set[i]; // 获取当前状态
//		for (int j = 0; j < Skips_empty[p][0]; j++)
//		{
//			// 将以p为起始状态的空闭包状态存入当期状态集set中
//			add_s_to_set(set, Skips_empty[p][j + 1]);
//		}
//	}
//	//	cout << "当前闭包: ";
//	//	for(int i = 0; i < set.length; i++) cout << set.st_set[i] << " ";
//	//	cout << endl;
//}
//
//void Smove(states_set set, states_set& next_set, int ab_char, Skips_graph& NFA)
//{
//	// 对每个状态都计算经过a得到的新状态
//	for (int i = 0; i < set.length; i++)
//	{
//		int nfa_i = set.st_set[i]; // 在nfa中的状态
//		if (NFA.Skips[nfa_i][ab_char] != MaxNum) // 有存在经过ab_char的跳转
//			add_s_to_set(next_set, NFA.Skips[nfa_i][ab_char]);
//	}
//}
//
////// 在状态集合集sets中 找 是否存在当前状态集set
////int if_exist(states_set* sets, states_set set, int sets_len)
////{
////	// 找长度相等的再对比
////	int T;
////	for (int i = 0; i < sets_len; i++)
////	{
////		if (sets[i].length == sets_len)
////		{
////			T = 1; // 假设是
////			for (int j = 0; j < set.length; j++)
////			{
////				bool flag = false;
////				for (int k = 0; k < sets[i].length; k++)
////				{
////					if (sets[i].st_set[k] == set.st_set[j]) 
////					{
////						flag = true;
////						break;
////					}
////				}
////				// 找不到相同元素，当前sets中集合与set不相同
////				if (flag == false) { T = 0; break; }
////			}
////			if (T == 1) return i; // 状态集合集sets中已存在当前状态集set
////		}
////	}
////	return -1; // 在sets中不存在与set相同的状态集
////}
//
////判断当前状态集是否存在，若存在则返回新状态，若不存在则返回0
//int if_exist(states_set* Sets, states_set Set, int sets_length) {
//	//找到长度相等的进行比对
//	int T; //判断是否有相同的状态集
//	for (int i = 0; i < sets_length; i++) {
//		if (Sets[i].length == Set.length) {
//			T = 1; //先假设该集合是
//			for (int j = 0; j < Set.length; j++)
//			{
//				int t = 0; //判断是否找到相同元素
//				for (int k = 0; k < Sets[i].length; k++)
//					if (Set.st_set[j] == Sets[i].st_set[k]) { t = 1; break; }
//				if (t == 0) { T = 0; break; } //找不到，该集合不是
//			}
//			if (T == 1) return i;
//		}
//	}
//	return 0;
//}
//
//void Analysis::createDFA(Skips_graph& NFA, Skips_graph& DFA, states_set* sets)
//{
//	for (int i = 0; i < DFA.states_size; i++)
//	{
//		for (int c = 0; c < DFA.ab_char_size; c++)
//		{
//			states_set next_set;
//			//对当前状态集中每一个元素进行c的跳转，加入到next_set中
//			Smove(sets[i], next_set, c, NFA);
//			if (next_set.length == 0) continue; // 当前状态集不吸收c
//			empty_closure(next_set, NFA.Skips_empty); // 求next_set的空闭包的空闭包
//			// 判断该状态集是否已经存在，如果不存在则作为新状态集加入DFA
//			int S = if_exist(sets, next_set, DFA.states_size);
//			if (!S) { // 不存在 新增进sets中
//				S = DFA.states_size;
//				sets[S] = next_set;
//				DFA.states_size++; // 状态集数+1
//			}
//			// 在DFA上添加当前状态吸收字符的跳转 
//			DFA.Skips[i][c] = S; // 记录的是状态集的编号
//		}
//	}
//}
//
//// 判断集合中是否有某个元素
//bool find_s(states_set set, int s)
//{
//	for (int i = 0; i < set.length; i++)
//		if (set.st_set[i] == s) return true;
//	return false;
//}
//
//// 子集构造算法生成DFA
//void Analysis::getDFA(Skips_graph& NFA, Skips_graph& DFA)
//{
//	// 初始化DFA
//	default_graph(DFA, NFA.ab_char, NFA.ab_char_size);
//	states_set* sets = DFA.sets; // 状态表
//
//	// 求起始状态的空闭包
//	states_set begin_set;
//	add_s_to_set(begin_set, NFA.start_s);
//	empty_closure(begin_set, NFA.Skips_empty);
//	DFA.begin_set = begin_set; // 存储开始集
//
//	// 将0号起始状态集作为DFA的起始状态加入
//	DFA.start_s = 0;
//	// 将起始状态集加入状态集语新状态的对照表中
//	sets[DFA.states_size] = begin_set;
//	DFA.states_size++;
//	createDFA(NFA, DFA, sets); // 构造DFA
//
//	// 找到DFA所有的终点
//	int end_s = NFA.end_s;
//	for (int i = 0; i < DFA.states_size; i++)
//	{
//		// 如果当前状态集sets[i]中存在终点元素 则将其纳入终点集中
//		if (find_s(sets[i], end_s))
//			add_s_to_set(DFA.end_set, i);
//	}
//}
//
///*-----------------------------------------------------
//-------------------------------------------------------*/
//
//
//void crack_end(Skips_graph& DFA, int* new_states, int& find_num)
//{
//	states_set new_end_sets;
//	if (DFA.end_set.length == 1)
//	{
//		new_states[DFA.end_set.st_set[0]] = DFA.end_set.st_set[0];
//		find_num++;
//		add_s_to_set(new_end_sets, DFA.end_set.st_set[0]);
//	}
//	else {
//		int not_find_num = DFA.end_set.length; // 未找到的元素
//		for (int c = 0; c < DFA.ab_char_size; c++)
//		{
//			int T = MaxNum; // 表示还未找到第一个终态
//			for (int i = DFA.states_size - 1; i >= 0; i--)
//			{
//				// 在接受该字符的跳转里找到终态
//				if (find_s(DFA.end_set, DFA.Skips[i][c]))
//				{
//					if (T == MaxNum)
//					{
//						T = DFA.Skips[i][c];
//						add_s_to_set(new_end_sets, T);
//					}
//					// 用第一个发现的终态 作为该吸收的终态
//					new_states[DFA.Skips[i][c]] = T;
//					//new_states[i] = T;
//					not_find_num--;
//					find_num++;
//				}
//				if (not_find_num == 0) break;
//			}
//			if (not_find_num == 0) break;
//		}
//	}
//	DFA.end_set = new_end_sets;
//}
//
//// 分裂，分裂成功返回1，不可再分返回0
//bool crack(Skips_graph& DFA, int* new_states, int& find_num)
//{
//	for (int i = 0; i < DFA.states_size; i++)
//	{
//		if (new_states[i] != MaxNum) continue;
//		for (int c = 0; c < DFA.ab_char_size; c++)
//		{
//			int T = 1; // 是否可以区分
//			for (int j = 0; j < DFA.states_size; j++)
//			{
//				if (i == j) continue;
//				// 跳转的结果在一个集合
//				if (new_states[DFA.Skips[i][c]] == new_states[DFA.Skips[j][c]])
//				{
//					T = 0; break;
//				}
//			}
//			// 可再分
//			if (T == 1) { new_states[i] = i; find_num++; return 1; }
//		}
//	}
//	return 0; // 不可再分
//}
//
////判断数组前n个是否出现过当前字符
//bool find_int(int* array_, int n, int s) {
//	for (int i = 0; i < n; i++) {
//		if (array_[i] == s) return 1;
//	}
//	return 0;
//}
////查找旧状态为s的新状态
//int find_s_in_array(int* array, int s, int length) {
//	for (int i = 0; i < length; i++) {
//		if (array[i] == s) return i;
//	}
//}
////整理合并后的dfa
//void combine(Skips_graph DFA, Skips_graph& minDFA, int* old_states) {
//	default_graph(minDFA, DFA.ab_char, DFA.ab_char_size);//初始化
//	int new_states[MaxNum];
//	//将没有出现过的状态加入
//	for (int i = 0; i < DFA.states_size; i++) {
//		if (find_int(old_states, i, old_states[i])) continue;//判断之前是否出现过
//		for (int c = 0; c < DFA.ab_char_size; c++)
//			minDFA.Skips[minDFA.states_size][c] = DFA.Skips[i][c];  //复制进去
//		new_states[minDFA.states_size] = old_states[i];//保存老状态
//		minDFA.states_size++;
//	}
//	//将所有状态调整
//	for (int i = 0; i < minDFA.states_size; i++) {
//		for (int c = 0; c < minDFA.ab_char_size; c++)
//			minDFA.Skips[i][c] = find_s_in_array(new_states, minDFA.Skips[i][c], minDFA.states_size);
//	}
//	//记录起点
//	minDFA.start_s = find_s_in_array(new_states, DFA.start_s, minDFA.states_size);
//	//终结状态作调整
//	for (int i = 0; i < DFA.end_set.length; i++) {
//
//		add_s_to_set(minDFA.end_set, find_s_in_array(new_states, DFA.end_set.st_set[i], minDFA.states_size));
//	}
//	//    cout<<min_dfa.end_set.length;
//	//        for(int i=0;i<min_dfa.end_set.length;i++)
//	//            cout<<min_dfa.end_set.st_set[i]<<" ";
//}
//
//// 最小化DFA
//void Analysis::minimizeDFA(Skips_graph DFA, Skips_graph& minDFA)
//{
//	// 新状态
//	int find_num = 0; // 确定新状态的状态数
//	int new_states[MaxNum];
//	for (int i = 0; i < DFA.states_size; i++)
//		new_states[i] = MaxNum;
//	/* 将状态分出：构造NFA到DFA，每个状态只能吸收一个符号，
//	找到每个符号对应的终点集，将其标为相同的状态 */
//	crack_end(DFA, new_states, find_num);
//	// 将每个可区分的状态区分出来
//	// 条件：未找完或还可以继续区分
//	while (find_num != DFA.states_size && crack(DFA, new_states, find_num)) { ; }
//	// 将剩余的状态划分为一个状态
//	if (find_num != DFA.states_size)
//	{
//		int T = MaxNum;
//		for (int i = 0; i < DFA.states_size; i++)
//		{
//			if (new_states[i] != MaxNum) continue;
//			if (T == MaxNum) T = i;
//		}
//	}
//	// 将新旧状态不同的 修改符号
//	for (int i = 0; i < DFA.states_size; i++)
//	{
//		if (i == new_states[i]) continue;
//		for (int j = 0; j < DFA.states_size; j++) {
//			for (int c = 0; c < DFA.ab_char_size; c++) {
//				if (DFA.Skips[j][c] == i) DFA.Skips[j][c] = new_states[i];
//			}
//		}
//	}
//	DFA.start_s = new_states[DFA.start_s];//将改变的起点记录下来
//	combine(DFA, minDFA, new_states);
//}
//
//
//void Analysis::analyse(string inputstr)
//{
//	// 获得配置好的regex
//	string regex = this->getRegex(inputstr);
//	for (int i = 0; i < regex.size(); i++) cout << regex[i] << " ";
//	cout << endl;
//
//	// 构建语法树
//	this->synTree = this->Syntax_Tree(regex);
//	cout << "PostTraverse: ";
//	PostTravelse(synTree);
//	cout << endl << endl;
//
//	// Thompson算法构造NFA
//	cout << "Thompson->NFA:" << endl;
//	this->getNFA(synTree, this->NFA, regex); // 存入this->NFA中
//	this->display_Graph(NFA, "nfa");
//
//	// 子集构造算法构造DFA
//	cout << "子集构造算法构造DFA:" << endl;
//	getDFA(this->NFA, this->DFA); // 存入this->DFA中
//	display_Graph(this->DFA, "dfa");
//
//	// 最小化DFA
//	cout << "minDFA: " << endl;
//	minimizeDFA(DFA, minDFA); // 存入this->minDFA中
//	display_Graph(minDFA, "dfa");
//}
//
//
