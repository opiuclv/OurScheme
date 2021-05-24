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

typedef enum {LEFT_PAREN, RIGHT_PAREN, INT, STRING, DOT, FLOAT, NIL, T, QUOTE, SYMBOL} token_enum;

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


void BuildTree( Tree_node *ptr, int num ) { // �ؾ� �����ŦXSyntax ��X    (exit)�nreturn 

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


// ********************************�B�zToken************************************************ 

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
	bool have_print = false ; // ��W�ԥX�ӳB�z�� token ex.string , command 
	bool for_break = false ; // �� PAL�Ϊ� break()
	bool is_float = true ; // �ΨӧP�_�O���O float (���Y��DOT���ϥ�)
	bool only_dot = false ; 
	bool have_minus = false ; // �P�_�Ʀr�}�Y���L '-' 
	string space = "";

	while ( temp != EOF ) { // Ū�쵲��

		while ( temp != EOF && ch[0] != '\n' ) { // Ū�촫�� \n

			// ********************************************************************
			if ( ch[0] == '+' || ch[0] == '-' ) { // �J�� + �NŪ��
				if ( ch[0] == '-') have_minus == true ;
				strcat( text , ch ) ;
				strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
				scanf( "%c", &ch );
				if ( ch[0] > 57 && ch[0] < 48  && ch[0] != '.' ) { // �p�G�᭱���O���Ʀr���I�}�Y�N��L�OSymbol 
					strcat( text , ch ) ;
					strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch );
					while ( ch[0] != EOF && ch[0] != '\n' ) {
						strcat( text , ch ) ;
						strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
						scanf( "%c", &ch );
					} // while
					
					have_minus = false ; // �e���N�⦳��o�ӲŸ��]���O�Ʀr�� 
					strcpy( token_info.data, text ) ; // �s��token��T 
					token_info.token_type = SYMBOL ;
					gToken.push_back(token_info) ;
					for_break = true ;
				} // if
			} // if
			// ********************************************************************
			if ( ch[0] == '.' && !for_break ) { // �J���I �I�b�̫e���N���i��O�B�I�� ���O���ܳ����i��O�B�I�� (�]���|�t�~�B�z) 
				if (have_minus) strcat( text, "-" ) ;
				strcat( text, "." ) ;
				
				if ( temp_ch[0] == ' ' ) { // �N��o���I���i��O�Ʀr �]���i��u�Osymbol 
					strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch );
					if (ch[0] == ' ') {
						only_dot = true ; // �e�᳣�O�ť� �N�N���I��W�s�b 
						strcpy( token_info.data, "." ) ; // �s��token��T 
						token_info.token_type = DOT ;
						gToken.push_back(token_info) ;
						for_break = true ;
						strcpy( text , "" ) ; // �M��text 
					} // if 
					else if ( ch[0] < 58 && ch[0] > 47 ) { // �I�᭱�������Ʀr EX: .12abc or .123 
						strcat( text , ch ) ;
						strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
						scanf( "%c", &ch );
						while (ch[0] != ' ' && ch[0] != '\n' && ch[0] > 47 && ch[0] < 58 || ch[0] == '.' ){
							strcat( text , ch ) ;
							strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
							scanf( "%c", &ch );
							if( ch[0] == '.' ) is_float = true;
						} // while
						
						if ( ch[0] != ' ' && ch[0] != '(' && ch[0] != ')' && ch[0] != '\n' ) { // �᭱���O�ťդ]���O���k�A�� �N��L���O�¼Ʀr �L�Osymbol EX: .12abc
							strcat( text , ch ) ;
							strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
							scanf( "%c", &ch );
							while (ch[0] != ' ' && ch[0] != '\n'){ // ��symbolŪ�� 
								strcat( text , ch ) ;
								strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
								scanf( "%c", &ch );
							} // while
							strcpy( token_info.data, text ) ; // �s��token��T 
							token_info.token_type = SYMBOL ;
							gToken.push_back(token_info) ;
							have_print = true ; // �w�g�s�L�F 
							strcpy( text , "" ) ; // �M��text 
						} // if
						else { 																		// EX: .123																
							strcpy( token_info.data, text ) ; // �s��token��T 
							if ( is_float ) token_info.token_type = FLOAT ; // �P�_�O�_�OFLOAT 
							else token_info.token_type = INT ;
							gToken.push_back(token_info) ;
							have_print = true ; // �w�g�s�L�F 
							strcpy( text , "" ) ; // �M��text 
						} // else
						cout << "> " << text << "\n" << "\n" ;
						for_break = true ;
					} // else if
					else { //                                                              EX: .abc
						strcat( text , ch ) ;	
						strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 															
						scanf( "%c", &ch );
						while (ch[0] != ' ' && ch[0] != '\n'){ // ��symbolŪ�� 
							strcat( text , ch ) ;
							strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
							scanf( "%c", &ch );
						} // while
						strcpy( token_info.data, text ) ; // �s��token��T 
						token_info.token_type = SYMBOL ;
						gToken.push_back(token_info) ;
						have_print = true ; // �w�g�s�L�F 
						for_break = true ;
						strcpy( text , "" ) ; // �M��text 
					} // else
				} // if
				else if (temp_ch == "" || temp_ch[0] == '+' || temp_ch[0] == '-'){ // �N��o���I���i��O�Ʀr �]���i��u�Osymbol 
				
					strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch );
					
					if ( ch[0] < 58 && ch[0] > 47 ) { // �I�᭱�������Ʀr 
						strcat( text , ch ) ;
						strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
						scanf( "%c", &ch );
						while (ch[0] != ' ' && ch[0] != '\n' && ch[0] > 47 && ch[0] < 58 || ch[0] == '.' ){
							strcat( text , ch ) ;
							strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
							scanf( "%c", &ch );
							if( ch[0] == '.' ) is_float = true;
						} // while
						
						if ( ch[0] != ' ' && ch[0] != '(' && ch[0] != ')' && ch[0] != '\n' ) { // �᭱���O�ťդ]���O���k�A�� �N��L���O�¼Ʀr �L�Osymbol 
							while (ch[0] != ' ' && ch[0] != '\n'){ // ��symbolŪ�� 
								strcat( text , ch ) ;
								strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
								scanf( "%c", &ch );
							} // while
							strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
							scanf( "%c", &ch );
							strcpy( token_info.data, text ) ; // �s��token��T 
							token_info.token_type = SYMBOL ;
							gToken.push_back(token_info) ;
							have_print = true ; // �w�g�s�L�F 
							for_break = true ;
							strcpy( text , "" ) ; // �M��text 
						} // if
						else {
							strcpy( token_info.data, text ) ; // �s��token��T 
							if ( is_float ) token_info.token_type = FLOAT ; // �P�_�O�_�OFLOAT 
							else token_info.token_type = INT ;
							gToken.push_back(token_info) ;
							have_print = true ; // �w�g�s�L�F 
							strcpy( text , "" ) ; // �M��text 
						} // else
						for_break = true ;
					} // if
				} // else if
				else { // �N��o���I���i��O�Ʀr�̪� ���i��O symbol �]���i���W�s�b 
					strcat( text , ch ) ;
					strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch );
					while (ch[0] != ' ' && ch[0] != '\n' ){
						strcat( text , ch ) ;
						strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
						scanf( "%c", &ch );
						if( ch[0] == '.' ) is_float = true;
					} // while
					strcpy( token_info.data, text ) ; // �s��token��T 
					token_info.token_type = SYMBOL ;
					gToken.push_back(token_info) ;
					have_print = true ; // �w�g�s�L�F 
					for_break = true ;
					strcpy( text , "" ) ; // �M��text 
					
				} // else

			} // if

			// ********************************************************************
			
			if ( ch[0] == '(' ) {
				strcat( text , ch ) ;
				strcpy( token_info.data, ch ) ; // �s��token��T 
				token_info.token_type = LEFT_PAREN ;
				gToken.push_back(token_info) ;
				strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
				scanf( "%c", &ch );
				strcpy( text , "" ) ; // �M��text 
				for_break = true ;
				have_print = true ;
			} // if
			
			if ( ch[0] == ')') {
				strcat( text , ch ) ;
				strcpy( token_info.data, ch ) ; // �s��token��T 
				token_info.token_type = RIGHT_PAREN ;
				gToken.push_back(token_info) ;
				strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
				scanf( "%c", &ch );
				strcpy( text , "" ) ; // �M��text 
				for_break = true ;
				have_print = true ;
			} // if

			// ********************************************************************

			if ( ch[0] == ';' && !for_break ) {
				if ( text == "" ) { // ��ܾ��O�� ; �}�Y command 
					while ( ch[0] != '\n' ) {
						strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
						scanf( "%c", &ch );
					} // while
					for_break = true ;
					have_print = true ; // ���F��泣���n�L
					strcpy( text , "" ) ; // �M��text 
				} // if
			} // if

			// ********************************************************************

			if ( ch[0] == 't' && !for_break ) { // text == "" �O�]���u��t��W�~��i
			    if ( temp_ch[0] == ' ' || temp_ch == "" || temp_ch[0] == '\n' ){
				    strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch );
					if ( ch[0] == '\n' || ch[0] == ' ' ) {
						strcpy( token_info.data, "#t" ) ; // �s��token��T 
						token_info.token_type = T ;
						gToken.push_back(token_info) ;
						have_print = true ; // �w�g�s�L�F 
						for_break = true ;
						strcpy( text , "" ) ; // �M��text 
					} // if
				} // if
			} // if

			// ********************************************************************
			if ( ch[0] == '#' && !for_break ) {
				strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
				scanf( "%c", &ch );
				if ( ch[0] == 't' ) {
					strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch );
					if ( ch[0] == '\n' ) {
						strcat( text , "#t" ) ;
						strcpy( token_info.data, text ) ; // �s��token��T 
						token_info.token_type = T ;
						gToken.push_back(token_info) ;
						for_break = true ;
						have_print = true ; // �w�g�s�L�F 
						strcpy( text , "" ) ; // �M��text 
					} // if
					else {
						while( ch[0] != '\n' ) { // Ū�촫��Ū���b �᭱�~���|��
						    strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
							scanf( "%c", &ch );
						} // while
						strcat( text , "#tt" ) ;
						strcpy( token_info.data, text ) ; // �s��token��T 
						token_info.token_type = T ;
						gToken.push_back(token_info) ;
						for_break = true ;
						have_print = true ; // �w�g�s�L�F 
						strcpy( text , "" ) ; // �M��text 
					} // else
				} // if
				else {
					strcat( text , "#" ) ;
					strcat( text , ch ) ; // �쥻�n����
					strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch );
					if ( ch[0] == '\n' ) { // �p�G�n���S���p �����令nil
						strcat( text , "nil" ) ;
						strcpy( token_info.data, text ) ; // �s��token��T 
						token_info.token_type = NIL ;
						gToken.push_back(token_info) ;
						for_break = true ;
						have_print = true ; // �w�g�s�L�F 
						strcpy( text , "" ) ; // �M��text 
					} // if
				} // else
			} // if
			// ********************************************************************

			if ( ch[0] > 47 && ch[0] < 58 && !for_break ) { // �Ʀr�}�Y���B�z
				if ( have_minus ) strcat( text , "-" ) ; // �N��e������Ū��t�� 
				strcat( text , ch ) ; 
				strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
				scanf( "%c", &ch );
				while ( ch[0] != '\n' && ch[0] != EOF && ch[0] != ' ' && ch[0] > 47 && ch[0] < 58 || ch[0] == '.' ){
					strcat( text , ch ) ;
					strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch );
					if( ch[0] == '.' ) is_float = true;
				} // while
				
				if ( ch[0] != ' ' && ch[0] != '(' && ch[0] != ')' && ch[0] != '\n' ) { // �᭱���O�ť� �N��L���O�¼Ʀr 
					strcat( text , ch ) ;
					strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch );
					while (ch[0] != ' ' && ch[0] != '\n'){ // ��symbolŪ�� 
						strcat( text , ch ) ;
						strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
						scanf( "%c", &ch );
					} // while
					strcat( text , ch ) ;
					strcpy( token_info.data, text ) ; // �s��token��T 
					token_info.token_type = SYMBOL ;
					gToken.push_back(token_info) ;
					have_print = true ; // �w�g�s�L�F 
					strcpy( text , "" ) ; // �M��text 
				} // if
				else {
					strcpy( token_info.data, text ) ; // �s��token��T 
					if ( is_float ) token_info.token_type = FLOAT ;
					else token_info.token_type = INT ;
					gToken.push_back(token_info) ;
					have_print = true ; // �w�g�s�L�F 
					strcpy( text , "" ) ; // �M��text 
				} // else
				for_break = true ;
			} // if 
			
			// ********************************************************************

			if ( temp_ch[0] == '\"' &&  ch[0] == ' ' && !for_break ) { // ���F�h���̫�[�F�ܦh�ťժ����p
				int num = 1 ;
				strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
				scanf( "%c", &ch );
				while ( ch[0] == ' ' ) {
					strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
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
				strcat( text , ch ) ;
				strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
				scanf( "%c", &ch );
				while ( ch[0] != EOF && ch[0] != '\n' && ch[0] != ' ' ) {
					strcat( text , ch ) ;
					strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch );
				} // while
				strcpy( token_info.data, text ) ; // �s��token��T 
				token_info.token_type = QUOTE ;
				gToken.push_back(token_info) ;
				
			} // if
			// ********************************************************************

			if ( ch[0] == '\"' && !for_break ) { // string�ݭn�t�~�B�z 
				strcat( text , ch ) ;
				strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
				scanf( "%c", &ch );
				while ( ch[0] != EOF && ch[0] != '\n' && ch[0] != '\"' ) {
					
					if ( ch[0] == '\\' && !for_break ) {

					    strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
						scanf( "%c", &ch );
						if ( ch[0] == 'n' ) { // Ū��\n�����O����r��
							strcat( text , "\n" ) ;
							strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
							scanf( "%c", &ch );
							while ( ch[0] != EOF && ch[0] != '\n' && ch[0] != ' ' ) {
								strcat( text , ch ) ;
								strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
								scanf( "%c", &ch );
							} // while
							strcpy( token_info.data, text ) ; // �s��token��T 
							token_info.token_type = STRING ;
							gToken.push_back(token_info) ;
							have_print = true ; // �w�g�B�z�L�F�᭱���ΦA�L�@��
						} // if
						else if ( ch[0] != '\\' && ch[0] != '\"' ) strcat( text , "\\" ) ; // �P�_�u���@��\�����p
						else if ( ch[0] == '\\' ) {  // �P�_���\�H�W�����p
							while ( ch[0] == '\\' ) {
								strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
								scanf( "%c", &ch );
							} // while
							strcat( text , "\\" );
						}
					} // if
					else {
						strcat( text , ch ) ;
						strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
						scanf( "%c", &ch );
					}
				} // while
				
		   } // if
		   if ( !for_break && ch[0] != ' ' ) { // �o��O�b�B�z �r�� �� �S��Ÿ��}�Y�� SYMBOL
		   		strcat( text , ch ) ;
		   		strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
		   		scanf( "%c", &ch );
		   		while ( ch[0] != EOF && ch[0] != '\n' && ch[0] != ' ' && ch[0] != ';' ) {
					strcat( text , ch ) ;
					strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch );
				} // while
				
				strcpy( token_info.data, text ) ; // �s��token��T 
				token_info.token_type = SYMBOL ;
				gToken.push_back(token_info) ;
		   	
		   } // if
		   
		   if( ch[0] == ' ' || ch[0] != '\n' ) {
		   	    strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
		        temp = scanf( "%c", &ch );
		   } //if 
		   have_minus = false ;
		   for_break = false ; // �]���@���N�i�H�Ѱ�for_break���A 
		} // while
		
		// *************************************************�B�zparse tree ************************************* 
		Tree_node *root = new Tree_node;
		root ->left= NULL;
		root ->right= NULL;
		strcpy(root ->tree_token.data, gToken[0].data ); // ��l�� tree , �é�J�Ĥ@�� data 
		// if ( ch[0] == '\n' || ch[0] == EOF )BuildTree(root, 0);
		// *************************************************�B�zparse tree ************************************* 
		int k = 0 ;
		while ( k < gToken.size()) {
			cout << gToken.size() << "JJ" << gToken[k].data << "\n" << gToken[k].token_type << "\n" ;
			k++ ;
		} //while
		strcpy( text, "") ;
		if( ch[0] == EOF || ch[0] == '\n') gToken.clear() ;
		have_print = false ;
		is_float = false ;
		only_dot = false ;
		if( ch[0] != ' ' || ch[0] == '\n' ) {
			strcpy( temp_ch , ch ) ; // �x�s�W�@�� ch 
		    temp = scanf( "%c", &ch );
		} // if
		else temp = ch[0] ;

	} // while
	if ( temp == EOF ) {
		cout << "> " << "ERROR (no more input) : END-OF-FILE encountered" << "\n" ;
		return ;
	} // if
}

int main() {
	Welcome_and_read() ;
	GetToken();
	
} // main()



