#include "stdafx.h"
#include "ScriptExpressionParser.h"

namespace Logic
{
   namespace Scripts
   {
      namespace Compiler
      {
         // -------------------------------- CONSTRUCTION --------------------------------

         ScriptExpressionParser::ScriptExpressionParser(TokenIterator& begin, TokenIterator& end, TokenArray& params)
         {
         }


         ScriptExpressionParser::~ScriptExpressionParser()
         {
         }

         // ------------------------------- STATIC METHODS -------------------------------

         // ------------------------------- PUBLIC METHODS -------------------------------

         // ------------------------------ PROTECTED METHODS -----------------------------

         // ------------------------------- PRIVATE METHODS ------------------------------

         bool  ScriptExpressionParser::MatchLiteral(const TokenIterator& pos)
         {
            if (pos >= End)
               return false;
            
            switch (pos->Type)
            {
            default:
               return false;
            }
         }

         bool  ScriptExpressionParser::Match(const TokenIterator& pos, TokenType  type)
         {
            return pos < Lexer.Tokens.end() && pos->Type == type;
         }

         bool  ScriptExpressionParser::Match(const TokenIterator& pos, TokenType  type, const TCHAR* txt)
         {
            return Match(pos, type) && pos->Text == txt;
         }

         bool MatchValue(TokenIterator& pos)
         {
            // Literal
            if (MatchLiteral(pos))
               return true;

            // ( expr )
            if (Match(pos, TokenType::Operator, L"("))
               return MatchExpression(++pos) && Match(pos, TokenType::Operator, L")") ? (++pos, true) : (pos=origin, false);
         }

         ////

         Expression  MatchExpression(TokenIterator& pos)
         {
            // Expression = Sum
            return MatchSum(pos);
         }

         Sum  MatchSum(TokenIterator& pos)
         {
            // Sum = Product (('+' / '-') Product)*
            Sum sum(MatchProduct(pos));

            // Check for ... (('+' / '-') Product)*
            while (op = MatchOperator(pos, plus) || op = MatchOperator(pos, minus) && product = MatchProduct(pos))
               sum.Add(op, product);

            return sum;
         }

         Product  MatchProduct(TokenIterator& pos)
         {
            // Product = Unary (('*' / '/') Unary)*
            Product prod(MatchUnary(pos));

            // Check for ... (('*' / '/') Unary)*
            while (op = MatchOperator(pos, multiply) || op = MatchOperator(pos, divide) && unary = MatchUnary(pos))
               prod.Add(op, unary);

            return prod;
         }

         Unary MatchUnary(TokenIterator& pos)
         {
            // Unary = '!' / '-' / '~' Value
            if (op = MatchOperator(pos, not) || op = MatchOperator(pos, unary_minus))
               return Unary(op, MatchValue(pos));
            
            // Unary = Value
            return Unary(MatchValue(pos));
         }

         Value MatchValue(TokenIterator& pos)
         {
            // Value = literal
            if (val = MatchLiteral(pos))
               return Value(val);

            // Value = '(' expr ')'
            if (open = MatchOperator(pos, open_bracket) && expr = MatchExpression(pos) && close = MatchOperator(pos, close_bracket))
               return Value(open, expr, close);
            
            throw;
         }

      }
   }
}
