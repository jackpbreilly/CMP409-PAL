//
//  PALSemantics.hpp - Semantic Analyser Demo for Block1
//  CompilerKit
//
//  Created by Amy Parent on 26/11/2021.
//
#ifndef _PALPARSERSEMANTICS_HPP_
#define _PALPARSERSEMANTICS_HPP_

#include "CompilerKit/Sema.hpp"
using namespace CompilerKit;

class PALSemantics : public Sema {
public:
    PALSemantics(Parser& parser);
    virtual ~PALSemantics();
    
    void define(const Token& var, Type type);
    
    Type checkVariable(const Token& var);
    Type variableType(const Token& var);
    
    Type checkExpression(Type lhs, const Token& op, Type rhs);
    void checkDefined(const Token& token);
    void checkBoolean(Type lhs, const Token& op, Type rhs);
    void checkAssign(const Token& var, Type rhs);
};

#endif /* ifndef _PALSemantics_HPP_ */
