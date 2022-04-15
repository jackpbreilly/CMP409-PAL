#include "PALParser.hpp"
#include "CompilerKit/Sema.hpp"


void PALParser::recStarter() {
    recProgram();

}

void PALParser::recProgram() 
{
   
    Scope::open();
    expect("PROGRAM");
    expect(Token::Identifier); //program name - no semantic
    expect("WITH");
    recVarDecls();
    expect("IN");
    recStatement();
    expect("END");
    Scope::close();
    
}


void PALParser::recVarDecls()
{
        Type type;

        std::vector<Token> identList;
        while (have(Token::Identifier))
        {
            identList = recIdentList();
            expect("AS");
            if (match("INTEGER")) {
                type = Type::Integer;
            }
            else if (match("REAL")) {
                type = Type::Real;
            }

            if (type != Type::Invalid)
            {
                for(Token token : identList)
                {
                    semantics.define(token, type);
                }
            }
        }
}


std::vector<Token> PALParser::recIdentList()
{
    std::vector<Token> identList;

    identList.push_back(current());
    expect(Token::Identifier);

    while (have(","))
    {
        expect(",");
        identList.push_back(current());
        expect(Token::Identifier);
    }

    return identList;
}

void PALParser::recStatement()
{
    do
    {
        if (have(Token::Identifier)) recAssignment();
        else if (have("UNTIL")) recLoop();
        else if (have("IF")) recConditional();
        else if (have("INPUT") || have("OUTPUT")) recIO();
        else syntaxError("<Statement>");
    } while (have(Token::Identifier) || have("UNTIL") || have("IF") || have("INPUT") || have("OUTPUT"));
}

void PALParser::recAssignment()
{
    string type = current().type();
    expect(Token::Identifier);
    expect("=");
    Token assignToken = current();
    Type exprType = recExpression();
}

Type PALParser::recExpression()
{
    auto type = recTerm();
    Token op = current();

    // We could also do this recursively -- try and do it that way too!
    while (match("+") || match("-")) {
        auto rhs = recTerm();
        type = semantics.checkExpression(type, op, rhs);
        op = current();
    }
    return type;
}

Type PALParser::recTerm() {
    auto type = recFactor();
    Token op = current();
    while (match("*") || match("/")) {
        auto rhs = recFactor();
        type = semantics.checkExpression(type, op, rhs);
        op = current();
    }
    return type;
}

Type PALParser::recFactor() {
    if (have(Token::Identifier)) {
        Token var = current();
        expect(Token::Identifier);
        return semantics.checkVariable(var);
    }
    else if (match(Token::Integer)) {
        return Type::Integer;
    }
    else if (match(Token::Real)) {
        return Type::Real;
    }
    else if (match("(")) {
        auto type = recExpression();
        expect(")");
        return type;
    }
    else {
        syntaxError("<factor>");
        return Type::Invalid;
    }
}

void PALParser::recLoop() {
    Type booleanExpr = recBooleanExpr();
    expect("REPEAT");

    if (booleanExpr != Type::Invalid)
    {
        while (have(Token::Identifier) || have("UNTIL") || have("IF") || have("INPUT") || have("OUTPUT"))
        {
            recStatement();
        }
    }


    expect("ENDLOOP");
}

Type PALParser::recBooleanExpr()
{
    auto type = recExpression();
    Token op = current();

    if (match("<") || match("=") || match(">")) {
        auto rhs = recExpression();
        type = semantics.checkExpression(type, op, rhs);
    }
    return type;
}

void PALParser::recConditional() {
    recBooleanExpr();
    expect("THEN");
    while (have(Token::Identifier) || have("UNTIL") || have("IF") || have("INPUT") || have("OUTPUT"))
    {
        recStatement();
    }

    if (have("ELSE"))
    {
        expect("ELSE");
        while (have(Token::Identifier) || have("UNTIL") || have("IF") || have("INPUT") || have("OUTPUT"))
        {
            recStatement();
        }
    }

    expect("ENDIF");
}

void PALParser::recIO()
{
    if (have("INPUT"))
    {
        expect("INPUT");
        std::vector<Token> identLists = recIdentList();
        for (Token token : identLists){
          
        }
        }
    else if (have("OUTPUT"))
    {
        expect("OUTPUT");
        Type expression = recExpression();
        while (have(","))
        {
            expect(",");
            recExpression();
        }
    }
}