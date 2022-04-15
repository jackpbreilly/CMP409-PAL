//
//  PALScanner.hpp
//  CompilerKit
//
//  Created by Amy Parent on 19/01/2022.
//  Based on Allan Milne's work
//
//  Description:
//  The language PAL is defined in the file PAL.EBNF.txt.
//  This scanner implementation is built on version 2 of the Ardkit compiler toolkit framework.
//  This version of the scanner uses a state transition FSM algorithm.
//  See 'PAL.FSM.txt' for the FSM.
//  Invalid characters and tokens are identified and appropriate tokens returned.

#ifndef PALScanner_hpp
#define PALScanner_hpp

#include "CompilerKit/Scanner.hpp"

using namespace CompilerKit;

class PALScanner : public Scanner {
public:
    using Scanner::Scanner;
    virtual ~PALScanner() {}
    
    virtual Token lex() override;
private:
};

#endif /* PALScanner_hpp */
