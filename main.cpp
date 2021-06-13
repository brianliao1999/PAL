//
//  main.cpp
//  PAL
//
//  Created by 廖奕銘 on 2021/3/14.
//
//
/*
 進度：
    確認要用scanf or cin
    main() -> readSExp() -> getToken() :
        scan 到有東西 or EOF, 回傳一個token, 不知道是哪個
    isSth( TokenString ) :
        確認是什麼
    readSExp() :
        recursion
 
 */
//


#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using namespace std ;

//---       class head        ---

//An object of a Token that contain every data about Token include the Token it point to , linked by pointer( link list ).
class Token ;

//An object which has every data about s-expression.
class S_Expression ;

//An object which has every data about error.
class Error ;

//An object which has a job to get input in.
class Scanner ;

//An object which has jobs to check syntax and evaluate.
class Parser ;

//---       type head         ---



//---       type define       ---

//A type of Token.
enum TokenType { LEFTPAREN, RIGHTPAREN, INT, STRING, DOT, FLOAT, NIL, T, QUOTE, SYMBOL, DEFAULTTOKEN } ;

//A type of Error.
enum ErrorType { HASEOF, OTHERS, DEFAULTERROR } ;

typedef Token * TokenPtr ;

//---       function head     ---

//---       class             ---

class Token {
    
public:
    TokenType tokenType ;
    string token ;
    int line ;
    int column ;
    TokenPtr next ;
    
    Token() { // Constructor
        tokenType = DEFAULTTOKEN ;
        token = "" ;
        line = -1 ;
        column = -1 ;
        next = NULL ;
    } // Token()
    
} ; //  class Token

class S_Expression {
    
public:
    TokenPtr tokenString ;
    string completeSExp ;
    
    S_Expression() { // Constructor
        tokenString = NULL ;
        completeSExp = "" ;
        
        return ;
    } // S_Expression()
    
} ; // class S_Expression

class Error {
    
public:
    ErrorType errorType ;
    
    Error() { // Constructor
        errorType = DEFAULTERROR ;
        
        return ;
    } // Error()
    
    Error( ErrorType in ) { // Constructor with parameter
        errorType = in ;
        
        return ;
    } // Error()
    
} ; // class Error

class Scanner {
    string loadedLine ;
    vector<Error> errorVct ;
    int line ;
    int column ;
public:
    Scanner() { // Constructor
        loadedLine = "" ;
        errorVct.clear() ;
        errorVct.shrink_to_fit() ;
        line = 0 ;
        column = 0 ;
        
        return ;
    } // Scanner()
    
    // A function which has to read in a bunch of Token until them can be combined to a S-Expression or there are errors.
    // get a line per times.
    bool readSExp( S_Expression & sExp, bool hasEof ) {
        
        return true ;
    } // resdSExp()
    
    //A function which has to get a Token and return a TokenPtr that point to a Token contain a string of this Token and its line and column.
    bool getToken( TokenPtr & token ) {
        while ( ! cin.eof() && loadedLine.empty() ) {
            getline( cin, loadedLine ) ;
            if ( ! loadedLine.empty() )
                line++ ;
            
        } // while
        
        
        return true ;
    } // getToken()
    
    void printError() {
        
        return ;
    } // printError()
    
} ; // class Scanner

class Parser {
    vector<Error> errorVct ;
public:
    Parser() { // Constructor
        errorVct.clear() ;
        errorVct.shrink_to_fit() ;
        
        return ;
    } // Parser()
    
    bool printSExp( S_Expression sExp ) {
        
        return true ;
    } // printSExp
    
    bool eval( S_Expression sExp, bool notEnd ) {
        if ( isExit( sExp.tokenString ) ) {
            notEnd = false ;
            return true ;
        } // if
        else {
            Error temp( OTHERS ) ;
            errorVct.push_back( temp ) ;
            return true ; //return false ;
        } // else
        
    } // eval()
    
    bool isExit ( TokenPtr tokenString ) {
        if ( tokenString != NULL &&
             tokenString->tokenType == LEFTPAREN &&
             tokenString->next != NULL &&
             tokenString->next->token == "exit" &&
             tokenString->next->next != NULL &&
             tokenString->next->next->tokenType == RIGHTPAREN )
            return true ;
        else
            return false ;
        
    } // isExit()
    
    void printError() {
        
        return ;
    } // printError()
    
} ; // class parser

//---       general function  ---

int main( int argc, const char * argv[] ) {
    
    int inputID ;
    bool notEnd = true ;
    bool hasEof = false ;
    S_Expression sExp ;
    Scanner scanner ;
    Parser parser ;
    
    //printf( "Welcome to OurScheme!" ) ;
    
    string test ;
    cin >> inputID ;
    if ( inputID == 1 ) {
        while( ! cin.eof() ) {
            getline( cin, test ) ;
            cout << test ;
        } // while
        
    } // if
    
    /*
    while ( notEnd ) {    // not exit && not EOF
        printf( "\n> " ) ;
        
        if ( scanner.readSExp( sExp, hasEof ) )
            parser.printSExp( sExp ) ; // pretty print
        else
            scanner.printError() ;
        
        if ( parser.eval( sExp, notEnd ) )
            parser.printSExp( sExp ) ;
        else
            parser.printError() ;
         
    } // while
    */
    //printf( "\nThanks for using OurScheme!" ) ;
    
    return 0 ;
} // main()
