// -*- mode: c++; c-file-style: "linux"; c-basic-offset: 2; indent-tabs-mode: nil -*-
//
//  Copyright (C) 2004-2015 Andrej Vodopivec <andrej.vodopivec@gmail.com>
//            (C) 2014-2018 Gunter Königsmann <wxMaxima@physikbuch.de>
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
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//
//  SPDX-License-Identifier: GPL-2.0+

/*! \file
  This file declares the class SumCell

  SumCell is the Cell type that represents maxima's <code>sum()</code>, 
  <code>lsum</code> and <code>product()</code> 
  commands.
*/

#ifndef SUMCELL_H
#define SUMCELL_H

#include "precomp.h"
#include "Cell.h"
#include "ParenCell.h"

class TextCell;

enum sumStyle : int8_t
{
  SM_SUM,
  SM_PROD
};

class SumCell final : public Cell
{
public:
  SumCell(GroupCell *group, Configuration **config);
  SumCell(const SumCell &cell);
  std::unique_ptr<Cell> Copy() const override;

  InnerCellIterator InnerBegin() const override { return InnerCellIterator(&m_under); }
  InnerCellIterator InnerEnd() const override { return ++InnerCellIterator(&m_paren); }
  
  void Recalculate(AFontSize fontsize) override;

  void Draw(wxPoint point) override;

  void SetBase(std::unique_ptr<Cell> &&base);
  void SetUnder(std::unique_ptr<Cell> &&under);
  void SetOver(std::unique_ptr<Cell> &&over);

  void SetSumStyle(sumStyle style);

  wxString ToMathML() const override;
  wxString ToMatlab() const override;
  wxString ToOMML() const override;
  wxString ToString() const override;
  wxString ToTeX() const override;
  wxString ToXML() const override;

  void SetAltCopyText(const wxString &text) override { m_altCopyText = text; }
  const wxString GetAltCopyText() const override { return m_altCopyText; }

  Cell *GetNextToDraw() const override { return m_nextToDraw; }
  bool BreakUp() override;
  void SetNextToDraw(Cell *next) override;
  void Unbreak() override final;

private:
  ParenCell *Paren() const { return static_cast<ParenCell*>(m_paren.get()); }
  // The base cell is owned by the paren
  Cell *Base() const { return Paren() ? Paren()->GetInner() : nullptr; }

  //! Text that should end up on the clipboard if this cell is copied as text.
  wxString m_altCopyText;

  CellPtr<Cell> m_nextToDraw;
  CellPtr<Cell> m_displayedBase;
  CellPtr<Cell> m_baseWithoutParen;

  // The pointers below point to inner cells and must be kept contiguous.
  // ** All pointers must be the same: either Cell * or std::unique_ptr<Cell>.
  // ** NO OTHER TYPES are allowed.
  std::unique_ptr<Cell> m_under;
  std::unique_ptr<Cell> m_start;
  std::unique_ptr<Cell> m_var;
  std::unique_ptr<Cell> m_end;
  std::unique_ptr<Cell> m_comma1;
  std::unique_ptr<Cell> m_comma2;
  std::unique_ptr<Cell> m_comma3;
  std::unique_ptr<Cell> m_open;
  std::unique_ptr<Cell> m_close;
  std::unique_ptr<Cell> m_paren;
  // The pointers above point to inner cells and must be kept contiguous.

  double m_signWidth = 30;
  int m_signHeight = 50;
  int m_signWCenter = 15;
  sumStyle m_sumStyle = SM_SUM;

//** Bitfield objects (0 bytes)
//**
  void InitBitFields()
  { // Keep the initailization order below same as the order
    // of bit fields in this class!
  }
};

#endif // SUMCELL_H
