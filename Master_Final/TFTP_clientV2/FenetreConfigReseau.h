#pragma once

#include "UDPReseauMaster.h"
#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/event.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#define wxUSE_SOCKETS 1


//include pour le transfert des information réseaux
#include <list>
//#include "icone.xpm"


/*class MyApp : public wxApp
{

public:

	virtual bool OnInit();

};*/


class FenetreConfigReseau : public wxFrame
{

public:

	FenetreConfigReseau(const wxString& title);
	virtual ~FenetreConfigReseau();
	std::string get_Config_Choisie();
	void recuperer_AdrIP(std::list<std::string>* ip);
	void recuperer_AdrIPXML(std::list<std::string>* ip, std::string nomConfig);

private:

	//Organisation Affichage
	wxPanel* panelAffichage;
	wxStaticBoxSizer* cadre;
	wxStaticLine* ligneHoriz;
	wxBoxSizer* sizer_boutons;
	wxBoxSizer* sizer_intermed;

	//Liste configurations disponible
	wxListBox* choixConfig;
	wxArrayString str;

	//Boutons
	wxButton* m_btnRechercher;
	wxButton* btnOK;
	wxButton* btnEffacerTout;
	wxButton* btnEffacer;
	wxButton* btnConsulter;

	//UDPModule
	UDPReseauMaster admin;

	wxTextEntryDialog* titre = nullptr;
	wxString titre1;

	//Configuration choisie
	std::string config_choisie;

	// Méthodes évènementielles
	
	//EVT_CLOSE(MyFrame::OnClose)
	DECLARE_EVENT_TABLE();
	void OnBtnOKClicked(wxCommandEvent& event);
	void OnBtnRechercherClicked(wxCommandEvent& event);
	void OnBtnEffacerToutClicked(wxCommandEvent& event);
	void OnBtnEffacerClicked(wxCommandEvent& event);
	void OnBtnConsulterClicked(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);

	

};

enum
{
	ID_BTN_RECHERCHER = wxID_HIGHEST + 1,
	ID_BTN_OK,
	ID_BTN_EFFACER_TOUT,
	ID_BTN_EFFACER,
	ID_BTN_CHOIX,
	ID_BTN_CONSULTER
};