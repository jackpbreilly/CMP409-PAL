//
//  PALScanner.cpp
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

#include "PALScanner.hpp"
#include <string>
#include <set>
#include <sstream>

Token PALScanner::lex() {
    // list of (case-sensitive) keywords used in the language.
    static const std::set<std::string> keywords = {
        "PROGRAM", "WITH", "IN", "END",
        "AS", "INTEGER", "REAL", "UNTIL", "REPEAT", "ENDLOOP",
        "IF", "THEN", "ELSE", "ENDIF", "INPUT", "OUTPUT"
    };
    
    // Enum that lets us keep track of our state machine
    enum class State {
        Start           = 0,
        Identifier      = 1,
        Integer         = 2,
        Real            = 3,
        Punct           = 4,
        EndOfFile       = 98,
        InvalidChar     = 99,
    };
    
    State state = State::Start;
    std::stringstream buf;
    
    while(true) {
        switch(state) {
            case State::Start:
                if(std::isspace(peek())) {
                    state = State::Start;
                } else {
                    updateTokenStart(); // This ensures we don't start the token on whitespace
                    if(std::isalpha(peek())) state = State::Identifier;
                    else if(std::isdigit(peek())) state = State::Integer;
                    else if(peek() == '\0') state = State::EndOfFile;
                    else if(std::strchr("+-*/(),=<>", peek()) != nullptr) state = State::Punct;
                    else state = State::InvalidChar;
                }
                break;
                
            case State::Identifier:
                if(std::isalnum(peek())) {
                    state = State::Identifier;
                } else {
                    // We have an identifier, but we must check first if it is a keyword or not
                    std::string str = buf.str();
                    if(keywords.find(str) != keywords.end()) {
                        return makeToken(str);
                    } else {
                        return makeToken(Token::Identifier, str);
                    }
                }
                break;
                
            case State::Integer:
                if(std::isdigit(peek())) {
                    state = State::Integer;
                } else if(peek() == '.') {
                    state = State::Real;
                } else {
                    return makeToken(Token::Integer, buf.str());
                }
                break;
                
            case State::Real:
                if(std::isdigit(peek())) {
                    state = State::Real;
                } else {
                    return makeToken(Token::Real, buf.str());
                }
                break;
                
            case State::Punct:
                return makeToken(buf.str());
                
            case State::EndOfFile:
                return makeToken(Token::EndOfFile);
                
            case State::InvalidChar:
                return makeToken(Token::InvalidChar);
        }
        
        if(state != State::Start) {
            // We are parsing a token, so we keep adding to our token string's buffer.
            buf << peek();
        }
        advance();
    }
    return makeToken(Token::InvalidToken);
}
