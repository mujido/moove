//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 * \brief Expression AST types
 */
#ifndef MOOVE_EXPR_AST_HPP
#define MOOVE_EXPR_AST_HPP

#include "ast_base.hpp"
#include "symbol_table.hpp"

#include <memory>
#include <string>
#include <vector>

namespace Moove {

struct ASTVisitor;

///Containts all AST classes used for representing expressions
namespace Expr {

struct Expr;

/**
 * \brief A container of Expr pointers
 *
 * Although called ArgList, this type is used whenever a container of
 * Expr objects is needed by the AST classes. The name is derived from the 
 * original MOO parser. It is a managed AutoContainer with memory pooling
 * features as well.
 */
typedef std::vector<std::unique_ptr<Expr>> ArgList;

///Abstract base for AST expression types
struct Expr : public ASTPoolObject {
   virtual ~Expr()
   {}

   /**
    * \brief Determine if expression type is an l-value expression
    * \return Boolean value as to whether or not this is an l-value expression
    *
    * This method is used by the parser to perform symantic checking in certain
    * expressions where only an l-value would make sense.
    */
   virtual bool assignable()const
   { return false; }

   /**
    * \brief Visit an expression tree
    * \param visitor An ASTVisitor object to visit the tree
    */
   virtual void accept(ASTVisitor& visitor)const = 0;
};

///Represent an integer value (uses the C++ \c int type for storage)
class Integer : public Expr {
private:
   int m_value;

public:
   /**
    * \brief Construct a new Integer object with given value
    * \param value Value of Integer object
    */
   Integer(int value) : m_value(value)
   {}

   /**
    * \brief Retrieve value represented
    * \return \c int value represented by the object
    */
   int value()const
   { return m_value; }

   virtual void accept(ASTVisitor& visitor)const override;
};

///Represent an objnum value, i.e. #1234 (uses the C++ \c int type for storage)
class Objnum : public Expr {
private:
   int m_objnum;

public:
   /**
    * \brief Construct a new Objnum object with given value
    * \param objnum \c int value that appears after the pound sign
    */
   Objnum(int objnum) : m_objnum(objnum)
   {}

   /**
    * \brief Retrieve objnum value represented
    * \return \c int value that appears after the pound sign
    */
   int objnum()const
   { return m_objnum; }

   void accept(ASTVisitor& visitor)const;
};

/**
 * \brief Represent a string value 
 *
 * Internally implemented with a C++ \c std::string value
 */
class Str : public Expr {
private:
   std::unique_ptr<std::string> m_str;

public:
   /** 
    * \brief Construct a new Str object with given value
    * \param str String value to represent
    */
   Str(std::unique_ptr<std::string>&& str) : m_str(std::move(str))
   {}

   /**
    * \brief Retrieve string value represented
    * \return String value represented by the object
    */
   const std::string& str()const
   { return *m_str; }

   void accept(ASTVisitor& visitor)const;
};

/**
 * \brief Represent a real (floating point) number value 
 *
 * Internally implemented with a C++ \c double value
 */
class Real : public Expr {
private:
   double m_value;

public:
   /**
    * \brief Construct a new Real object with given value
    * \param value Value to represent
    */
   Real(double value) : m_value(value)
   {}

   /**
    * \brief Retrieve value represented
    * \return \c double value represented
    */
   const double& value()const
   { return m_value; }

   void accept(ASTVisitor& visitor)const;
};

/**
 * \brief Represent a list (array-like) value
 *
 * Internally implemented with an Expr::ArgList value
 */
class List : public Expr {
private:
   ArgList m_elements;

public:
   /**
    * \brief Create an instance using an ArgList object
    * \param elements A vector of Expr pointers to assume ownership of
    */
   List(ArgList&& elements) : m_elements(std::move(elements))
   {}

   /**
    * \brief Retrieve elements of list represented
    * \return Elements contained by the list expression
    */
   const ArgList& elements()const
   { return m_elements; }

   void accept(ASTVisitor& visitor)const;
};

/**
 * \brief Represent a variable
 *
 * Uses Symbol to represent the variable uniquely in the script.
 */
class Variable : public Expr {
private:
   Symbol m_id;

public:
   /**
    * \brief Create a Variable with given symbol
    * \param id Symbol of variable name (as created by SymbolTable)
    */
   Variable(const Symbol id) : m_id(id)
   {}

   /**
    * \brief Retrieve Symbol value for variable represented
    * \return Symbol for variable represented
    */
   Symbol id()const
   { return m_id; }

   bool assignable()const
   { return true; }

   void accept(ASTVisitor& visitor)const;
};

///Represent a scattering assignment ({x, y} = z) expression
class Scatter : public Expr {
public:
   ///Represents a destination target in a scatter expression
   class Target : public ASTPoolObject {
   public:
      ///Type of scatter target
      enum Type {
	 REQUIRED,       ///< Target requires a value
	 OPTIONAL,       ///< Target optionally has a value (also maybe 
	                 ///< default value)
	 REST            ///< Target will be a list of all remaining values
      };
      
   private:
      Type                m_type;
      Symbol              m_id;
      std::unique_ptr<Expr> m_expr;
      
   public:
      /**
       * \brief Construct a new Target object
       * \param type Type of target
       * \param id Symbol value of the variable represented by this Target 
       * \param expr Default value expression or 0 if none
       *
       * Although any Target object can have a default value, it is only
       * relevant for Target::OPTIONAL targets. There is no supported syntax 
       * for assigning a default value to any type other than Target::OPTIONAL.
       */
      Target(Type type, 
	     Symbol id, 
	     std::unique_ptr<Expr> expr = std::unique_ptr<Expr>()) : 
			 m_type(type), m_id(id), m_expr(std::move(expr))
      {}
      
      /**
       * \brief Determine if target has a default value
       * \return Boolean value specifying if a valid Expr object was given
       *         for value
       *
       * A Target object can have a default value even if not of type 
       * Target::OPTIONAL. However, it is meaningless in this context and 
       * has no supported syntax.
       */
      bool hasDefault()const
      { return m_expr.get() != 0; }
      
      /**
       * \brief Retrieve type of target
       * \return Type of target
       */
      Type type()const
      { return m_type; }
      
      /**
       * \brief Retrieve Symbol value of variable represented by this Target
       * \return Symbol value for variable represented by this Target
       */
      Symbol id()const
      { return m_id; }
      
      /**
       * \brief Retrieve default value Expr
       * \return Default value Expr object
       *
       * \warning
       * hasDefault() should be used prior to this method to ensure that a
       * valid default value was set. Otherwise a null pointer access will
       * occur.
       */
      const Expr& expr()const
      { return *m_expr; }
   
      /**
       * \brief Visit a scatter target
       * \param visitor An ASTVisitor object to visit the tree
       */
      void accept(ASTVisitor& visitor)const;
   };

   ///Contain a list of Target objects for a Scatter
   typedef ASTAutoContainer<std::vector<Target*> > TargetList;

private:
   std::unique_ptr<TargetList> m_targets;

public:
   /**
    * \brief Construct a Scatter object with given targets
    * \param targets TargetList of Target objects used in scatter
    */
   Scatter(std::unique_ptr<TargetList> targets) : m_targets(std::move(targets))
   {}

   /**
    * \brief Retrieve TargetList of targets
    * \return TargetList of Target objects
    */
   const TargetList& targets()const
   { return *m_targets; }

   void accept(ASTVisitor& visitor)const;
};

///Abstract base for unary operators
class UnaryExpr : public Expr {
private:
   std::unique_ptr<Expr> m_operand;

public:
   /**
    * \brief Construct UnaryExpr object
    * \param operand Operand to the operation given as an Expr object
    */
   UnaryExpr(std::unique_ptr<Expr> operand) : m_operand(std::move(operand))
   {}

   /**
    * \brief Retrieve operand expression
    * \return Expr object used as operand
    */
   const Expr& operand()const
   { return *m_operand; }
};

///Represent a boolean NOT (!x) operation
struct Not : public UnaryExpr {
   /**
    * \brief Construct Not object
    * \param operand Operand to the operation given as an Expr object
    */
   Not(std::unique_ptr<Expr> operand) : UnaryExpr(std::move(operand))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent a numeric sign negation (-x) operation
struct Negate : public UnaryExpr {
   /**
    * \brief Construct Negate object
    * \param operand Operand to the operation given as an Expr object
    */
   Negate(std::unique_ptr<Expr> operand) : UnaryExpr(std::move(operand))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent a list splice (\@x) operation
struct Splice : public UnaryExpr {
   /**
    * \brief Construct Splice object
    * \param operand Operand to the operation given as an Expr object
    */
   Splice(std::unique_ptr<Expr> operand) : UnaryExpr(std::move(operand))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent a numeric pre-increment (++x) operation
struct PreInc : public UnaryExpr {
   /**
    * \brief Construct PreInc object
    * \param operand Operand to the operation given as an Expr object
    */
   PreInc(std::unique_ptr<Expr> operand) : UnaryExpr(std::move(operand))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent a numeric pre-decrement (--x) operation
struct PreDec : public UnaryExpr {
   /**
    * \brief Construct PreDec object
    * \param operand Operand to the operation given as an Expr object
    */
   PreDec(std::unique_ptr<Expr> operand) : UnaryExpr(std::move(operand))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent a numeric post-increment (x++) operation
struct PostInc : public UnaryExpr {
   /**
    * \brief Construct PostInc object
    * \param operand Operand to the operation given as an Expr object
    */
   PostInc(std::unique_ptr<Expr> operand) : UnaryExpr(std::move(operand))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent a numeric post-decrement (x--) operation
struct PostDec : public UnaryExpr {
   /**
    * \brief Construct PostDec object
    * \param operand Operand to the operation given as an Expr object
    */
   PostDec(std::unique_ptr<Expr> operand) : UnaryExpr(std::move(operand))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Abstract base for binary operations
class BinaryExpr : public Expr {
private:
   std::unique_ptr<Expr> m_left;
   std::unique_ptr<Expr> m_right;

public:
   /**
    * \brief Construct BinaryExpr object
    * \param left Left-hand Expr object for binary operation
    * \param right Right-hand Expr object for binary operation
    */
   BinaryExpr(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) : 
      m_left(std::move(left)), m_right(std::move(right))
   {}

   /**
    * \brief Retrieve left-hand Expr object
    * \return Left-hand Expr object
    */
   const Expr& left()const
   { return *m_left; }
   
   /**
    * \brief Retrieve right-hand Expr object
    * \return Right-hand Expr object
    */
   const Expr& right()const
   { return *m_right; }
};

///Represent a logical OR (x || y) operation
struct Or : public BinaryExpr {
   /**
    * \brief Construct Or object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   Or(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent a logical AND (x && y) operation
struct And : public BinaryExpr {
   /**
    * \brief Construct And object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   And(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent an equality comparison (x == y) operation
struct Equal : public BinaryExpr {
   /**
    * \brief Construct Equal object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   Equal(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent an inequality comparison (x != y) operation
struct NotEqual : public BinaryExpr {
   /**
    * \brief Construct NotEqual object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   NotEqual(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent a less-than comparison (x < y) operation
struct Less : public BinaryExpr {
   /**
    * \brief Construct Less object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   Less(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent a less-than/equality comparison (x <= y) operation
struct LessEqual : public BinaryExpr {
   /**
    * \brief Construct LessEqual object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   LessEqual(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent a greater-than/equality comparison (x >= y) operation
struct GreaterEqual : public BinaryExpr {
   /**
    * \brief Construct GreaterEqual object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   GreaterEqual(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent a greater-than comparison (x > y) operation
struct Greater : public BinaryExpr {
   /**
    * \brief Construct Greater object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   Greater(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent an element in set (x in y) operation
struct In : public BinaryExpr {
   /**
    * \brief Construct In object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   In(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent an addition (x + y) operation
struct Add : public BinaryExpr {
   /**
    * \brief Construct Add object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   Add(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent a subraction (x - y) operation
struct Sub : public BinaryExpr {
   /**
    * \brief Construct Sub object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   Sub(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent a multiplication (x * y) operation
struct Mul : public BinaryExpr {
   /**
    * \brief Construct Mul object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   Mul(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent a division (x / y) operation
struct Div : public BinaryExpr {
   /**
    * \brief Construct Div object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   Div(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent a modulus (x % y) operation
struct Mod : public BinaryExpr {
   /**
    * \brief Construct Mod object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   Mod(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent an exponential (x ^ y) operation
struct Exp : public BinaryExpr {
   /**
    * \brief Construct Exp object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   Exp(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent an assignment (x = y) operation
struct Assign : public BinaryExpr {
   /**
    * \brief Construct Assign object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   Assign(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent an addition/assignment (x += y) operation
struct AddEqual : public BinaryExpr {
   /**
    * \brief Construct AddEqual object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   AddEqual(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent a subtraction/assignment (x -= y) operation
struct SubEqual : public BinaryExpr {
   /**
    * \brief Construct SubEqual object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   SubEqual(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent a multiplication/assignment (x *= y) operation
struct MulEqual : public BinaryExpr {
   /**
    * \brief Construct MulEqual object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   MulEqual(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent a division/assignment (x /= y) operation
struct DivEqual : public BinaryExpr {
   /**
    * \brief Construct DivEqual object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   DivEqual(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent a modulus/assignment (x %= y) operation
struct ModEqual : public BinaryExpr {
   /**
    * \brief Construct ModEqual object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   ModEqual(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent an exponential/assignment (x ^= y) operation
struct ExpEqual : public BinaryExpr {
   /**
    * \brief Construct ExpEqual object
    * \param left Left-hand expression for binary operation
    * \param right Right-hand expression for binary operation
    */
   ExpEqual(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
      BinaryExpr(std::move(left), std::move(right))
   {}

   void accept(ASTVisitor& visitor)const;
};

///Represent a conditional (x ? y : z) operation
class Conditional : public Expr {
private:
   std::unique_ptr<Expr> m_test;
   std::unique_ptr<Expr> m_trueExpr;
   std::unique_ptr<Expr> m_falseExpr;

public:
   /**
    * \brief Construct Conditional object
    * \param test Expression whose value will be logically tested
    * \param trueExpr Value of operation if \a test evaluates \c true.
    * \param falseExpr Value of operation if \a test evaluates \c false.
    */
   Conditional(std::unique_ptr<Expr> test, 
	       std::unique_ptr<Expr> trueExpr, 
	       std::unique_ptr<Expr> falseExpr) : 
      m_test(std::move(test)), m_trueExpr(std::move(trueExpr)), m_falseExpr(std::move(falseExpr))
   {}

   /**
    * \brief Retrieve test Expr object
    * \return Test Expr object
    */
   const Expr& test()const
   { return *m_test; }

   /**
    * \brief Retrieve trueExpr Expr object
    * \return Expr object returned if test expression evaluates \c true.
    */
   const Expr& trueExpr()const
   { return *m_trueExpr; }

   /**
    * \brief Retrieve falseExpr Expr object
    * \return Expr object returned if test expression evaluates \c false.
    */
   const Expr& falseExpr()const
   { return *m_falseExpr; }

   void accept(ASTVisitor& visitor)const;
};

///Represent a list indexing (x[y]) operation
class Index : public Expr {
private:
   std::unique_ptr<Expr> m_expr;
   std::unique_ptr<Expr> m_index;

public:
   /**
    * \brief Construct Index object
    * \param expr Expression that evaluates to a list
    * \param index Expression whose value is an index into the list
    */
   Index(std::unique_ptr<Expr> expr, std::unique_ptr<Expr> index) :
      m_expr(std::move(expr)), m_index(std::move(index))
   {}

   /**
    * \brief Retrieve list expression
    * \return Expr object for list
    */
   const Expr& expr()const
   { return *m_expr; }

   /**
    * \brief Retrieve index expression
    * \return Expr object for index into list
    */
   const Expr& index()const
   { return *m_index; }

   bool assignable()const
   { return true; }

   void accept(ASTVisitor& visitor)const;
};

///Represent a list subset index (x[y..z]) operation
class Range : public Expr {
private:
   std::unique_ptr<Expr> m_expr;
   std::unique_ptr<Expr> m_start;
   std::unique_ptr<Expr> m_end;

public:
   /**
    * \brief Construct Range object
    * \param expr Expression that evaluates to a list
    * \param start Expression that indexes the first element to subset
    * \param end Expression that indexes the last element to subset
    */
   Range(std::unique_ptr<Expr> expr, 
	 std::unique_ptr<Expr> start, std::unique_ptr<Expr> end) : 
      m_expr(std::move(expr)), m_start(std::move(start)), m_end(std::move(end))
   {}

   /**
    * \brief Retrieve list expression
    * \return Expr object for list
    */
   const Expr& expr()const
   { return *m_expr; }

   /**
    * \brief Retrieve beginning index expression of subset 
    * \return Expr object for subrange's beginning index
    */
   const Expr& start()const
   { return *m_start; }

   /**
    * \brief Retrieve ending index expression of subset
    * \return Expr object for subrange's ending index
    */
   const Expr& end()const
   { return *m_end; }

   bool assignable()const
   { return true; }

   void accept(ASTVisitor& visitor)const;
};

///Represent a try/catch (`x ! y => z') expression 
class Catch : public Expr {
private:
   std::unique_ptr<Expr>    m_expr;
   std::unique_ptr<ArgList> m_codes;
   std::unique_ptr<Expr>    m_exceptValue;

public:
   /**
    * \brief Construct Catch object
    * \param expr Expression to evaluate with exception handling
    * \param codes ArgList of exception values to catch and handle.
    * \param exceptValue Expression to evaluate and return if an exception
    *        that appears inside codes is raised by \a expr. Can be 0 if
    *        the value of the exception should be returned instead.
    */
   Catch(std::unique_ptr<Expr> expr, 
	 std::unique_ptr<ArgList> codes, 
	 std::unique_ptr<Expr> exceptValue) : 
      m_expr(std::move(expr)), m_codes(std::move(codes)), m_exceptValue(std::move(exceptValue))
   {}

   /**
    * \brief Determine if an exception value has been set
    * \return Boolean flag specifying whether or not an exception value
    *         was given.
    */
   bool hasExceptValue()const
   { return m_exceptValue.get() != 0; }

   /** 
    * \brief Retrieve expression that will be monitored for exceptions
    * \return Expr object that will be monitored for exceptions
    */
   const Expr& expr()const
   { return *m_expr; }

   /**
    * \brief Retrieve list of exceptions to be handled
    * \return ArgList of Expr objects whose values will be handled if
    *         raised
    */
   const ArgList& codes()const
   { return *m_codes; }

   /**
    * \brief Retrieve expression to evaluate if an exception is handled
    * \return Expr object to be evaluated if an exception is handled
    *
    *
    * \warning
    * hasExceptValue() should be used prior to this method to ensure that 
    * a default value was set. Otherwise, a null pointer access will
    * occur.
    */
   const Expr& exceptValue()const
   { return *m_exceptValue; }

   void accept(ASTVisitor& visitor)const;
};

///Represent a property (x.name) reference expression
class Prop : public Expr {
private:
   std::unique_ptr<Expr> m_object;
   std::unique_ptr<Expr> m_name;

public:
   /**
    * \brief Construct Prop object
    * \param object Expression on which to access a property
    * \param name Expression that evaluates to property name
    */
   Prop(std::unique_ptr<Expr> object, std::unique_ptr<Expr> name) :
      m_object(std::move(object)), m_name(std::move(name))
   {}

   /**
    * \brief Retrieve object expression on which to access a property 
    * \return Expr object whose property will be accessed
    */
   const Expr& object()const
   { return *m_object; }

   /**
    * \brief Retrieve property name expression
    * \return Expr object representing the name of the property to access
    */
   const Expr& name()const
   { return *m_name; }

   bool assignable()const
   { return true; }

   void accept(ASTVisitor& visitor)const;
};

///Represent verb call (x:name(...)) expression
class VerbCall : public Expr {
private:
   std::unique_ptr<Expr>    m_object;
   std::unique_ptr<Expr>    m_name;
   std::unique_ptr<ArgList> m_args;

public:
   /**
    * \brief Construct VerbCall object
    * \param object Expression whose verb will be called
    * \param name Expression that evaluates to a verb name
    * \param args Argument list to verb
    */
   VerbCall(std::unique_ptr<Expr> object, 
	    std::unique_ptr<Expr> name, 
	    std::unique_ptr<ArgList> args) : 
      m_object(std::move(object)), m_name(std::move(name)), m_args(std::move(args))
   {}

   /**
    * \brief Retrieve object expression whose verb will be called
    * \return Expr object whose verb will be called
    */
   const Expr& object()const
   { return *m_object; }

   /**
    * \brief Retrieve verb name expression
    * \return Expr object representing the verb name to be called
    */
   const Expr& name()const
   { return *m_name; }

   /**
    * \brief Retrieve argument list to verb
    * \return ArgList object containing each argument to the verb
    */
   const ArgList& args()const
   { return *m_args; }

   void accept(ASTVisitor& visitor)const;
};

///Represent a system property reference ($name) expression
class SystemProp : public Expr {
private:
   std::unique_ptr<std::string> m_name;

public:
   /** 
    * \brief Construct SystemProp object
    * \param name Property name string
    */
   SystemProp(std::unique_ptr<std::string> name) : m_name(std::move(name))
   {}

   /**
    * \brief Retrieve property name string
    * \return Property name string
    */
   const std::string& name()const
   { return *m_name; }

   bool assignable()const
   { return true; }

   void accept(ASTVisitor& visitor)const;
};

///Represent a system call ($name(...)) expression
class SystemCall : public Expr {
private:
   std::unique_ptr<std::string> m_name;
   std::unique_ptr<ArgList>     m_args;

public:
   /**
    * \brief Construct SystemCall object
    * \param name Verb name string
    * \param args Argument list to verb
    */
   SystemCall(std::unique_ptr<std::string> name, 
	      std::unique_ptr<ArgList> args) : 
      m_name(std::move(name)), m_args(std::move(args))
   {}

   /**
    * \brief Retrieve verb name string
    * \return Verb name string
    */
   const std::string& name()const
   { return *m_name; }

   /**
    * \brief Retrieve verb call arguments
    * \return ArgList object containing all verb call arguments
    */
   const ArgList& args()const
   { return *m_args; }

   void accept(ASTVisitor& visitor)const;
};

///Represent Builtin function call (name(...)) expression
class Builtin : public Expr {
private:
   std::unique_ptr<std::string> m_name;
   std::unique_ptr<ArgList>     m_args;

public:
   /**
    * \brief Construct Builtin object
    * \param name Function name string,
    * \param args Argument list to function
    */
   Builtin(std::unique_ptr<std::string> name, 
	   std::unique_ptr<ArgList> args) : m_name(std::move(name)), m_args(std::move(args))
   {}

   /**
    * \brief Retrieve function name
    * \return Function name string
    */
   const std::string& name()const
   { return *m_name; }

   /**
    * \brief Retrieve argument list
    * \return ArgList object containing all function call arguments
    */
   const ArgList& args()const
   { return *m_args; }

   void accept(ASTVisitor& visitor)const;
};

///Represent a list length ($) expression
struct Length : public Expr {
   void accept(ASTVisitor& visitor)const;
};

}   //namespace Expr

}   //namespace Moove

#endif   //MOOVE_EXPR_AST_HPP

