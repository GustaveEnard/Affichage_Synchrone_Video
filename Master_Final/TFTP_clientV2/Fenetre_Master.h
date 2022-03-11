#include "UDPReseauMaster.h"

#include <iostream>
#include <list>
#include <wx/filepicker.h>
#include "wx/wx.h"
#include <wx/statline.h>
#include "Extension.h"
#include "tftp_client.h"

#include "FenetreConfigReseau.h"


#define wxWin1H

#define ID_APP_QUIT 1
#define ID_CHG_TITRE 2
#define ID_PORT 3
#define ID_SEND 4
#define ID_RESEAU 5
#define ID_VIDEO 6
#define ID_DEST 7
//------------------------------------------------------------------------------
class TMyApp : public wxApp
{
public:
    virtual bool OnInit();
};
//------------------------------------------------------------------------------
class Fenetre_Master : public wxFrame
{
private:
    wxButton* Quit;
    wxButton* Send;
    wxButton* ConfigReseau;
    wxTextCtrl* Port;
    wxTextCtrl* Destination;
    wxTextCtrl* File2;
    wxFilePickerCtrl* File;
    int cpt;
    DECLARE_EVENT_TABLE();
    TFTPClient client;
    Extension extension;
    FenetreConfigReseau* fenetreConfigReseau = nullptr;

public:
    Fenetre_Master(const wxString& title, const wxPoint& pos, const wxSize& size);
    void OnQuit(wxCommandEvent& event);
    void OnPortChanged(wxCommandEvent& event);
    void OnSend(wxCommandEvent& event); 
    void OnConfigReseau(wxCommandEvent& event);
    void OnVideo(wxFileDirPickerEvent& ev);
    void OnDestChanged(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
};
//------------------------------------------------------------------------------
