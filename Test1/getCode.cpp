//#include<iostream>
//#include<vector>
//#include"RegExp.h"
//using namespace std;


//// ��������
//string getTabs(int n)
//{
//    string texts = "";
//    for (int i = 0; i < n; i++)
//    {
//        texts += "    ";
//    }
//    return texts;
//}

//// ��ȡ����״̬�ַ�����ʽ
//string getEndStates_string(IntSet endStates)
//{
//    string str = "";
//    for(int state : endStates)
//    {
//        str += to_string(state) + ",";
//    }
//    str.pop_back();
//    return str;
//}



//// �ڲ�switch
//void getSwitchCharCodes(Skips_graph minDfa, vector<string>& lines, int& tab, int curState)
//{
//    lines.push_back(getTabs(tab) + "switch (ch)");
//    lines.push_back(getTabs(tab) + "{");
//    int ab_char_Count = 0;
//    // ����ab_char�����ַ�����
//    while (ab_char_Count < minDfa.ab_char_size)
//    {
//        char ab_char = minDfa.ab_char[ab_char_Count]; // ȡ���ַ�
//        int next = minDfa.Skips[curState][ab_char_Count]; // ȡ����ת״̬
//        if (next == MaxNum) {
//            continue;
//        }
//        lines.push_back(getTabs(tab) + "case '" + ab_char + "':");
//        tab++;
//        lines.push_back(getTabs(tab) + "state = " + to_string(next) + ";");
//        lines.push_back(getTabs(tab) + "break;");
//        tab--;
//        ab_char_Count++;
//    }
//    // defaultͳһ��������շ�
//    lines.push_back(getTabs(tab) + "default:");
//    lines.push_back(getTabs(tab+1) + "error = true;");
//    lines.push_back(getTabs(tab) + "}// end ch_switch");
//}

//// ���switch
//void getSwitchStateCodes(Skips_graph minDfa, vector<string>& lines, int& tab)
//{
//    int start_state = minDfa.start_s; // ��ʼ״̬���
//    int end_state = minDfa.end_set.st_set[0]; // ����״̬���
//    lines.push_back(getTabs(tab) + "switch (state)");
//    lines.push_back(getTabs(tab) + "{");
//    int stateCount = 0;
//    while (stateCount < minDfa.states_size-1)
//    {
//        lines.push_back(getTabs(tab) + "case " + to_string(stateCount) + ":");
//        tab++;
//        lines.push_back(getTabs(tab) + "ch = getchar();");
//        // ����ڲ�switch
//        getSwitchCharCodes(minDfa, lines, tab, stateCount);
//        lines.push_back(getTabs(tab) + "break;\n");
//        tab--;
//        stateCount++;
//    }
//    lines.push_back(getTabs(tab) + "}// end state_switch");
//}


//// whileѭ��
//void getWhileCodes(DFA minDfa, vector<string>& lines, int& tab)
//{
//    int start_state = minDfa.startState; // ��ʼ״̬���
//    IntSet end_state = minDfa.endStates; // ����״̬���
//    lines.push_back(getTabs(tab) + "while ( state != " + to_string(end_state) + ")");
//    lines.push_back(getTabs(tab) + "{");
//    tab++;
//    getSwitchStateCodes(minDfa, lines, tab);
//    lines.push_back(getTabs(tab) + "if (error == true) break;");
//    tab--;
//    lines.push_back(getTabs(tab) + "}");
//}




//// ������
//string AnalyseClass::getCode(DFA minDfa)
//{
//    string codes = "";
//    vector<string> lines;
//    int start_state = minDfa.startState; // ��ʼ״̬���
//    IntSet end_state = minDfa.endStates; // ����״̬��ż�
//    int tab = 0;
	
//    // ����ǰ׺
//    lines.push_back("#include<iostream>");
//    lines.push_back("using namespace std;\n");
//    lines.push_back("int main()");
//    lines.push_back("{");
//    // main����ǰ����
//    tab++;
//    lines.push_back(getTabs(tab) + "\/\/ total state count: " + to_string(minDfaStateNum));
//    lines.push_back(getTabs(tab) + "\/\/ start_state: "
//        + to_string(start_state) + "   accept_state: " + getEndStates_string(end_state)+"\n");

//    lines.push_back(getTabs(tab) + "int state = ;" + to_string(start_state));
//    lines.push_back(getTabs(tab) + "char ch;");
//    lines.push_back(getTabs(tab) + "bool error = false;");

//    // whileѭ������
//    getWhileCodes(minDfa, lines, tab);
	
//    // ����main�����󲿷�
//    // �ж� + ��� д�ļ�
//    lines.push_back(getTabs(tab) + "if (state == " + to_string(end_state) + ")");
//    lines.push_back(getTabs(tab+1) + "cout << \"accept\" << endl;");
//    lines.push_back(getTabs(tab) + "else");
//    lines.push_back(getTabs(tab+1) + "cout << \"failed\" << endl;");
//    lines.push_back("}");

//    for (string line : lines)
//    {
//        codes += line;
//        codes += '\n';
//        cout << line << endl;
//    }
//    return codes;
//}


