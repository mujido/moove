//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_STMT_AST_HPP
#define MOOVE_STMT_AST_HPP

#include "expr_ast.hpp"

namespace Moove {

struct ASTVisitor;

///Contains all AST classes used for representing statements
namespace Stmt {

class Stmt;

///Abstract base for all statement AST classes
struct Stmt {
   virtual ~Stmt()
   {}

   /**
    * \brief Visit a statement tree
    * \param visitor An ASTVisitor object to visit the tree
    */
   virtual void accept(ASTVisitor& visitor)const = 0;
};

/**
 * \brief Contains a collection of Stmt objects
 * 
 * This container will hold Stmt objects in sequential order as the original
 * source code presented them.
 */
using Block = std::vector<std::unique_ptr<Stmt>>;

///Represent an if statement
class If : public Stmt {
public:
   ///Represent an elseif/else clauses
   class ElseIf {
   private:
      std::unique_ptr<Expr::Expr> m_test;
      Block m_body;

      ElseIf(const ElseIf&) = delete;
      ElseIf& operator= (const ElseIf&) = delete;
      
   public:
      /**
       * \brief Construct ElseIf object (for use as an else clause)
       * \param body Statements found within this section of the if statement
       */
      ElseIf(Block&& body) : m_body(std::move(body))
      {}

      /**
       * \brief Construct an ElseIf object
       * \param test Test condition. Can be 0 if this is actually for an 
       *             else clause
       * \param body Statements found within this section of the if statement
       */
      ElseIf(std::unique_ptr<Expr::Expr> test, Block&& body) : 
	      m_test(std::move(test)), m_body(std::move(body))
      {}

      ElseIf() = default;
      ElseIf(ElseIf&&) = default;
      ElseIf& operator= (ElseIf&&) = default;

      /**
       * \brief Determine if a test condition was supplied
       * \return Whether or not a test condition was supplied
       */
      bool hasTest()const
      { return m_test.get() != 0; }

      /**
       * \brief Retrieve test condition expression
       * \return Expr::Expr object to use for test condition
       *
       * \warning
       * hasTest() should be used prior to this method to ensure that a
       * valid default value was set. Otherwise a null pointer access will
       * occur.
       */
      const Expr::Expr& test()const
      { return *m_test; }

      /**
       * \brief Retrieve statement body
       * \return Block of Stmt objects
       */
      const Block& body()const
      { return m_body; }

      /**
       * \brief Visit an elseif/else clause
       * \param visitor An ASTVisitor object to visit the tree
       */
      void accept(ASTVisitor& visitor)const;
   };

   ///Provided for readability in parser code
   typedef ElseIf Else;

   ///Contains a collection of ElseIf objects for elseif/else clauses
   typedef std::vector<ElseIf> ElseList;

private:
   std::unique_ptr<Expr::Expr> m_test;
   Block      m_body;
   ElseList   m_elseList;

   If(const If&) = delete;
   If& operator= (const If&) = delete;

public:
   /**
    * \brief Construct If object
    * \param test Test expression for if clause of statement
    * \param body Statement body for if clause
    * \param elseList Collection of elseif/else clauses attached to statement
    */
   If(std::unique_ptr<Expr::Expr> test, 
      Block&& body, 
      ElseList&& elseList) :
      m_test(std::move(test)), m_body(std::move(body)), m_elseList(std::move(elseList))
   {}

   If(If&&) = default;
   If& operator= (If&&) = default;

   /**
    * \brief Retrieve test condition
    * \return Expr::Expr object for test condition
    */
   const Expr::Expr& test()const
   { return *m_test; }

   /**
    * \brief Retrieve if clause's statement body
    * \return Block of statements
    */
   const Block& body()const
   { return m_body; }

   /**
    * \brief Retrieve collection of elseif/else clauses for statement
    * \return ElseList containing elseif/else clauses
    */
   const ElseList& elseList()const
   { return m_elseList; }

   void accept(ASTVisitor& visitor)const;
};

///Represent while statement
class While : public Stmt {
private:
   Symbol m_id;
   Block m_body;
   std::unique_ptr<Expr::Expr> m_test;

public:
   /**
    * \brief Construct While object
    * \param id Syntactically optional identifier for this loop. Should be
    *           <tt>Symbol()</tt> if no identifier is needed
    * \param test Test condition
    * \param body Statement body
    */
   While(Symbol id, std::unique_ptr<Expr::Expr> test, Block&& body) : 
      m_id(id), m_body(std::move(body)), m_test(std::move(test))
   {}
   
   /**
    * \brief Retrieve identifier for this loop
    * \return Symbol value for identifier
    *
    * Result will be <tt>Symbol()</tt> if no identifier given
    */
   Symbol id()const
   { return m_id; }

   /**
    * \brief Retrieve statement body
    * \return Block of statements
    */
   const Block& body()const
   { return m_body; }

   /**
    * \brief Retrieve test condition of loop
    * \return Expr::Expr object for test condition
    */
   const Expr::Expr& test()const
   { return *m_test; }

   void accept(ASTVisitor& visitor)const;
};

///Represent a for statement that iterates over a list
class ForList : public Stmt {
private:
   Symbol m_id;
   std::unique_ptr<Expr::Expr> m_expr;
   Block m_body;

public:
   /**
    * \brief Construct ForList object
    * \param id Variable to assign current loop value to through iterations
    * \param expr An expression that evaluates to a list to iterate through
    * \param body Statement body
    */
   ForList(Symbol id, std::unique_ptr<Expr::Expr> expr, Block&& body) : 
      m_id(id), m_expr(std::move(expr)), m_body(std::move(body))
   {}

   /**
    * \brief Retrieve loop variable
    * \return Symbol value for loop variable
    * 
    * Result will be <tt>Symbol()</tt> if no identifier given
    */
   Symbol id()const
   { return m_id; }

   /**
    * \brief Retrieve list expression to iterate over
    * \return Expr::Expr object for list expression
    */
   const Expr::Expr& expr()const
   { return *m_expr; }

   /**
    * \brief Retrieve statement body
    * \return Block of Statements
    */
   const Block& body()const
   { return m_body; }

   void accept(ASTVisitor& visitor)const;
};

///Represent a for statement that iterates over a range
class ForRange : public Stmt {
private:
   Symbol m_id;
   std::unique_ptr<Expr::Expr> m_start;
   std::unique_ptr<Expr::Expr> m_end;
   Block m_body;

public:
   /**
    * \brief Construct ForRange object
    * \param id Variable to assign current loop value to through iterations
    * \param start Expression for initial value of loop
    * \param end Expression for final value of loop
    * \param body Statement body
    */
   ForRange(Symbol id,
	    std::unique_ptr<Expr::Expr> start, 
	    std::unique_ptr<Expr::Expr> end, 
	    Block&& body) :
      m_id(id), m_start(std::move(start)), m_end(std::move(end)), m_body(std::move(body))
   {}

   /**
    * \brief Retrieve loop variable
    * \return Symbol value for loop variable
    */
   Symbol id()const
   { return m_id; }

   /**
    * \brief Retrieve expression for initial value of loop
    * \return Expr::Expr object for initial value of loop
    */
   const Expr::Expr& start()const
   { return *m_start; }

   /**
    * \brief Retrieve expression for final value of loop
    * \return Expr::Expr object for final value of loop
    */
   const Expr::Expr& end()const
   { return *m_end; }

   /**
    * \brief Retrieve statement body
    * \return Block of statements
    */
   const Block& body()const
   { return m_body; }

   void accept(ASTVisitor& visitor)const;
};

///Represent a try/except statement
class TryExcept : public Stmt {
public:
   ///Represent an except clause to a try/except statement
   class Except {
   private:
      Symbol m_id;
      Expr::ArgList m_codes;
      Block m_body;

      Except(const Except&) = delete;
      Except& operator= (const Except&) = delete;
      
   public:
      Except(Except&&) = default;
      Except& operator= (Except&&) = default;

      /**
       * \brief Construct Except object
       * \param id Variable to assign exception info to. Can be
       *           <tt>Symbol()</tt> if no such variable needed.
       * \param codes List of exceptions expressions to handle
       * \param body Statement body
       */
      Except(Symbol id, Expr::ArgList&& codes, Block&& body) : 
         m_id(id), m_codes(std::move(codes)), m_body(std::move(body))
      {}

      /**
       * \brief Retrieve exception info variable
       * \return Symbol value for exception info variable
       *
       * Will return Symbol() if was not given
       */
      Symbol id()const
      { return m_id; }

      /**
       * \brief Retrieve exception expressions to handle
       * \return Expr::ArgList of exception Expr::Expr objects
       */
      const Expr::ArgList& codes()const
      { return m_codes; }

      /**
       * \brief Retrieve exception clause body
       * \return Block of statements
       */
      const Block& body()const
      { return m_body; }

      /**
       * \brief Visit an except clause
       * \param visitor An ASTVisitor object to visit the tree
       */
      void accept(ASTVisitor& visitor)const;
   };

   ///Contain a collection of exception clauses
   typedef std::vector<Except> ExceptList;

private:
   Block m_body;
   ExceptList m_excepts;

public:
   /**
    * \brief Construct TryExcept object
    * \param body Try clause body to monitor for exceptions
    * \param excepts Collection of except clauses attached to statement
    */
   TryExcept(Block&& body, ExceptList&& excepts) : 
      m_body(std::move(body)), m_excepts(std::move(excepts))
   {}

   /**
    * \brief Retrieve try clause body
    * \return Block of statements
    */
   const Block& body()const
   { return m_body; }

   /**
    * \brief Retrieve collection of except clauses
    * \return ExceptList of Except clauses
    */
   const ExceptList& excepts()const
   { return m_excepts; }

   void accept(ASTVisitor& visitor)const;
};

///Represent try/finally statement
class TryFinally : public Stmt {
private:
   Block m_body;
   Block m_finally;

public:
   /**
    * \brief Construct TryFinally object
    * \param body Try clause body to monitor for exceptions
    * \param finally Finally clause body
    */
   TryFinally(Block&& body, Block&& finally) :
      m_body(std::move(body)), m_finally(std::move(finally))
   {}

   /**
    * \brief Retrieve try clause body
    * \return Block of statements
    */
   const Block& body()const
   { return m_body; }

   /**
    * \brief Retrieve finally clause body
    * \return Block of statements
    */
   const Block& finally()const
   { return m_finally; }

   void accept(ASTVisitor& visitor)const;
};

///Represent a fork statement
class Fork : public Stmt {
private:
   Symbol m_id;
   std::unique_ptr<Expr::Expr> m_delay;
   Block m_body;

public:
   /**
    * \brief Construct Fork object
    * \param id Syntactically optional variable to assign fork information to.
                Can be <tt>Symbol()</tt> if not needed.
    * \param delay Expression for time delay
    * \param body Statement body
    */
   Fork(Symbol id, std::unique_ptr<Expr::Expr>&& delay, Block&& body) : 
      m_id(id), m_delay(std::move(delay)), m_body(std::move(body))
   {}


    /**
    * \brief Retrieve fork info variable
    * \return Symbol value for fork info variable
    *
    * Will return Symbol() if was not given
    */
   Symbol id()const
   { return m_id; }

   /**
    * \brief Retrieve time delay expression
    * \return Expr::Expr object for time delay
    */
   const Expr::Expr& delay()const
   { return *m_delay; }

   /**
    * \brief Retrieve statement body
    * \return Block of statements
    */
   const Block& body()const
   { return m_body; }

   void accept(ASTVisitor& visitor)const;
};

///Represent a switch statement
class Switch : public Stmt {
public:
   ///Represent a case/default clause in a switch statement
   class Case {
   private:
      std::unique_ptr<Expr::Expr> m_expr;
      Block m_body;

      Case(const Case&) = delete;
      Case& operator= (const Case&) = delete;
      
   public:
      Case(Case&&) = default;
      Case& operator= (Case&&) = default;

      /**
       * \brief Construct Case object (for use as a default clause)
       * \param defaultBody Clause body
       */
      Case(Block&& defaultBody) : m_body(std::move(defaultBody))
      {}

      /**
       * \brief Construct Case object
       * \param expr Case test expression. Can be 0 for default clause
       * \param body Clause body
       */
      Case(std::unique_ptr<Expr::Expr>&& expr, Block&& body) : 
         m_expr(std::move(expr)), m_body(std::move(body))
      {}

      /**
       * \brief Determine if this a default clause
       * \return Whether or not this is a default clause
       * 
       * A default clause is a Case object with a null test expression, created
       * manually or through the special constructor for default clauses.
       */
      bool isDefault()const
      { return !m_expr.get(); }

      /**
       * \brief Retrieve test expression
       * \return Expr::Expr object for test
       */
      const Expr::Expr& expr()const
      { return *m_expr; }

      /**
       * \brief Retrieve clause body
       * \return Block of statements
       */
      const Block& body()const
      { return m_body; }
      
      /**
       * \brief Visit a case/default clause
       * \param visitor An ASTVisitor object to visit the tree
       */
      void accept(ASTVisitor& visitor)const;
   };

   ///Done for readability in parser code
   typedef Case Default;

   ///Contain a collection of case/default clauses
   typedef std::vector<Case> CaseList;

private:
   std::unique_ptr<Expr::Expr> m_expr;
   CaseList m_cases;

public:
   /**
    * \brief Construct Switch object
    * \param expr Expression to compare to case clause test expressions
    * \param cases Collection of case/default clauses
    */
   Switch(std::unique_ptr<Expr::Expr> expr, CaseList&& cases) :
      m_expr(std::move(expr)), m_cases(std::move(cases))
   {}

   /**
    * \brief Retrieve switch expression used for testing
    * \return Expr::Expr object used in switch testing
    */
   const Expr::Expr& expr()const
   { return *m_expr; }

   /**
    * \brief Retrieve collection of case/default clauses
    * \return CaseList of Case objects
    */
   const CaseList& cases()const
   { return m_cases; }

   void accept(ASTVisitor& visitor)const;
};

///Represent a return statement
class Return : public Stmt {
private:
   std::unique_ptr<Expr::Expr> m_expr;

public:
   ///Construct Return object (with no return value)
   Return() : m_expr(nullptr)
   {}

   /**
    * \brief Construct Return object
    * \param expr Expression to return. Can be 0 if no return value
    */
   explicit Return(std::unique_ptr<Expr::Expr> expr) : m_expr(std::move(expr))
   {}

   /**
    * \brief Determine if a return value was given
    * \return Whether or not a return value was given
    */
   bool hasExpr()const
   { return m_expr.get(); }

   /**
    * \brief Retrieve return value expression
    * \return Expr::Expr object for return value
    *
    * \warning
    * hasExpr() should be used prior to this method to ensure that a
    * valid default value was set. Otherwise a null pointer access will
    * occur.
    */
   const Expr::Expr& expr()const
   { return *m_expr; }

   void accept(ASTVisitor& visitor)const;
};

///Represent a continue statement
class Continue : public Stmt {
private:
   Symbol m_id;

public:
   /**
    * \brief Construct Continue object
    * \param id Identifier of the loop to perform a continue on. If
    *           Symbol(), will use inner most loop.
    */
   explicit Continue(Symbol id = Symbol()) : m_id(id)
   {}

   /**
    * \brief Retrieve loop identifier
    * \return Symbol value for loop to continue. Symbol() is returned if
    *         inner most loop is to be used.
    */       
   Symbol id()const
   { return m_id; }

   void accept(ASTVisitor& visitor)const;
};

///Represent a break statement
class Break : public Stmt {
private:
   Symbol m_id;

public:
   /**
    * \brief Construct Break object
    * \param id Identifier of the loop to break out of. If
    *           Symbol(), will use inner most loop.
    */
   explicit Break(Symbol id = Symbol()) : m_id(id)
   {}

   /**
    * \brief Retrieve loop identifier
    * \return Symbol value for loop to break out of. Symbol() is returned if
    *         inner most loop is to be used.
    */       
   Symbol id()const
   { return m_id; }

   void accept(ASTVisitor& visitor)const;
};

///Represent an expression statement
class ExprStmt : public Stmt {
private:
   std::unique_ptr<Expr::Expr> m_expr;

public:
   /**
    * \brief Construct Expr object
    * \param expr Expression to evaluate and discard value
    */
   explicit ExprStmt(std::unique_ptr<Moove::Expr::Expr> expr) : m_expr(std::move(expr))
   {}

   /**
    * \brief Retrieve expression to evaluate
    * \return Expr::Expr object to evaluate
    */
   const Expr::Expr& expr()const
   { return *m_expr; }

   void accept(ASTVisitor& visitor)const;
};

}    //namespace Stmt

}    //namespace Moove

#endif   //MOOVE_STMT_AST_HPP

