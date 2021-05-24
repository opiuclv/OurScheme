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


# include <cstdio>
# include <cstdlib>
# include <string>
# include <vector>
# include <iostream>
# include <sstream>
# include <cstring>
 
using namespace std ;

typedef enum {
	LEFT_PAREN = 1,
	RIGHT_PAREN = 2, 
	INT = 3, 
	STRING = 4, 
	DOT = 5, 
	FLOAT = 6, 
	NIL = 7, 
	T = 8, 
	QUOTE = 9, 
	SYMBOL = 10
} Token_enum
;

struct Token {
	Token_enum token_type ;// �x�sgettoken����ƫ��O
	char data[5000] ;// �x�sgettoken�����e
};

vector<Token> gToken;
Token gToken_info ;
int gNum = 0 ;
bool gIn_LP = false ; // �P�_�O���O�bLP�̭��A�p�G�O���ܭn���� 
bool gHave_print_left = false ; // �ݬݦ��S���L�L���A���F 

// **********************************Binary tree �ŧi**************************************
struct Tree_node {
	Token tree_token;
	Tree_node *left;
	Tree_node *right;
};

// **********************************Binary tree �ŧi**************************************

// ���A�����U�@�ӬO�k�A�����ܭn�զX�b�@�_��@�@��ATOM 
bool Syntax_Check( Tree_node *ptr, int num ) { // ���ˬd��k �S������ �A�ؾ� �M��~�L 
    Tree_node *exp = new Tree_node ; // �إ߷s��node
	
    if ( gToken.size() == 0 ) {
        printf( "SyntaxError('unexpected EOF')" ) ; // �����S����J���
        return false ; 
    } // if
    return true ;
    
} // Syntax_Check()

Tree_node * Initial( Tree_node *ptr ){ // �Ω��_��l��node 
    ptr = new Tree_node ;
    ptr->left = NULL ;
	ptr->right = NULL ;
	
	return ptr ;
} // Initial() 

void PrintTree( Tree_node *ptr ) {    // (exit)�nreturn  ���j�O�^��W�@�h�i�J���I 

	if ( ptr->left != NULL && !gHave_print_left ) {
		gHave_print_left = true ;
		gIn_LP = true ;
		printf( "( " ); // �����N�@�w�A�L�@�ӥ��A�� 
	} // if

	if ( ptr->left != NULL ) { // ���X
		PrintTree( ptr->left );
	} // if
	
	if ( ptr->left != NULL && ptr->right != NULL) { 
		PrintTree( ptr->right );
	} // if
	
	else if ( ptr->left != NULL && ptr->right == NULL ) { 
		gIn_LP = false ;
		if ( gHave_print_left ) printf( ")\n" ); // �����A���~�L�k�A��
	} // else if
	
	if ( ptr->left == NULL && ptr->right == NULL ) {
		if ( gIn_LP && strcmp( ptr->tree_token.data, gToken[1].data ) != 0 && strcmp( ptr->tree_token.data, "(" ) != 0 ) printf( "  " ) ;

		if (ptr->tree_token.token_type == FLOAT  ) {
			printf( "> %.3f\n\n", atof( ptr->tree_token.data ) );
		} // if
		else printf( "> %s\n\n", ptr->tree_token.data );
		return ;
	} // if
	
} // PrintTree()

// �C�ӫ��г��|����address(�sptr��������m)�� value(ptr���V����m)
// , funtion�Ƕi�Ӫ�ptr��̭�ptr��address���P�A��value�ۦP ,�C�����j�ѼƳ��n�ݦ��~���� 
// �ҥH�����p�A�bfunction�����m(value) �b��i���j�ѼƸ̴N�|�]����m�s���_�Ӧ�crach
// ���p�G�Q�ΰѼƶǶi�Ӫ�pointer ���k�Ω����� �����зs��node �|���ܸ̭�ptr��address 
//��i���j���ɭԷ|���ܰѼ�ptr�� value �M���ǥ� ���j��s�b�@�_ 
Tree_node * BuildTree( Tree_node *ptr, Tree_node *exp  ) { // �ؾ� �s���@���O�Ū� ���j�O�^�W�@�h���i�J�I 

	// ���j�C return�@���N�|�^�h�@�h 
	// exp��ptr��address���P��value�ۦP �N��L�̫��V�P�@�Ӧ�m 
	
	if ( gToken.size() == 0 ) {
		printf("SyntaxError('unexpected EOF')") ; // �����S����J���  
		return exp ; 
	} // if

	if ( gNum == gToken.size() ) { // �פ���� 
		return exp;
	} // if
	
	if ( ptr->left == NULL && strcmp( gToken[gNum].data,"(") == 0 ) { // �p�G�O���A�� ����]���T�S�F�� token�N������s 	
		strcpy(ptr ->tree_token.data, ""); // �]���O���A�����Φs�� 
		ptr->tree_token.token_type = gToken[gNum].token_type ;
		ptr->left = Initial( ptr->left ) ; // ��l�ƨëإ߷s��node 
		gNum = gNum + 1 ;	
		
		if ( strcmp(gToken[gNum].data, "(") != 0 && strcmp( gToken[gNum].data, ")" ) != 0 ) { 
			// �o��else if���O�b�B�zdata���s���򰽬ݤU�@��token���X�۹������B�z 
			strcpy(ptr->left->tree_token.data, gToken[gNum].data); // �{�b�o��data���s�i�h 
			ptr->left->tree_token.token_type = gToken[gNum].token_type ; // �{�b�o��data���s�i�h 
			
			if ( strcmp(gToken[gNum+1].data, "(") == 0 ) { // �p�G�O���A���N�~�򩹥���  
				gIn_LP = true ; // ���@�ӥH�W�����A�� 
			} // else if
			else if ( gNum == gToken.size() - 1 ) {
				gNum = gNum + 1 ;
			} // else
			else if ( gNum == gToken.size() - 2 && strcmp( gToken[gNum+1].data, ")" ) == 0 ) { // EX. ( 12 ) 
				gNum = gNum + 1 ;
				return exp ;
			} // else if
		
		} // if
		else if ( strcmp( gToken[gNum].data, "(") == 0 ) { // �S�L�����A�������A��
			strcpy(ptr ->tree_token.data, ""); // �]���O���A�����Φs�� 
			ptr->tree_token.token_type = gToken[gNum].token_type ;
			ptr->left = Initial( ptr->left ) ; // ��l�ƨëإ߷s��node 
			BuildTree(ptr->left, exp); // ������
		} // else if
		
				
		BuildTree(ptr, exp); // ���j���`�I 
	} // if
	
	// �o�̭n�B�z�s��data����Areturn�^�W�@�Ӥ��`�I�����p�A�B�{�b�n�B�z��data�������A�� �n�~�򩵦��s�� 
	if ( ptr->left != NULL && ptr->right == NULL ) { 
		ptr->right = Initial( ptr->right ) ; // ��l�ƨëإ߷s��node 
		gNum = gNum + 1 ;
		BuildTree(ptr->right, exp); // ���k��� �]��data�٬O�e�@�� �ҥH�n gNum+1 
	} // if
	
	// �P�e�@�� if ���P�b�󥦳B�z�Ʀr�����A�� gNum����+1�O�]���o�ӥ��A���ٻݭn�B�z
	if ( ptr->left != NULL && ptr->right == NULL && strcmp(gToken[gNum].data, "(") == 0 ) { 
		ptr->right = Initial( ptr->right ) ; // ��l�ƨëإ߷s��node 
		BuildTree(ptr->right, exp); // ���k��� �]��data�٬O�e�@�� �ҥH�n gNum+1   
	} // if

	// �o��else if���O�b�B�zdata���s���򰽬ݤU�@��token���X�۹������B�z 
	if ( strcmp(gToken[gNum].data, "(") != 0 && strcmp(gToken[gNum].data, ")") != 0 ) { 
		
		if ( ptr->left == NULL ) { // �P�_�s���e�n���n������		
			if ( gNum != 0 && strcmp(gToken[gNum-1].data, "(") != 0 ) { // �e�@�Ӧp�G�O���A���άO�w�g�����L�F �N���ΦA�������F 			
				if ( strcmp(gToken[gNum+1].data, "(") != 0 ) { // �p�G���O�I�����A�� 
					ptr->left = Initial( ptr->left ) ;
					strcpy(ptr->left->tree_token.data, gToken[gNum].data); // �{�b�o��data���s�i�h 
					ptr->left->tree_token.token_type = gToken[gNum].token_type ;
				} // if
				else if ( strcmp(gToken[gNum].data, ".") == 0 && strcmp(gToken[gNum+1].data, "(") == 0 ) {
					gNum = gNum + 1 ; // ���L�I�򥪬A�� 
				} // else if
			} // if
			if ( gIn_LP ) { // �ĤG�ӥ��A���X�{ ���if���|Ĳ�o 
				gIn_LP = false ;
				ptr->left = Initial( ptr->left ) ;
				gNum = gNum + 1 ;
				BuildTree(ptr->left, exp);
			} // if
		} // if	
		// �N��O���token�s�b �����A������  ���A����Ĥ@�ӭȤw�g�B�z�L�F 
		if ( gNum == 0 ) {
			strcpy(ptr->tree_token.data, gToken[gNum].data); 
			ptr->tree_token.token_type = gToken[gNum].token_type ;
		} // else if
		if ( strcmp(gToken[gNum+1].data, ")") == 0 && gNum == gToken.size() - 2 ) { // �n�����F 
			gNum = gNum + 1 ;
			return exp ;
		} // if	
		// �s�������ݤU�@��token ���O���A���N��return�^�W�@�h�s�� 
		else if ( gNum != gToken.size() - 1 && strcmp(gToken[gNum+1].data, "(") != 0 ) { 
			BuildTree(ptr, exp); // ���j���`�I 
		} // else if
		else if ( strcmp(gToken[gNum+1].data, "(") == 0 ) { // �p�G�O���A���N�~�򩹥���      �|������!!!!!!! 
			gIn_LP = true ; // ���@�ӥH�W�����A�� 
			return exp ; // ���j��W�@�i�J�I 
		} // else if
		else if ( gNum == gToken.size() - 1 ) {
			gNum = gNum + 1 ;
			BuildTree(ptr, exp); // �p�G�᭱�w�g�S�F��F�N���Ϋ��F gNum + 1 �O���Freturn 
		} // else if
		
	} // if
	
	// �`�N!! �J��k�A���A���ӬO�n�@�����j��W�@���k�}�O�Ū����p  
	
	if ( strcmp(gToken[gNum].data, ")") == 0 ) { // �J��k�A���A���ӬO�n�@�����j��W�@���k�}�O�Ū����p  
		if( gNum != gToken.size() - 1 ) {
			gNum = gNum + 1 ;
			BuildTree(ptr, exp); // �n�������٦��S��token�A�D�n�O�] gNum+1 
		} // if
		else return exp ; // �S���F��F�ǳƤ���ؾ�  
	} // if

	return exp ;
	
} // BuildTree()


// ********************************�B�zToken************************************************ 

void Welcome_and_read() {

	cout << "Welcome to OurScheme!\n" << "\n";
	string text = "";
	char ch ;
	char temp ;
	temp = scanf( "%c", &ch ); // ��ƦrŪ��
	temp = scanf( "%c", &ch ); // Ū������ 
	if ( temp == EOF ) return ;
} // Welcome_and_read()

void GetToken () { // GetToken �������ӳ�����A�n�L���ɭԥ�C�y�����k���� float��token�A��%3f����ƥh�L 

	char text[5000] = "" ; // �ΨӼȦstoken 
	char ch[] = "0" ; // �Ψ�Ū�� 
	char temp_ch[] = "0" ; // ���UŪ�� 
	scanf( "%c", &ch[0] ); // �@��Ū�@�Ӧr�� 
	char temp ;
	bool have_print = false ; // ��W�ԥX�ӳB�z�� token ex.string , command 
	bool for_break = false ; // �� PAL�Ϊ� break()
	bool is_float = false ; // �ΨӧP�_�O���O float (���Y��DOT���ϥ�)
	bool only_dot = false ; 
	bool have_minus = false ; // �P�_�Ʀr�}�Y���L '-' 

	while ( temp != EOF ) { // Ū�쵲��

		while ( temp != EOF && ch[0] != '\n' ) { // Ū�촫�� \n

			// ********************************************************************
			if ( ch[0] == '+' || ch[0] == '-' ) { // �J�� + �NŪ��
				if ( ch[0] == '-' ) have_minus = true ;
				else strcat( text , ch ) ;
				strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
				scanf( "%c", &ch[0] );

				if ( ch[0] > 57 && ch[0] < 48  && ch[0] != '.' ) { // �p�G�᭱���O���Ʀr���I�}�Y�N��L�OSymbol 
					strcat( text, ch ) ;
					strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch[0] );
					while ( ch[0] != EOF && ch[0] != '\n' ) {
						strcat( text, ch ) ;
						strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
						scanf( "%c", &ch[0] );
					} // while
					
					have_minus = false ; // �e���N�⦳��o�ӲŸ��]���O�Ʀr�� 
					strcpy( gToken_info.data, text ) ; // �s��token��T 
					gToken_info.token_type = SYMBOL ;
					gToken.push_back( gToken_info ) ;
					for_break = true ;
				} // if
			} // if
			// ********************************************************************

			if ( ch[0] == '.' && !for_break ) { // �J���I �I�b�̫e���N���i��O�B�I�� ���O���ܳ����i��O�B�I�� (�]���|�t�~�B�z) 
				if (have_minus) strcat( text, "-" ) ;
				strcat( text, "." ) ;
				
				if ( temp_ch[0] == ' ' ) { // �N��o���I���i��O�Ʀr �]���i��u�Osymbol 
					strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch[0] );
					if (ch[0] == ' ') {
						only_dot = true ; // �e�᳣�O�ť� �N�N���I��W�s�b 
						strcpy( gToken_info.data, "." ) ; // �s��token��T 
						gToken_info.token_type = DOT ;
						gToken.push_back( gToken_info ) ;
						for_break = true ;
						strcpy( text , "" ) ; // �M��text 
					} // if 
					else if ( ch[0] < 58 && ch[0] > 47 ) { // �I�᭱�������Ʀr EX: .12abc or .123 
						strcat( text, ch ) ;
						strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
						scanf( "%c", &ch[0] );
						while (ch[0] != ' ' && ch[0] != '\n' && ch[0] > 47 && ch[0] < 58 ) {
							strcat( text, ch ) ;
							strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
							scanf( "%c", &ch[0] );
						} // while

						if ( ch[0] == '.' ) {
							is_float = true;
							strcat( text, ch ) ;
							strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
							scanf( "%c", &ch[0] );
							while (ch[0] != ' ' && ch[0] != '\n' && ch[0] > 47 && ch[0] < 58 ) {
								strcat( text, ch ) ;
								strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
								scanf( "%c", &ch[0] );
							} // while

						} // if
						
						// �᭱���O�ťդ]���O���k�A�� �N��L���O�¼Ʀr �L�Osymbol EX: .12abc
						if ( ch[0] != ' ' && ch[0] != '(' && ch[0] != ')' && ch[0] != '\n' ) { 
							is_float = false ;
							strcat( text, ch ) ;
							strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
							scanf( "%c", &ch[0] );
							while (ch[0] != ' ' && ch[0] != '\n'){ // ��symbolŪ�� 
								strcat( text, ch ) ;
								strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
								scanf( "%c", &ch[0] );
							} // while
							
							strcpy( gToken_info.data, text ) ; // �s��token��T 
							gToken_info.token_type = SYMBOL ;
							gToken.push_back( gToken_info ) ;
							have_print = true ; // �w�g�s�L�F 
							strcpy( text , "" ) ; // �M��text 
						} // if
						else { 																		// EX: .123																
							strcpy( gToken_info.data, text ) ; // �s��token��T 
							if ( is_float ) gToken_info.token_type = FLOAT ; // �P�_�O�_�OFLOAT 
							else gToken_info.token_type = INT ;
							gToken.push_back( gToken_info ) ;
							have_print = true ; // �w�g�s�L�F 
							strcpy( text , "" ) ; // �M��text 
						} // else

						for_break = true ;
					} // else if
					else { //                                                              EX: .abc
						strcat( text, ch ) ;	
						strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 															
						scanf( "%c", &ch[0] );
						while (ch[0] != ' ' && ch[0] != '\n'){ // ��symbolŪ�� 
							strcat( text, ch ) ;
							strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
							scanf( "%c", &ch[0] );
						} // while
						
						strcpy( gToken_info.data, text ) ; // �s��token��T 
						gToken_info.token_type = SYMBOL ;
						gToken.push_back( gToken_info ) ;
						have_print = true ; // �w�g�s�L�F 
						for_break = true ;
						strcpy( text , "" ) ; // �M��text 
					} // else
				} // if
				else if ( strcmp(temp_ch, "") || temp_ch[0] == '+' || temp_ch[0] == '-') { // �N��o���I���i��O�Ʀr �]���i��u�Osymbol 
				
					strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch[0] );
					
					if ( ch[0] < 58 && ch[0] > 47 && ch[0] != '\n' ) { // �I�᭱�������Ʀr 
						strcat( text, ch ) ;
						strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
						scanf( "%c", &ch[0] );
						while (ch[0] != ' ' && ch[0] != '\n' && ch[0] > 47 && ch[0] < 58 ){
							strcat( text, ch ) ;
							strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
							scanf( "%c", &ch[0] );
						} // while

						if ( ch[0] == '.' ) {
							is_float = true;
							strcat( text, ch ) ;
							strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
							scanf( "%c", &ch[0] );
							while (ch[0] != ' ' && ch[0] != '\n' && ch[0] > 47 && ch[0] < 58 ){
								strcat( text, ch ) ;
								strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
								scanf( "%c", &ch[0] );
							} // while

						} // if
						
						// �o�̥N�� .123 �o�ت� �]���I�b�e�����QŪ���F �ҥH�Ʀr����Ū���N�Ofloat 
						if ( ch[0] == ' ' || ch[0] == '\n' ) is_float = true ; 
						
						// �᭱���O�ťդ]���O���k�A�� �N��L���O�¼Ʀr �L�Osymbol 
						if ( ch[0] != ' ' && ch[0] != '(' && ch[0] != ')' && ch[0] != '\n' ) { 
							is_float = false ;
							while (ch[0] != ' ' && ch[0] != '\n'){ // ��symbolŪ�� 
								strcat( text, ch ) ;
								strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
								scanf( "%c", &ch[0] );
							} // while
							
							strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
							scanf( "%c", &ch[0] );
							strcpy( gToken_info.data, text ) ; // �s��token��T 
							gToken_info.token_type = SYMBOL ;
							gToken.push_back( gToken_info ) ;
							have_print = true ; // �w�g�s�L�F 
							for_break = true ;
							strcpy( text , "" ) ; // �M��text 
						} // if
						else {
							strcpy( gToken_info.data, text ) ; // �s��token��T 
							if ( is_float ) gToken_info.token_type = FLOAT ; // �P�_�O�_�OFLOAT 
							else gToken_info.token_type = INT ;
							gToken.push_back( gToken_info ) ;
							have_print = true ; // �w�g�s�L�F 
							strcpy( text , "" ) ; // �M��text 
						} // else
						
						for_break = true ;
					} // if
					else { // EX: -.
						strcpy( gToken_info.data, text ) ; // �s��token��T 
						gToken_info.token_type = SYMBOL ; // �P�_�O�_�OFLOAT 
						gToken.push_back( gToken_info ) ;
						have_print = true ; // �w�g�s�L�F 
						strcpy( text , "" ) ; // �M��text 
						for_break = true ;
					} // else
				} // else if
				else if ( ch[0] != '\n' ) { // �N��o���I���i��O�Ʀr�̪� ���i��O symbol �]���i���W�s�b 
					if ( have_minus ) strcat( text , "-" ) ; // �Osymbol�i�O�e�����t���٨S�[ �ҥH�b�o�[
					strcat( text, ch ) ;
					strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch[0] );
					while (ch[0] != ' ' && ch[0] != '\n' ){
						strcat( text, ch ) ;
						strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
						scanf( "%c", &ch[0] );
						if ( ch[0] == '.' ) is_float = true;
					} // while
					
					strcpy( gToken_info.data, text ) ; // �s��token��T 
					gToken_info.token_type = SYMBOL ;
					gToken.push_back( gToken_info ) ;
					have_print = true ; // �w�g�s�L�F 
					for_break = true ;
					strcpy( text , "" ) ; // �M��text 
					
				} // else

			} // if

			// ********************************************************************
			
			if ( ch[0] == '(' ) {
				strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
				scanf( "%c", &ch[0] );
				if ( ch[0] == 'e' ) {
					strcat( text, "(" ) ;
					while ( ch[0] != '\n' ) {
						strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
						strcat( text, ch ) ;
						scanf( "%c", &ch[0] );
					} // while 
					if ( strcmp( text, "(exit)") == 0 ) {
						printf( "Thanks for using OurScheme!" ) ;
						return ;
					} // if
				} // if
				else if ( ch[0] == ')' ){
					strcat( text, "()" ) ;
					strcpy( gToken_info.data, text ) ; // �s��token��T 
					gToken_info.token_type = SYMBOL ;
					gToken.push_back( gToken_info ) ;
					scanf( "%c", &ch[0] );
				} // else if
				else {
					strcpy( gToken_info.data, "(" ) ; // �s��token��T 
					gToken_info.token_type = LEFT_PAREN ;
					gToken.push_back( gToken_info ) ;
					strcpy( text, "" ) ;
				} // else
				
				for_break = true ;
				have_print = true ;
			} // if
			
			if ( ch[0] == ')') {
				strcat( text, ch ) ;
				strcpy( gToken_info.data, ch ) ; // �s��token��T 
				gToken_info.token_type = RIGHT_PAREN ;
				gToken.push_back( gToken_info ) ;
				strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
				scanf( "%c", &ch[0] );
				strcpy( text , "" ) ; // �M��text 
				for_break = true ;
				have_print = true ;
			} // if

			// ********************************************************************
			
			if ( ch[0] == ';' && !for_break ) { // ��ܾ��O�� ; �}�Y command 
				while ( ch[0] != '\n' ) {
					strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch[0] );
				} // while
				
				for_break = true ;
				have_print = true ; // ���F��泣���n�L
				strcpy( text , "" ) ; // �M��text 
			} // if

			// ********************************************************************

			if ( ch[0] == 't' && !for_break ) { // text == "" �O�]���u��t��W�~��i
			    if ( temp_ch[0] == ' ' || strcmp(temp_ch, "") == 0 || temp_ch[0] == '\n' ) {
				    strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch[0] );
					if ( ch[0] == '\n' || ch[0] == ' ' ) {
						strcpy( gToken_info.data, "#t" ) ; // �s��token��T 
						gToken_info.token_type = T ;
						gToken.push_back( gToken_info ) ;
						have_print = true ; // �w�g�s�L�F 
						for_break = true ;
						strcpy( text , "" ) ; // �M��text 
					} // if
					else strcat( text, temp_ch ) ;
				} // if
			} // if

			// ********************************************************************
			if ( ch[0] == '#' && !for_break ) {
				strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
				scanf( "%c", &ch[0] );
				if ( ch[0] == 't' ) {
					strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch[0] );
					if ( ch[0] == '\n' ) {
						strcat( text , "#t" ) ;
						strcpy( gToken_info.data, text ) ; // �s��token��T 
						gToken_info.token_type = T ;
						gToken.push_back( gToken_info ) ;
						for_break = true ;
						have_print = true ; // �w�g�s�L�F 
						strcpy( text , "" ) ; // �M��text 
					} // if
					else {
						strcat( text , "#t" ) ; // ���M�O#t�}�Y ���L���O��ª�#t �ҥH�O��symbol�d��᭱�B�z 
					} // else
				} // if
				else if ( ch[0] == 'f') { // EX: #f
					strcat( text , "#" ) ;
					strcat( text, ch ) ; // �쥻�n����
					strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch[0] );
					if ( ch[0] == '\n' ) { // �p�G�n���S���p �����令nil
						strcpy( text , "nil" ) ;
						strcpy( gToken_info.data, text ) ; // �s��token��T 
						gToken_info.token_type = NIL ;
						gToken.push_back( gToken_info ) ;
						for_break = true ;
						have_print = true ; // �w�g�s�L�F 
						strcpy( text , "" ) ; // �M��text 
					} // if
				} // else
				else strcat( text , "#" ) ;
			} // if
			// ********************************************************************

			if ( ch[0] > 47 && ch[0] < 58 && !for_break ) { // �Ʀr�}�Y���B�z
				if ( have_minus ) strcat( text , "-" ) ; // �N��e������Ū��t�� 
				strcat( text, ch ) ; 
				strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
				scanf( "%c", &ch[0] );
				while ( ch[0] != '\n' && ch[0] != EOF && ch[0] != ' ' && ch[0] > 47 && ch[0] < 58  ) {
					strcat( text, ch ) ;
					strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch[0] );
				} // while
				if ( ch[0] == '.' ) {
					is_float = true;
					strcat( text, ch ) ;
					strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch[0] );
					while (ch[0] != ' ' && ch[0] != '\n' && ch[0] > 47 && ch[0] < 58 ){
						strcat( text, ch ) ;
						strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
						scanf( "%c", &ch[0] );
					} // while

				} // if
				if ( ch[0] != ' ' && ch[0] != '(' && ch[0] != ')' && ch[0] != '\n' ) { // �᭱���O�ť� �N��L���O�¼Ʀr 
					strcat( text, ch ) ;
					strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch[0] );
					while (ch[0] != ' ' && ch[0] != '\n'){ // ��symbolŪ�� 
						strcat( text, ch ) ;
						strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
						scanf( "%c", &ch[0] );
					} // while
					
					strcat( text, ch ) ;
					strcpy( gToken_info.data, text ) ; // �s��token��T 
					gToken_info.token_type = SYMBOL ;
					gToken.push_back( gToken_info ) ;
					have_print = true ; // �w�g�s�L�F 
					strcpy( text , "" ) ; // �M��text 
				} // if
				else {
					strcpy( gToken_info.data, text ) ; // �s��token��T 
					if ( is_float ) gToken_info.token_type = FLOAT ;
					else gToken_info.token_type = INT ;
					gToken.push_back( gToken_info ) ;
					have_print = true ; // �w�g�s�L�F 
					strcpy( text , "" ) ; // �M��text 
				} // else
				for_break = true ;
			} // if 
			
			// ********************************************************************

			if ( temp_ch[0] == '\"' &&  ch[0] == ' ' && !for_break ) { // ���F�h���̫�[�F�ܦh�ťժ����p
				int num = 1 ;
				strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
				scanf( "%c", &ch[0] );
				while ( ch[0] == ' ' ) {
					strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch[0] );
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
				strcat( text, ch ) ;
				strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
				scanf( "%c", &ch[0] );
				while ( ch[0] != EOF && ch[0] != '\n' && ch[0] != ' ' && ch[0] != '\'' ) {
					strcat( text, ch ) ;
					strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch[0] );
				} // while
				
				strcat( text, ch ) ;
				strcpy( gToken_info.data, text ) ; // �s��token��T 
				gToken_info.token_type = QUOTE ;
				gToken.push_back( gToken_info ) ;
				have_print = true ;
				for_break = true ;
				
			} // if
			// ********************************************************************

			if ( ch[0] == '\"' && !for_break ) { // string�ݭn�t�~�B�z 
				strcat( text, ch ) ;
				strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
				scanf( "%c", &ch[0] );
				bool in_Quote = false ;
				bool have_more_backslash = false ;
				// "OurScheme allows the use of '\\n', '\\t' and  '\\\"' in a string." �o�̦����D 
				while ( ch[0] != EOF && ch[0] != '\n' && ch[0] != '\"' ) { 
					
					if ( ch[0] == '\'' ) in_Quote = true ;
					
					//   OurScheme��string��C/Java��printf()��escape�������A���u����'\n', '\"', '\t', '\n'�P'\\'�o����case�C ��޸��n���O���n��
					if ( in_Quote && ch[0] == '\\' ) {  

					    strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
						scanf( "%c", &ch[0] );
						while ( ch[0] != '\'' && ch[0] != EOF && ch[0] != '\n' ) { // Quote��W�ϥγo�Ӱj��B�z�� 	
												
							if ( ch[0] == 'n' || ch[0] == '\\' || ch[0] == 't' || ch[0] == '\"' ) { // Ū��o�Ǩ��ӲĤ@��'/'���n���� 
								if ( temp_ch[0] == '\\' ) {
									strcat( text, ch ) ;																	
								} // if
								
								if ( ch[0] == '\\' ) { // �o���if�]�t�̭����F�賣�u�O���F�B�z�ܦh��\�����p 
									strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
									scanf( "%c", &ch[0] );
									have_more_backslash = true ;
									if ( ch[0] == '\\') {
										while (ch[0] == '\\' ) {
											strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
											scanf( "%c", &ch[0] );
										} // while
									} //if
								} // if
								else {
									strcat( text , ch );
								} // else 
								
							} // if
							else if (ch[0] != '\'') { // �S�S�O�N�q��\�N������J 					
								strcat( text , temp_ch );
								strcat( text , ch );								
							} // else if
							strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
							if ( !have_more_backslash && ch[0] != '\n' ) scanf( "%c", &ch[0] ); // �]����u�O���F���n�A�hŪ�@�� 
							have_more_backslash = false ;
						} // while
						
						strcat( text , ch ); // �s�̫�@�ӳ�޸� 
						scanf( "%c", &ch[0] ); 
						in_Quote = false ;
					} // if
					else if ( ch[0] == '\\' ) { // ���O�b��޸��̪�\�B�z "Please enter YES\NO below this line >\n" 
						strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
						scanf( "%c", &ch[0] );
						if ( ch[0] == 'n' || ch[0] == '\\' || ch[0] == 't' || ch[0] == '\"' ) { // Ū��o�Ǩ��ӲĤ@��'/'���n���� 
							if ( temp_ch[0] == '\\' ) {
								if ( ch[0] == 'n' ) strcat( text , "\n" ) ; // ����r���n�o�˦s ���M�L�X�Ӥ��|���� 
								else strcat( text, ch ) ;																	
							} // if
							
							if ( ch[0] == '\\' ) { // �o���if�]�t�̭����F�賣�u�O���F�B�z�ܦh��\�����p 
								strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
								scanf( "%c", &ch[0] );
								have_more_backslash = true ;
								if ( ch[0] == '\\') {
									while (ch[0] == '\\' ) {
										strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
										scanf( "%c", &ch[0] );
									} // while
								} //if
							} // if
							
						} // if
						else if (ch[0] != '\'') { // �S�S�O�N�q��\�N������J 					
							strcat( text , temp_ch );
							strcat( text , ch );							
						} // else if
						strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
						if ( !have_more_backslash && ch[0] != '\n' ) scanf( "%c", &ch[0] ); // �]����u�O���F���n�A�hŪ�@�� 
						have_more_backslash = false ;
					} // else if
					else {
						strcat( text, ch ) ;
						strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
						scanf( "%c", &ch[0] );
					} // else
				} // while
				strcat( text, ch ) ; // �s�̫�@�����޸� 				
				strcpy( gToken_info.data, text ) ; // �s��token��T 
				gToken_info.token_type = STRING ;
				gToken.push_back( gToken_info ) ;
				have_print = true ; // �w�g�B�z�L�F�᭱���ΦA�L�@��
				for_break = true ;
				strcpy( text, "" ) ;
				scanf( "%c", &ch[0] );
				
			} // if
		   
		    // ********************************************************************
		   
		    if ( !for_break && ch[0] != ' ' ) { // �o��O�b�B�z �r�� �� �S��Ÿ��}�Y�� SYMBOL
		   		strcat( text, ch ) ;
		   		strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
		   		scanf( "%c", &ch[0] );
		   		bool is_nil = false ;
		   		
		   		if ( temp_ch[0] == 'n' && ch[0] == 'i' && strcmp( text, "" ) ) { // for nil
		   			strcat( text, ch ) ;
		   			strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
		   			scanf( "%c", &ch[0] );
		   			if ( ch[0] == 'l' ) { // �o�ӬOL���O1 ���n�h�� 
		   				strcat( text, ch ) ;
		   				strcpy( gToken_info.data, text ) ; // �s��token��T 
						gToken_info.token_type = NIL;
						gToken.push_back( gToken_info ) ;
		   				is_nil = true ;
					} // if
				} // if
		   		while ( ch[0] != EOF && ch[0] != '\n' && ch[0] != ' ' && ch[0] != ';' && ch[0] != ')' ) {
					strcat( text, ch ) ;
					strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
					scanf( "%c", &ch[0] );
				} // while
				
				if ( !is_nil ) { // ���Onil�~�Osymbol 
					strcpy( gToken_info.data, text ) ; // �s��token��T 
					gToken_info.token_type = SYMBOL ;
					gToken.push_back( gToken_info ) ;
				} // if
		   	
		    } // if
		   
		    // ********************************************************************
		   
		    if ( ch[0] == ' ' ) {
		   	    strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
		        temp = scanf( "%c", &ch[0] );
		    } //if 
		    have_minus = false ;
		    for_break = false ; // �]���@���N�i�H�Ѱ�for_break���A 
		} // while
		
		// *************************************************�B�zparse tree ************************************* 
		Tree_node *root = new Tree_node;
		root ->left= NULL;
		root ->right= NULL;
		if ( gToken.size() != 0 ) strcpy(root ->tree_token.data, gToken[0].data ); // ��l�� tree , �é�J�Ĥ@�� data 
		if ( ch[0] == '\n' || ch[0] == EOF ) {
			if ( gToken.size() != 0 ) root = BuildTree(root, root);
			gNum = 0 ;
			if ( gToken.size() != 0 ) {
				PrintTree(root) ;
				gHave_print_left = false ;
			} // if
		} // if
		// *************************************************�B�zparse tree ************************************* 

		strcpy( text, "" ) ;
		if ( ch[0] == EOF || ch[0] == '\n') gToken.clear() ;
		have_print = false ;
		is_float = false ;
		only_dot = false ;
		if ( ch[0] == ' ' || ch[0] == '\n' ) {
			strcpy( temp_ch, ch ) ; // �x�s�W�@�� ch 
		    temp = scanf( "%c", &ch[0] );
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



