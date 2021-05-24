// 10527124
// ��T�T��
// �����q
// �{���y�� PL �Ĥ@���{���@�~ OurScheme


/*
	�{�b�n�諸�O��gettoken�^��token��type�A�ثe�O�Q���vector�̭�  
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
# include <cstdlib>
# include <string>
# include <vector>
# include <iostream>
# include <sstream>
# include<string.h>
 
using namespace std ;

typedef enum {INT, FLOAT, SYMBOL, LEFT_PAREN, RIGHT_PAREN, NIL, STRING, DOT, T, QUOTE} token_enum;

struct token {
	token_enum token_type ;// �x�sgettoken����ƫ��O
	char data[50] ;// �x�sgettoken�����e
}  ;

vector<struct token> gToken;
token token_info ;
int gNum = 0 ;
bool gIn_LP = false ; // �P�_�O���O�bLP�̭��A�p�G�O���ܭn���� 

// **********************************Binary tree �ŧi**************************************
struct Tree_node {
	token tree_token ; 
	Tree_node *left;
	Tree_node *right;
} ;


// **********************************Binary tree �ŧi**************************************

/*
void Atom() {
	
	
} // Atom


void read_form_token() {
	char console[100] = "" ;
	bool Last_ch_is_LP = false ;
	int count = 0 ;

	if ( gToken.size() == 0 ) {
		printf("SyntaxError('unexpected EOF')") ; // �����S����J���
		return ; 
	} // if
	
    if ( gToken[0].data == "(" ){
        while (gToken[0].data != ")") {
        	strcat(console, gToken[0].data );
        	// read_from_tokens(gToken[0].data);
		}

        return console;
	}
	
    else if ( gToken[0].data == ")") {
    	printf("SyntaxError('unexpected')") ;
	}    
    else {
    	return Atom(token) ;
	}
}

void Parse () {
	read_form_token() ;
} // Parse
*/
// ********************************�B�zToken************************************************ 

void PrintTree( Tree_node *ptr ) {
	
	printf("EE") ;
	
	if(ptr->left != NULL) { // ���X
		printf("%s ", ptr->tree_token.data);
		PrintTree(ptr->left);
	} // if
	else if(ptr->right != NULL) { // ���X
		printf("%s ", ptr->tree_token.data);
		PrintTree(ptr->right);
	} // else if
	else return ;
	
	printf("HH") ;

} // PrintTree()


void BuildTree( Tree_node *ptr, int num ) { // �ؾ� �����ŦXSyntax ��X 

	//	���j�C return�@���N�|�^�h�@�h 

	Tree_node *exp = new Tree_node ; // �إ߷s��node
	if ( exp == NULL) {	
		exp = ptr ; // �i�H�� exp�Q�� root ���V��ڡA�M��Optr�b�] 
	} // if
	
	if ( gToken.size() == 0 ) {
		printf("SyntaxError('unexpected EOF')") ; // �����S����J���
		return ; 
	} // if
	
	if ( num == gToken.size()) { //�פ���� 
		//if ( exp != NULL ) {
		//	PrintTree( exp ) ;
		//	exp = NULL ;
		//} // if
		return ;
	} // if

	if ( ptr->right == NULL && gToken[num].data != "(" && gToken[num].data != ")" ) {
		strcpy(ptr ->tree_token.data, gToken[num].data); // �s�ȶidata�A�`�N�r��n��strcpy
		if ( gToken[num+1].data != "(" && gToken[num+1].data != ")" ){ // ()�n�t�~�B�z�A���ઽ�����k 
			BuildTree(ptr->right, num + 1);
		} // if
	} // if
		
	// �`�N!! �J��k�A���A���ӬO�n�@�����j��W�@���k�}�O�Ū����p
	 
	if ( ptr->left == NULL && gToken[num].data == "(" ) {
		// �p�G�O���A�� token������s 
		BuildTree(ptr->left, num + 1);
	} // if
	
	if ( gToken[num].data == ")" ) {
		// �p�G�O�k�A�� token���k��s 
		if ( ptr->right != NULL ) {
			return ;
		} // if
		else BuildTree(ptr->right, num + 1);
	} // if

	return ;
} // BuildTree()

void Welcome_and_read() {

	cout << "Welcome to OurScheme!\n" << "\n";
	string text = "";
	char ch ;
	char temp ;
	temp = scanf( "%c", &ch ); // ��ƦrŪ��
	temp = scanf( "%c", &ch );
	if ( temp == EOF ) return ;
} // Welcome_and_read()

void GetToken () { // GetToken �������ӳ�����A�n�L���ɭԥ�C�y�����k���� float��token�A��%3f����ƥh�L 

	char text[1000] = "" ; // �ΨӼȦstoken 
	char ch[1] ; // �Ψ�Ū�� 
	char temp_ch[1] ; // ���UŪ�� 
	scanf( "%c", &ch ); // �@��Ū�@�Ӧr�� 
	char temp ;
	int count_decimal = 0 ; // �ΨӳB�z float�᭱���p�� 
	int count_decimal2 = 3 ;
	int num = 0 ;
	bool have_print = false ; // ��W�ԥX�ӳB�z�� token ex.string , command 
	bool for_break = false ; // �� PAL�Ϊ� break()
	bool is_float = true ; // �ΨӧP�_�O���O float (���Y��DOT���ϥ�)
	bool only_dot = false ; 
	string space = "";

	while ( temp != EOF ) { // Ū�쵲��

		while ( temp != EOF && ch[0] != '\n' ) { // Ū�촫�� \n
			// ********************************************************************
			if ( ch[0] == '+' ) { // �J�� + �NŪ��
				scanf( "%c", &ch );
				if ( ch[0] == '-' ) { // �p�G�O+-�}�Y��������Ū�����U�@��
					strcat( text , "+-" ) ;
					scanf( "%c", &ch );
					while ( ch[0] != EOF && ch[0] != '\n' ) {
						strcat( text , ch ) ;
						scanf( "%c", &ch );
					} // while

					for_break = true ;
				} // if
			} // if
			// ********************************************************************
			if ( ch[0] == '.' && !for_break ) { // �J���I
				scanf( "%c", &ch );
				if ( ch[0] < 48 || ch[0] > 57 ) {
					if ( ch[0] != '+' && ch[0] != '-' && ch[0] != '.' && ch[0] != '\n' ) {
						is_float = false ;
						only_dot = true ;
					} // if
				} // if

				if ( ch[0] == '\n' ) { // �I�b�Ʀr�̫᭱
					if ( is_float && !only_dot ) strcat( text , ".000" ) ;
					else strcat( text , "." ) ;
					for_break = true ;
				} // if
				else if ( ch[0] == ' ' ) {
					scanf( "%c", &ch );
					strcat( text , ". " ) ;
					while ( ch[0] != EOF && ch[0] != '\n' ) {
						strcat( text , ch ) ;
						scanf( "%c", &ch );
					} // while

					for_break = true ;
				} // else if
				else if ( text == "" || text == "-" ) { // �I�b�Ʀr�̫e���A�B�I��̤֦��@�ӼƦr
					strcat( text , "0." ) ;
					while ( ch[0] != '\n' ) { // �ϥ��o���while�N�O�b��p�Ʀ�
						if ( count_decimal < 2 ) strcat( text , ch ) ; // �p�ƫ�e���@�w�L
						if ( count_decimal == 2 ) { // �ĤT����s�_�ӵ��ݧP�_�n���n�[�@
							temp_ch[0] = ch[0] ;
						} // if

						if ( count_decimal == 3 ) { // �ĥ|��H�᪺�B�z
							if ( ch[0] >= 53 ) {
								temp_ch[0] = temp_ch[0] + 1 ;
								strcat( text , temp_ch ) ; // �o�̦b�B�z���O4��5�J 
							} // if
							else if ( ch[0] <= 52 ) strcat( text , temp_ch ) ;
						} // if

						scanf( "%c", &ch );
						count_decimal ++ ;
					} // while

					if ( ch[0] == '\n' && count_decimal == 3 ) strcat( text , temp_ch ) ; // �ɨ��p�ƫ��n���T�쪺�B�I��
					while ( count_decimal2 > count_decimal && is_float ) { // ������ƴN��0
						strcat( text , "0" ) ;
						count_decimal2 -- ;
					} // while

					count_decimal = 0 ;
					count_decimal2 = 3 ;
					num = 0 ;
					for_break = true ;
				} // else if
				else { // �I�b����
					strcat( text , "." );
					while ( ch[0] != '\n' ) { // �ϥ��o���while�N�O�b��p�Ʀ�
						if ( count_decimal < 2 ) strcat( text , ch ) ; // �p�ƫ�e���@�w�L
						if ( count_decimal == 2 ) { // �ĤT����s�_�ӵ��ݧP�_�n���n�[�@
							temp_ch[0] = ch[0] ;
						} // if

						if ( count_decimal == 3 ) { // �ĥ|��H�᪺�B�z
							if ( ch[0] >= 53 ) {
								temp_ch[0] = temp_ch[0] + 1 ;
								strcat( text , temp_ch ) ; // �o�̦A�B�z���O4��5�J 
							} // if
							else if ( ch[0] <= 52 ) strcat( text , temp_ch ) ;
						} // if

						scanf( "%c", &ch );
						count_decimal ++ ;
					} // while

					if ( ch[0] == '\n' && count_decimal == 3 ) strcat( text , temp_ch ) ; // �ɨ��p�ƫ��n���T�쪺�B�I��
					while ( count_decimal2 > count_decimal && is_float ) { // ������ƴN��0
						strcat( text , "0" ) ;
						count_decimal2 -- ;
					} // while

					count_decimal = 0 ;
					count_decimal2 = 3 ;
					num = 0 ;
					for_break = true ;
				} // else
			} // if

			// ********************************************************************
			
			if ( ch[0] == '(' && !for_break ) {
				strcat( text , ch ) ;
				strcpy( token_info.data, ch ) ; // �s��token��T 
				token_info.token_type = LEFT_PAREN ;
				gToken.push_back(token_info) ;
				scanf( "%c", &ch );
			} // if
			
			if ( ch[0] == ')' && !for_break ) {
				strcat( text , ch ) ;
				strcpy( token_info.data, ch ) ; // �s��token��T 
				token_info.token_type = RIGHT_PAREN ;
				gToken.push_back(token_info) ;
				scanf( "%c", &ch );
			} // if

			// ********************************************************************

			if ( ch[0] == ';' && !for_break ) {
				if ( text == "" ) { // ��ܾ��O�� ; �}�Y command 
					while ( ch[0] != '\n' ) {
						scanf( "%c", &ch );
					} // while
					for_break = true ;
					have_print = true ; // ���F��泣���n�L
				} // if
			} // if

			// ********************************************************************

			if ( ch[0] == 't' && text == "" && !for_break ) { // text == "" �O�]���u��t��W�~��i
				scanf( "%c", &ch );
				if ( ch[0] == '\n' ) {
					strcpy( text , "#t" ) ; // �u���Ostrcpy�S���� 
					for_break = true ;
				} // if
				else {
					strcat( text , "t" ) ;
				} // else
			} // if

			// ********************************************************************
			if ( ch[0] == '#' && !for_break ) {
				scanf( "%c", &ch );
				if ( ch[0] == 't' ) {
					scanf( "%c", &ch );
					if ( ch[0] == '\n' ) {
						strcat( text , "#t" ) ;
						for_break = true ;
					} // if
					else {
						while( ch[0] != '\n' ) { // Ū�촫��Ū���b �᭱�~���|��
							scanf( "%c", &ch );
						} // while
						strcat( text , "#tt" ) ;
						for_break = true ;
					} // else
				} // if
				else {
					strcat( text , "#" ) ;
					strcat( text , ch ) ; // �쥻�n����
					scanf( "%c", &ch );
					if ( ch[0] == '\n' ) { // �p�G�n���S���p �����令nil
						strcat( text , "nil" ) ;
						for_break = true ;
					} // if
				} // else
			} // if
			// ********************************************************************
			
			if ( ch[0] > 47 && ch[0] < 58 ) { // �Ʀr�}�Y���B�z
				strcpy(text, "") ;
				strcat( text , ch ) ; 
				scanf( "%c", &ch );
				while (ch[0] != ' ' && ch[0] != '\n' && ch[0] > 47 && ch[0] < 58 || ch[0] == '.' ){
					strcat( text , ch ) ;
					scanf( "%c", &ch );
					if( ch[0] == '.' ) is_float = true;
				} // while
				
				if ( ch[0] != ' ' && ch[0] != '(' && ch[0] != ')' ) { // �᭱���O�ť� �N��L���O�¼Ʀr 
					strcat( text , ch ) ;
					scanf( "%c", &ch );
					strcpy( token_info.data, text ) ; // �s��token��T 
					token_info.token_type = SYMBOL ;
					gToken.push_back(token_info) ;
					have_print = true ; // �w�g�s�L�F 
				} // if
				else {
					strcpy( token_info.data, text ) ; // �s��token��T 
					if ( is_float ) token_info.token_type = FLOAT ;
					else token_info.token_type = INT ;
					gToken.push_back(token_info) ;
					have_print = true ; // �w�g�s�L�F 
				} // else
				cout << "> " << text << "\n" << "\n" ;
				for_break = true ;
			} // if
			
			// ********************************************************************

			if ( !for_break ) strcat( text , ch ) ;
			if ( ch[0] < 48 || ch[0] > 57 ) { // �n�ˬd2�M�~���� �D�Ʀr 
				if ( ch[0] != '+' && ch[0] != '-' && ch[0] != '.' && ch[0] != '\n' ) {
					is_float = false ;
					only_dot = true ;
				} // if
			} // if
			temp_ch[0] = ch[0] ; // �Ȧs�e�@��ch[0]
			if ( !for_break ) scanf( "%c", &ch );
			if ( ch[0] < 48 || ch[0] > 57 ) { // �n�ˬd2�M�~����
				if ( ch[0] != '+' && ch[0] != '-' && ch[0] != '.' && ch[0] != '\n' ) {
					is_float = false ;
					only_dot = true ;
				} // if
			} // if

			bool is_string = false ;
			// ********************************************************************
			if ( temp_ch[0] == '\"' &&  ch[0] == ' ' ) { // ���F�h���̫�[�F�ܦh�ťժ����p
				int num = 1 ;
				scanf( "%c", &ch );
				while ( ch[0] == ' ' ) {
					scanf( "%c", &ch );
					num ++ ;
				} // while
				if ( ch[0] != '\n' ) {
					while ( num != 0 ) {
						strcat( text , " " ) ;
						num -- ;
					} // while
				} // if
			} // if

			// ********************************************************************
			if ( ch[0] == '\'' && !for_break ) { // �J��'\'' ===> QUOTE
				is_string = true ;
				strcat( text , ch ) ;
				scanf( "%c", &ch );
			} // if
			// ********************************************************************
			if ( ch[0] == '\"' ) { // string�ݭn�t�~�B�z 
				strcat( text , ch ) ;
				scanf( "%c", &ch );
				while ( ch[0] != EOF && ch[0] != '\n' && ch[0] != '\"' ) {
					if ( ch[0] == '\\' && !for_break ) { // Ū��\���D�O����r�����M���L����˱�@�� 
						scanf( "%c", &ch );
						if ( ch[0] == 'n' ) { // Ū��\n�����O����r��
							cout << "> " << text << "\n" ; // �L�X����r���H�e����
							strcat( text , "" ) ;
							scanf( "%c", &ch );
							while ( ch[0] != EOF && ch[0] != '\n' && ch[0] != ' ' ) {
								strcat( text , ch ) ;
								scanf( "%c", &ch );
							} // while
							strcpy( token_info.data, text ) ; // �s��token��T 
							token_info.token_type = STRING ;
							gToken.push_back(token_info) ;
							cout << text << "\n" << "\n"; // �L�X����᪺��
							have_print = true ; // �w�g�B�z�L�F�᭱���ΦA�L�@��
						} // if
						else if ( ch[0] != '\\' && ch[0] != '"' ) strcat( text , "\\" ) ; // �P�_�u���@��\�����p
						else if ( ch[0] == '\\' ) {  // �P�_���\�H�W�����p
							while ( ch[0] == '\\' && ch[0] != ' ' ) {
								scanf( "%c", &ch );
							} // while
							strcat( text , "\\" );
						}
					} // if
					else {
						strcat( text , ch ) ;
						scanf( "%c", &ch );
					}
				} // while
				
		   } // if
		   
		} // while
		if ( text == "(exit)" ) {
			cout << "> " << "\n" ;
			return ;
		} // if
		if ( !have_print ) { // �W���S���s�i�h �ҥH�b�o�s 
			strcpy( token_info.data, text ) ; // �s��token��T 
			token_info.token_type = STRING ;
			gToken.push_back(token_info) ;
			cout << "> " << text << "\n" << "\n" ;
		} // if 
		
		// *************************************************�B�zparse tree ************************************* 
		Tree_node *root = new Tree_node;
		root ->left= NULL;
		root ->right= NULL;
		strcpy(root ->tree_token.data, gToken[0].data ); // ��l�� tree , �é�J�Ĥ@�� data 
		// BuildTree(root, 0);
		// *************************************************�B�zparse tree ************************************* 
		int k = 0 ;
		while ( k < gToken.size()) {
			cout << gToken.size() << "JJ" << gToken[k].data << "\n" ;
			k++ ;
		} //while
		gToken.clear() ;
		have_print = false ;
		for_break = false ;
		is_float = true ;
		only_dot = false ;
		strcpy( text , "" ) ;
		temp = scanf( "%c", &ch );

	} // while
	if ( temp == EOF ) {
		cout << "> " << "ERROR (no more input) : END-OF-FILE encountered" << "\n" ;
		return ;
	} // if
}

int main() {
	Welcome_and_read() ;
	GetToken();
	
	if( gToken.size() == 0 ) {
		printf("you are without input") ;
		return 0 ;
	}
	
} // main()



