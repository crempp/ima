/***************************************************************
 * Name:      imaApp.h
 * Purpose:   Header for imaApp class
 * Author:    Chad Rempp (crempp@gmail.com)
 * Created:   2008-10-28
 * Copyright: Chad Rempp (http://www.tetractys.org)
 * License:   GPLv3 copyright 2008 Chad Rempp
 **************************************************************/
#ifndef IMAAPP_H_INCLUDED
#define IMAAPP_H_INCLUDED

#include <wx/app.h>
#include "mainframe.h"

class imaApp : public wxApp
{
    public:
        ~imaApp(void);
        virtual bool OnInit();
};

DECLARE_APP(imaApp)

#endif // IMAAPP_H_INCLUDED
