#include "PALParser.hpp"
#include "CompilerKit/Sema.hpp"


void PALParser::recStarter() {
    recProgram();

}

void PALParser::recProgram() 
{
   
    //Scope::open();
    //expect("PROGRAM");
    //expect(Token::Identifier);
    //expect("WITH");
    //recVarDecls();
    //expect("IN");
    //recStatement();
    //expect("END");
    //Scope::close();

    Scope::open();
    if (have("PROGRAM"))
    {
        expect("PROGRAM");
        expect(Token::Identifier);
        expect("WITH");
        recVarDecls();
        expect("IN");

        do {
            recStatement();
        } while (have(Token::Identifier) || have("UNTIL") || have("IF") || have("INPUT") || have("OUTPUT"));

        expect("END");
    }
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
   /* do
    {
        if (have(Token::Identifier)) recAssignment();
        else if (have("UNTIL")) recLoop();
        else if (have("IF")) recConditional();
        else if (have("INPUT") || have("OUTPUT")) recIO();
        else syntaxError("<Statement>");
    } while (have(Token::Identifier) || have("UNTIL") || have("IF") || have("INPUT") || have("OUTPUT"));*/


    //semantics..ResetCurrentType();
    if (have(Token::Identifier))
    {
        recAssignment();
    }
    else if (have("UNTIL"))
    {
        recLoop();
    }
    else if (have("IF"))
    {
        recConditional();
    }
    else if (have("INPUT") || have("OUTPUT"))
    {
        recIO();
    }
    else
    {
        syntaxError("<Statement>");
    }
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
    //auto type = recTerm();
    //Token op = current();

    //// We could also do this recursively -- try and do it that way too!
    //while (match("+") || match("-")) {
    //    auto rhs = recTerm();
    //    type = semantics.checkExpression(type, op, rhs);
    //    op = current();
    //}
    //return type;

    Type type = recTerm();

    while (have("+") || have("-"))
    {
        if (have("+"))
        {
            expect("+");
        }
        else if (have("-"))
        {
            expect("-");
        }

        Token exprToken = current();
        Type termType2 = recTerm();

        type = semantics.checkExpression(type, exprToken, termType2);
    }

    return type;
}

Type PALParser::recTerm() {
    //auto type = recFactor();
    //Token op = current();
    //while (match("*") || match("/")) {
    //    auto rhs = recFactor();
    //    type = semantics.checkExpression(type, op, rhs);
    //    op = current();
    //}
    //return type;

    Token termToken1 = current();
    Type factType = recFactor();

    while (have("*") || have("/"))
    {
        if (have("*"))
        {
            expect("*");
        }
        else if (have("/"))
        {
            expect("/");
        }

        Token termToken = termToken1 = current();
        Type factType2 = recFactor();


        if (factType != Type::Invalid)
        {
            semantics.checkExpression(factType, termToken1, factType2);
        }
    }

    return factType;


}

Type PALParser::recFactor() {
   /* if (have(Token::Identifier)) {
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
    }*/

    if (have("+")) {
        expect("+");
    }
    else if (have("-"))
    {
        expect("-");
    }

    if (have(Token::Identifier) || have(Token::Real) || have(Token::Integer))
    {
        Type valType = recValue();
        return valType;
    }
    else if (have("("))
    {
        expect("(");
        Type exprType = recExpression();
        expect(")");
        return exprType;
    }
    else
    {
        syntaxError("Value or Expression");
        return semantics.variableType(current());
    }
}

Type PALParser::recValue() {
    Type valType;

    if (have(Token::Identifier))
    {
        valType = semantics.variableType(current());
        expect(Token::Identifier);
    }
    else if (have(Token::Integer))
    {
        valType = semantics.variableType(current());
        expect(Token::Integer);
    }
    else if (have(Token::Real))
    {
        valType = semantics.variableType(current());
        expect(Token::Real);
    }
    else
    {
        valType = semantics.variableType(current());
    }

    return valType;
}

void PALParser::recLoop() {
    //Type booleanExpr = recBooleanExpr();
    //expect("REPEAT");

    //if (booleanExpr != Type::Invalid)
    //{
    //    while (have(Token::Identifier) || have("UNTIL") || have("IF") || have("INPUT") || have("OUTPUT"))
    //    {
    //        recStatement();
    //    }
    //}


    //expect("ENDLOOP");

    expect("UNTIL");
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
    //auto type = recExpression();
    //Token op = current();

    //if (match("<") || match("=") || match(">")) {
    //    auto rhs = recExpression();
    //    type = semantics.checkExpression(type, op, rhs);
    //}
    //return type;

    Type exprType = recExpression();
    if (have("<"))
    {
        expect("<");
    }
    else if (have("="))
    {
        expect("=");
    }
    else if (have(">"))
    {
        expect(">");
    }

    Token boolToken = current();
    Type exprType2 = recExpression();

    Type booleanExprType = Type::Invalid;

    if (exprType != Type::Invalid)
    {
        booleanExprType = semantics.checkExpression(exprType, boolToken, exprType2);
    }

    return booleanExprType;
}

void PALParser::recConditional() {
    //recBooleanExpr();
    //expect("THEN");
    //while (have(Token::Identifier) || have("UNTIL") || have("IF") || have("INPUT") || have("OUTPUT"))
    //{
    //    recStatement();
    //}

    //if (have("ELSE"))
    //{
    //    expect("ELSE");
    //    while (have(Token::Identifier) || have("UNTIL") || have("IF") || have("INPUT") || have("OUTPUT"))
    //    {
    //        recStatement();
    //    }
    //}

    //expect("ENDIF");

    expect("IF");
    Type booleanExpr = recBooleanExpr();
    expect("THEN");

    if (booleanExpr != Type::Invalid)
    {
        while (have(Token::Identifier) || have("UNTIL") || have("IF") || have("INPUT") || have("OUTPUT"))
        {
            recStatement();
        }
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
            semantics.checkVariable(token);
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