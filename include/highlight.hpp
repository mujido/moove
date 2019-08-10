//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_HIGHLIGHT_HPP
#define MOOVE_HIGHLIGHT_HPP

#include <string>

namespace Moove {

namespace Extra {

class HighlightAttributes {
public:
   enum Color {
      BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE
   };

private:
   unsigned char m_attrs;

   static const unsigned char FG_COLOR_MASK  = 0x07;
   static const unsigned char BG_COLOR_MASK  = 0x38;
   static const unsigned char BOLD_MASK      = 0x40;
   static const unsigned char UNDERLINE_MASK = 0x80; 

   static const unsigned char BG_COLOR_SHIFT = 3;

   unsigned char packAttrs(Color fg, 
			   Color bg,
			   bool bold, 
			   bool underline)
   { 
      return (fg | 
	      bg << BG_COLOR_SHIFT | 
	      (bold ? BOLD_MASK : 0) | 
	      (underline ? UNDERLINE_MASK : 0));
   }
      
public:
   HighlightAttributes(Color fg = WHITE, 
		       bool bold = false, 
		       bool underline = false) :
      m_attrs(packAttrs(fg, BLACK, bold, underline))
   {}

   HighlightAttributes(Color fg, 
		       Color bg,
		       bool bold = false,
		       bool underline = false) : 
      m_attrs(packAttrs(fg, bg, bold, underline))
   {}


   Color fg()const
   { return static_cast<Color>(m_attrs & FG_COLOR_MASK); }

   Color bg()const
   { return static_cast<Color>((m_attrs & BG_COLOR_MASK) >> BG_COLOR_SHIFT); }

   bool bold()const
   { return m_attrs & BOLD_MASK; }

   bool underline()const
   { return m_attrs & UNDERLINE_MASK; }
};

template<class StringType>
class SyntaxHighlighter {
public:
   enum SyntaxElement {
      IDENTIFIER, RESERVED_WORD, SYMBOL, STRING, INTEGER, REAL, OBJNUM
   };

   static const unsigned NUM_SYNTAX_ELEMENTS = 7;
   
private:
   HighlightAttributes m_allAttrs[NUM_SYNTAX_ELEMENTS];
   HighlightAttributes m_defaultAttrs;

public:
   SyntaxHighlighter(HighlightAttributes defaultAttrs = HighlightAttributes())
      : m_defaultAttrs(defaultAttrs)
   {}

   SyntaxHighlighter(const HighlightAttributes* allAttrs,
		     HighlightAttributes defaultAttrs = HighlightAttributes())
   { setAllAttrs(allAttrs); }

   virtual ~SyntaxHighlighter()
   {}

   const HighlightAttributes* allAttrs()const
   { return m_allAttrs; }

   HighlightAttributes attrs(SyntaxElement elem)const
   { return m_allAttrs[elem]; }

   HighlightAttributes defaultAttrs()const
   { return m_defaultAttrs; }

   void setAllAttrs(const HighlightAttributes* allAttrs)
   { std::copy(attrs, attrs + NUM_SYNTAX_ELEMENTS, m_allAttrs); }

   void setAttrs(SyntaxElement elem, HighlightAttributes attrs)
   { m_allAttrs[elem] = attrs; }

   void setDefaultAttrs(HighlightAttributes defaultAttrs)
   { m_defaultAttrs = defaultAttrs; }

   StringType id(const StringType& str)const
   { return highlight(IDENTIFIER, str); }

   StringType reserved(const StringType& str)const
   { return highlight(RESERVED_WORD, str); }

   StringType symbol(const StringType& str)const
   { return highlight(SYMBOL, str); }

   StringType str(const StringType& str)const
   { return highlight(STRING, str); }

   StringType integer(const StringType& str)const
   { return highlight(INTEGER, str); }

   StringType real(const StringType& str)const
   { return highlight(REAL, str); }
   
   StringType objnum(const StringType& str)const
   { return highlight(OBJNUM, str); }

   virtual StringType highlight(SyntaxElement elem, 
				const StringType& str)const = 0;
};

   
}   //namespace Extra

}   //namespace Moove

#endif   //MOOVE_HIGHLIGHT_HPP

