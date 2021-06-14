
# include <iostream>
# include <stdio.h>
# include <stdlib.h>
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
enum TokenType { LEFTPAREN, RIGHTPAREN, INT, STRING, DOT, FLOAT,
                 NIL, T, QUOTE, SYMBOL, SEMICOLON, DEFAULTTOKEN } ;

// A type of Error.
enum ErrorType { HASEOF, EXPECTRIGHT, EXPECTLEFT, NOCLOSE, OTHERS, DEFAULTERROR } ;

typedef Token * TokenPtr ;

typedef string * StringPtr ;

typedef vector<Error> * ErrorVctPtr ;

typedef S_Expression * SExpressionPtr ;



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
  int mLine ;
  int mColumn ;
  string mToken ;
    
  Error() { // Constructor
    mErrorType = DEFAULTERROR ;
    mLine = -1 ;
    mColumn = -1 ;
    mToken.clear() ;
        
    return ;
  } // Error()
    
  Error( ErrorType in ) { // Constructor with parameter
    mErrorType = in ;
        
    return ;
  } // Error()
    
} ; // class Error

class Scanner {
  StringPtr mLoadedLine ;
  TokenPtr mPeekedToken ;
  ErrorVctPtr mErrorVct ;
  int mLine ;
  int mColumn ;
public:
  Scanner() { // Constructor
    mLoadedLine = new string ;
    mLoadedLine->clear() ;
    mPeekedToken = NULL ;
    mErrorVct = new vector<Error> ;
    mErrorVct->clear() ;
    mLine = -1 ;
    mColumn = 0 ;
        
    return ;
  } // Scanner()
    
  // A function which has to read in a bunch of Token until them can
  // be combined to a S-Expression or there are errors.
  // get a line per times.
  bool ReadSExp( SExpressionPtr & sExp ) {
    sExp = new S_Expression ;
    bool error = false ;
    sExp->mTokenString = GetSExp( error, true ) ;
    mColumn = 0 ;
    mLine = 0 ;
    
    return ! error ;
  } // ReadSExp()
  
  // A function which has to get a S-Expression( Token string )
  TokenPtr GetSExp( bool & hasError, bool first ) {
    // <S-exp> :: = <ATOM> | LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN | QUOTE <S-exp>
    
    TokenPtr head = NULL ;
    TokenPtr tail = NULL ;
    TokenType mPeekedTokenType = DEFAULTTOKEN ;
    
    if ( GetToken( head ) ) {
      // get a Token
      
      if ( IsAtom( head->mTokenType ) ) {
        // :: = <ATOM>
        // the Token is an ATOM, also a S-EXP
        
        hasError = false ;
        
        return head ;
      } // if
      else if ( head->mTokenType == LEFTPAREN ) {
        // :: = <ATOM> | LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN
        // the Token is a LEFT-PAREN, could be an ATOM or sth else.
        
        if ( PeekToken( mPeekedTokenType ) ) {
          // peek what next Token is
          
          if ( mPeekedTokenType == RIGHTPAREN ) {
            // :: = <ATOM> :: = RIGHT-PAREN LEFT-PAREN
            // the Token string could be combined to an ATOM
            // also a S-EXP
            
            GetToken( head->mNext ) ;
            hasError = false ;
            
            return head ;
          } // if
          else {
            // :: = LEFT-PAREN !at here! <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN
            // it couldn't to be an ATOM, should be sth else.
            // and the next should be a S-EXP or DOT or RIGHT-PAREN
            
            head->mNext = GetSExp( hasError, false ) ;
            if ( hasError ) {
              // there are some errors when getting next S-EXP.
              
              DeleteTokenPtr( head ) ;
              
              return head ;
            } // if
            else { // if ( no error )
              tail = GetTail( head ) ;
              
              // :: = LEFT-PAREN <S-exp> !at here! { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN
              // have got a S-EXP, peek what next Token is.
              
              if ( PeekToken( mPeekedTokenType ) ) {
                
                while ( mPeekedTokenType != DOT && mPeekedTokenType != RIGHTPAREN && ! hasError ) {
                  // the next Token isn't a DOT or RIGHT-PAREN
                  // there are no errors when peeking and getting next S-EXP
                  // continue with getting S-EXP
                  // until there has some errors or the next Token
                  // is DO or RIGHT-PAREN
                  
                  tail->mNext = GetSExp( hasError, false ) ;
                  tail = GetTail( head ) ;
                  if ( ! hasError ) {
                    hasError = ! PeekToken( mPeekedTokenType ) ;
                  } // if
                  
                } // while
                
                // :: = LEFT-PAREN <S-exp> { <S-exp> } !at here! [ DOT <S-exp> ] RIGHT-PAREN
                // there has some errors or the next Token
                // is DOT or RIGHT-PAREN
                
                if ( hasError ) {
                  // there has some errors
                  
                  if ( ! first ) {
                    mErrorVct->at( mErrorVct->size() - 1 ).mErrorType = NOCLOSE ;
                  } // if
                  
                  DeleteTokenPtr( head ) ;
                  
                  return head ;
                } // if
                else if ( mPeekedTokenType == DOT ) {
                  // the next Token is DOT
                  
                  GetToken( tail->mNext ) ;
                  tail = tail->mNext ;
                  // :: = LEFT-PAREN <S-exp> { <S-exp> } [ DOT !at here! <S-exp> ] RIGHT-PAREN
                  // next should be a S-EXP or RIGHT-PAREN
                  
                  tail->mNext = GetSExp( hasError, false ) ;
                  if ( hasError ) {
                    // there has some errors.
                    
                    DeleteTokenPtr( head ) ;
                    
                    return head ;
                  } // if
                  else { // if ( no error )
                    tail = GetTail( tail ) ;
                    // :: = LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] !at here! RIGHT-PAREN
                    // the next Token should be RIGHT-PAREN
                    
                    if ( GetToken( tail->mNext ) ) {
                      // have got next Token.
                      tail = tail->mNext ;
                      
                      if ( tail->mTokenType == RIGHTPAREN ) {
                        // :: = LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN !at here!
                        // it is RIGHT-PAREN !
                        // the Token string is a S-EXP !
                        
                        hasError = false ;
                        
                        return head ;
                      } // if
                      else {
                        // here should has a right-paren, but it doesn't
                        // so it should be an error
                        
                        Error temp ;
                        temp.mLine = tail->mLine ;
                        temp.mColumn = tail->mColumn ;
                        temp.mToken = * tail->mToken ;
                        temp.mErrorType = EXPECTRIGHT ;
                        mErrorVct->push_back( temp ) ;
                        
                        hasError = true ;
                        DeleteTokenPtr( head ) ;
                        
                        return head ;
                      } // else
                      
                    } // if
                    else {
                      // next Token has some errors
                      
                      if ( ! first ) {
                        mErrorVct->at( mErrorVct->size() - 1 ).mErrorType = NOCLOSE ;
                      } // if
                      
                      DeleteTokenPtr( head ) ;
                      
                      return head ;
                    } // else
                      
                  } // else
                  
                } // if
                else if ( mPeekedTokenType == RIGHTPAREN ) {
                  // :: = LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN !at here!
                  // the Token string is a S-exp !
                  
                  GetToken( tail->mNext ) ;
                  hasError = false ;
                  
                  return head ;
                } // else if
                else { // if ( mPeekedTokenType != DOT && mPeekedTokenType != RIGHTPAREN )
                  // :: = LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] !at here! RIGHT-PAREN
                  // here should has a RIGHT-PAREN, but it doesn't
                  // so it should be an error.
                  
                  Error temp ;
                  temp.mLine = tail->mLine ;
                  temp.mColumn = tail->mColumn ;
                  temp.mToken = * tail->mToken ;
                  temp.mErrorType = EXPECTRIGHT ;
                  mErrorVct->push_back( temp ) ;
                  
                  hasError = true ;
                  DeleteTokenPtr( head ) ;
                  
                  return head ;
                } // else
                
              } // if
              else {
                // next Token has some errors
                
                if ( ! first ) {
                  mErrorVct->at( mErrorVct->size() - 1 ).mErrorType = NOCLOSE ;
                } // if
                
                DeleteTokenPtr( head ) ;
                
                return head ;
              } // else
              
            } // else
            
          } // else
          
        } // if
        else {
          // next Token has some errors
          
          if ( ! first ) {
            mErrorVct->at( mErrorVct->size() - 1 ).mErrorType = NOCLOSE ;
          } // if
          
          DeleteTokenPtr( head ) ;
          
          return head ;
        } // else
        
      } // else if
      else if ( head->mTokenType == QUOTE ) {
        // :: = QUOTE <S-exp>
        // the next should be a S-EXP
        
        head->mNext = GetSExp( hasError, false ) ;
        if ( hasError ) {
          // there has some errors
          
          DeleteTokenPtr( head ) ;
          
          return head ;
        } // if
        else { // if ( no error )
          // it is a S-EXP !
          // the Token string is a S-EXP !
          
          tail = GetTail( head ) ;
          hasError = false ;
          
          return head ;
        } // else
        
      } // else if
      else {
        // S-Expression should has <ATOM> or LAFT-PAREN or QUOTE to be first Token
        // but it isn't, so it should be an error.
        
        Error temp ;
        temp.mLine = head->mLine ;
        temp.mColumn = head->mColumn ;
        temp.mToken = * head->mToken ;
        temp.mErrorType = EXPECTLEFT ;
        mErrorVct->push_back( temp ) ;
        
        hasError = true ;
        DeleteTokenPtr( head ) ;
        
        return head ;
      } // else
      
    } // if
    else {
      // next Token has some errors
      
      if ( ! first ) {
        mErrorVct->at( mErrorVct->size() - 1 ).mErrorType = NOCLOSE ;
      } // if
      
      DeleteTokenPtr( head ) ;
      
      hasError = true ;
      
      return head ;
    } // else
    
  } // GetSExp()
  
  TokenPtr GetTail( TokenPtr head ) {
    if ( head->mNext == NULL )
      return head ;
    else
      return GetTail( head->mNext ) ;
      
  } // GetTail()
  
  void DeleteTokenPtr( TokenPtr & head ) {
    if ( head == NULL )
      return ;
    else if ( head->mNext == NULL ) {
      delete head ;
      head = NULL ;
      
      return ;
    } // else if
    else {
      DeleteTokenPtr( head->mNext ) ;
      delete head ;
      head = NULL ;
      
      return ;
    } // else
    
  } // DeleteTokenPtr()
    
  // A function which has to get a Token and return a TokenPtr that
  // point to a Token contain a string of this Token and
  // its TokenType and line and column.
  bool GetToken( TokenPtr & token ) {
    int i = 0 ;
    int index = 0 ;
    StringPtr temp = new string ;
    
    if ( mPeekedToken != NULL ) {
      token = mPeekedToken ;
      mPeekedToken = NULL ;
      
      return true ;
    } // if
    else { // if ( mPeekedToken == NULL )
      
      while ( mLoadedLine->empty() && ! cin.eof() ) {
        mLoadedLine = GetLine() ;
      } // while
      
      if ( ! cin.eof() ) {
        token = new Token ;
        
        if ( GetString( index, temp ) ) {
          mColumn = mColumn + index + 1 ;
          token->mToken = temp ;
          temp = NULL ;
          token->mLine = mLine ;
          token->mColumn = mColumn ;
          token->mTokenType = STRING ;
          
          mLoadedLine->erase( mLoadedLine->begin(), mLoadedLine->begin() + index + 1 ) ;
          
          return true ;
        } // if
        else if ( mLoadedLine->at( 0 ) == ';' ) {
          temp->clear() ;
          temp = NULL ;
          token = NULL ;
          
          mLoadedLine->clear() ;
          
          return GetToken( token ) ;
        } // else if
        else if ( mLoadedLine->at( 0 ) == '\'' ) {
          mColumn = mColumn + 1 ;
          temp->clear() ;
          temp->push_back( '\'' ) ;
          token->mToken = temp ;
          temp = NULL ;
          token->mLine = mLine ;
          token->mColumn = mColumn ;
          token->mTokenType = QUOTE ;
          
          mLoadedLine->erase( mLoadedLine->begin() ) ;
          
          return true ;
        } // else if
        else if ( mLoadedLine->at( 0 ) == '(' ) {
          mColumn = mColumn + 1 ;
          temp->clear() ;
          temp->push_back( '(' ) ;
          token->mToken = temp ;
          temp = NULL ;
          token->mLine = mLine ;
          token->mColumn = mColumn ;
          token->mTokenType = LEFTPAREN ;
          
          mLoadedLine->erase( mLoadedLine->begin() ) ;
          
          return true ;
        } // else if
        else if ( mLoadedLine->at( 0 ) == ')' ) {
          mColumn = mColumn + 1 ;
          temp->clear() ;
          temp->push_back( ')' ) ;
          token->mToken = temp ;
          temp = NULL ;
          token->mLine = mLine ;
          token->mColumn = mColumn ;
          token->mTokenType = RIGHTPAREN ;
          
          mLoadedLine->erase( mLoadedLine->begin() ) ;
          
          return true ;
        } // else if
        else if ( mLoadedLine->at( 0 ) == ' ' ||
                  mLoadedLine->at( 0 ) == '\t' || mLoadedLine->at( 0 ) == '\n' ) {
          
          mLoadedLine->erase( mLoadedLine->begin() ) ;
          mColumn++ ;
          
          return GetToken( token ) ;
        } // else if
        else {
          while ( temp->empty() && ! mLoadedLine->empty() ) {
            for ( i = 0 ; i < mLoadedLine->size() &&
                          mLoadedLine->at( i ) != '(' &&
                          mLoadedLine->at( i ) != ')' &&
                          mLoadedLine->at( i ) != '\'' &&
                          mLoadedLine->at( i ) != '\"' &&
                          mLoadedLine->at( i ) != ' ' &&
                          mLoadedLine->at( i ) != '\t' &&
                          mLoadedLine->at( i ) != '\n' &&
                          mLoadedLine->at( i ) != ';' ; i++ ) {
              temp->push_back( mLoadedLine->at( i ) ) ;
              
            } // for
            
            mLoadedLine->erase( mLoadedLine->begin(), mLoadedLine->begin() + i ) ;
            
            if ( temp->empty() && ! mLoadedLine->empty() ) {
              mLoadedLine->erase( mLoadedLine->begin() ) ;
              mColumn++ ;
            } // if
            
          } // while
          
          if ( temp->empty() ) { // mLoadedLine is empty
            return GetToken( token ) ;
          } // if
          else { // temp isn't empty
            if ( IsT( temp ) ) {
              mColumn = mColumn + temp->size() ;
              token->mToken = temp ;
              temp = NULL ;
              token->mLine = mLine ;
              token->mColumn = mColumn ;
              token->mTokenType = T ;
              
              return true ;
            } // if
            else if ( IsDot( temp ) ) {
              mColumn = mColumn + temp->size() ;
              token->mToken = temp ;
              temp = NULL ;
              token->mLine = mLine ;
              token->mColumn = mColumn ;
              token->mTokenType = DOT ;
              
              return true ;
            } // else if
            else if ( IsNIL( temp ) ) {
              mColumn = mColumn + temp->size() ;
              token->mToken = temp ;
              temp = NULL ;
              token->mLine = mLine ;
              token->mColumn = mColumn ;
              token->mTokenType = NIL ;
              
              return true ;
            } // else if
            else if ( IsFloat( temp ) ) {
              mColumn = mColumn + temp->size() ;
              token->mToken = temp ;
              temp = NULL ;
              token->mLine = mLine ;
              token->mColumn = mColumn ;
              token->mTokenType = FLOAT ;
              
              return true ;
            } // else if
            else if ( IsInteger( temp ) ) {
              mColumn = mColumn + temp->size() ;
              token->mToken = temp ;
              temp = NULL ;
              token->mLine = mLine ;
              token->mColumn = mColumn ;
              token->mTokenType = INT ;
              
              return true ;
            } // else if
            else {
              mColumn = mColumn + temp->size() ;
              token->mToken = temp ;
              temp = NULL ;
              token->mLine = mLine ;
              token->mColumn = mColumn ;
              token->mTokenType = SYMBOL ;
              
              return true ;
            } // else
            
          } // else
          
          
          
        } // else
        
      } // if
      else { // if ( cin.eof() )
        Error temp ;
        temp.mErrorType = HASEOF ;
        temp.mColumn = mColumn ;
        temp.mLine = mLine ;
        mErrorVct->push_back( temp ) ;
        
        return false ;
      } // else
      
    } // else
    
  } // GetToken()
  
  // A function to peek what next Token is ane use GetToken() to get a Token
  // and put it into mPeekedToken
  bool PeekToken( TokenType & tokenType ) {
    if ( mPeekedToken != NULL ) {
      
      tokenType = mPeekedToken->mTokenType ;
      
      return true ;
    } // if
    else {
      
      TokenPtr temp ;
      bool noError = GetToken( temp ) ;
      mPeekedToken = temp ;
      temp = NULL ;
      
      if ( noError ) {
        tokenType = mPeekedToken->mTokenType ;
        
        return true ;
      } // if
      else { // if ( hasError )
        
        return false ;
      } // else
      
    } // else
    
  } // PeekToken()
    
  StringPtr GetLine() {
    StringPtr lineIn = new string ;
    char temp ;
        
    temp = cin.get() ;
    while ( temp != '\n' && ! cin.eof() ) {
      lineIn->push_back( temp ) ;
      temp = cin.get() ;
    } // while
    
    mLine++ ;
    mColumn = 0 ;
        
    return lineIn ;
  } // GetLine()
  
  // return true if next Token is a string
  // and return its index in mLoadedLine
  // else return false.
  // must be start at index 0.
  bool GetString( int & index, StringPtr string ) {
    int escape = 0 ;
    index = 0 ;
    
    if ( mLoadedLine->at( index ) == '\"' ) {
      string->push_back( '"' ) ;
      for (  index = 1 ; index < mLoadedLine->size() ; index++ ) {
        if ( mLoadedLine->at( index ) == '\\' ) {
          for ( int j = index ; index < mLoadedLine->size() && mLoadedLine->at( j ) == '\\' ; j++ ) {
            escape++ ;
          } // for
          
          for ( int k = escape / 2 ; k > 0 ; k-- ) {
            string->push_back( '\\' ) ;
          } // for
          
          index = index + escape ;
          
          if ( escape % 2 == 1 ) {
            if ( index < mLoadedLine->size() ) {
              if ( mLoadedLine->at( index ) == 'n' ) {
                string->push_back( '\n' ) ;
                
              } // if
              else if ( mLoadedLine->at( index ) == 't' ) {
                string->push_back( '\t' ) ;
              } // else if
              else if ( mLoadedLine->at( index ) == '"' ) {
                string->push_back( '\"' ) ;
              } // else if
              else {
                index-- ;
                string->push_back( '\\' ) ;
              } // else
              
            } // if
            else {
              string->push_back( '\\' ) ;
            } // else
            
          } // if
          else {
            index-- ;
          } // else
          escape = 0 ;
          
        } // if
        else if ( mLoadedLine->at( index ) == '\"' ) {
          string->push_back( '\"' ) ;
          return true ;
        } // else if
        else {
          string->push_back( mLoadedLine->at( index ) ) ;
        } // else
        
      } // for
      return false ;
      
    } // if
    else
      return false ;
    
  } // GetString()
  
  // A function to check if a string is a Token whose Token type is Integer.
  bool IsInteger( StringPtr string ) {
    
    if ( string->at( 0 ) != '0' && string->at( 0 ) != '1' &&
         string->at( 0 ) != '2' && string->at( 0 ) != '3' &&
         string->at( 0 ) != '4' && string->at( 0 ) != '5' &&
         string->at( 0 ) != '6' && string->at( 0 ) != '7' &&
         string->at( 0 ) != '8' && string->at( 0 ) != '9' &&
         string->at( 0 ) != '+' && string->at( 0 ) != '-' ) {
      
      return false ;
    } // if
    
    for ( int i = 1 ; i < string->size() ; i++ ) {
      if ( string->at( i ) != '0' && string->at( i ) != '1' &&
           string->at( i ) != '2' && string->at( i ) != '3' &&
           string->at( i ) != '4' && string->at( i ) != '5' &&
           string->at( i ) != '6' && string->at( i ) != '7' &&
           string->at( i ) != '8' && string->at( i ) != '9' ) {
        
        return false ;
      } // if
      
    } // for
    
    return true ;
  } // IsInteger()
  
  // A function to check if a string is a Token whose Token type is Float.
  bool IsFloat( StringPtr string ) {
    int i = 0 ;
    bool hasDot = false ;
    
    if ( string->at( 0 ) != '0' && string->at( 0 ) != '1' &&
         string->at( 0 ) != '2' && string->at( 0 ) != '3' &&
         string->at( 0 ) != '4' && string->at( 0 ) != '5' &&
         string->at( 0 ) != '6' && string->at( 0 ) != '7' &&
         string->at( 0 ) != '8' && string->at( 0 ) != '9' &&
         string->at( 0 ) != '+' && string->at( 0 ) != '-' && string->at( 0 ) != '.' ) {
      
      return false ;
    } // if
    
    if ( string->at( 0 ) == '.' ) {
      hasDot = true ;
    } // if
    
    for ( i = 1 ; i < string->size() ; i++ ) {
      if ( string->at( i ) != '0' && string->at( i ) != '1' &&
           string->at( i ) != '2' && string->at( i ) != '3' &&
           string->at( i ) != '4' && string->at( i ) != '5' &&
           string->at( i ) != '6' && string->at( i ) != '7' &&
           string->at( i ) != '8' && string->at( i ) != '9' && string->at( i ) != '.' ) {
        
        return false ;
      } // if
      
      if ( string->at( i ) == '.' ) {
        if ( hasDot ) {
          return false ;
        } // if
        else {
          hasDot = true ;
        } // else
        
      } // if
      
    } // for
    
    return true ;
  } // IsFloat()
  
  bool IsNIL( StringPtr string ) {
    
    if ( string->size() == 3 && string->at( 0 ) == 'n' &&
        string->at( 1 ) == 'i' && string->at( 2 ) == 'l' ) {
      
      return true ;
    } // if
    else if ( string->size() == 2 && string->at( 0 ) == '#' && string->at( 1 ) == 'f' ) {
      
      return true ;
    } // else if
    else {
      
      return false ;
    } // else
    
  } // IsNIL()
  
  bool IsT( StringPtr string ) {
    
    if ( string->size() == 1 && string->at( 0 ) == 't' ) {
      
      return true ;
    } // if
    else if ( string->size() == 2 && string->at( 0 ) == '#' && string->at( 1 ) == 't' ) {
      
      return true ;
    } // else if
    else {
      
      return false ;
    } // else
    
  } // IsT()
  
  bool IsDot( StringPtr string ) {
    if ( string->size() == 1 && string->at( 0 ) == '.' ) {
      
      return true ;
    } // if
    else {
      
      return false ;
    } // else
    
  } // IsDot()
  
  bool IsAtom( TokenType tokenType ) {
    if ( tokenType == SYMBOL || tokenType == INT ||
         tokenType == FLOAT || tokenType == STRING ||
         tokenType == NIL || tokenType == T ) {
      
      return true ;
    } // if
    else {
      
      return false ;
    } // else
      
  } // IsAtom()
    
  void PrintError( bool & hasEof ) {
    for ( int i = 0 ; i < mErrorVct->size() ; i++ ) {
      if ( mErrorVct->at( i ).mErrorType == HASEOF ) {
        cout << "ERROR (no more input) : END-OF-FILE encountered" << endl ;
      } // if
      else if ( mErrorVct->at( i ).mErrorType == NOCLOSE ) {
        cout << "ERROR (no closing quote) : END-OF-LINE encountered at Line " <<
                mErrorVct->at( i ).mLine << " Column " << mErrorVct->at( i ).mColumn << endl ;
      } // else if
      else if ( mErrorVct->at( i ).mErrorType == EXPECTRIGHT ) {
        cout << "ERROR (unexpected token) : ')' expected when token at Line " <<
                mErrorVct->at( i ).mLine << " Column " << mErrorVct->at( i ).mColumn <<
                " is >>" << mErrorVct->at( i ).mToken << "<<" << endl ;
      } // else if
      else if ( mErrorVct->at( i ).mErrorType == EXPECTLEFT ) {
        cout << "ERROR (unexpected token) : atom or '(' expected when token at Line " <<
                mErrorVct->at( i ).mLine << " Column " << mErrorVct->at( i ).mColumn <<
                " is >>" << mErrorVct->at( i ).mToken << "<<" << endl ;
      } // else if
      else {
        cout << "ERROR (unexpected error)" << endl ;
      } // else
      
    } // for
    
    mErrorVct->clear() ;
    mLoadedLine->clear() ;
    mLine = 0 ;
    mColumn = 0 ;
    
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
    
  bool PrintSExp( SExpressionPtr sExp ) {
    TokenPtr walk = sExp->mTokenString ;
    
    while ( walk != NULL ) {
      cout << * walk->mToken ;
      
      walk = walk->mNext ;
    } // while
    
    return true ;
  } // PrintSExp()
    
  bool Eval( SExpressionPtr sExp, bool & notEnd ) {
    if ( IsExit( sExp->mTokenString ) ) {
      notEnd = false ;
      return true ;
    } // if
    else {
      // Error temp( OTHERS ) ;
      // mErrorVct->push_back( temp ) ;
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
  SExpressionPtr sExpPtr = NULL ;
  Scanner scanner ;
  Parser parser ;
    
  cout << "Welcome to OurScheme!" ;
  
  cin >> inputID ;
  
  while ( notEnd && inputID == 1 ) {    // not exit && not EOF
    cout <<  "\n> " ;
    
    delete sExpPtr ;
    sExpPtr = NULL ;
    
    if ( scanner.ReadSExp( sExpPtr ) ) {
      parser.PrintSExp( sExpPtr ) ; // pretty print
      
      if ( parser.Eval( sExpPtr, notEnd ) )
        ; // parser.PrintSExp( sExpPtr ) ;
      else
        ; // parser.PrintError() ;
      
    } // if
    else
      scanner.PrintError( hasEof ) ;
     
  } // while

  // printf( "\nThanks for using OurScheme!" ) ;
    
  return 0 ;
} // main()
