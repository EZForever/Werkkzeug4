/*+**************************************************************************/
/***                                                                      ***/
/***   Copyright (C) by Dierk Ohlerich                                    ***/
/***   all rights reserverd                                               ***/
/***                                                                      ***/
/***   To license this software, please contact the copyright holder.     ***/
/***                                                                      ***/
/**************************************************************************+*/

#include "main.hpp"
#include "wz4gui.hpp"
#include "gui/gui.hpp"
#include "base/windows.hpp"

/****************************************************************************/


void sMain()
{
  sInit(sISF_2D|sISF_3D,sDpiScale(1280),sDpiScale(800));
  sInitGui();
  sGui->AddBackWindow(new MainWindow);
  sSetWindowName(L"werkkzeug4");
}

/****************************************************************************/

