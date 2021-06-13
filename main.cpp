
# include <iostream>
# include <cstdio>
# include <cstdlib>
# include <string>
# include <vector>

using namespace std ;

// ---       class head        ---

// An object of a Token that contain every data about Token include the Token it point to,
// linked by pointer( link list ).
class Token ;

// An object which has every data about s-expression.
class S_Expression ;

// An object which has every data about error.
class Error ;

// An object which has a job to get input in.
class Scanner ;

// An object which has jobs to check syntax and evaluate.
class Parser ;

// ---       type head         ---



// ---       type define       ---

// A type of Token.
enum TokenType { LEFTPAREN, RIGHTPAREN, INT, STRING, DOT, FLOAT, NIL, T, QUOTE, SYMBOL, DEFAULTTOKEN } ;

// A type of Error.
enum ErrorType { HASEOF, OTHERS, DEFAULTERROR } ;

typedef Token * TokenPtr ;

typedef string * StringPtr ;

typedef vector<Error> * ErrorVctPtr ;

typedef vector<S_Expression> * SExpressionVctPtr ;

// ---       function head     ---

// ---       class             ---

class Token {
    
public:
  TokenType mTokenType ;
  StringPtr mToken ;
  int mLine ;
  int mColumn ;
  TokenPtr mNext ;
    
  Token() { // Constructor
    mTokenType = DEFAULTTOKEN ;
    mToken = new string ;
    mToken->clear() ;
    mLine = -1 ;
    mColumn = -1 ;
    mNext = NULL ;
  } // Token()
    
} ; //  class Token

class S_Expression {
    
public:
  TokenPtr mTokenString ;
  StringPtr mCompleteSExp ;
    
  S_Expression() { // Constructor
    mTokenString = NULL ;
    mCompleteSExp = new string ;
    mCompleteSExp->clear() ;
        
    return ;
  } // S_Expression()
    
} ; // class S_Expression

class Error {
    
public:
  ErrorType mErrorType ;
    
  Error() { // Constructor
    mErrorType = DEFAULTERROR ;
        
    return ;
  } // Error()
    
  Error( ErrorType in ) { // Constructor with parameter
    mErrorType = in ;
        
    return ;
  } // Error()
    
} ; // class Error

class Scanner {
  StringPtr mLoadedLine ;
  ErrorVctPtr mErrorVct ;
  int mLine ;
  int mColumn ;
public:
  Scanner() { // Constructor
    mLoadedLine = new string ;
    mLoadedLine->clear() ;
    mErrorVct = new vector<Error> ;
    mErrorVct->clear() ;
    mLine = 0 ;
    mColumn = 0 ;
        
    return ;
  } // Scanner()
    
  // A function which has to read in a bunch of Token until them can
  // be combined to a S-Expression or there are errors.
  // get a line per times.
  bool ReadSExp( S_Expression & sExp, bool hasEof ) {
        
    return true ;
  } // ReadSExp()
    
  // A function which has to get a Token and return a TokenPtr that
  // point to a Token contain a string of this Token and its line and column.
  bool GetToken( TokenPtr & token ) {
        
        
    return true ;
  } // GetToken()
    
  StringPtr GetLine() {
    StringPtr lineIn = new string ;
    char temp ;
        
    temp = cin.get() ;
    while ( temp != '\n' && ! cin.eof() ) {
      lineIn->push_back( temp ) ;
      temp = cin.get() ;
    } // while
    
    mLine++ ;
        
    return lineIn ;
  } // GetLine()
    
  void PrintError() {
        
    return ;
  } // PrintError()
    
} ; // class Scanner

class Parser {
  ErrorVctPtr mErrorVct ;
public:
  Parser() { // Constructor
    mErrorVct = new vector<Error> ;
    mErrorVct->clear() ;
        
    return ;
  } // Parser()
    
  bool PrintSExp( S_Expression sExp ) {
        
    return true ;
  } // PrintSExp()
    
  bool Eval( S_Expression sExp, bool notEnd ) {
    if ( IsExit( sExp.mTokenString ) ) {
      notEnd = false ;
      return true ;
    } // if
    else {
      Error temp( OTHERS ) ;
      mErrorVct->push_back( temp ) ;
      return true ; // return false ;
    } // else
        
  } // Eval()
    
  bool IsExit( TokenPtr tokenString ) {
    if ( tokenString != NULL &&
         tokenString->mTokenType == LEFTPAREN &&
         tokenString->mNext != NULL &&
         * tokenString->mNext->mToken == "exit" &&
         tokenString->mNext->mNext != NULL &&
         tokenString->mNext->mNext->mTokenType == RIGHTPAREN )
      return true ;
    else
      return false ;
        
  } // IsExit()
    
  void PrintError() {
        
    return ;
  } // PrintError()
    
} ; // class parser

// ---       general function  ---

int main() {
    
  int inputID ;
  bool notEnd = true ;
  bool hasEof = false ;
  SExpressionVctPtr sExpVct = new vector<S_Expression> ;
  Scanner scanner ;
  Parser parser ;
    
  // printf( "Welcome to OurScheme!" ) ;
    
  StringPtr test ;
  cin >> inputID ;
  int i = 0 ;
  if ( inputID == 1 ) {
    while ( ! cin.eof() || i != 100 ) {
      test = scanner.GetLine() ;
      cout << * test << endl ;
      i++ ;
    } // while
        
  } // if
    
  /*
  while ( notEnd ) {    // not exit && not EOF
    printf( "\n> " ) ;
    
    if ( scanner.ReadSExp( sExp, hasEof ) )
      parser.PrintSExp( sExp ) ; // pretty print
    else
      scanner.PrintError() ;
    
    if ( parser.Eval( sExp, notEnd ) )
      parser.PrintSExp( sExp ) ;
    else
      parser.PrintError() ;
     
  } // while
  */
  // printf( "\nThanks for using OurScheme!" ) ;
    
  return 0 ;
} // main()
