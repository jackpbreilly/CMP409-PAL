#ifndef _PALPARSER_HPP_
#define _PALPARSER_HPP_

#include "CompilerKit/Parser.hpp"
#include "PALSemantics.hpp"

using namespace CompilerKit;

class PALParser : public Parser {
public:
    
    PALParser(Scanner& scanner) : Parser(scanner) {}
    virtual ~PALParser() {}
    
private:
    PALSemantics semantics = PALSemantics(*this);
    PALParser();
    virtual void recStarter() override;
    
    void recProgram();
    void recVarDecls();
    std::vector<Token> recIdentList();
    void recStatement();
    void recAssignment();
    Type recExpression();
    Type recTerm();
    Type recFactor();
    Type recValue();
    void recLoop();
    Type recBooleanExpr();
    void recConditional();
    void recIO();
};

#endif /* ifndef _PALPARSER_HPP_ */