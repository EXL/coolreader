/** \file lvdocview.h
    \brief XML/CSS document view

    CoolReader Engine

    (c) Vadim Lopatin, 2000-2006

    This source code is distributed under the terms of
    GNU General Public License.
    See LICENSE file for details.
*/

#ifndef __LV_TEXT_VIEW_H_INCLUDED__
#define __LV_TEXT_VIEW_H_INCLUDED__

#include "lvtinydom.h"
#include "lvpagesplitter.h"
#include "lvdrawbuf.h"


enum LVDocCmd
{
    DCMD_BEGIN,
    DCMD_LINEUP,
    DCMD_PAGEUP,
    DCMD_PAGEDOWN,
    DCMD_LINEDOWN,
    DCMD_END,
    DCMD_GO_POS,
    DCMD_GO_PAGE,
    DCMD_ZOOM_IN,
    DCMD_ZOOM_OUT,
};

enum LVDocViewMode
{
    DVM_SCROLL,
    DVM_PAGES,
};

class LVScrollInfo
{
public:
    int pos;
    int maxpos;
    int pagesize;
    int scale;
    lString16 posText;
};

//typedef lUInt64 LVPosBookmark;

/**
    \brief XML document view

    Platform independant document view implementation.

    Supports scroll view of document.
*/
class LVDocView
{
private:
    int m_dx;
    int m_dy;
    int m_pos;
    int m_font_size;
    bool m_is_rendered;
    LVDocViewMode m_view_mode;
#if (COLOR_BACKBUFFER==1)
    LVColorDrawBuf m_drawbuf;
#else
    LVGrayDrawBuf  m_drawbuf;
#endif
    lUInt32 m_backgroundColor;
    font_ref_t     m_font;
    LVStreamRef    m_stream;
    LVContainerRef m_arc;
    ldomDocument * m_doc;
    lString8 m_stylesheet;
    LVRendPageList m_pages;
    LVScrollInfo m_scrollinfo;

    lString16 m_title;
    lString16 m_authors;
    
    ldomXPointer _posBookmark;
    void updateScroll();
    void goToPage( int page );
    
public:
    /// returns background color
    lUInt32 getBackgroundColor()
    {
        return m_backgroundColor;
    }
    /// sets background color
    void setBackgroundColor( lUInt32 cl )
    {
        m_backgroundColor = cl;
        Draw();
    }
    /// returns xpointer for specified window point
    ldomXPointer getNodeByPoint( lvPoint pt );
    /// returns document
    ldomDocument * getDocument() { return m_doc; }
    /// returns book title
    lString16 getTitle() { return m_title; }
    /// returns book author(s)
    lString16 getAuthors() { return m_authors; }
    /// export to WOL format
    bool exportWolFile( const char * fname, bool flgGray, int levels );
    /// export to WOL format
    bool exportWolFile( const wchar_t * fname, bool flgGray, int levels );
    /// export to WOL format
    bool exportWolFile( LVStream * stream, bool flgGray, int levels );
    /// returns page count
    int getPageCount() { return m_pages.length(); }
    /// draws page to image
    void drawPageTo(LVDrawBuf * drawBuf, LVRendPageInfo & page);
    /// returns bookmark
    ldomXPointer getBookmark();
    /// moves position to bookmark
    void goToBookmark(ldomXPointer bm);
    /// returns scrollbar control info
    const LVScrollInfo * getScrollInfo() { return &m_scrollinfo; }
    /// converts scrollbar pos to doc pos
    int scrollPosToDocPos( int scrollpos );
    /// execute command
    void doCommand( LVDocCmd cmd, int param=0 );
    /// set document stylesheet text
    void setStyleSheet( lString8 css_text );
    /// change font size
    void ZoomFont( int delta );
    /// get drawing buffer
    LVDrawBuf * GetDrawBuf() { return &m_drawbuf; }
    /// draw document into buffer
    void Draw();
    /// resize view
    void Resize( int dx, int dy );
    /// get view height
    int GetHeight() { return m_dy; }
    /// get view width
    int GetWidth() { return m_dx; }
    /// get full document height
    int GetFullHeight();
    /// get vertical position of view inside document
    int GetPos() { return m_pos; }
    /// set vertical position of view inside document
    void SetPos( int pos, bool savePos=true );
    /// clear view
    void Clear();
    /// load document from file
    bool LoadDocument( const char * fname );
    /// load document from file
    bool LoadDocument( const lChar16 * fname );
    /// load document from stream
    bool LoadDocument( LVStreamRef stream );
    /// render (format) document
    void Render( int dx=0, int dy=0, LVRendPageList * pages=NULL );
    /// Constructor
    LVDocView();
    /// Destructor
    virtual ~LVDocView();
};


#endif
