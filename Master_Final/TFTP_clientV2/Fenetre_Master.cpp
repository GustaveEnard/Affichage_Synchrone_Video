#include "Fenetre_Master.h"

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
//------------------------------------------------------------------------------



BEGIN_EVENT_TABLE(Fenetre_Master, wxFrame)
EVT_BUTTON(ID_APP_QUIT, Fenetre_Master::OnQuit)
EVT_TEXT(ID_PORT, Fenetre_Master::OnPortChanged)
EVT_BUTTON(ID_SEND, Fenetre_Master::OnSend)
EVT_BUTTON(ID_RESEAU, Fenetre_Master::OnConfigReseau)
EVT_FILEPICKER_CHANGED(ID_VIDEO, Fenetre_Master::OnVideo)
EVT_TEXT(ID_DEST, Fenetre_Master::OnDestChanged)
EVT_CLOSE(Fenetre_Master::OnClose)
END_EVENT_TABLE()

IMPLEMENT_APP(TMyApp)
//------------------------------------------------------------------------------
bool TMyApp::OnInit()
{
    Fenetre_Master* frame = new Fenetre_Master("TFTP Client", wxPoint(400, 250), wxSize(570, 250));
    frame->Show(true);
    SetTopWindow(frame);
    return true;
}
//------------------------------------------------------------------------------
Fenetre_Master::Fenetre_Master(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame(NULL, -1, title, pos, size), client("127.0.0.1", 5555)
{
    SetIcon(wxICON(monicone));
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
    /*  wxArrayString str;
        str.Add("yes");
        str.Add("no");
      */
    wxPanel* panelAffichage = new wxPanel(this, -1);

    wxBoxSizer* sizer_intermed = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer* cadre = new wxStaticBoxSizer(wxVERTICAL, panelAffichage, _T("Configurations des paramètres TFTP : "));

    wxFlexGridSizer* grille = new wxFlexGridSizer(2, 3, 20, 5);
    wxFlexGridSizer* grille2 = new wxFlexGridSizer(2, 3, 30, 5);

    wxBoxSizer* sizer_boutons = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* txtPort1 = new wxStaticText(panelAffichage, ID_PORT, wxT("TFTP Port :"), wxDefaultPosition);

    wxStaticText* txtPort2 = new wxStaticText(panelAffichage, ID_PORT, wxT("Default : 5555"), wxDefaultPosition);

    Port = new wxTextCtrl(panelAffichage, ID_PORT, wxT("..."), wxDefaultPosition, wxSize(80, 20));

    wxStaticText* txtDest1 = new wxStaticText(panelAffichage, ID_DEST, wxT("Nom du fichier de réception :"), wxDefaultPosition);

    wxStaticText* txtDest2 = new wxStaticText(panelAffichage, ID_DEST, wxT("Exemple : Destination.avi ou Destination.txt"), wxDefaultPosition);

    Destination = new wxTextCtrl(panelAffichage, ID_DEST, wxT("..."), wxDefaultPosition, wxSize(80, 20));

    Quit = new wxButton(panelAffichage, ID_APP_QUIT, "Quitter");

    ConfigReseau = new wxButton(panelAffichage, ID_RESEAU, "Configuration réseau");

    Send = new wxButton(panelAffichage, ID_SEND, "Envoyer");

    wxStaticText* fileVideo = new wxStaticText(panelAffichage, ID_VIDEO, wxT("Sélectionner un fichier :"), wxDefaultPosition);
    File2 = new wxTextCtrl(panelAffichage, ID_VIDEO, wxT("..."), wxDefaultPosition, wxSize(300, 20));
    File = new wxFilePickerCtrl(panelAffichage, ID_VIDEO, _T(""), wxFileSelectorPromptStr, wxFileSelectorDefaultWildcardStr, wxDefaultPosition, wxSize(80, wxDefaultCoord), wxDIRP_CHANGE_DIR);

    grille->Add(fileVideo, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
    grille->Add(File2, 0, wxEXPAND);
    grille->Add(File, 0, wxEXPAND);

    grille2->Add(txtDest1, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
    grille2->Add(Destination, 0, wxEXPAND);
    grille2->Add(txtDest2, 0, wxEXPAND);

    grille2->Add(txtPort1, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
    grille2->Add(Port, 0, wxEXPAND);
    grille2->Add(txtPort2, 0, wxEXPAND);


    sizer_intermed->Add(cadre, 1, wxALL | wxEXPAND, 5);

    cadre->Add(grille, 1, wxALL | wxEXPAND, 5);
    cadre->Add(grille2, 1, wxALL | wxEXPAND, 5);
    wxStaticLine* ligneHoriz = new wxStaticLine(panelAffichage, -1);
    sizer_intermed->Add(ligneHoriz, 0, wxALL | wxEXPAND, 5);

    sizer_boutons->Add(Quit, 0);
    sizer_boutons->AddSpacer(5);
    sizer_boutons->Add(ConfigReseau, 0);
    sizer_boutons->AddSpacer(5);
    sizer_boutons->Add(Send, 0);

    sizer_intermed->Add(sizer_boutons, 0, wxALIGN_RIGHT | wxALL, 5);

    panelAffichage->SetSizer(sizer_intermed);

    //fenetreConfigReseau

    fenetreConfigReseau = new FenetreConfigReseau("Configuration Réseau");

}
//------------------------------------------------------------------------------
void Fenetre_Master::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    if (fenetreConfigReseau != nullptr)
    {
        fenetreConfigReseau->Destroy();
    }
    Close(true);
}
//------------------------------------------------------------------------------

void Fenetre_Master::OnPortChanged(wxCommandEvent& WXUNUSED(event))
{

}

void Fenetre_Master::OnDestChanged(wxCommandEvent& event)
{
    
}

void Fenetre_Master::OnConfigReseau(wxCommandEvent& WXUNUSED(event))
{
    if (fenetreConfigReseau == nullptr) {
        std::cerr << "probleme initialisation classe";
    }
    fenetreConfigReseau->Show(true);

}

void Fenetre_Master::OnSend(wxCommandEvent& WXUNUSED(event))
{
    int port = wxAtoi(Port->GetValue());

    wxString Src = (File->GetPath());
    const char* source = Src.mb_str();

    wxString Dest = (Destination->GetValue());
    const char* destination = Dest.mb_str();

    if (extension.verifierExtension(destination) == 0)
    {
        if (File->GetPath() != "")
        {
            //Récupérer une list
            if (fenetreConfigReseau == nullptr) {

                wxMessageBox(wxT("Pas de configuration réseau choisie !"));
            }
            else {
                std::string config_choisie = fenetreConfigReseau->get_Config_Choisie();

                list<std::string> ip;

                if (config_choisie == "") {
                    wxMessageBox(wxT("Pas de configuration réseau choisie !"));
                }
                else if (config_choisie == "list") {
                    fenetreConfigReseau->recuperer_AdrIP(&ip);

                    
                   if (client.tftpSend(port, source, destination, &ip) == 1)
                   {
                       wxMessageBox(wxT("Envoie interrompue, réassayer !"));
                   }
                   else
                   {
                       
                       wxMessageBox(wxT("Envoie Terminer !"));
                   }
                }
                else {
                    fenetreConfigReseau->recuperer_AdrIPXML(&ip, config_choisie);
                    
                    if (client.tftpSend(port, source, destination, &ip) == 1)
                    {
                        wxMessageBox(wxT("Envoie interrompue, réassayer !"));
                    }
                    else
                    {
                        
                        wxMessageBox(wxT("Envoie Terminer !"));
                    }
                    config_choisie = "";
                }

                /*std::string config;
                for (auto i = ip.begin(); i != ip.end(); i++) {

                    config += i->c_str() +'\n';
                }
                wxMessageBox(config);*/


                //ajouter un argument list

            }
        }
        else
        {
            wxMessageBox(wxT("Selectionner un fichier !"));
        }

    }
    else
    {
        wxMessageBox(wxT("Ecrivez le nom du fichier de destination en .avi ou en .txt !"));
    }
}

void Fenetre_Master::OnVideo(wxFileDirPickerEvent& event)
{
    File2->SetValue(File->GetPath());
}

void Fenetre_Master::OnClose(wxCloseEvent& event)
{
    if (fenetreConfigReseau != nullptr)
    {
        fenetreConfigReseau->Destroy();
    }
    Destroy();
}