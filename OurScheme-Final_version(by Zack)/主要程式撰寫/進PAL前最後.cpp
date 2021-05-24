// 10527124
// 資訊三甲
// 邱正皓
// 程式語言 PL 第一次程式作業 OurScheme


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
	Token_enum token_type ;// 儲存gettoken的資料型別
	char data[5000] ;// 儲存gettoken的內容
};

vector<Token> gToken;
Token gToken_info ;
int gNum = 0 ;
bool gIn_LP = false ; // 判斷是不是在LP裡面，如果是的話要換行 
bool gHave_print_left = false ; // 看看有沒有印過左括號了 

// **********************************Binary tree 宣告**************************************
struct Tree_node {
	Token tree_token;
	Tree_node *left;
	Tree_node *right;
};

// **********************************Binary tree 宣告**************************************

// 左括號的下一個是右括號的話要組合在一起當作一個ATOM 
bool Syntax_Check( Tree_node *ptr, int num ) { // 先檢查文法 沒錯的話 再建樹 然後才印 
    Tree_node *exp = new Tree_node ; // 建立新的node
	
    if ( gToken.size() == 0 ) {
        printf( "SyntaxError('unexpected EOF')" ) ; // 完全沒有輸入資料
        return false ; 
    } // if
    return true ;
    
} // Syntax_Check()

Tree_node * Initial( Tree_node *ptr ){ // 用於不斷初始化node 
    ptr = new Tree_node ;
    ptr->left = NULL ;
	ptr->right = NULL ;
	
	return ptr ;
} // Initial() 

void PrintTree( Tree_node *ptr ) {    // (exit)要return  遞迴是回到上一層進入的點 

	if ( ptr->left != NULL && !gHave_print_left ) {
		gHave_print_left = true ;
		gIn_LP = true ;
		printf( "( " ); // 往左就一定再印一個左括號 
	} // if

	if ( ptr->left != NULL ) { // 走訪
		PrintTree( ptr->left );
	} // if
	
	if ( ptr->left != NULL && ptr->right != NULL) { 
		PrintTree( ptr->right );
	} // if
	
	else if ( ptr->left != NULL && ptr->right == NULL ) { 
		gIn_LP = false ;
		if ( gHave_print_left ) printf( ")\n" ); // 有左括號才印右括號
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

// 每個指標都會有個address(存ptr本身的位置)跟 value(ptr指向的位置)
// , funtion傳進來的ptr跟裡面ptr的address不同，但value相同 ,每次遞迴參數都要看成外面的 
// 所以但假如你在function內改位置(value) 在丟進遞迴參數裡就會因為位置連不起來而crach
// 但如果利用參數傳進來的pointer 往右或往左指 直接創新的node 會改變裡面ptr的address 
//放進遞迴的時候會改變參數ptr的 value 然後藉由 遞迴串連在一起 
Tree_node * BuildTree( Tree_node *ptr, Tree_node *exp  ) { // 建樹 龍骨一直是空的 遞迴是回上一層的進入點 

	// 遞迴每 return一次就會回去一層 
	// exp跟ptr的address不同但value相同 代表他們指向同一個位置 
	
	if ( gToken.size() == 0 ) {
		printf("SyntaxError('unexpected EOF')") ; // 完全沒有輸入資料  
		return exp ; 
	} // if

	if ( gNum == gToken.size() ) { // 終止條件 
		return exp;
	} // if
	
	if ( ptr->left == NULL && strcmp( gToken[gNum].data,"(") == 0 ) { // 如果是左括號 左邊也的確沒東西 token就往左邊存 	
		strcpy(ptr ->tree_token.data, ""); // 因為是左括號不用存值 
		ptr->tree_token.token_type = gToken[gNum].token_type ;
		ptr->left = Initial( ptr->left ) ; // 初始化並建立新的node 
		gNum = gNum + 1 ;	
		
		if ( strcmp(gToken[gNum].data, "(") != 0 && strcmp( gToken[gNum].data, ")" ) != 0 ) { 
			// 這個else if都是在處理data的存取跟偷看下一個token做出相對應的處理 
			strcpy(ptr->left->tree_token.data, gToken[gNum].data); // 現在這個data先存進去 
			ptr->left->tree_token.token_type = gToken[gNum].token_type ; // 現在這個data先存進去 
			
			if ( strcmp(gToken[gNum+1].data, "(") == 0 ) { // 如果是左括號就繼續往左指  
				gIn_LP = true ; // 有一個以上的左括號 
			} // else if
			else if ( gNum == gToken.size() - 1 ) {
				gNum = gNum + 1 ;
			} // else
			else if ( gNum == gToken.size() - 2 && strcmp( gToken[gNum+1].data, ")" ) == 0 ) { // EX. ( 12 ) 
				gNum = gNum + 1 ;
				return exp ;
			} // else if
		
		} // if
		else if ( strcmp( gToken[gNum].data, "(") == 0 ) { // 又他媽左括號接左括號
			strcpy(ptr ->tree_token.data, ""); // 因為是左括號不用存值 
			ptr->tree_token.token_type = gToken[gNum].token_type ;
			ptr->left = Initial( ptr->left ) ; // 初始化並建立新的node 
			BuildTree(ptr->left, exp); // 往左指
		} // else if
		
				
		BuildTree(ptr, exp); // 遞迴父節點 
	} // if
	
	// 這裡要處理存完data之後，return回上一個父節點的情況，且現在要處理的data不為左括號 要繼續延伸龍骨 
	if ( ptr->left != NULL && ptr->right == NULL ) { 
		ptr->right = Initial( ptr->right ) ; // 初始化並建立新的node 
		gNum = gNum + 1 ;
		BuildTree(ptr->right, exp); // 往右邊指 因為data還是前一個 所以要 gNum+1 
	} // if
	
	// 與前一個 if 不同在於它處理數字接左括號 gNum不用+1是因為這個左括號還需要處理
	if ( ptr->left != NULL && ptr->right == NULL && strcmp(gToken[gNum].data, "(") == 0 ) { 
		ptr->right = Initial( ptr->right ) ; // 初始化並建立新的node 
		BuildTree(ptr->right, exp); // 往右邊指 因為data還是前一個 所以要 gNum+1   
	} // if

	// 這個else if都是在處理data的存取跟偷看下一個token做出相對應的處理 
	if ( strcmp(gToken[gNum].data, "(") != 0 && strcmp(gToken[gNum].data, ")") != 0 ) { 
		
		if ( ptr->left == NULL ) { // 判斷存之前要不要往左指		
			if ( gNum != 0 && strcmp(gToken[gNum-1].data, "(") != 0 ) { // 前一個如果是左括號或是已經左指過了 就不用再往左指了 			
				if ( strcmp(gToken[gNum+1].data, "(") != 0 ) { // 如果不是點接左括號 
					ptr->left = Initial( ptr->left ) ;
					strcpy(ptr->left->tree_token.data, gToken[gNum].data); // 現在這個data先存進去 
					ptr->left->tree_token.token_type = gToken[gNum].token_type ;
				} // if
				else if ( strcmp(gToken[gNum].data, ".") == 0 && strcmp(gToken[gNum+1].data, "(") == 0 ) {
					gNum = gNum + 1 ; // 跳過點跟左括號 
				} // else if
			} // if
			if ( gIn_LP ) { // 第二個左括號出現 兩個if都會觸發 
				gIn_LP = false ;
				ptr->left = Initial( ptr->left ) ;
				gNum = gNum + 1 ;
				BuildTree(ptr->left, exp);
			} // if
		} // if	
		// 代表是單個token存在 有左括號的話  左括號跟第一個值已經處理過了 
		if ( gNum == 0 ) {
			strcpy(ptr->tree_token.data, gToken[gNum].data); 
			ptr->tree_token.token_type = gToken[gNum].token_type ;
		} // else if
		if ( strcmp(gToken[gNum+1].data, ")") == 0 && gNum == gToken.size() - 2 ) { // 要結束了 
			gNum = gNum + 1 ;
			return exp ;
		} // if	
		// 存完先偷看下一個token 不是左括號就先return回上一層龍骨 
		else if ( gNum != gToken.size() - 1 && strcmp(gToken[gNum+1].data, "(") != 0 ) { 
			BuildTree(ptr, exp); // 遞迴父節點 
		} // else if
		else if ( strcmp(gToken[gNum+1].data, "(") == 0 ) { // 如果是左括號就繼續往左指      尚未完成!!!!!!! 
			gIn_LP = true ; // 有一個以上的左括號 
			return exp ; // 遞迴到上一進入點 
		} // else if
		else if ( gNum == gToken.size() - 1 ) {
			gNum = gNum + 1 ;
			BuildTree(ptr, exp); // 如果後面已經沒東西了就不用指了 gNum + 1 是為了return 
		} // else if
		
	} // if
	
	// 注意!! 遇到右括號，應該是要一路遞迴到上一次右腳是空的情況  
	
	if ( strcmp(gToken[gNum].data, ")") == 0 ) { // 遇到右括號，應該是要一路遞迴到上一次右腳是空的情況  
		if( gNum != gToken.size() - 1 ) {
			gNum = gNum + 1 ;
			BuildTree(ptr, exp); // 要先偷看還有沒有token，主要是跑 gNum+1 
		} // if
		else return exp ; // 沒有東西了準備中止建樹  
	} // if

	return exp ;
	
} // BuildTree()


// ********************************處理Token************************************************ 

void Welcome_and_read() {

	cout << "Welcome to OurScheme!\n" << "\n";
	string text = "";
	char ch ;
	char temp ;
	temp = scanf( "%c", &ch ); // 把數字讀掉
	temp = scanf( "%c", &ch ); // 讀掉換行 
	if ( temp == EOF ) return ;
} // Welcome_and_read()

void GetToken () { // GetToken 先全部照單全收，要印的時候用C語言把歸類成 float的token，用%3f的函數去印 

	char text[5000] = "" ; // 用來暫存token 
	char ch[] = "0" ; // 用來讀檔 
	char temp_ch[] = "0" ; // 輔助讀檔 
	scanf( "%c", &ch[0] ); // 一次讀一個字元 
	char temp ;
	bool have_print = false ; // 單獨拉出來處理的 token ex.string , command 
	bool for_break = false ; // 給 PAL用的 break()
	bool is_float = false ; // 用來判斷是不是 float (關係到DOT的使用)
	bool only_dot = false ; 
	bool have_minus = false ; // 判斷數字開頭有無 '-' 

	while ( temp != EOF ) { // 讀到結束

		while ( temp != EOF && ch[0] != '\n' ) { // 讀到換行 \n

			// ********************************************************************
			if ( ch[0] == '+' || ch[0] == '-' ) { // 遇到 + 就讀掉
				if ( ch[0] == '-' ) have_minus = true ;
				else strcat( text , ch ) ;
				strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
				scanf( "%c", &ch[0] );

				if ( ch[0] > 57 && ch[0] < 48  && ch[0] != '.' ) { // 如果後面不是接數字或點開頭代表他是Symbol 
					strcat( text, ch ) ;
					strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch[0] );
					while ( ch[0] != EOF && ch[0] != '\n' ) {
						strcat( text, ch ) ;
						strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
						scanf( "%c", &ch[0] );
					} // while
					
					have_minus = false ; // 前面就算有減這個符號也不是數字的 
					strcpy( gToken_info.data, text ) ; // 存取token資訊 
					gToken_info.token_type = SYMBOL ;
					gToken.push_back( gToken_info ) ;
					for_break = true ;
				} // if
			} // if
			// ********************************************************************

			if ( ch[0] == '.' && !for_break ) { // 遇到點 點在最前面就有可能是浮點數 不是的話都不可能是浮點數 (因為會另外處理) 
				if (have_minus) strcat( text, "-" ) ;
				strcat( text, "." ) ;
				
				if ( temp_ch[0] == ' ' ) { // 代表這個點有可能是數字 也有可能只是symbol 
					strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch[0] );
					if (ch[0] == ' ') {
						only_dot = true ; // 前後都是空白 就代表點單獨存在 
						strcpy( gToken_info.data, "." ) ; // 存取token資訊 
						gToken_info.token_type = DOT ;
						gToken.push_back( gToken_info ) ;
						for_break = true ;
						strcpy( text , "" ) ; // 清空text 
					} // if 
					else if ( ch[0] < 58 && ch[0] > 47 ) { // 點後面直接接數字 EX: .12abc or .123 
						strcat( text, ch ) ;
						strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
						scanf( "%c", &ch[0] );
						while (ch[0] != ' ' && ch[0] != '\n' && ch[0] > 47 && ch[0] < 58 ) {
							strcat( text, ch ) ;
							strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
							scanf( "%c", &ch[0] );
						} // while

						if ( ch[0] == '.' ) {
							is_float = true;
							strcat( text, ch ) ;
							strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
							scanf( "%c", &ch[0] );
							while (ch[0] != ' ' && ch[0] != '\n' && ch[0] > 47 && ch[0] < 58 ) {
								strcat( text, ch ) ;
								strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
								scanf( "%c", &ch[0] );
							} // while

						} // if
						
						// 後面不是空白也不是左右括號 代表他不是純數字 他是symbol EX: .12abc
						if ( ch[0] != ' ' && ch[0] != '(' && ch[0] != ')' && ch[0] != '\n' ) { 
							is_float = false ;
							strcat( text, ch ) ;
							strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
							scanf( "%c", &ch[0] );
							while (ch[0] != ' ' && ch[0] != '\n'){ // 把symbol讀完 
								strcat( text, ch ) ;
								strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
								scanf( "%c", &ch[0] );
							} // while
							
							strcpy( gToken_info.data, text ) ; // 存取token資訊 
							gToken_info.token_type = SYMBOL ;
							gToken.push_back( gToken_info ) ;
							have_print = true ; // 已經存過了 
							strcpy( text , "" ) ; // 清空text 
						} // if
						else { 																		// EX: .123																
							strcpy( gToken_info.data, text ) ; // 存取token資訊 
							if ( is_float ) gToken_info.token_type = FLOAT ; // 判斷是否是FLOAT 
							else gToken_info.token_type = INT ;
							gToken.push_back( gToken_info ) ;
							have_print = true ; // 已經存過了 
							strcpy( text , "" ) ; // 清空text 
						} // else

						for_break = true ;
					} // else if
					else { //                                                              EX: .abc
						strcat( text, ch ) ;	
						strcpy( temp_ch, ch ) ; // 儲存上一個 ch 															
						scanf( "%c", &ch[0] );
						while (ch[0] != ' ' && ch[0] != '\n'){ // 把symbol讀完 
							strcat( text, ch ) ;
							strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
							scanf( "%c", &ch[0] );
						} // while
						
						strcpy( gToken_info.data, text ) ; // 存取token資訊 
						gToken_info.token_type = SYMBOL ;
						gToken.push_back( gToken_info ) ;
						have_print = true ; // 已經存過了 
						for_break = true ;
						strcpy( text , "" ) ; // 清空text 
					} // else
				} // if
				else if ( strcmp(temp_ch, "") || temp_ch[0] == '+' || temp_ch[0] == '-') { // 代表這個點有可能是數字 也有可能只是symbol 
				
					strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch[0] );
					
					if ( ch[0] < 58 && ch[0] > 47 && ch[0] != '\n' ) { // 點後面直接接數字 
						strcat( text, ch ) ;
						strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
						scanf( "%c", &ch[0] );
						while (ch[0] != ' ' && ch[0] != '\n' && ch[0] > 47 && ch[0] < 58 ){
							strcat( text, ch ) ;
							strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
							scanf( "%c", &ch[0] );
						} // while

						if ( ch[0] == '.' ) {
							is_float = true;
							strcat( text, ch ) ;
							strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
							scanf( "%c", &ch[0] );
							while (ch[0] != ' ' && ch[0] != '\n' && ch[0] > 47 && ch[0] < 58 ){
								strcat( text, ch ) ;
								strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
								scanf( "%c", &ch[0] );
							} // while

						} // if
						
						// 這裡代表 .123 這種的 因為點在前面先被讀掉了 所以數字直接讀完就是float 
						if ( ch[0] == ' ' || ch[0] == '\n' ) is_float = true ; 
						
						// 後面不是空白也不是左右括號 代表他不是純數字 他是symbol 
						if ( ch[0] != ' ' && ch[0] != '(' && ch[0] != ')' && ch[0] != '\n' ) { 
							is_float = false ;
							while (ch[0] != ' ' && ch[0] != '\n'){ // 把symbol讀完 
								strcat( text, ch ) ;
								strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
								scanf( "%c", &ch[0] );
							} // while
							
							strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
							scanf( "%c", &ch[0] );
							strcpy( gToken_info.data, text ) ; // 存取token資訊 
							gToken_info.token_type = SYMBOL ;
							gToken.push_back( gToken_info ) ;
							have_print = true ; // 已經存過了 
							for_break = true ;
							strcpy( text , "" ) ; // 清空text 
						} // if
						else {
							strcpy( gToken_info.data, text ) ; // 存取token資訊 
							if ( is_float ) gToken_info.token_type = FLOAT ; // 判斷是否是FLOAT 
							else gToken_info.token_type = INT ;
							gToken.push_back( gToken_info ) ;
							have_print = true ; // 已經存過了 
							strcpy( text , "" ) ; // 清空text 
						} // else
						
						for_break = true ;
					} // if
					else { // EX: -.
						strcpy( gToken_info.data, text ) ; // 存取token資訊 
						gToken_info.token_type = SYMBOL ; // 判斷是否是FLOAT 
						gToken.push_back( gToken_info ) ;
						have_print = true ; // 已經存過了 
						strcpy( text , "" ) ; // 清空text 
						for_break = true ;
					} // else
				} // else if
				else if ( ch[0] != '\n' ) { // 代表這個點不可能是數字裡的 有可能是 symbol 也不可能單獨存在 
					if ( have_minus ) strcat( text , "-" ) ; // 是symbol可是前面有負號還沒加 所以在這加
					strcat( text, ch ) ;
					strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch[0] );
					while (ch[0] != ' ' && ch[0] != '\n' ){
						strcat( text, ch ) ;
						strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
						scanf( "%c", &ch[0] );
						if ( ch[0] == '.' ) is_float = true;
					} // while
					
					strcpy( gToken_info.data, text ) ; // 存取token資訊 
					gToken_info.token_type = SYMBOL ;
					gToken.push_back( gToken_info ) ;
					have_print = true ; // 已經存過了 
					for_break = true ;
					strcpy( text , "" ) ; // 清空text 
					
				} // else

			} // if

			// ********************************************************************
			
			if ( ch[0] == '(' ) {
				strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
				scanf( "%c", &ch[0] );
				if ( ch[0] == 'e' ) {
					strcat( text, "(" ) ;
					while ( ch[0] != '\n' ) {
						strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
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
					strcpy( gToken_info.data, text ) ; // 存取token資訊 
					gToken_info.token_type = SYMBOL ;
					gToken.push_back( gToken_info ) ;
					scanf( "%c", &ch[0] );
				} // else if
				else {
					strcpy( gToken_info.data, "(" ) ; // 存取token資訊 
					gToken_info.token_type = LEFT_PAREN ;
					gToken.push_back( gToken_info ) ;
					strcpy( text, "" ) ;
				} // else
				
				for_break = true ;
				have_print = true ;
			} // if
			
			if ( ch[0] == ')') {
				strcat( text, ch ) ;
				strcpy( gToken_info.data, ch ) ; // 存取token資訊 
				gToken_info.token_type = RIGHT_PAREN ;
				gToken.push_back( gToken_info ) ;
				strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
				scanf( "%c", &ch[0] );
				strcpy( text , "" ) ; // 清空text 
				for_break = true ;
				have_print = true ;
			} // if

			// ********************************************************************
			
			if ( ch[0] == ';' && !for_break ) { // 表示整行是由 ; 開頭 command 
				while ( ch[0] != '\n' ) {
					strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch[0] );
				} // while
				
				for_break = true ;
				have_print = true ; // 為了整行都不要印
				strcpy( text , "" ) ; // 清空text 
			} // if

			// ********************************************************************

			if ( ch[0] == 't' && !for_break ) { // text == "" 是因為只有t單獨才能進
			    if ( temp_ch[0] == ' ' || strcmp(temp_ch, "") == 0 || temp_ch[0] == '\n' ) {
				    strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch[0] );
					if ( ch[0] == '\n' || ch[0] == ' ' ) {
						strcpy( gToken_info.data, "#t" ) ; // 存取token資訊 
						gToken_info.token_type = T ;
						gToken.push_back( gToken_info ) ;
						have_print = true ; // 已經存過了 
						for_break = true ;
						strcpy( text , "" ) ; // 清空text 
					} // if
					else strcat( text, temp_ch ) ;
				} // if
			} // if

			// ********************************************************************
			if ( ch[0] == '#' && !for_break ) {
				strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
				scanf( "%c", &ch[0] );
				if ( ch[0] == 't' ) {
					strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch[0] );
					if ( ch[0] == '\n' ) {
						strcat( text , "#t" ) ;
						strcpy( gToken_info.data, text ) ; // 存取token資訊 
						gToken_info.token_type = T ;
						gToken.push_back( gToken_info ) ;
						for_break = true ;
						have_print = true ; // 已經存過了 
						strcpy( text , "" ) ; // 清空text 
					} // if
					else {
						strcat( text , "#t" ) ; // 雖然是#t開頭 但他不是單純的#t 所以是個symbol留到後面處理 
					} // else
				} // if
				else if ( ch[0] == 'f') { // EX: #f
					strcat( text , "#" ) ;
					strcat( text, ch ) ; // 原本要做的
					strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch[0] );
					if ( ch[0] == '\n' ) { // 如果要做特殊情況 直接改成nil
						strcpy( text , "nil" ) ;
						strcpy( gToken_info.data, text ) ; // 存取token資訊 
						gToken_info.token_type = NIL ;
						gToken.push_back( gToken_info ) ;
						for_break = true ;
						have_print = true ; // 已經存過了 
						strcpy( text , "" ) ; // 清空text 
					} // if
				} // else
				else strcat( text , "#" ) ;
			} // if
			// ********************************************************************

			if ( ch[0] > 47 && ch[0] < 58 && !for_break ) { // 數字開頭的處理
				if ( have_minus ) strcat( text , "-" ) ; // 代表前面有先讀到負號 
				strcat( text, ch ) ; 
				strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
				scanf( "%c", &ch[0] );
				while ( ch[0] != '\n' && ch[0] != EOF && ch[0] != ' ' && ch[0] > 47 && ch[0] < 58  ) {
					strcat( text, ch ) ;
					strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch[0] );
				} // while
				if ( ch[0] == '.' ) {
					is_float = true;
					strcat( text, ch ) ;
					strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch[0] );
					while (ch[0] != ' ' && ch[0] != '\n' && ch[0] > 47 && ch[0] < 58 ){
						strcat( text, ch ) ;
						strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
						scanf( "%c", &ch[0] );
					} // while

				} // if
				if ( ch[0] != ' ' && ch[0] != '(' && ch[0] != ')' && ch[0] != '\n' ) { // 後面不是空白 代表他不是純數字 
					strcat( text, ch ) ;
					strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch[0] );
					while (ch[0] != ' ' && ch[0] != '\n'){ // 把symbol讀完 
						strcat( text, ch ) ;
						strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
						scanf( "%c", &ch[0] );
					} // while
					
					strcat( text, ch ) ;
					strcpy( gToken_info.data, text ) ; // 存取token資訊 
					gToken_info.token_type = SYMBOL ;
					gToken.push_back( gToken_info ) ;
					have_print = true ; // 已經存過了 
					strcpy( text , "" ) ; // 清空text 
				} // if
				else {
					strcpy( gToken_info.data, text ) ; // 存取token資訊 
					if ( is_float ) gToken_info.token_type = FLOAT ;
					else gToken_info.token_type = INT ;
					gToken.push_back( gToken_info ) ;
					have_print = true ; // 已經存過了 
					strcpy( text , "" ) ; // 清空text 
				} // else
				for_break = true ;
			} // if 
			
			// ********************************************************************

			if ( temp_ch[0] == '\"' &&  ch[0] == ' ' && !for_break ) { // 為了去除最後加了很多空白的情況
				int num = 1 ;
				strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
				scanf( "%c", &ch[0] );
				while ( ch[0] == ' ' ) {
					strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
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

			if ( ch[0] == '\'' && !for_break ) { // 遇到'\'' ===> QUOTE
				strcat( text, ch ) ;
				strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
				scanf( "%c", &ch[0] );
				while ( ch[0] != EOF && ch[0] != '\n' && ch[0] != ' ' && ch[0] != '\'' ) {
					strcat( text, ch ) ;
					strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch[0] );
				} // while
				
				strcat( text, ch ) ;
				strcpy( gToken_info.data, text ) ; // 存取token資訊 
				gToken_info.token_type = QUOTE ;
				gToken.push_back( gToken_info ) ;
				have_print = true ;
				for_break = true ;
				
			} // if
			// ********************************************************************

			if ( ch[0] == '\"' && !for_break ) { // string需要另外處理 
				strcat( text, ch ) ;
				strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
				scanf( "%c", &ch[0] );
				bool in_Quote = false ;
				bool have_more_backslash = false ;
				// "OurScheme allows the use of '\\n', '\\t' and  '\\\"' in a string." 這裡有問題 
				while ( ch[0] != EOF && ch[0] != '\n' && ch[0] != '\"' ) { 
					
					if ( ch[0] == '\'' ) in_Quote = true ;
					
					//   OurScheme的string有C/Java的printf()的escape的概念，但只限於'\n', '\"', '\t', '\n'與'\\'這五個case。 單引號好像是必要的
					if ( in_Quote && ch[0] == '\\' ) {  

					    strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
						scanf( "%c", &ch[0] );
						while ( ch[0] != '\'' && ch[0] != EOF && ch[0] != '\n' ) { // Quote單獨使用這個迴圈處理完 	
												
							if ( ch[0] == 'n' || ch[0] == '\\' || ch[0] == 't' || ch[0] == '\"' ) { // 讀到這些那個第一個'/'都要拿掉 
								if ( temp_ch[0] == '\\' ) {
									strcat( text, ch ) ;																	
								} // if
								
								if ( ch[0] == '\\' ) { // 這整個if包含裡面的東西都只是為了處理很多個\的情況 
									strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
									scanf( "%c", &ch[0] );
									have_more_backslash = true ;
									if ( ch[0] == '\\') {
										while (ch[0] == '\\' ) {
											strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
											scanf( "%c", &ch[0] );
										} // while
									} //if
								} // if
								else {
									strcat( text , ch );
								} // else 
								
							} // if
							else if (ch[0] != '\'') { // 沒特別意義的\就直接放入 					
								strcat( text , temp_ch );
								strcat( text , ch );								
							} // else if
							strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
							if ( !have_more_backslash && ch[0] != '\n' ) scanf( "%c", &ch[0] ); // 設條件只是為了不要再多讀一次 
							have_more_backslash = false ;
						} // while
						
						strcat( text , ch ); // 存最後一個單引號 
						scanf( "%c", &ch[0] ); 
						in_Quote = false ;
					} // if
					else if ( ch[0] == '\\' ) { // 不是在單引號裡的\處理 "Please enter YES\NO below this line >\n" 
						strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
						scanf( "%c", &ch[0] );
						if ( ch[0] == 'n' || ch[0] == '\\' || ch[0] == 't' || ch[0] == '\"' ) { // 讀到這些那個第一個'/'都要拿掉 
							if ( temp_ch[0] == '\\' ) {
								if ( ch[0] == 'n' ) strcat( text , "\n" ) ; // 換行字元要這樣存 不然印出來不會換行 
								else strcat( text, ch ) ;																	
							} // if
							
							if ( ch[0] == '\\' ) { // 這整個if包含裡面的東西都只是為了處理很多個\的情況 
								strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
								scanf( "%c", &ch[0] );
								have_more_backslash = true ;
								if ( ch[0] == '\\') {
									while (ch[0] == '\\' ) {
										strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
										scanf( "%c", &ch[0] );
									} // while
								} //if
							} // if
							
						} // if
						else if (ch[0] != '\'') { // 沒特別意義的\就直接放入 					
							strcat( text , temp_ch );
							strcat( text , ch );							
						} // else if
						strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
						if ( !have_more_backslash && ch[0] != '\n' ) scanf( "%c", &ch[0] ); // 設條件只是為了不要再多讀一次 
						have_more_backslash = false ;
					} // else if
					else {
						strcat( text, ch ) ;
						strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
						scanf( "%c", &ch[0] );
					} // else
				} // while
				strcat( text, ch ) ; // 存最後一個雙引號 				
				strcpy( gToken_info.data, text ) ; // 存取token資訊 
				gToken_info.token_type = STRING ;
				gToken.push_back( gToken_info ) ;
				have_print = true ; // 已經處理過了後面不用再印一次
				for_break = true ;
				strcpy( text, "" ) ;
				scanf( "%c", &ch[0] );
				
			} // if
		   
		    // ********************************************************************
		   
		    if ( !for_break && ch[0] != ' ' ) { // 這邊是在處理 字母 或 特殊符號開頭的 SYMBOL
		   		strcat( text, ch ) ;
		   		strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
		   		scanf( "%c", &ch[0] );
		   		bool is_nil = false ;
		   		
		   		if ( temp_ch[0] == 'n' && ch[0] == 'i' && strcmp( text, "" ) ) { // for nil
		   			strcat( text, ch ) ;
		   			strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
		   			scanf( "%c", &ch[0] );
		   			if ( ch[0] == 'l' ) { // 這個是L不是1 不要懷疑 
		   				strcat( text, ch ) ;
		   				strcpy( gToken_info.data, text ) ; // 存取token資訊 
						gToken_info.token_type = NIL;
						gToken.push_back( gToken_info ) ;
		   				is_nil = true ;
					} // if
				} // if
		   		while ( ch[0] != EOF && ch[0] != '\n' && ch[0] != ' ' && ch[0] != ';' && ch[0] != ')' ) {
					strcat( text, ch ) ;
					strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch[0] );
				} // while
				
				if ( !is_nil ) { // 不是nil才是symbol 
					strcpy( gToken_info.data, text ) ; // 存取token資訊 
					gToken_info.token_type = SYMBOL ;
					gToken.push_back( gToken_info ) ;
				} // if
		   	
		    } // if
		   
		    // ********************************************************************
		   
		    if ( ch[0] == ' ' ) {
		   	    strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
		        temp = scanf( "%c", &ch[0] );
		    } //if 
		    have_minus = false ;
		    for_break = false ; // 跑完一次就可以解除for_break狀態 
		} // while
		
		// *************************************************處理parse tree ************************************* 
		Tree_node *root = new Tree_node;
		root ->left= NULL;
		root ->right= NULL;
		if ( gToken.size() != 0 ) strcpy(root ->tree_token.data, gToken[0].data ); // 初始化 tree , 並放入第一筆 data 
		if ( ch[0] == '\n' || ch[0] == EOF ) {
			if ( gToken.size() != 0 ) root = BuildTree(root, root);
			gNum = 0 ;
			if ( gToken.size() != 0 ) {
				PrintTree(root) ;
				gHave_print_left = false ;
			} // if
		} // if
		// *************************************************處理parse tree ************************************* 

		strcpy( text, "" ) ;
		if ( ch[0] == EOF || ch[0] == '\n') gToken.clear() ;
		have_print = false ;
		is_float = false ;
		only_dot = false ;
		if ( ch[0] == ' ' || ch[0] == '\n' ) {
			strcpy( temp_ch, ch ) ; // 儲存上一個 ch 
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



