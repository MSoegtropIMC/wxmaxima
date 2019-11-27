﻿// -*- mode: c++; c-file-style: "linux"; c-basic-offset: 2; indent-tabs-mode: nil -*-
//
//  Copyright (C) 2004-2015 Andrej Vodopivec <andrej.vodopivec@gmail.com>
//  Copyright (C) 2014-2016 Gunter Königsmann <wxMaxima@physikbuch.de>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  SPDX-License-Identifier: GPL-2.0+

/*! \file
  This file declares the class ParenCell

  ParenCell is the Cell type that represents a math element that is kept
  between parenthesis.
 */

#ifndef PARENCELL_H
#define PARENCELL_H

#include "Cell.h"

/*! The class that represents parenthesis that are wrapped around text

  In the case that this cell is broken into two lines in the order of
  m_nextToDraw this cell is represented by the following individual 
  cells:
  
   - The ParenCell itself
   - The opening "("
   - The contents
   - The closing ")".
   
  If it isn't broken into multiple cells m_nextToDraw points to the 
  cell that follows this Cell.
 */
class ParenCell : public Cell
{
public:
  ParenCell(Cell *parent, Configuration **config, CellPointers *cellPointers);
  ParenCell(const ParenCell &cell);
  Cell *Copy() override {return new ParenCell(*this);}

  ~ParenCell();

  //! This class can be derived from wxAccessible which has no copy constructor
  ParenCell &operator=(const ParenCell&) = delete;

  std::list<Cell *> GetInnerCells() override;

  void SetInner(Cell *inner, CellType  type);

  void SetPrint(bool print)
  {
    m_print = print;
  }

  void RecalculateHeight(int fontsize) override;

  void RecalculateWidths(int fontsize) override;

  virtual void Draw(wxPoint point) override;

  bool BreakUp() override;

  void Unbreak() override;

  wxString ToString() override;

  wxString ToMatlab() override;

  wxString ToTeX() override;

  wxString ToMathML() override;

  wxString ToOMML() override;

  wxString ToXML() override;

protected:
   /*! How to create a big parenthesis sign?
   */
  Configuration::drawMode m_bigParenType;
  void SetFont(int fontsize);
  Cell *m_innerCell, *m_open, *m_close;
  Cell *m_last1;
  bool m_print;
  int m_numberOfExtensions;
  int m_charWidth, m_charHeight;
  int m_charWidth1, m_charHeight1;
  int m_signWidth, m_signHeight, m_signTopHeight, m_signBotHeight, m_extendHeight;
};

#endif // PARENCELL_H