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
//// ��ʼ�������
//void default_Node(Node* root) {
//	root->lchild = NULL;
//	root->rchild = NULL;
//}
//
//// ������Ҷ
//Node* default_L_Node(char c) {
//	Node* N = new Node;
//	default_Node(N);
//	N->Char = c;
//	N->type = '1';
//	return N;
//}
//
//// �������ڵ�
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
//// ����������������ڵ�
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
//// ��������
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
//// �����﷨��
//Tree Analysis::Syntax_Tree(string regex)
//{
//	// ����ջ
//	stack<char> opStack;
//	// �﷨ջ �洢���ڵ�
//	stack<Node*> synStack;
//	// ��#ѹ��opջ��
//	opStack.push(regex[0]);
//
//	// ɨ��#�м������
//	for (int i = 1; i < regex.size() - 1; i++)
//	{
//		char c = regex[i];
//		//printStack(opStack);
//
//		// �﷨����
//		if (!o_astype(c))
//			synStack.push(default_L_Node(c));
//		else // ����
//		{
//			switch (c)
//			{
//			case '(': opStack.push(c); break;
//			case ')': // �����ţ��費�ϳ�ջֱ��ƥ�俪����
//				while (opStack.top() != '(')
//				{
//					char op = opStack.top();
//					opStack.pop();
//					Node* tmp = connect_Nodes(op, synStack); // Ϊ��ǰ�������������
//					synStack.push(tmp); // �������ٴδ����﷨ջ
//				}
//				opStack.pop(); // ��������(����
//				break;
//			case '*': // �������ȼ����
//				synStack.push(connect_Nodes(c, synStack));
//				break;
//			case '?': // �������ȼ����
//				synStack.push(connect_Nodes(c, synStack));
//				break;
//			default: // | + # 
//				// ���αȽϵ�ǰ�������ջ��Ԫ��
//				// ֱ�� ��ǰ > ջ��
//				while (!if_high(opStack.top(), c))
//				{
//					char op = opStack.top(); // ȡջ��Ԫ��
//					opStack.pop();
//					synStack.push(connect_Nodes(op, synStack));// �õ��µĸ����
//				}
//				opStack.push(c); // ���cѹ��opջ
//			}
//
//		}
//	}
//
//	// ɨ��ȫ������ ��ջ��ʣ�ಿ������
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
//// �����opת��Ϊ��Ӧ���ȼ� (�ӵ�->�ߣ�
//// ���룺�����  ��������ȼ���
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
//	default: return 0; // ����op
//	}
//}
//
//// �Ƚ���������ȼ�
//// ����Ƿ�����ұ�
//bool if_high(char a, char b)
//{
//	int odd_ = o_astype(a);
//	int new_ = o_astype(b);
//	return (new_ > odd_) ? 1 : 0;
//}
//
//// �ж��Ƿ�������������
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
//// ��ȡ������ʽ
//string Analysis::getRegex(string inputstr)
//{
//	//cout << "������������ʽ:";
//	std::string regex = inputstr;
//	regex.insert(0, "#"); // ͷ���뿪ʼ��
//	regex.append("#"); // β���������
//
//	// ��regex����cat���ӷ�
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
//// ����������
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
//// ��ʼ��״̬ת�ƾ���
//void default_graph(Skips_graph& NFA, char* ab_char, int ab_char_size)
//{
//	NFA.states_size = 0;
//	NFA.ab_char_size = ab_char_size;
//	// ��ʼ�������ַ�����
//	for (int i = 0; i < ab_char_size; i++)
//		NFA.ab_char[i] = ab_char[i];
//	// ��ʼ��״̬��ת��
//	for (int i = 0; i < MaxNum; i++)
//	{
//		for (int j = 0; j < MaxNum; j++)
//		{
//			NFA.Skips[i][j] = MaxNum; // ��ʾ����ת
//		}
//	}
//	// ��ʼ������ת��
//	for (int i = 0; i < MaxNum; i++)
//		NFA.Skips_empty[i][0] = 0; // ����ת����Ϊ0
//}
//
//
//// ���ַ������м�����Ԫ�أ����Ѵ����򷵻�������
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
//			// �Ѵ��� �����
//			if (*(arr + i) == c) return i;
//		}
//		arr[i] = c; size++; // ������ ���
//		return MaxNum;
//	}
//}
//
//// �õ�������ʽ�����շ���
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
//// ����µ�״̬��ת ͼ ��� �յ� �����ַ�
//void add_Skips(Skips_graph& NFA, int begin, int end, char c) {
//	int ab_char = add_find_char(NFA.ab_char, c, NFA.ab_char_size);
//	NFA.Skips[begin][ab_char] = end;
//}
//// ����µĿ�״̬��ת ͼ ��� �յ� �����ַ�
//void add_empty_Skips(Skips_graph& NFA, int begin, int end) {
//	NFA.Skips_empty[begin][0] = NFA.Skips_empty[begin][0] + 1; //����ת����һ
//	int size_ = NFA.Skips_empty[begin][0];
//	NFA.Skips_empty[begin][size_] = end;
//}
//
//// ����Ҷ�ӽڵ�״̬ת��
//void Leaf_Skip(Skips_graph& NFA, Node& N) {
//	N.start_S = NFA.states_size; NFA.states_size++;
//	N.end_S = NFA.states_size; NFA.states_size++;
//	add_Skips(NFA, N.start_S, N.end_S, N.Char); // ���ʼĩ״̬ת��
//}
//// ��������״̬ת��
//void Root_Skip(Skips_graph& NFA, Node& N)
//{
//	// �������
//	if (N.type == '|')
//	{
//		N.start_S = NFA.states_size; NFA.states_size++;
//		N.end_S = NFA.states_size; NFA.states_size++;
//		add_empty_Skips(NFA, N.start_S, N.lchild->start_S);
//		add_empty_Skips(NFA, N.start_S, N.rchild->start_S);
//		add_empty_Skips(NFA, N.lchild->end_S, N.end_S);
//		add_empty_Skips(NFA, N.rchild->end_S, N.end_S);
//	}
//	// �հ�����
//	else if (N.type == '*')
//	{
//		N.start_S = NFA.states_size; NFA.states_size++;
//		N.end_S = NFA.states_size; NFA.states_size++;
//		add_empty_Skips(NFA, N.start_S, N.lchild->start_S);
//		add_empty_Skips(NFA, N.start_S, N.end_S);
//		add_empty_Skips(NFA, N.lchild->end_S, N.lchild->start_S);
//		add_empty_Skips(NFA, N.lchild->end_S, N.end_S);
//	}
//	// ��ѡ����
//	else if (N.type == '?')
//	{
//		// ����״̬��
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
//	// ���ӹ���
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
//// �ݹ鹹��NFA
//void Analysis::createNFA(Tree tree, Skips_graph& NFA, string regex)
//{
//	// ����״̬ת�ƾ���
//	if (tree->type == 'l') // Ҷ��״̬ת��
//		Leaf_Skip(NFA, *tree);
//	else
//	{
//		createNFA(tree->lchild, NFA, regex); // ����������״̬ת��
//		if (tree->rchild != NULL) createNFA(tree->rchild, NFA, regex); //����������״̬ת��
//		Root_Skip(NFA, *tree); // �����״̬ת��
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
//			Root_Skip(NFA, *tree); // �����״̬ת��
//		}
//	}
//}
//
//// Tompson�㷨�õ�NFA
//void Analysis::getNFA(Tree tree, Skips_graph& NFA, string regex)
//{
//	// �õ����շ���
//	char ab_chars[MaxNum]; // ������ʽ�е����շ���
//	int ab_chars_size = get_absorb_char(regex, ab_chars);
//	cout << "ab_chars[]:";
//	for (int i = 0; i < ab_chars_size; i++) cout << ab_chars[i] << " ";
//	cout << endl;
//	// ��ʼ��״̬ת�Ʊ�NFA
//	default_graph(NFA, ab_chars, ab_chars_size);
//	// ����NFA ����Ϣ����synTree��
//	PostCreate(tree, NFA, regex);
//	NFA.start_s = tree->start_S;
//	NFA.end_s = tree->end_S;
//	cout << "NFA start_state: " << tree->start_S << endl;
//	cout << "NFA end_state: " << tree->end_S << endl;
//	cout << endl;
//}
//
////��NFA
//void Analysis::display_Graph(Skips_graph& S, string type)
//{
//	cout << "skip matrix:" << endl;
//	// ��ӡ������
//	cout << "state";
//	for (int i = 0; i < S.ab_char_size; i++)
//		cout << setw(6) << S.ab_char[i];
//	cout << setw(10) << "null" << endl;
//	// ��ӡNFA
//	for (int i = 0; i < S.states_size; i++)
//	{
//		cout << setw(3) << i;
//		for (int j = 0; j < S.ab_char_size; j++)
//			if (S.Skips[i][j] == MaxNum) cout << setw(6) << "-";
//			else cout << setw(6) << S.Skips[i][j];
//		//�������ת
//		cout << setw(6) << " ";
//		for (int j = 0; j < S.Skips_empty[i][0]; j++)
//			cout << S.Skips_empty[i][j + 1] << " ";
//		cout << endl;
//	}
//	// ��ӡ����յ�
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
//// ��״̬�������Ԫ��
//// ���룺״̬�����顢״̬��ֵ
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
//// ��ǰ״̬���ϵĿձհ�
//void empty_closure(states_set& set, int Skips_empty[][MaxNum])
//{
//	for (int i = 0; i < set.length; i++)
//	{
//		int p = set.st_set[i]; // ��ȡ��ǰ״̬
//		for (int j = 0; j < Skips_empty[p][0]; j++)
//		{
//			// ����pΪ��ʼ״̬�Ŀձհ�״̬���뵱��״̬��set��
//			add_s_to_set(set, Skips_empty[p][j + 1]);
//		}
//	}
//	//	cout << "��ǰ�հ�: ";
//	//	for(int i = 0; i < set.length; i++) cout << set.st_set[i] << " ";
//	//	cout << endl;
//}
//
//void Smove(states_set set, states_set& next_set, int ab_char, Skips_graph& NFA)
//{
//	// ��ÿ��״̬�����㾭��a�õ�����״̬
//	for (int i = 0; i < set.length; i++)
//	{
//		int nfa_i = set.st_set[i]; // ��nfa�е�״̬
//		if (NFA.Skips[nfa_i][ab_char] != MaxNum) // �д��ھ���ab_char����ת
//			add_s_to_set(next_set, NFA.Skips[nfa_i][ab_char]);
//	}
//}
//
////// ��״̬���ϼ�sets�� �� �Ƿ���ڵ�ǰ״̬��set
////int if_exist(states_set* sets, states_set set, int sets_len)
////{
////	// �ҳ�����ȵ��ٶԱ�
////	int T;
////	for (int i = 0; i < sets_len; i++)
////	{
////		if (sets[i].length == sets_len)
////		{
////			T = 1; // ������
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
////				// �Ҳ�����ͬԪ�أ���ǰsets�м�����set����ͬ
////				if (flag == false) { T = 0; break; }
////			}
////			if (T == 1) return i; // ״̬���ϼ�sets���Ѵ��ڵ�ǰ״̬��set
////		}
////	}
////	return -1; // ��sets�в�������set��ͬ��״̬��
////}
//
////�жϵ�ǰ״̬���Ƿ���ڣ��������򷵻���״̬�����������򷵻�0
//int if_exist(states_set* Sets, states_set Set, int sets_length) {
//	//�ҵ�������ȵĽ��бȶ�
//	int T; //�ж��Ƿ�����ͬ��״̬��
//	for (int i = 0; i < sets_length; i++) {
//		if (Sets[i].length == Set.length) {
//			T = 1; //�ȼ���ü�����
//			for (int j = 0; j < Set.length; j++)
//			{
//				int t = 0; //�ж��Ƿ��ҵ���ͬԪ��
//				for (int k = 0; k < Sets[i].length; k++)
//					if (Set.st_set[j] == Sets[i].st_set[k]) { t = 1; break; }
//				if (t == 0) { T = 0; break; } //�Ҳ������ü��ϲ���
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
//			//�Ե�ǰ״̬����ÿһ��Ԫ�ؽ���c����ת�����뵽next_set��
//			Smove(sets[i], next_set, c, NFA);
//			if (next_set.length == 0) continue; // ��ǰ״̬��������c
//			empty_closure(next_set, NFA.Skips_empty); // ��next_set�Ŀձհ��Ŀձհ�
//			// �жϸ�״̬���Ƿ��Ѿ����ڣ��������������Ϊ��״̬������DFA
//			int S = if_exist(sets, next_set, DFA.states_size);
//			if (!S) { // ������ ������sets��
//				S = DFA.states_size;
//				sets[S] = next_set;
//				DFA.states_size++; // ״̬����+1
//			}
//			// ��DFA����ӵ�ǰ״̬�����ַ�����ת 
//			DFA.Skips[i][c] = S; // ��¼����״̬���ı��
//		}
//	}
//}
//
//// �жϼ������Ƿ���ĳ��Ԫ��
//bool find_s(states_set set, int s)
//{
//	for (int i = 0; i < set.length; i++)
//		if (set.st_set[i] == s) return true;
//	return false;
//}
//
//// �Ӽ������㷨����DFA
//void Analysis::getDFA(Skips_graph& NFA, Skips_graph& DFA)
//{
//	// ��ʼ��DFA
//	default_graph(DFA, NFA.ab_char, NFA.ab_char_size);
//	states_set* sets = DFA.sets; // ״̬��
//
//	// ����ʼ״̬�Ŀձհ�
//	states_set begin_set;
//	add_s_to_set(begin_set, NFA.start_s);
//	empty_closure(begin_set, NFA.Skips_empty);
//	DFA.begin_set = begin_set; // �洢��ʼ��
//
//	// ��0����ʼ״̬����ΪDFA����ʼ״̬����
//	DFA.start_s = 0;
//	// ����ʼ״̬������״̬������״̬�Ķ��ձ���
//	sets[DFA.states_size] = begin_set;
//	DFA.states_size++;
//	createDFA(NFA, DFA, sets); // ����DFA
//
//	// �ҵ�DFA���е��յ�
//	int end_s = NFA.end_s;
//	for (int i = 0; i < DFA.states_size; i++)
//	{
//		// �����ǰ״̬��sets[i]�д����յ�Ԫ�� ���������յ㼯��
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
//		int not_find_num = DFA.end_set.length; // δ�ҵ���Ԫ��
//		for (int c = 0; c < DFA.ab_char_size; c++)
//		{
//			int T = MaxNum; // ��ʾ��δ�ҵ���һ����̬
//			for (int i = DFA.states_size - 1; i >= 0; i--)
//			{
//				// �ڽ��ܸ��ַ�����ת���ҵ���̬
//				if (find_s(DFA.end_set, DFA.Skips[i][c]))
//				{
//					if (T == MaxNum)
//					{
//						T = DFA.Skips[i][c];
//						add_s_to_set(new_end_sets, T);
//					}
//					// �õ�һ�����ֵ���̬ ��Ϊ�����յ���̬
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
//// ���ѣ����ѳɹ�����1�������ٷַ���0
//bool crack(Skips_graph& DFA, int* new_states, int& find_num)
//{
//	for (int i = 0; i < DFA.states_size; i++)
//	{
//		if (new_states[i] != MaxNum) continue;
//		for (int c = 0; c < DFA.ab_char_size; c++)
//		{
//			int T = 1; // �Ƿ��������
//			for (int j = 0; j < DFA.states_size; j++)
//			{
//				if (i == j) continue;
//				// ��ת�Ľ����һ������
//				if (new_states[DFA.Skips[i][c]] == new_states[DFA.Skips[j][c]])
//				{
//					T = 0; break;
//				}
//			}
//			// ���ٷ�
//			if (T == 1) { new_states[i] = i; find_num++; return 1; }
//		}
//	}
//	return 0; // �����ٷ�
//}
//
////�ж�����ǰn���Ƿ���ֹ���ǰ�ַ�
//bool find_int(int* array_, int n, int s) {
//	for (int i = 0; i < n; i++) {
//		if (array_[i] == s) return 1;
//	}
//	return 0;
//}
////���Ҿ�״̬Ϊs����״̬
//int find_s_in_array(int* array, int s, int length) {
//	for (int i = 0; i < length; i++) {
//		if (array[i] == s) return i;
//	}
//}
////����ϲ����dfa
//void combine(Skips_graph DFA, Skips_graph& minDFA, int* old_states) {
//	default_graph(minDFA, DFA.ab_char, DFA.ab_char_size);//��ʼ��
//	int new_states[MaxNum];
//	//��û�г��ֹ���״̬����
//	for (int i = 0; i < DFA.states_size; i++) {
//		if (find_int(old_states, i, old_states[i])) continue;//�ж�֮ǰ�Ƿ���ֹ�
//		for (int c = 0; c < DFA.ab_char_size; c++)
//			minDFA.Skips[minDFA.states_size][c] = DFA.Skips[i][c];  //���ƽ�ȥ
//		new_states[minDFA.states_size] = old_states[i];//������״̬
//		minDFA.states_size++;
//	}
//	//������״̬����
//	for (int i = 0; i < minDFA.states_size; i++) {
//		for (int c = 0; c < minDFA.ab_char_size; c++)
//			minDFA.Skips[i][c] = find_s_in_array(new_states, minDFA.Skips[i][c], minDFA.states_size);
//	}
//	//��¼���
//	minDFA.start_s = find_s_in_array(new_states, DFA.start_s, minDFA.states_size);
//	//�ս�״̬������
//	for (int i = 0; i < DFA.end_set.length; i++) {
//
//		add_s_to_set(minDFA.end_set, find_s_in_array(new_states, DFA.end_set.st_set[i], minDFA.states_size));
//	}
//	//    cout<<min_dfa.end_set.length;
//	//        for(int i=0;i<min_dfa.end_set.length;i++)
//	//            cout<<min_dfa.end_set.st_set[i]<<" ";
//}
//
//// ��С��DFA
//void Analysis::minimizeDFA(Skips_graph DFA, Skips_graph& minDFA)
//{
//	// ��״̬
//	int find_num = 0; // ȷ����״̬��״̬��
//	int new_states[MaxNum];
//	for (int i = 0; i < DFA.states_size; i++)
//		new_states[i] = MaxNum;
//	/* ��״̬�ֳ�������NFA��DFA��ÿ��״ֻ̬������һ�����ţ�
//	�ҵ�ÿ�����Ŷ�Ӧ���յ㼯�������Ϊ��ͬ��״̬ */
//	crack_end(DFA, new_states, find_num);
//	// ��ÿ�������ֵ�״̬���ֳ���
//	// ������δ����򻹿��Լ�������
//	while (find_num != DFA.states_size && crack(DFA, new_states, find_num)) { ; }
//	// ��ʣ���״̬����Ϊһ��״̬
//	if (find_num != DFA.states_size)
//	{
//		int T = MaxNum;
//		for (int i = 0; i < DFA.states_size; i++)
//		{
//			if (new_states[i] != MaxNum) continue;
//			if (T == MaxNum) T = i;
//		}
//	}
//	// ���¾�״̬��ͬ�� �޸ķ���
//	for (int i = 0; i < DFA.states_size; i++)
//	{
//		if (i == new_states[i]) continue;
//		for (int j = 0; j < DFA.states_size; j++) {
//			for (int c = 0; c < DFA.ab_char_size; c++) {
//				if (DFA.Skips[j][c] == i) DFA.Skips[j][c] = new_states[i];
//			}
//		}
//	}
//	DFA.start_s = new_states[DFA.start_s];//���ı������¼����
//	combine(DFA, minDFA, new_states);
//}
//
//
//void Analysis::analyse(string inputstr)
//{
//	// ������úõ�regex
//	string regex = this->getRegex(inputstr);
//	for (int i = 0; i < regex.size(); i++) cout << regex[i] << " ";
//	cout << endl;
//
//	// �����﷨��
//	this->synTree = this->Syntax_Tree(regex);
//	cout << "PostTraverse: ";
//	PostTravelse(synTree);
//	cout << endl << endl;
//
//	// Thompson�㷨����NFA
//	cout << "Thompson->NFA:" << endl;
//	this->getNFA(synTree, this->NFA, regex); // ����this->NFA��
//	this->display_Graph(NFA, "nfa");
//
//	// �Ӽ������㷨����DFA
//	cout << "�Ӽ������㷨����DFA:" << endl;
//	getDFA(this->NFA, this->DFA); // ����this->DFA��
//	display_Graph(this->DFA, "dfa");
//
//	// ��С��DFA
//	cout << "minDFA: " << endl;
//	minimizeDFA(DFA, minDFA); // ����this->minDFA��
//	display_Graph(minDFA, "dfa");
//}
//
//
