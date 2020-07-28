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
  This file defines the class SqrtCell

  SqrtCell is the Cell type that represents a square root.
 */

#include "SqrtCell.h"
#include "VisiblyInvalidCell.h"

#define SIGN_FONT_SCALE 2.0

SqrtCell::SqrtCell(GroupCell *parent, Configuration **config) :
    Cell(parent, config),
    m_innerCell(std::make_unique<VisiblyInvalidCell>(parent,config)),
    m_open(std::make_unique<TextCell>(parent, config, "sqrt(")),
    m_close(std::make_unique<TextCell>(parent, config, ")"))
{
  InitBitFields();
  m_open->SetStyle(TS_FUNCTION);
  static_cast<TextCell&>(*m_open).DontEscapeOpeningParenthesis();
}

// cppcheck-suppress uninitMemberVar symbolName=SqrtCell::m_open
// cppcheck-suppress uninitMemberVar symbolName=SqrtCell::m_comma
// cppcheck-suppress uninitMemberVar symbolName=SqrtCell::m_signWidth
// cppcheck-suppress uninitMemberVar symbolName=SqrtCell::m_signSize
// cppcheck-suppress uninitMemberVar symbolName=SqrtCell::m_signTop
// cppcheck-suppress uninitMemberVar symbolName=SqrtCell::m_signType
// cppcheck-suppress uninitMemberVar symbolName=SqrtCell::m_signFontScale
SqrtCell::SqrtCell(const SqrtCell &cell):
    SqrtCell(cell.m_group, cell.m_configuration)
{
  CopyCommonData(cell);
  if(cell.m_innerCell)
    SetInner(cell.m_innerCell->CopyList());
  m_isBrokenIntoLines = cell.m_isBrokenIntoLines;
}

std::unique_ptr<Cell> SqrtCell::Copy() const
{
  return std::make_unique<SqrtCell>(*this);
}

void SqrtCell::SetInner(std::unique_ptr<Cell> &&inner)
{
  if (!inner)
    return;
  m_innerCell = std::move(inner);
}

void SqrtCell::Recalculate(AFontSize fontsize)
{
  if(!NeedsRecalculation(fontsize))
    return;

  Configuration *configuration = (*m_configuration);
  m_innerCell->RecalculateList(fontsize);
  m_open->Recalculate(fontsize);
  m_close->Recalculate(fontsize);

  if (configuration->CheckTeXFonts())
  {
    wxDC *dc = configuration->GetDC();
    m_innerCell->RecalculateList(fontsize);

    m_signFontScale = 1.0;
    auto fontsize1 = AFontSize(Scale_Px(SIGN_FONT_SCALE * fontsize * m_signFontScale));
    wxASSERT(fontsize1.IsValid());

    auto style = Style(fontsize1)
                   .FontName(configuration->GetTeXCMEX());

    dc->SetFont(style.GetFont());
    dc->GetTextExtent(wxT("s"), &m_signWidth, &m_signSize);
    m_signTop = m_signSize / 5;
    // The Scale_Px(2) leaves space for the serif at the root.
    m_width = m_innerCell->GetFullWidth() + m_signWidth + Scale_Px(2);
    
    int size = m_innerCell->GetHeightList();

    if (size <= (m_signSize) / 5)
    {
      m_signType = 1;
      m_signFontScale = (5.0 * size) / (1.5 * m_signSize);
    }
    else if (size <= (2 * m_signSize) / 5)
    {
      m_signType = 2;
      m_signFontScale = (5.0 * size) / (2.2 * m_signSize);
    }
    else if (size <= (3 * m_signSize) / 5)
    {
      m_signType = 3;
      m_signFontScale = (5.0 * size) / (3.0 * m_signSize);
    }
    else if (size <= (4 * m_signSize) / 5)
    {
      m_signType = 4;
      m_signFontScale = (5.0 * size) / (3.8 * m_signSize);
    }
    else
    {
      m_signType = 5;
      m_signFontScale = 1.0;
    }

    fontsize1.Set(Scale_Px(SIGN_FONT_SCALE * fontsize * m_signFontScale));
    wxASSERT(fontsize1.IsValid());

    style = Style(fontsize1)
              .FontName(configuration->GetTeXCMEX());

    dc->SetFont(style.GetFont());
    dc->GetTextExtent(wxT("s"), &m_signWidth, &m_signSize);
    m_signTop = m_signSize / 5;
    m_width = m_innerCell->GetFullWidth() + m_signWidth;
  }
  else
    m_width = m_innerCell->GetFullWidth() + Scale_Px(13) + 1;
  m_height = wxMax(m_innerCell->GetHeightList(), m_open->GetHeightList()) + Scale_Px(3);
  m_center = wxMax(m_innerCell->GetCenterList(), m_open->GetCenterList()) + Scale_Px(3);
  if(m_isBrokenIntoLines)
    m_height = m_center = m_width = 0;
  Cell::Recalculate(fontsize);
}

void SqrtCell::Draw(wxPoint point)
{
  Cell::Draw(point);
  if (DrawThisCell(point))
  {
    Configuration *configuration = (*m_configuration);
    wxDC *dc = configuration->GetDC();

    wxPoint in(point);

    if (configuration->CheckTeXFonts())
    {
      SetPen();

      in.x += m_signWidth;

      auto fontsize1 = AFontSize(Scale_Px(SIGN_FONT_SCALE * m_fontSize * m_signFontScale));
      wxASSERT(fontsize1.IsValid());

      auto style = Style(fontsize1)
                     .FontName(configuration->GetTeXCMEX());

      dc->SetFont(style.GetFont());
      SetForeground();
      if (m_signType < 4)
      {
        dc->DrawText(
                m_signType == 1 ? wxT("p") :
                m_signType == 2 ? wxT("q") :
                m_signType == 3 ? wxT("r") : wxT("s"),
                point.x,
                point.y - m_innerCell->GetCenterList() - m_signTop);
      }
      else
      {
        int yBottom = point.y + m_innerCell->GetMaxDrop() - 3.2 * m_signTop;
        int yTop = point.y - m_innerCell->GetCenterList() - m_signTop;
        int dy = m_signSize / 10;
        wxASSERT_MSG((yTop != 0) || (yBottom != 0), _("Font issue? The contents of a sqrt() has the size 0."));
        wxASSERT_MSG(dy > 0,
                     _("Font issue: The sqrt() sign has the size 0!"));
        if (dy <= 0)
          dy = 1;
        dc->DrawText(wxT("t"),
                    point.x,
                    yBottom);
        dc->DrawText(wxT("v"),
                    point.x,
                    yTop);
        while (yTop < yBottom)
        {
          yTop += dy;
          dc->DrawText(wxT("u"),
                      point.x,
                      yTop);
        }
      }

      wxDC *adc = configuration->GetAntialiassingDC();
      adc->DrawLine(point.x + m_signWidth,
                  point.y - m_innerCell->GetCenterList(),
                  point.x + m_signWidth + m_innerCell->GetFullWidth(),
                  point.y - m_innerCell->GetCenterList());

    }
    else
    {
      wxDC *adc = configuration->GetAntialiassingDC();
      in.x += Scale_Px(11) + 1;
      SetPen(1.2);

      const wxPoint points[12] = {
        {0, 0},
        {Scale_Px(3),           -Scale_Px(1)},
        //  A wider line
        {Scale_Px(3),           -Scale_Px(1)},
        {Scale_Px(7),            m_height - m_center - Scale_Px(4)},
        {Scale_Px(7.5),          m_height - m_center - Scale_Px(4)},
        {Scale_Px(3.5),         -Scale_Px(1)},
        {Scale_Px(3.5),         -Scale_Px(1)},
        {Scale_Px(3),           -Scale_Px(1)},
        {Scale_Px(8),            m_height - m_center - Scale_Px(4)},
        // The upwards line
        {Scale_Px(10),          -m_center + Scale_Px(2)},
        // The horizontal line
        {m_width - Scale_Px(1), -m_center + Scale_Px(2)},
        // The serif at the end of the root
        {m_width - Scale_Px(1), -m_center + Scale_Px(6)}
      };
      adc->DrawLines(12, points, point.x, point.y);
    }

    m_innerCell->DrawList(in);
  }
}

wxString SqrtCell::ToString() const
{
  if (m_isBrokenIntoLines)
    return wxEmptyString;
  else
    return wxT("sqrt(") + m_innerCell->ListToString() + wxT(")");
}

wxString SqrtCell::ToMatlab() const
{
  if (m_isBrokenIntoLines)
	return wxEmptyString;
  else
	return wxT("sqrt(") + m_innerCell->ListToMatlab() + wxT(")");
}

wxString SqrtCell::ToTeX() const
{
  if (m_isBrokenIntoLines)
    return wxEmptyString;
  else
    return wxT("\\sqrt{") + m_innerCell->ListToTeX() + wxT("}");
}

wxString SqrtCell::ToMathML() const
{
  return wxT("<msqrt>") + m_innerCell->ListToMathML() + wxT("</msqrt>\n");
}

wxString SqrtCell::ToOMML() const
{
  return wxT("<m:rad><m:radPr m:degHide=\"1\"></m:radPr><m:deg></m:deg><m:e>") + m_innerCell->ListToOMML() +
         wxT("</m:e></m:rad>\n");
}

wxString SqrtCell::ToXML() const
{
//  if (m_isBrokenIntoLines)
//    return wxEmptyString;
  wxString flags;
  if (m_forceBreakLine)
    flags += wxT(" breakline=\"true\"");

  return wxT("<q") + flags + wxT(">") + m_innerCell->ListToXML() + wxT("</q>");
}

bool SqrtCell::BreakUp()
{
  if (!m_isBrokenIntoLines)
  {
    Cell::BreakUp();
    m_isBrokenIntoLines = true;
    m_open->SetNextToDraw(m_innerCell);
    m_innerCell->last()->SetNextToDraw(m_close);
    m_close->SetNextToDraw(m_nextToDraw);
    m_nextToDraw = m_open;

    ResetCellListSizes();
    m_height = 0;
    m_center = 0;
    return true;
  }
  return false;
}

void SqrtCell::SetNextToDraw(Cell *next)
{
  if(m_isBrokenIntoLines)
    m_close->SetNextToDraw(next);
  else
    m_nextToDraw = next;
}
