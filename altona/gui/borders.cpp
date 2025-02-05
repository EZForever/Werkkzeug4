/****************************************************************************/
/***                                                                      ***/
/***   (C) 2005 Dierk Ohlerich, all rights reserved                       ***/
/***                                                                      ***/
/****************************************************************************/

#include "gui/borders.hpp"
#include "base/windows.hpp"

/****************************************************************************/
/***                                                                      ***/
/***   Focus Border                                                       ***/
/***                                                                      ***/
/****************************************************************************/

void sFocusBorder::OnCalcSize()
{
  ReqSizeX = sDpiScale(2) & ~1;
  ReqSizeY = sDpiScale(2) & ~1;
}

void sFocusBorder::OnLayout()
{
  Parent->Inner.Extend(-sDpiScale(2)/2);
}

void sFocusBorder::OnPaint2D()
{
  sRect r = Client;
  for (sInt i = 0; i < sDpiScale(2) / 2; i++)
  {
    sGui->RectHL(r,Parent->Flags & sWF_CHILDFOCUS);
    r.Extend(-1);
  }
}

/****************************************************************************/
/***                                                                      ***/
/***   Thick Border                                                       ***/
/***                                                                      ***/
/****************************************************************************/

sThickBorder::sThickBorder()
{
  Inverted = sFALSE;
}

void sThickBorder::OnCalcSize()
{
  ReqSizeX = sDpiScale(4) & ~1;
  ReqSizeY = sDpiScale(4) & ~1;
}

void sThickBorder::OnLayout()
{
  Parent->Inner.Extend(-sDpiScale(4)/2);
}

void sThickBorder::OnPaint2D()
{
  sRect r;
  r = Client;
  if (Inverted)
    sGui->RectHL(r,sGC_LOW2,sGC_HIGH2);
  else
    sGui->RectHL(r,sGC_HIGH2,sGC_LOW2);
  for (sInt i = 1; i < sDpiScale(4) / 2; i++)
  {
    r.Extend(-1);
    if (Inverted)
      sGui->RectHL(r,sGC_LOW,sGC_HIGH);
    else
      sGui->RectHL(r,sGC_HIGH,sGC_LOW);
  }
}

/****************************************************************************/
/***                                                                      ***/
/***   Thin Border                                                        ***/
/***                                                                      ***/
/****************************************************************************/

void sThinBorder::OnCalcSize()
{
  ReqSizeX = sDpiScale(2) & ~1;
  ReqSizeY = sDpiScale(2) & ~1;
}

void sThinBorder::OnLayout()
{
  Parent->Inner.Extend(-sDpiScale(2)/2);
}

void sThinBorder::OnPaint2D()
{
  sRect r = Client;
  for (sInt i = 0; i < sDpiScale(2) / 2; i++)
  {
    sGui->RectHL(r,sGC_DRAW,sGC_DRAW);
    r.Extend(-1);
  }
}

/****************************************************************************/
/***                                                                      ***/
/***   Space Border. Thin border with extra space inside                  ***/
/***                                                                      ***/
/****************************************************************************/

sSpaceBorder::sSpaceBorder(sInt pen)
{
  Pen = pen;
}

void sSpaceBorder::OnCalcSize()
{
  ReqSizeX = sDpiScale(6) & ~1;
  ReqSizeY = sDpiScale(6) & ~1;
}

void sSpaceBorder::OnLayout()
{
  Parent->Inner.Extend(-sDpiScale(6)/2);
}

void sSpaceBorder::OnPaint2D()
{
  sRect r = Client;
  sGui->RectHL(r,sGC_DRAW,sGC_DRAW);
  for (sInt i = 1; i < sDpiScale(6) / 2; i++)
  {
    r.Extend(-1);
    sGui->RectHL(r,Pen,Pen);
  }
}
/****************************************************************************/
/***                                                                      ***/
/***   Toolbar Border                                                     ***/
/***                                                                      ***/
/****************************************************************************/

sToolBorder::sToolBorder()
{
  Bottom = 0;

  Flags |= sWF_CLIENTCLIPPING;
}

void sToolBorder::OnCalcSize()
{
  sWindow *w;

  ReqSizeX = 0;
  ReqSizeY = 0;
  sFORALL(Childs,w)
  {
    ReqSizeY = sMax(ReqSizeY,w->ReqSizeY);
  }
  ReqSizeY += sDpiScale(1);
}

void sToolBorder::OnLayout()
{
  sInt y0,y1;

  if(Bottom)
  {
    y0 = Client.y1-ReqSizeY+sDpiScale(1);
    y1 = Client.y1;
    Parent->Inner.y1 = Client.y0 = y0;
    y0 += sDpiScale(1);
  }
  else
  {
    y0 = Client.y0;
    y1 = Client.y0+ReqSizeY-sDpiScale(1);
    Parent->Inner.y0 = Client.y1 = y1;
    y1 -= sDpiScale(1);
  }

  OnLayout(y0,y1);
}

void sToolBorder::OnLayout(sInt y0,sInt y1)
{
  sWindow *w;
  sInt xl, xr;

  xl = Client.x0;
  xr = Client.x1;
  sFORALL(Childs,w)
  {
    w->Outer.y0 = y0;
    w->Outer.y1 = y1;

    if (w->Flags & WF_TB_RIGHTALIGNED)
    {
      w->Outer.x1 = xr;
      xr -= w->ReqSizeX;
      w->Outer.x0 = xr;
    }
    else
    {
      w->Outer.x0 = xl;
      xl += w->ReqSizeX;
      w->Outer.x1 = xl;
    }
  }
}

void sToolBorder::OnPaint2D()
{
  sRect r;

  r = Client;
  sWindow *c;
  sFORALL(Childs,c)
  {
    if (c->Flags & WF_TB_RIGHTALIGNED)
      r.x1 = c->Outer.x0;
    else
      r.x0 = c->Outer.x1;
  }
  sRect2D(r,sGC_BACK);

  r = Client;
  if(Bottom)
    r.y1 = r.y0+sDpiScale(1);
  else
    r.y0 = r.y1-sDpiScale(1);
  sRect2D(r,sGC_DRAW);
}

void sToolBorder::AddMenu(sChar *name,const sMessage &msg)
{
  AddChild(new sButtonControl(name,msg,sBCS_NOBORDER|sBCS_IMMEDIATE));
}

void sToolBorder::AddSpace(sBool rightaligned)
{
  sWindow *w=new sButtonControl(L"   ",sMessage(),sBCS_NOBORDER|sBCS_STATIC);
  AddChild(w);
  if (rightaligned) w->Flags|=WF_TB_RIGHTALIGNED;
}

void sToolBorder::AddRightAligned(sWindow *w)
{
  AddChild(w);
  w->Flags|=WF_TB_RIGHTALIGNED;
}

/****************************************************************************/
/***                                                                      ***/
/***   Toolbar Border                                                     ***/
/***                                                                      ***/
/****************************************************************************/

sScrollBorder::sScrollBorder()
{
  Width = sDpiScale(10);
  KnopMin = sDpiScale(20);

  DragStartX = 0;
  DragStartY = 0;
  DragMode = 0;
}

void sScrollBorder::OnCalcSize()
{
  if(Parent->Flags & sWF_SCROLLX) ReqSizeY = Width;
  if(Parent->Flags & sWF_SCROLLY) ReqSizeX = Width;
}

void sScrollBorder::OnLayout()
{
  if(Parent->Flags & sWF_SCROLLX) Parent->Inner.y1 -= Width;
  if(Parent->Flags & sWF_SCROLLY) Parent->Inner.x1 -= Width;

  if((Parent->Flags & sWF_SCROLLX) && (Parent->Flags & sWF_SCROLLY))
  {
    ButtonX.Init(Client.x0,Client.y1-Width,Client.x1-Width,Client.y1);
    ButtonY.Init(Client.x1-Width,Client.y0,Client.x1,Client.y1-Width);
    ButtonZ.Init(Client.x1-Width,Client.y1-Width,Client.x1,Client.y1);
  }
  else
  {
    if(Parent->Flags & sWF_SCROLLX)
      ButtonX.Init(Client.x0,Client.y1-Width,Client.x1,Client.y1);
    if(Parent->Flags & sWF_SCROLLY)
      ButtonY.Init(Client.x1-Width,Client.y0,Client.x1,Client.y1);
    ButtonZ.Init(0,0,0,0);
  }

  // you can actually put buttons in the X scroller!

  if(Parent->Flags & sWF_SCROLLX)
  {
    sWindow *w;
    sInt x;

    x = ButtonX.x0;
    sFORALL(Childs,w)
    {

      w->Outer.x0 = x;
      w->Outer.y0 = ButtonX.y0;
      if(x+w->ReqSizeX < ButtonX.x1-sDpiScale(4))
        x += w->ReqSizeX;
      w->Outer.x1 = x;
      w->Outer.y1 = ButtonX.y1;
    }
    ButtonX.x0 = x;
  }
}

sBool sScrollBorder::CalcKnop(sInt &a,sInt &b,sInt client,sInt inner,sInt button,sInt scroll)
{
  sInt w = client - inner;
  if(w>0)
  {
    sInt k = sMulDiv(inner,button,client);
    if(k<KnopMin) k=sMin(KnopMin,button);
    a = sMulDiv(scroll,button-k,w);
    b = a+k;
    return 1;
  }
  else
  {
    a = 0;
    b = button;
    return 0;
  }
}

void sScrollBorder::OnPaint2D()
{
  sInt a,b;
  sRect r;

  if((Parent->Flags & sWF_SCROLLY) && (Parent->Flags & sWF_SCROLLY))
  {
    sRect2D(ButtonZ,sGC_BACK);
  }

  if(Parent->Flags & sWF_SCROLLX)
  {
    r = ButtonX;
    sInt color=(DragMode&1)?sGC_LOW2:sGC_LOW;
    if(r.SizeX()>sDpiScale(4))
    {
      sGui->RectHL(r);
      r.Extend(sDpiScale(-1));

      if(CalcKnop(a,b,Parent->Client.SizeX(),Parent->Inner.SizeX(),r.SizeX(),Parent->ScrollX))
      {
        sRect2D(r.x0  ,r.y0,r.x0+a,r.y1,sGC_BACK);
        sRect2D(r.x0+b,r.y0,r.x1  ,r.y1,sGC_BACK);

        r.x1 = r.x0+b;
        r.x0 = r.x0+a;
        sGui->RectHL(r,DragMode & 1);
        r.Extend(sDpiScale(-1));
        sRect2D(r,color);
      }
      else
      {
        sRect2D(r,sGC_BACK);
      }
    }
    else
    {
      sRect2D(r,color);
    }
  }

  if(Parent->Flags & sWF_SCROLLY)
  {
    r = ButtonY;
    sInt color=(DragMode&2)?sGC_LOW2:sGC_LOW;
    if(r.SizeY()>sDpiScale(4))
    {
      sGui->RectHL(r);
      r.Extend(sDpiScale(-1));

      if(CalcKnop(a,b,Parent->Client.SizeY(),Parent->Inner.SizeY(),r.SizeY(),Parent->ScrollY))
      {
        sRect2D(r.x0,r.y0  ,r.x1,r.y0+a,sGC_BACK);    
        sRect2D(r.x0,r.y0+b,r.x1,r.y1  ,sGC_BACK);

        r.y1 = r.y0+b;
        r.y0 = r.y0+a;
        sGui->RectHL(r,DragMode & 2);
        r.Extend(sDpiScale(-1));
        sRect2D(r,color);
      }
      else
      {
        sRect2D(r,sGC_BACK);
      }
    }
    else
    {
      sRect2D(r,color);
    }
  }
}

/****************************************************************************/

void sScrollBorder::OnDrag(const sWindowDrag &dd)
{
  sInt speed = (sGetKeyQualifier()&sKEYQ_SHIFT) ? 8 : 1;
  sInt a,b;
  sBool update = 0;

  switch(dd.Mode)
  {
  case sDD_START:
    DragStartX = Parent->ScrollX;
    DragStartY = Parent->ScrollY;
    DragMode = 0;
    if(ButtonX.Hit(dd.MouseX,dd.MouseY))
    {
      sInt client = Parent->Client.SizeX();
      sInt inner = Parent->Inner.SizeX();
      sInt button = ButtonX.SizeX()-sDpiScale(2);
      CalcKnop(a,b,client,inner,button,Parent->ScrollX);
      if(dd.MouseX<Client.x0+a)
        Parent->ScrollX -= inner;
      else if(dd.MouseX>Client.x0+b)
        Parent->ScrollX += inner;
      else
        DragMode = 1;
      update = 1;
    }
    if(ButtonY.Hit(dd.MouseX,dd.MouseY))
    {
      sInt client = Parent->Client.SizeY();
      sInt inner = Parent->Inner.SizeY();
      sInt button = ButtonY.SizeY()-sDpiScale(2);
      CalcKnop(a,b,client,inner,button,Parent->ScrollY);
      if(dd.MouseY<Client.y0+a)
        Parent->ScrollY -= inner;
      else if(dd.MouseY>Client.y0+b)
        Parent->ScrollY += inner;
      else
        DragMode = 2;
      update = 1;
    }
    if(ButtonZ.Hit(dd.MouseX,dd.MouseY))
    {
      DragMode = 3;
    }
    break;

  case sDD_DRAG:
    if(DragMode & 1)
    {
      sInt client = Parent->Client.SizeX();
      sInt inner = Parent->Inner.SizeX();
      sInt button = ButtonX.SizeX()-sDpiScale(2);
      if(button>2 && client>inner)
      {
        CalcKnop(a,b,client,inner,button,Parent->ScrollX);
        Parent->ScrollX = DragStartX + sMulDiv(dd.MouseX-dd.StartX , client-inner , button-(b-a))*speed;
        update = 1;
      }
    }
    if(DragMode & 2)
    {
      sInt client = Parent->Client.SizeY();
      sInt inner = Parent->Inner.SizeY();
      sInt button = ButtonY.SizeY()-sDpiScale(2);

      if(button>2 && client>inner)
      {
        CalcKnop(a,b,client,inner,button,Parent->ScrollY);
        Parent->ScrollY = DragStartY + sMulDiv(dd.MouseY-dd.StartY , client-inner , button-(b-a))*speed;
        update = 1;
      }
    }
    break;

  case sDD_STOP:
    DragMode = 0;
    sGui->Update(Parent->Outer);
    break;
  }

  if(update)
  {
    Parent->Layout();
    sGui->Update(Parent->Outer);
  }
}


/****************************************************************************/
/***                                                                      ***/
/***   Status Border                                                      ***/
/***                                                                      ***/
/****************************************************************************/

sStatusBorderItem::sStatusBorderItem() : Buffer(0,0)
{
  Width = 0;
  ProgressMax = 0;
  ProgressValue = 0;

  Client.Init();
}

/****************************************************************************/

sStatusBorder::sStatusBorder()
{
  Height = sGui->PropFont->GetHeight()+sDpiScale(6);
}

sStatusBorder::~sStatusBorder()
{
  sStatusBorderItem *item;
  sFORALL(Items,item)
    sDelete(item->Buffer.Buffer);
}

void sStatusBorder::AddTab(sInt width,sInt maxstring)
{
  sStatusBorderItem *item = Items.AddMany(1);
  item->Width = width;
  item->Buffer.Size = maxstring;
  item->Buffer.Buffer = new sChar[maxstring];
  item->Buffer.Buffer[0] = 0;
  item->Color = 0;
}

void sStatusBorder::Print(sInt tab,const sChar *string,sInt len,sU32 color)
{
  if(sCmpString(Items[tab].Buffer.Buffer,string)!=0)
  {
    Items[tab].Color = color;
    if(len==-1)
      sCopyString(Items[tab].Buffer.Buffer,string,Items[tab].Buffer.Size);
    else
      sCopyString(Items[tab].Buffer.Buffer,string,sMin(len+1,Items[tab].Buffer.Size));
    sGui->Update(Items[tab].Client);
  }
}

void sStatusBorder::SetColor(sInt tab,sU32 col)
{
  Items[tab].Color = col;
}

void sStatusBorder::Progress(sInt tab,sInt value,sInt max)
{
  Items[tab].ProgressMax = max;
  Items[tab].ProgressValue = sClamp(value,0,max);
}

/****************************************************************************/

void sStatusBorder::OnCalcSize()
{
  ReqSizeY = Height;
}

void sStatusBorder::OnLayout()
{
  sStatusBorderItem *item;
  sInt max = Items.GetCount()-1;
  sInt min = 0;

  Client = Parent->Inner;
  Parent->Inner.y1 -= Height;
  Client.y0 = Parent->Inner.y1;


  sInt x0 = Client.x0;
  sInt x1 = Client.x1;
  if(max>=min)
  {
    while(min<=max)                 // left aligned
    {
      if(Items[min].Width<=0)
        break;
      Items[min].Client.x0 = x0;
      x0 = sMin(x0+Items[min].Width,x1);
      Items[min].Client.x1 = x0;
      min++;
    }
    while(max>=min)                 // right aligned
    {
      if(Items[min].Width>=0)
        break;
      Items[min].Client.x1 = x1;
      x1 = sMax(x1-Items[min].Width,x0);
      Items[min].Client.x0 = x1;
      max--;
    }
    for(sInt i=0;i<=max-min;i++)      // centered
    {
      Items[i+min].Client.x0 = x0 + (x1-x0) *  i    / (max-min+1);
      Items[i+min].Client.x1 = x0 + (x1-x0) * (i+1) / (max-min+1);
    }
    Items[0].Client.x0 = Client.x0;   // pin border cases
    Items[Items.GetCount()-1].Client.x1 = Client.x1;
  }

  sFORALL(Items,item)
  {
    item->Client.y0 = Client.y0+sDpiScale(2);
    item->Client.y1 = Client.y1;
  }
}

void sStatusBorder::OnPaint2D()
{
  sStatusBorderItem *item;

  sRect2D(Client.x0,Client.y0,Client.x1,Client.y0+sDpiScale(2),sGC_BACK);
  sFORALL(Items,item)
  {
    sRect r = item->Client;

    sInt gc = sGC_BACK;
    if(item->Color)
    {
      sSetColor2D(sGC_MAX,item->Color);
      gc = sGC_MAX;
    }

    sGui->RectHL(r,sGC_BACK,sGC_BACK); r.Extend(sDpiScale(-1));
    sGui->RectHL(r,sGC_LOW,sGC_HIGH); r.Extend(sDpiScale(-1));
    sGui->PropFont->SetColor(sGC_TEXT,gc);
    sGui->PropFont->Print(sF2P_OPAQUE|sF2P_LEFT|sF2P_SPACE,r,item->Buffer.Buffer);
  }
}


/****************************************************************************/
