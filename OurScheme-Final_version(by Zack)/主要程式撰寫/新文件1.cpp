// 10527124
// ��T�T��
// �����q
// �{���y�� PL �Ĥ@���{���@�~ OurScheme


/*
	terminal (token) :
	  LEFT-PAREN       // '('
	  RIGHT-PAREN      // ')'
	  INT              // e.g., '123', '+123', '-123'
	  STRING           // "string's (example)." (strings do not extend across lines)
	  DOT              // '.'
	  FLOAT            // '123.567', '123.', '.567', '+123.4', '-.123'
	  NIL              // 'nil' or '#f', but not 'NIL' nor 'nIL'
	  T                // 't' or '#t', but not 'T' nor '#T'
	  QUOTE            // '
	  SYMBOL           // a consecutive sequence of printable characters that are
					   // not numbers, strings, #t or nil, and do not contain
					   // '(', ')', single-quote, double-quote, semi-colon and
					   // white-spaces ;
					   // Symbols are case-sensitive
					   // (i.e., uppercase and lowercase are different);
*/


# include <stdio.h>
# include <stdlib.h>
# include <string>
# include <vector>
# include <iostream>
# include <sstream>
# include<string.h>

using namespace std;

typedef enum { LEFT_PAREN = 1, RIGHT_PAREN = 2, INT = 3, STRING = 4, DOT = 5, FLOAT = 6, NIL = 7, T = 8, QUOTE = 9, SYMBOL = 10 } token_enum;

struct token {
	token_enum token_type;// �x�sgettoken����ƫ��O
	char data[5000];// �x�sgettoken�����e
};

vector<struct token> gToken;
token token_info;
int gNum = 0;
bool gIn_LP = false; // �P�_�O���O�bLP�̭��A�p�G�O���ܭn�L�k�A�� 
bool gHave_print_left = false; // �ݬݦ��S���L�L���A���F 

// **********************************Binary tree �ŧi**************************************
struct Tree_node {
	token tree_token;
	Tree_node* left;
	Tree_node* right;
};

// **********************************Binary tree �ŧi**************************************

// ���A�����U�@�ӬO�k�A�����ܭn�զX�b�@�_��@�@��ATOM 
bool Syntax_Check(Tree_node* ptr, int num) { // ���ˬd��k �S������ �A�ؾ� �M��~�L 
	Tree_node* exp = new Tree_node; // �إ߷s��node


	if (gToken.size() == 0) {
		printf("SyntaxError('unexpected EOF')"); // �����S����J���
		return false;
	} // if
	return true;

} // Syntax_Check()

Tree_node* initial(Tree_node* ptr) { // �Ω��_��l��node 
	ptr = new Tree_node;
	ptr->left = NULL;
	ptr->right = NULL;

	return ptr;
} // initial 

void PrintTree(Tree_node* ptr) {    // (exit)�nreturn  ���j�O�^��W�@�h�i�J���I 

	if (ptr->left != NULL && !gHave_print_left) {
		gHave_print_left = true;
		gIn_LP = true;
		printf("( "); // �����N�@�w�A�L�@�ӥ��A�� 
	} // if

	if (ptr->left != NULL) { // ���X
		PrintTree(ptr->left);
	} // if

	if (ptr->left != NULL && ptr->right != NULL) {
		PrintTree(ptr->right);
	} // if

	else if (ptr->left != NULL && ptr->right == NULL) {
		gIn_LP = false;
		if (gHave_print_left) printf(")\n"); // �����A���~�L�k�A��
	} // else if

	if (ptr->left == NULL && ptr->right == NULL) {
		if (gIn_LP && strcmp(ptr->tree_token.data, gToken[1].data) != 0 && strcmp(ptr->tree_token.data, "(") != 0) printf("  ");
		printf("%s \n", ptr->tree_token.data);
		return;
	} // else

} // PrintTree()

// �C�ӫ��г��|����address(�sptr��������m)�� value(ptr���V����m), funtion�Ƕi�Ӫ�ptr��̭�ptr��address���P�A��value�ۦP ,�C�����j�ѼƳ��n�ݦ��~���� 
// �ҥH�����p�A�bfunction�����m(value) �b��i���j�ѼƸ̴N�|�]����m�s���_�Ӧ�crach
// ���p�G�Q�ΰѼƶǶi�Ӫ�pointer ���k�Ω����� �����зs��node �|���ܸ̭�ptr��address ��i���j���ɭԷ|���ܰѼ�ptr�� value �M���ǥ� ���j��s�b�@�_ 
Tree_node* BuildTree(Tree_node* ptr, Tree_node* exp) { // �ؾ� �s���@���O�Ū�  ���j�O�^�W�@�h���i�J�I 

	//	���j�C return�@���N�|�^�h�@�h 
	//  exp��ptr��address���P��value�ۦP �N��L�̫��V�P�@�Ӧ�m 

	if (gToken.size() == 0) {
		printf("SyntaxError('unexpected EOF')"); // �����S����J���  
		return exp;
	} // if

	if (gNum == gToken.size()) { // �פ���� 
		return exp;
	} // if

	if (ptr->left == NULL && strcmp(gToken[gNum].data, "(") == 0) { // �p�G�O���A�� ����]���T�S�F�� token�N������s 	
		strcpy_s(ptr->tree_token.data, ""); // �]���O���A�����Φs�� 
		ptr->left = initial(ptr->left); // ��l�ƨëإ߷s��node 
		gNum = gNum + 1;

		if (strcmp(gToken[gNum].data, "(") != 0 && strcmp(gToken[gNum].data, ")") != 0) { // �o��else if���O�b�B�zdata���s���򰽬ݤU�@��token���X�۹������B�z 
			strcpy_s(ptr->left->tree_token.data, gToken[gNum].data); // �{�b�o��data���s�i�h 

			if (strcmp(gToken[gNum + 1].data, "(") == 0) { // �p�G�O���A���N�~�򩹥���  
				gIn_LP = true; // ���@�ӥH�W�����A�� 
			} // else if
			else if (gNum == gToken.size() - 1) {
				gNum = gNum + 1;
			} // else

		} // if
		else if (strcmp(gToken[gNum].data, "(") == 0) { // �S�L�����A�������A��
			strcpy_s(ptr->tree_token.data, ""); // �]���O���A�����Φs�� 
			ptr->left = initial(ptr->left); // ��l�ƨëإ߷s��node 
			BuildTree(ptr->left, exp); // ������
		} // else is


		BuildTree(ptr, exp); // ���j���`�I 
	} // if

	if (ptr->left != NULL && ptr->right == NULL) { // �o�̭n�B�z�s��data����Areturn�^�W�@�Ӥ��`�I�����p�A�B�{�b�n�B�z��data�������A�� �n�~�򩵦��s�� 
		ptr->right = initial(ptr->right); // ��l�ƨëإ߷s��node 
		gNum = gNum + 1;
		BuildTree(ptr->right, exp); // ���k��� �]��data�٬O�e�@�� �ҥH�n gNum+1 
	} // if

	if (ptr->left != NULL && ptr->right == NULL && strcmp(gToken[gNum].data, "(") == 0) { // �P�e�@�� if ���P�b�󥦳B�z�Ʀr�����A�� gNum����+1�O�]���o�ӥ��A���ٻݭn�B�z
		ptr->right = initial(ptr->right); // ��l�ƨëإ߷s��node 
		BuildTree(ptr->right, exp); // ���k��� �]��data�٬O�e�@�� �ҥH�n gNum+1   
	} // if

	if (strcmp(gToken[gNum].data, "(") != 0 && strcmp(gToken[gNum].data, ")") != 0) { // �o��else if���O�b�B�zdata���s���򰽬ݤU�@��token���X�۹������B�z 

		if (ptr->left == NULL) { // �P�_�s���e�n���n������		
			if (gNum != 0 && strcmp(gToken[gNum - 1].data, "(") != 0) { // �e�@�Ӧp�G�O���A���άO�w�g�����L�F �N���ΦA�������F 

				if (strcmp(gToken[gNum + 1].data, "(") != 0) { // �p�G���O�I�����A�� 
					ptr->left = initial(ptr->left);
					strcpy_s(ptr->left->tree_token.data, gToken[gNum].data); // �{�b�o��data���s�i�h 
				} // if
				else if (strcmp(gToken[gNum].data, ".") == 0 && strcmp(gToken[gNum + 1].data, "(") == 0) {
					gNum = gNum + 1; // ���L�I�򥪬A�� 
				} // else if
			} // if
			if (gIn_LP) { // �ĤG�ӥ��A���X�{ ���if���|Ĳ�o 
				gIn_LP = false;
				ptr->left = initial(ptr->left);
				gNum = gNum + 1;
				BuildTree(ptr->left, exp);
			} // if
		} // if	
		else if (gNum == 0) strcpy_s(ptr->tree_token.data, gToken[gNum].data); // �N��O���token�s�b �����A������  ���A����Ĥ@�ӭȤw�g�B�z�L�F 	

		if (strcmp(gToken[gNum + 1].data, ")") == 0 && gNum == gToken.size() - 2) { // �n�����F 
			gNum = gNum + 1;
			return exp;
		} // if	
		else if (gNum != gToken.size() - 1 && strcmp(gToken[gNum + 1].data, "(") != 0) { // �s�������ݤU�@��token ���O���A���N��return�^�W�@�h�s�� 
			BuildTree(ptr, exp); // ���j���`�I 
		} // else if
		else if (strcmp(gToken[gNum + 1].data, "(") == 0) { // �p�G�O���A���N�~�򩹥���      �|������!!!!!!! 
			gIn_LP = true; // ���@�ӥH�W�����A�� 
			return exp; // ���j��W�@�i�J�I 
		} // else if
		else if (gNum == gToken.size() - 1) {
			gNum = gNum + 1;
			BuildTree(ptr, exp); // �p�G�᭱�w�g�S�F��F�N���Ϋ��F gNum + 1 �O���Freturn 
		} // else

	} // if

	// �`�N!! �J��k�A���A���ӬO�n�@�����j��W�@���k�}�O�Ū����p  

	if (strcmp(gToken[gNum].data, ")") == 0) { // �J��k�A���A���ӬO�n�@�����j��W�@���k�}�O�Ū����p  
		if (gNum != gToken.size() - 1) {
			gNum = gNum + 1;
			BuildTree(ptr, exp); // �n�������٦��S��token�A�D�n�O�] gNum+1 
		} // if
		else return exp; // �S���F��F�ǳƤ���ؾ�  
	} // if

	return exp;

} // BuildTree()


// ********************************�B�zToken************************************************ 

void Welcome_and_read() {

	cout << "Welcome to OurScheme!\n" << "\n";
	string text = "";
	char ch = '0';
	char temp = '0';
	temp = scanf_s("%c", &ch, 1); // ��ƦrŪ��
	temp = scanf_s("%c", &ch, 1);
	if (temp == EOF) return;
} // Welcome_and_read()

void GetToken() { // GetToken �������ӳ�����A�n�L���ɭԥ�C�y�����k���� float��token�A��%3f����ƥh�L 

	char text[5000] = ""; // �ΨӼȦstoken 
	char ch[1] = ""; // �Ψ�Ū�� 
	char temp_ch[1] = ""; // ���UŪ�� 
	char temp = '0';
	temp = scanf_s("%c", &ch[0], 1); // �@��Ū�@�Ӧr�� 
	bool have_print = false; // ��W�ԥX�ӳB�z�� token ex.string , command 
	bool for_break = false; // �� PAL�Ϊ� break()
	bool is_float = false; // �ΨӧP�_�O���O float (���Y��DOT���ϥ�)
	bool only_dot = false;
	bool have_minus = false; // �P�_�Ʀr�}�Y���L '-' 

	while (temp != EOF) { // Ū�쵲��

		while (temp != EOF && ch[0] != '\n') { // Ū�촫�� \n

			// ********************************************************************
			if (ch[0] == '+' || ch[0] == '-') { // �J�� + �NŪ��
				if (ch[0] == '-') have_minus = true;
				else strcat_s(text,sizeof(text), ch);
				strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
				temp = scanf_s("%c", &ch[0], 1);

				if (ch[0] > 57 && ch[0] < 48 && ch[0] != '.') { // �p�G�᭱���O���Ʀr���I�}�Y�N��L�OSymbol 
					strcat_s(text,sizeof(text), ch);
					strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
					temp = scanf_s("%c", &ch[0], 1);
					while (ch[0] != EOF && ch[0] != '\n') {
						strcat_s(text,sizeof(text), ch);
						strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
						temp = scanf_s("%c", &ch[0], 1);
					} // while

					have_minus = false; // �e���N�⦳��o�ӲŸ��]���O�Ʀr�� 
					strcpy_s(token_info.data, text); // �s��token��T 
					token_info.token_type = SYMBOL;
					gToken.push_back(token_info);
					for_break = true;
				} // if
			} // if
			// ********************************************************************

			if (ch[0] == '.' && !for_break) { // �J���I �I�b�̫e���N���i��O�B�I�� ���O���ܳ����i��O�B�I�� (�]���|�t�~�B�z) 
				if (have_minus) strcat_s(text,sizeof(text), "-");
				strcat_s(text,sizeof(text), ".");

				if (temp_ch[0] == ' ') { // �N��o���I���i��O�Ʀr �]���i��u�Osymbol 
					strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
					temp = scanf_s("%c", &ch[0], 1);
					if (ch[0] == ' ') {
						only_dot = true; // �e�᳣�O�ť� �N�N���I��W�s�b 
						strcpy_s(token_info.data, "."); // �s��token��T 
						token_info.token_type = DOT;
						gToken.push_back(token_info);
						for_break = true;
						strcpy_s(text,sizeof(text), ""); // �M��text 
					} // if 
					else if (ch[0] < 58 && ch[0] > 47) { // �I�᭱�������Ʀr EX: .12abc or .123 
						strcat_s(text,sizeof(text), ch);
						strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
						temp = scanf_s("%c", &ch[0], 1);
						while (ch[0] != ' ' && ch[0] != '\n' && ch[0] > 47 && ch[0] < 58) {
							strcat_s(text,sizeof(text), ch);
							strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
							temp = scanf_s("%c", &ch[0], 1);
						} // while

						if (ch[0] == '.') {
							is_float = true;
							strcat_s(text,sizeof(text), ch);
							strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
							temp = scanf_s("%c", &ch[0], 1);
							while (ch[0] != ' ' && ch[0] != '\n' && ch[0] > 47 && ch[0] < 58) {
								strcat_s(text,sizeof(text), ch);
								strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
								temp = scanf_s("%c", &ch[0], 1);
							} // while

						} // if

						if (ch[0] != ' ' && ch[0] != '(' && ch[0] != ')' && ch[0] != '\n') { // �᭱���O�ťդ]���O���k�A�� �N��L���O�¼Ʀr �L�Osymbol EX: .12abc
							is_float = false;
							strcat_s(text,sizeof(text), ch);
							strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
							temp = scanf_s("%c", &ch[0], 1);
							while (ch[0] != ' ' && ch[0] != '\n') { // ��symbolŪ�� 
								strcat_s(text,sizeof(text), ch);
								strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
								temp = scanf_s("%c", &ch[0], 1);
							} // while
							strcpy_s(token_info.data, text); // �s��token��T 
							token_info.token_type = SYMBOL;
							gToken.push_back(token_info);
							have_print = true; // �w�g�s�L�F 
							strcpy_s(text,sizeof(text), ""); // �M��text 
						} // if
						else { 																		// EX: .123																
							strcpy_s(token_info.data, text); // �s��token��T 
							if (is_float) token_info.token_type = FLOAT; // �P�_�O�_�OFLOAT 
							else token_info.token_type = INT;
							gToken.push_back(token_info);
							have_print = true; // �w�g�s�L�F 
							strcpy_s(text,sizeof(text), ""); // �M��text 
						} // else
						cout << "> " << text << "\n" << "\n";
						for_break = true;
					} // else if
					else { //                                                              EX: .abc
						strcat_s(text,sizeof(text), ch);
						strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 															
						temp = scanf_s("%c", &ch[0], 1);
						while (ch[0] != ' ' && ch[0] != '\n') { // ��symbolŪ�� 
							strcat_s(text,sizeof(text), ch);
							strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
							temp = scanf_s("%c", &ch[0], 1);
						} // while
						strcpy_s(token_info.data, text); // �s��token��T 
						token_info.token_type = SYMBOL;
						gToken.push_back(token_info);
						have_print = true; // �w�g�s�L�F 
						for_break = true;
						strcpy_s(text,sizeof(text), ""); // �M��text 
					} // else
				} // if
				else if (strcmp(temp_ch, "") || temp_ch[0] == '+' || temp_ch[0] == '-') { // �N��o���I���i��O�Ʀr �]���i��u�Osymbol 

					strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
					temp = scanf_s("%c", &ch[0], 1);

					if (ch[0] < 58 && ch[0] > 47 && ch[0] != '\n') { // �I�᭱�������Ʀr 
						strcat_s(text,sizeof(text), ch);
						strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
						temp = scanf_s("%c", &ch[0], 1);
						while (ch[0] != ' ' && ch[0] != '\n' && ch[0] > 47 && ch[0] < 58) {
							strcat_s(text,sizeof(text), ch);
							strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
							temp = scanf_s("%c", &ch[0], 1);
						} // while

						if (ch[0] == '.') {
							is_float = true;
							strcat_s(text,sizeof(text), ch);
							strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
							temp = scanf_s("%c", &ch[0], 1);
							while (ch[0] != ' ' && ch[0] != '\n' && ch[0] > 47 && ch[0] < 58) {
								strcat_s(text,sizeof(text), ch);
								strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
								temp = scanf_s("%c", &ch[0], 1);
							} // while

						} // if

						if (ch[0] == ' ' || ch[0] == '\n') is_float = true; // �o�̥N�� .123 �o�ت� �]���I�b�e�����QŪ���F �ҥH�Ʀr����Ū���N�Ofloat 

						if (ch[0] != ' ' && ch[0] != '(' && ch[0] != ')' && ch[0] != '\n') { // �᭱���O�ťդ]���O���k�A�� �N��L���O�¼Ʀr �L�Osymbol 
							is_float = false;
							while (ch[0] != ' ' && ch[0] != '\n') { // ��symbolŪ�� 
								strcat_s(text,sizeof(text), ch);
								strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
								temp = scanf_s("%c", &ch[0], 1);
							} // while
							strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
							temp = scanf_s("%c", &ch[0], 1);
							strcpy_s(token_info.data, text); // �s��token��T 
							token_info.token_type = SYMBOL;
							gToken.push_back(token_info);
							have_print = true; // �w�g�s�L�F 
							for_break = true;
							strcpy_s(text,sizeof(text), ""); // �M��text 
						} // if
						else {
							strcpy_s(token_info.data, text); // �s��token��T 
							if (is_float) token_info.token_type = FLOAT; // �P�_�O�_�OFLOAT 
							else token_info.token_type = INT;
							gToken.push_back(token_info);
							have_print = true; // �w�g�s�L�F 
							strcpy_s(text,sizeof(text), ""); // �M��text 
						} // else
						for_break = true;
					} // if
					else { // EX: -.
						strcpy_s(token_info.data, text); // �s��token��T 
						token_info.token_type = SYMBOL; // �P�_�O�_�OFLOAT 
						gToken.push_back(token_info);
						have_print = true; // �w�g�s�L�F 
						strcpy_s(text,sizeof(text), ""); // �M��text 
						for_break = true;
					} // else
				} // else if
				else if (ch[0] != '\n') { // �N��o���I���i��O�Ʀr�̪� ���i��O symbol �]���i���W�s�b 
					if (have_minus) strcat_s(text,sizeof(text), "-"); // �Osymbol�i�O�e�����t���٨S�[ �ҥH�b�o�[
					strcat_s(text,sizeof(text), ch);
					strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
					temp = scanf_s("%c", &ch[0], 1);
					while (ch[0] != ' ' && ch[0] != '\n') {
						strcat_s(text,sizeof(text), ch);
						strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
						temp = scanf_s("%c", &ch[0], 1);
						if (ch[0] == '.') is_float = true;
					} // while
					strcpy_s(token_info.data, text); // �s��token��T 
					token_info.token_type = SYMBOL;
					gToken.push_back(token_info);
					have_print = true; // �w�g�s�L�F 
					for_break = true;
					strcpy_s(text,sizeof(text), ""); // �M��text 

				} // else

			} // if

			// ********************************************************************

			if (ch[0] == '(') {
				strcat_s(text,sizeof(text), ch);
				strcpy_s(token_info.data, ch); // �s��token��T 
				token_info.token_type = LEFT_PAREN;
				gToken.push_back(token_info);
				strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
				temp = scanf_s("%c", &ch[0], 1);
				strcpy_s(text,sizeof(text), ""); // �M��text 
				for_break = true;
				have_print = true;
			} // if

			if (ch[0] == ')') {
				strcat_s(text,sizeof(text), ch);
				strcpy_s(token_info.data, ch); // �s��token��T 
				token_info.token_type = RIGHT_PAREN;
				gToken.push_back(token_info);
				strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
				temp = scanf_s("%c", &ch[0], 1);
				strcpy_s(text,sizeof(text), ""); // �M��text 
				for_break = true;
				have_print = true;
			} // if

			// ********************************************************************

			if (ch[0] == ';' && !for_break) { // ��ܾ��O�� ; �}�Y command 
				while (ch[0] != '\n') {
					strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
					temp = scanf_s("%c", &ch[0], 1);
				} // while
				for_break = true;
				have_print = true; // ���F��泣���n�L
				strcpy_s(text,sizeof(text), ""); // �M��text 
			} // if

			// ********************************************************************

			if (ch[0] == 't' && !for_break) { // text == "" �O�]���u��t��W�~��i
				if (temp_ch[0] == ' ' || strcmp(temp_ch, "") || temp_ch[0] == '\n') {
					strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
					temp = scanf_s("%c", &ch[0], 1);
					if (ch[0] == '\n' || ch[0] == ' ') {
						strcpy_s(token_info.data, "#t"); // �s��token��T 
						token_info.token_type = T;
						gToken.push_back(token_info);
						have_print = true; // �w�g�s�L�F 
						for_break = true;
						strcpy_s(text,sizeof(text), ""); // �M��text 
					} // if
				} // if
			} // if

			// ********************************************************************
			if (ch[0] == '#' && !for_break) {
				strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
				temp = scanf_s("%c", &ch[0], 1);
				if (ch[0] == 't') {
					strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
					temp = scanf_s("%c", &ch[0], 1);
					if (ch[0] == '\n') {
						strcat_s(text,sizeof(text), "#t");
						strcpy_s(token_info.data, text); // �s��token��T 
						token_info.token_type = T;
						gToken.push_back(token_info);
						for_break = true;
						have_print = true; // �w�g�s�L�F 
						strcpy_s(text,sizeof(text), ""); // �M��text 
					} // if
					else {
						strcat_s(text,sizeof(text), "#t"); // ���M�O#t�}�Y ���L���O��ª�#t �ҥH�O��symbol�d��᭱�B�z 
					} // else
				} // if
				else if (ch[0] == 'f') { // EX: #f
					strcat_s(text,sizeof(text), "#");
					strcat_s(text,sizeof(text), ch); // �쥻�n����
					strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
					temp = scanf_s("%c", &ch[0], 1);
					if (ch[0] == '\n') { // �p�G�n���S���p �����令nil
						strcpy_s(text,sizeof(text), "nil");
						strcpy_s(token_info.data, text); // �s��token��T 
						token_info.token_type = NIL;
						gToken.push_back(token_info);
						for_break = true;
						have_print = true; // �w�g�s�L�F 
						strcpy_s(text,sizeof(text), ""); // �M��text 
					} // if
				} // else
				else strcat_s(text,sizeof(text), "#");
			} // if
			// ********************************************************************

			if (ch[0] > 47 && ch[0] < 58 && !for_break) { // �Ʀr�}�Y���B�z
				if (have_minus) strcat_s(text,sizeof(text), "-"); // �N��e������Ū��t�� 
				strcat_s(text,sizeof(text), ch);
				strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
				temp = scanf_s("%c", &ch[0], 1);
				while (ch[0] != '\n' && ch[0] != EOF && ch[0] != ' ' && ch[0] > 47 && ch[0] < 58) {
					strcat_s(text,sizeof(text), ch);
					strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
					temp = scanf_s("%c", &ch[0], 1);
				} // while
				if (ch[0] == '.') {
					is_float = true;
					strcat_s(text,sizeof(text), ch);
					strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
					temp = scanf_s("%c", &ch[0], 1);
					while (ch[0] != ' ' && ch[0] != '\n' && ch[0] > 47 && ch[0] < 58) {
						strcat_s(text,sizeof(text), ch);
						strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
						temp = scanf_s("%c", &ch[0], 1);
					} // while

				} // if
				if (ch[0] != ' ' && ch[0] != '(' && ch[0] != ')' && ch[0] != '\n') { // �᭱���O�ť� �N��L���O�¼Ʀr 
					strcat_s(text,sizeof(text), ch);
					strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
					temp = scanf_s("%c", &ch[0], 1);
					while (ch[0] != ' ' && ch[0] != '\n') { // ��symbolŪ�� 
						strcat_s(text,sizeof(text), ch);
						strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
						temp = scanf_s("%c", &ch[0], 1);
					} // while
					strcat_s(text,sizeof(text), ch);
					strcpy_s(token_info.data, text); // �s��token��T 
					token_info.token_type = SYMBOL;
					gToken.push_back(token_info);
					have_print = true; // �w�g�s�L�F 
					strcpy_s(text,sizeof(text), ""); // �M��text 
				} // if
				else {
					strcpy_s(token_info.data, text); // �s��token��T 
					if (is_float) token_info.token_type = FLOAT;
					else token_info.token_type = INT;
					gToken.push_back(token_info);
					have_print = true; // �w�g�s�L�F 
					strcpy_s(text,sizeof(text), ""); // �M��text 
				} // else
				for_break = true;
			} // if 

			// ********************************************************************

			if (temp_ch[0] == '\"' && ch[0] == ' ' && !for_break) { // ���F�h���̫�[�F�ܦh�ťժ����p
				int num = 1;
				strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
				temp = scanf_s("%c", &ch[0], 1);
				while (ch[0] == ' ') {
					strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
					temp = scanf_s("%c", &ch[0], 1);
					num++;
				} // while
				if (ch[0] != '\n') {
					while (num != 0) {
						strcat_s(text,sizeof(text), " ");
						num--;
					} // while
				} // if
			} // if

			// ********************************************************************

			if (ch[0] == '\'' && !for_break) { // �J��'\'' ===> QUOTE
				strcat_s(text,sizeof(text), ch);
				strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
				temp = scanf_s("%c", &ch[0], 1);
				while (ch[0] != EOF && ch[0] != '\n' && ch[0] != ' ' && ch[0] != '\'') {
					strcat_s(text,sizeof(text), ch);
					strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
					temp = scanf_s("%c", &ch[0], 1);
				} // while
				strcat_s(text,sizeof(text), ch);
				strcpy_s(token_info.data, text); // �s��token��T 
				token_info.token_type = QUOTE;
				gToken.push_back(token_info);
				have_print = true;
				for_break = true;

			} // if
			// ********************************************************************

			if (ch[0] == '\"' && !for_break) { // string�ݭn�t�~�B�z 
				strcat_s(text,sizeof(text), ch);
				strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
				temp = scanf_s("%c", &ch[0], 1);
				bool in_Quote = false;
				bool have_more_backslash = false;
				while (ch[0] != EOF && ch[0] != '\n' && ch[0] != '\"') { // "OurScheme allows the use of '\\n', '\\t' and  '\\\"' in a string." �o�̦����D 

					if (ch[0] == '\'') in_Quote = true;

					if (in_Quote && ch[0] == '\\') { //   OurScheme��string��C/Java��printf()��escape�������A���u����'\n', '\"', '\t', '\n'�P'\\'�o����case�C ��޸��n���O���n�� 

						strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
						temp = scanf_s("%c", &ch[0], 1);
						while (ch[0] != '\'' && ch[0] != EOF && ch[0] != '\n') { // Quote��W�ϥγo�Ӱj��B�z�� 	

							if (ch[0] == 'n' || ch[0] == '\\' || ch[0] == 't' || ch[0] == '\"') { // Ū��o�Ǩ��ӲĤ@��'/'���n���� 
								if (temp_ch[0] == '\\') {
									strcat_s(text,sizeof(text), ch);
								} // if

								if (ch[0] == '\\') { // �o���if�]�t�̭����F�賣�u�O���F�B�z�ܦh��\�����p 
									strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
									temp = scanf_s("%c", &ch[0], 1);
									have_more_backslash = true;
									if (ch[0] == '\\') {
										while (ch[0] == '\\') {
											strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
											temp = scanf_s("%c", &ch[0], 1);
										} // while
									} //if
								} // if

							} // if
							else if (ch[0] != '\'') { // �S�S�O�N�q��\�N������J 					
								strcat_s(text,sizeof(text), temp_ch);
								strcat_s(text,sizeof(text), ch);
							} // else if
							strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
							if (!have_more_backslash && ch[0] != '\n') temp = scanf_s("%c", &ch[0], 1); // �]����u�O���F���n�A�hŪ�@�� 
							have_more_backslash = false;
						} // while
						strcat_s(text,sizeof(text), ch); // �s�̫�@�ӳ�޸� 
						temp = scanf_s("%c", &ch[0], 1);
						in_Quote = false;
					} // if
					else if (ch[0] == '\\') { // ���O�b��޸��̪�\�B�z "Please enter YES\NO below this line >\n" 
						strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
						temp = scanf_s("%c", &ch[0], 1);
						if (ch[0] == 'n' || ch[0] == '\\' || ch[0] == 't' || ch[0] == '\"') { // Ū��o�Ǩ��ӲĤ@��'/'���n���� 
							if (temp_ch[0] == '\\') {
								if (ch[0] == 'n') strcat_s(text,sizeof(text), "\n"); // ����r���n�o�˦s ���M�L�X�Ӥ��|���� 
								else strcat_s(text,sizeof(text), ch);
							} // if

							if (ch[0] == '\\') { // �o���if�]�t�̭����F�賣�u�O���F�B�z�ܦh��\�����p 
								strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
								temp = scanf_s("%c", &ch[0], 1);
								have_more_backslash = true;
								if (ch[0] == '\\') {
									while (ch[0] == '\\') {
										strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
										temp = scanf_s("%c", &ch[0], 1);
									} // while
								} //if
							} // if

						} // if
						else if (ch[0] != '\'') { // �S�S�O�N�q��\�N������J 					
							strcat_s(text,sizeof(text), temp_ch);
						} // else if
						strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
						if (!have_more_backslash && ch[0] != '\n') temp = scanf_s("%c", &ch[0], 1); // �]����u�O���F���n�A�hŪ�@�� 
						have_more_backslash = false;
					} // else if
					else {
						strcat_s(text,sizeof(text), ch);
						strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
						temp = scanf_s("%c", &ch[0], 1);
					} // else
				} // while
				strcat_s(text,sizeof(text), ch); // �s�̫�@�����޸� 				
				strcpy_s(token_info.data, text); // �s��token��T 
				token_info.token_type = STRING;
				gToken.push_back(token_info);
				have_print = true; // �w�g�B�z�L�F�᭱���ΦA�L�@��
				for_break = true;
				strcpy_s(text,sizeof(text), "");
				temp = scanf_s("%c", &ch[0], 1);

			} // if

			// ********************************************************************

			if (!for_break && ch[0] != ' ') { // �o��O�b�B�z �r�� �� �S��Ÿ��}�Y�� SYMBOL

				strcat_s(text,sizeof(text), ch);
				strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
				temp = scanf_s("%c", &ch[0], 1);
				bool is_nil = false;

				if (temp_ch[0] == 'n' && ch[0] == 'i' && strcmp(text, "")) { // for nil
					strcat_s(text,sizeof(text), ch);
					strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
					temp = scanf_s("%c", &ch[0], 1);
					if (ch[0] == 'l') { // �o�ӬOL���O1 ���n�h�� 
						strcat_s(text,sizeof(text), ch);
						strcpy_s(token_info.data, text); // �s��token��T 
						token_info.token_type = NIL;
						gToken.push_back(token_info);
						is_nil = true;
					} // if
				} // if
				while (ch[0] != EOF && ch[0] != '\n' && ch[0] != ' ' && ch[0] != ';') {
					strcat_s(text,sizeof(text), ch);
					strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
					temp = scanf_s("%c", &ch[0], 1);
				} // while

				if (!is_nil) { // ���Onil�~�Osymbol 
					strcpy_s(token_info.data, text); // �s��token��T 
					token_info.token_type = SYMBOL;
					gToken.push_back(token_info);
				} // if

			} // if

			// ********************************************************************

			if (ch[0] == ' ') {
				strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
				temp = temp = scanf_s("%c", &ch[0], 1);
			} //if 
			have_minus = false;
			for_break = false; // �]���@���N�i�H�Ѱ�for_break���A 
		} // while

		// *************************************************�B�zparse tree ************************************* 
		Tree_node* root = new Tree_node;
		root->left = NULL;
		root->right = NULL;
		strcpy_s(root->tree_token.data, gToken[0].data); // ��l�� tree , �é�J�Ĥ@�� data 
		if (ch[0] == '\n' || ch[0] == EOF) {
			root = BuildTree(root, root);
			gNum = 0;
			PrintTree(root);
		} // if
		// *************************************************�B�zparse tree ************************************* 
		int k = 0;
		while (k <= gToken.size() - 1) {
			cout << gToken.size() << "         " << gToken[k].data << "\n" << gToken[k].token_type << "\n";
			k++;
		} //while
		strcpy_s(text,sizeof(text), "");
		if (ch[0] == EOF || ch[0] == '\n') gToken.clear();
		have_print = false;
		is_float = false;
		only_dot = false;
		if (ch[0] == ' ' || ch[0] == '\n') {
			strcpy_s(temp_ch,sizeof(temp_ch), ch); // �x�s�W�@�� ch 
			temp = temp = scanf_s("%c", &ch[0], 1);
		} // if
		else temp = ch[0];

	} // while
	if (temp == EOF) {
		cout << "> " << "ERROR (no more input) : END-OF-FILE encountered" << "\n";
		return;
	} // if
}

int main() {
	Welcome_and_read();
	GetToken();

} // main()



