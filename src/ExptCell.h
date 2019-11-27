﻿// -*- mode: c++; c-file-style: "linux"; c-basic-offset: 2; indent-tabs-mode: nil -*-
//
//  Copyright (C) 2004-2015 Andrej Vodopivec <andrej.vodopivec@gmail.com>
//            (C) 2014-2016 Gunter Königsmann <wxMaxima@physikbuch.de>
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

#ifndef EXPTCELL_H
#define EXPTCELL_H

#include "Cell.h"
#include "TextCell.h"

/*!\file

  This file declares the class ExptCell which represents a exp() or %e^x-construct.
 */

/*! This cell represents a exp() or %e^x-construct.

  In the case that this cell is broken into two lines in the order of
  m_nextToDraw this cell is represented by the following individual 
  cells:
  
   - The ExptCell itself
   - The opening "exp("
   - The contents
   - The closing ")".
   
  If it isn't broken into multiple cells m_nextToDraw points to the 
  cell that follows this Cell. 
 */
class ExptCell : public Cell
{
public:
  ExptCell(Cell *parent, Configuration **config, CellPointers *cellpointers);
  ExptCell(const ExptCell &cell);
  Cell *Copy() override {return new ExptCell(*this);}
  ~ExptCell();

  //! This class can be derived from wxAccessible which has no copy constructor
  ExptCell &operator=(const ExptCell&) = delete;

  std::list<Cell *> GetInnerCells() override;

  //! Set the mantissa
  void SetBase(Cell *base);

  //! Set the exponent
  void SetPower(Cell *power);

  //! By how much do we want to rise the power?
  double PowRise() const {return Scale_Px(.8 * m_fontSize + MC_EXP_INDENT);}
  
  void RecalculateHeight(int fontsize) override;

  void RecalculateWidths(int fontsize) override;

  virtual void Draw(wxPoint point) override;

  wxString ToString() override;

  wxString ToMatlab() override;

  wxString ToTeX() override;

  wxString ToXML() override;

  wxString ToOMML() override;

  wxString ToMathML() override;

  wxString GetDiffPart() override;

  void IsMatrix(bool isMatrix)
  {
    m_isMatrix = isMatrix;
  }

  bool BreakUp() override;

  void Unbreak() override;

protected:
  Cell *m_baseCell, *m_exptCell;
  TextCell *m_open, *m_close;
  Cell *m_last2;
  Cell *m_exp, *m_last1;
  bool m_isMatrix;
};


#endif // EXPTCELL_H