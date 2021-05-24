// 10527124
// 資訊三甲
// 邱正皓
// 程式語言 PL 第一次程式作業 OurScheme


/*
	現在要改的是讓gettoken回傳token跟type，目前是想丟到vector裡面  
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
	token_enum token_type ;// 儲存gettoken的資料型別
	char data[50] ;// 儲存gettoken的內容
}  ;

vector<struct token> gToken;
token token_info ;
int gNum = 0 ;
bool gIn_LP = false ; // 判斷是不是在LP裡面，如果是的話要換行 

// **********************************Binary tree 宣告**************************************
struct Tree_node {
	token tree_token ; 
	Tree_node *left;
	Tree_node *right;
} ;


// **********************************Binary tree 宣告**************************************

void PrintTree( Tree_node *ptr ) {
	
	printf("EE") ;
	
	if(ptr->left != NULL) { // 走訪
		printf("%s ", ptr->tree_token.data);
		PrintTree(ptr->left);
	} // if
	else if(ptr->right != NULL) { // 走訪
		printf("%s ", ptr->tree_token.data);
		PrintTree(ptr->right);
	} // else if
	else return ;
	
	printf("HH") ;

} // PrintTree()


void BuildTree( Tree_node *ptr, int num ) { // 建樹 必須符合Syntax 輸出    (exit)要return 

	//	遞迴每 return一次就會回去一層 

	Tree_node *exp = new Tree_node ; // 建立新的node
	if ( exp == NULL) {	
		exp = ptr ; // 可以把 exp想成 root 指向樹根，然後是ptr在跑 
	} // if
	
	if ( gToken.size() == 0 ) {
		printf("SyntaxError('unexpected EOF')") ; // 完全沒有輸入資料
		return ; 
	} // if
	
	if ( num == gToken.size()) { //終止條件 
		//if ( exp != NULL ) {
		//	PrintTree( exp ) ;
		//	exp = NULL ;
		//} // if
		return ;
	} // if

	if ( ptr->right == NULL && gToken[num].data != "(" && gToken[num].data != ")" ) {
		strcpy(ptr ->tree_token.data, gToken[num].data); // 存值進data，注意字串要用strcpy
		if ( gToken[num+1].data != "(" && gToken[num+1].data != ")" ){ // ()要另外處理，不能直接往右 
			BuildTree(ptr->right, num + 1);
		} // if
	} // if
		
	// 注意!! 遇到右括號，應該是要一路遞迴到上一次右腳是空的情況
	 
	if ( ptr->left == NULL && gToken[num].data == "(" ) {
		// 如果是左括號 token往左邊存 
		BuildTree(ptr->left, num + 1);
	} // if
	
	if ( gToken[num].data == ")" ) {
		// 如果是右括號 token往右邊存 
		if ( ptr->right != NULL ) {
			return ;
		} // if
		else BuildTree(ptr->right, num + 1);
	} // if

	return ;
} // BuildTree()


// ********************************處理Token************************************************ 

void Welcome_and_read() {

	cout << "Welcome to OurScheme!\n" << "\n";
	string text = "";
	char ch ;
	char temp ;
	temp = scanf( "%c", &ch ); // 把數字讀掉
	temp = scanf( "%c", &ch );
	if ( temp == EOF ) return ;
} // Welcome_and_read()

void GetToken () { // GetToken 先全部照單全收，要印的時候用C語言把歸類成 float的token，用%3f的函數去印 

	char text[1000] = "" ; // 用來暫存token 
	char ch[1] ; // 用來讀檔 
	char temp_ch[1] ; // 輔助讀檔 
	scanf( "%c", &ch ); // 一次讀一個字元 
	char temp ;
	bool have_print = false ; // 單獨拉出來處理的 token ex.string , command 
	bool for_break = false ; // 給 PAL用的 break()
	bool is_float = true ; // 用來判斷是不是 float (關係到DOT的使用)
	bool only_dot = false ; 
	bool have_minus = false ; // 判斷數字開頭有無 '-' 
	string space = "";

	while ( temp != EOF ) { // 讀到結束

		while ( temp != EOF && ch[0] != '\n' ) { // 讀到換行 \n

			// ********************************************************************
			if ( ch[0] == '+' || ch[0] == '-' ) { // 遇到 + 就讀掉
				if ( ch[0] == '-') have_minus == true ;
				strcat( text , ch ) ;
				strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
				scanf( "%c", &ch );
				if ( ch[0] > 57 && ch[0] < 48  && ch[0] != '.' ) { // 如果後面不是接數字或點開頭代表他是Symbol 
					strcat( text , ch ) ;
					strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch );
					while ( ch[0] != EOF && ch[0] != '\n' ) {
						strcat( text , ch ) ;
						strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
						scanf( "%c", &ch );
					} // while
					
					have_minus = false ; // 前面就算有減這個符號也不是數字的 
					strcpy( token_info.data, text ) ; // 存取token資訊 
					token_info.token_type = SYMBOL ;
					gToken.push_back(token_info) ;
					for_break = true ;
				} // if
			} // if
			// ********************************************************************
			if ( ch[0] == '.' && !for_break ) { // 遇到點 點在最前面就有可能是浮點數 不是的話都不可能是浮點數 (因為會另外處理) 
				if (have_minus) strcat( text, "-" ) ;
				strcat( text, "." ) ;
				
				if ( temp_ch[0] == ' ' ) { // 代表這個點有可能是數字 也有可能只是symbol 
					strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch );
					if (ch[0] == ' ') {
						only_dot = true ; // 前後都是空白 就代表點單獨存在 
						strcpy( token_info.data, "." ) ; // 存取token資訊 
						token_info.token_type = DOT ;
						gToken.push_back(token_info) ;
						for_break = true ;
						strcpy( text , "" ) ; // 清空text 
					} // if 
					else if ( ch[0] < 58 && ch[0] > 47 ) { // 點後面直接接數字 EX: .12abc or .123 
						strcat( text , ch ) ;
						strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
						scanf( "%c", &ch );
						while (ch[0] != ' ' && ch[0] != '\n' && ch[0] > 47 && ch[0] < 58 || ch[0] == '.' ){
							strcat( text , ch ) ;
							strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
							scanf( "%c", &ch );
							if( ch[0] == '.' ) is_float = true;
						} // while
						
						if ( ch[0] != ' ' && ch[0] != '(' && ch[0] != ')' && ch[0] != '\n' ) { // 後面不是空白也不是左右括號 代表他不是純數字 他是symbol EX: .12abc
							strcat( text , ch ) ;
							strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
							scanf( "%c", &ch );
							while (ch[0] != ' ' && ch[0] != '\n'){ // 把symbol讀完 
								strcat( text , ch ) ;
								strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
								scanf( "%c", &ch );
							} // while
							strcpy( token_info.data, text ) ; // 存取token資訊 
							token_info.token_type = SYMBOL ;
							gToken.push_back(token_info) ;
							have_print = true ; // 已經存過了 
							strcpy( text , "" ) ; // 清空text 
						} // if
						else { 																		// EX: .123																
							strcpy( token_info.data, text ) ; // 存取token資訊 
							if ( is_float ) token_info.token_type = FLOAT ; // 判斷是否是FLOAT 
							else token_info.token_type = INT ;
							gToken.push_back(token_info) ;
							have_print = true ; // 已經存過了 
							strcpy( text , "" ) ; // 清空text 
						} // else
						cout << "> " << text << "\n" << "\n" ;
						for_break = true ;
					} // else if
					else { //                                                              EX: .abc
						strcat( text , ch ) ;	
						strcpy( temp_ch , ch ) ; // 儲存上一個 ch 															
						scanf( "%c", &ch );
						while (ch[0] != ' ' && ch[0] != '\n'){ // 把symbol讀完 
							strcat( text , ch ) ;
							strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
							scanf( "%c", &ch );
						} // while
						strcpy( token_info.data, text ) ; // 存取token資訊 
						token_info.token_type = SYMBOL ;
						gToken.push_back(token_info) ;
						have_print = true ; // 已經存過了 
						for_break = true ;
						strcpy( text , "" ) ; // 清空text 
					} // else
				} // if
				else if (temp_ch == "" || temp_ch[0] == '+' || temp_ch[0] == '-'){ // 代表這個點有可能是數字 也有可能只是symbol 
				
					strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch );
					
					if ( ch[0] < 58 && ch[0] > 47 ) { // 點後面直接接數字 
						strcat( text , ch ) ;
						strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
						scanf( "%c", &ch );
						while (ch[0] != ' ' && ch[0] != '\n' && ch[0] > 47 && ch[0] < 58 || ch[0] == '.' ){
							strcat( text , ch ) ;
							strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
							scanf( "%c", &ch );
							if( ch[0] == '.' ) is_float = true;
						} // while
						
						if ( ch[0] != ' ' && ch[0] != '(' && ch[0] != ')' && ch[0] != '\n' ) { // 後面不是空白也不是左右括號 代表他不是純數字 他是symbol 
							while (ch[0] != ' ' && ch[0] != '\n'){ // 把symbol讀完 
								strcat( text , ch ) ;
								strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
								scanf( "%c", &ch );
							} // while
							strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
							scanf( "%c", &ch );
							strcpy( token_info.data, text ) ; // 存取token資訊 
							token_info.token_type = SYMBOL ;
							gToken.push_back(token_info) ;
							have_print = true ; // 已經存過了 
							for_break = true ;
							strcpy( text , "" ) ; // 清空text 
						} // if
						else {
							strcpy( token_info.data, text ) ; // 存取token資訊 
							if ( is_float ) token_info.token_type = FLOAT ; // 判斷是否是FLOAT 
							else token_info.token_type = INT ;
							gToken.push_back(token_info) ;
							have_print = true ; // 已經存過了 
							strcpy( text , "" ) ; // 清空text 
						} // else
						for_break = true ;
					} // if
				} // else if
				else { // 代表這個點不可能是數字裡的 有可能是 symbol 也不可能單獨存在 
					strcat( text , ch ) ;
					strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch );
					while (ch[0] != ' ' && ch[0] != '\n' ){
						strcat( text , ch ) ;
						strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
						scanf( "%c", &ch );
						if( ch[0] == '.' ) is_float = true;
					} // while
					strcpy( token_info.data, text ) ; // 存取token資訊 
					token_info.token_type = SYMBOL ;
					gToken.push_back(token_info) ;
					have_print = true ; // 已經存過了 
					for_break = true ;
					strcpy( text , "" ) ; // 清空text 
					
				} // else

			} // if

			// ********************************************************************
			
			if ( ch[0] == '(' ) {
				strcat( text , ch ) ;
				strcpy( token_info.data, ch ) ; // 存取token資訊 
				token_info.token_type = LEFT_PAREN ;
				gToken.push_back(token_info) ;
				strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
				scanf( "%c", &ch );
				strcpy( text , "" ) ; // 清空text 
				for_break = true ;
				have_print = true ;
			} // if
			
			if ( ch[0] == ')') {
				strcat( text , ch ) ;
				strcpy( token_info.data, ch ) ; // 存取token資訊 
				token_info.token_type = RIGHT_PAREN ;
				gToken.push_back(token_info) ;
				strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
				scanf( "%c", &ch );
				strcpy( text , "" ) ; // 清空text 
				for_break = true ;
				have_print = true ;
			} // if

			// ********************************************************************

			if ( ch[0] == ';' && !for_break ) {
				if ( text == "" ) { // 表示整行是由 ; 開頭 command 
					while ( ch[0] != '\n' ) {
						strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
						scanf( "%c", &ch );
					} // while
					for_break = true ;
					have_print = true ; // 為了整行都不要印
					strcpy( text , "" ) ; // 清空text 
				} // if
			} // if

			// ********************************************************************

			if ( ch[0] == 't' && !for_break ) { // text == "" 是因為只有t單獨才能進
			    if ( temp_ch[0] == ' ' || temp_ch == "" || temp_ch[0] == '\n' ){
				    strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch );
					if ( ch[0] == '\n' || ch[0] == ' ' ) {
						strcpy( token_info.data, "#t" ) ; // 存取token資訊 
						token_info.token_type = T ;
						gToken.push_back(token_info) ;
						have_print = true ; // 已經存過了 
						for_break = true ;
						strcpy( text , "" ) ; // 清空text 
					} // if
				} // if
			} // if

			// ********************************************************************
			if ( ch[0] == '#' && !for_break ) {
				strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
				scanf( "%c", &ch );
				if ( ch[0] == 't' ) {
					strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch );
					if ( ch[0] == '\n' ) {
						strcat( text , "#t" ) ;
						strcpy( token_info.data, text ) ; // 存取token資訊 
						token_info.token_type = T ;
						gToken.push_back(token_info) ;
						for_break = true ;
						have_print = true ; // 已經存過了 
						strcpy( text , "" ) ; // 清空text 
					} // if
					else {
						while( ch[0] != '\n' ) { // 讀到換行讀乾淨 後面才不會錯
						    strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
							scanf( "%c", &ch );
						} // while
						strcat( text , "#tt" ) ;
						strcpy( token_info.data, text ) ; // 存取token資訊 
						token_info.token_type = T ;
						gToken.push_back(token_info) ;
						for_break = true ;
						have_print = true ; // 已經存過了 
						strcpy( text , "" ) ; // 清空text 
					} // else
				} // if
				else {
					strcat( text , "#" ) ;
					strcat( text , ch ) ; // 原本要做的
					strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch );
					if ( ch[0] == '\n' ) { // 如果要做特殊情況 直接改成nil
						strcat( text , "nil" ) ;
						strcpy( token_info.data, text ) ; // 存取token資訊 
						token_info.token_type = NIL ;
						gToken.push_back(token_info) ;
						for_break = true ;
						have_print = true ; // 已經存過了 
						strcpy( text , "" ) ; // 清空text 
					} // if
				} // else
			} // if
			// ********************************************************************

			if ( ch[0] > 47 && ch[0] < 58 && !for_break ) { // 數字開頭的處理
				if ( have_minus ) strcat( text , "-" ) ; // 代表前面有先讀到負號 
				strcat( text , ch ) ; 
				strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
				scanf( "%c", &ch );
				while ( ch[0] != '\n' && ch[0] != EOF && ch[0] != ' ' && ch[0] > 47 && ch[0] < 58 || ch[0] == '.' ){
					strcat( text , ch ) ;
					strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch );
					if( ch[0] == '.' ) is_float = true;
				} // while
				
				if ( ch[0] != ' ' && ch[0] != '(' && ch[0] != ')' && ch[0] != '\n' ) { // 後面不是空白 代表他不是純數字 
					strcat( text , ch ) ;
					strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch );
					while (ch[0] != ' ' && ch[0] != '\n'){ // 把symbol讀完 
						strcat( text , ch ) ;
						strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
						scanf( "%c", &ch );
					} // while
					strcat( text , ch ) ;
					strcpy( token_info.data, text ) ; // 存取token資訊 
					token_info.token_type = SYMBOL ;
					gToken.push_back(token_info) ;
					have_print = true ; // 已經存過了 
					strcpy( text , "" ) ; // 清空text 
				} // if
				else {
					strcpy( token_info.data, text ) ; // 存取token資訊 
					if ( is_float ) token_info.token_type = FLOAT ;
					else token_info.token_type = INT ;
					gToken.push_back(token_info) ;
					have_print = true ; // 已經存過了 
					strcpy( text , "" ) ; // 清空text 
				} // else
				for_break = true ;
			} // if 
			
			// ********************************************************************

			if ( temp_ch[0] == '\"' &&  ch[0] == ' ' && !for_break ) { // 為了去除最後加了很多空白的情況
				int num = 1 ;
				strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
				scanf( "%c", &ch );
				while ( ch[0] == ' ' ) {
					strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
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

			if ( ch[0] == '\'' && !for_break ) { // 遇到'\'' ===> QUOTE
				strcat( text , ch ) ;
				strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
				scanf( "%c", &ch );
				while ( ch[0] != EOF && ch[0] != '\n' && ch[0] != ' ' ) {
					strcat( text , ch ) ;
					strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch );
				} // while
				strcpy( token_info.data, text ) ; // 存取token資訊 
				token_info.token_type = QUOTE ;
				gToken.push_back(token_info) ;
				
			} // if
			// ********************************************************************

			if ( ch[0] == '\"' && !for_break ) { // string需要另外處理 
				strcat( text , ch ) ;
				strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
				scanf( "%c", &ch );
				while ( ch[0] != EOF && ch[0] != '\n' && ch[0] != '\"' ) {
					
					if ( ch[0] == '\\' && !for_break ) {

					    strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
						scanf( "%c", &ch );
						if ( ch[0] == 'n' ) { // 讀到\n但不是換行字元
							strcat( text , "\n" ) ;
							strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
							scanf( "%c", &ch );
							while ( ch[0] != EOF && ch[0] != '\n' && ch[0] != ' ' ) {
								strcat( text , ch ) ;
								strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
								scanf( "%c", &ch );
							} // while
							strcpy( token_info.data, text ) ; // 存取token資訊 
							token_info.token_type = STRING ;
							gToken.push_back(token_info) ;
							have_print = true ; // 已經處理過了後面不用再印一次
						} // if
						else if ( ch[0] != '\\' && ch[0] != '\"' ) strcat( text , "\\" ) ; // 判斷只有一個\的情況
						else if ( ch[0] == '\\' ) {  // 判斷兩個\以上的情況
							while ( ch[0] == '\\' ) {
								strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
								scanf( "%c", &ch );
							} // while
							strcat( text , "\\" );
						}
					} // if
					else {
						strcat( text , ch ) ;
						strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
						scanf( "%c", &ch );
					}
				} // while
				
		   } // if
		   if ( !for_break && ch[0] != ' ' ) { // 這邊是在處理 字母 或 特殊符號開頭的 SYMBOL
		   		strcat( text , ch ) ;
		   		strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
		   		scanf( "%c", &ch );
		   		while ( ch[0] != EOF && ch[0] != '\n' && ch[0] != ' ' && ch[0] != ';' ) {
					strcat( text , ch ) ;
					strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
					scanf( "%c", &ch );
				} // while
				
				strcpy( token_info.data, text ) ; // 存取token資訊 
				token_info.token_type = SYMBOL ;
				gToken.push_back(token_info) ;
		   	
		   } // if
		   
		   if( ch[0] == ' ' || ch[0] != '\n' ) {
		   	    strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
		        temp = scanf( "%c", &ch );
		   } //if 
		   have_minus = false ;
		   for_break = false ; // 跑完一次就可以解除for_break狀態 
		} // while
		
		// *************************************************處理parse tree ************************************* 
		Tree_node *root = new Tree_node;
		root ->left= NULL;
		root ->right= NULL;
		strcpy(root ->tree_token.data, gToken[0].data ); // 初始化 tree , 並放入第一筆 data 
		// if ( ch[0] == '\n' || ch[0] == EOF )BuildTree(root, 0);
		// *************************************************處理parse tree ************************************* 
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
			strcpy( temp_ch , ch ) ; // 儲存上一個 ch 
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



