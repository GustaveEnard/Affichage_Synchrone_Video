#include "FenetreConfigReseau.h"

BEGIN_EVENT_TABLE(FenetreConfigReseau, wxFrame)
EVT_CLOSE(FenetreConfigReseau::OnClose)
END_EVENT_TABLE()

FenetreConfigReseau::FenetreConfigReseau(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(700, 400)),admin(27015, "192.168.1.26", 27020)
{
	this->SetIcon(wxICON(icone_xpm));

	// Création du panel d'affichage
	panelAffichage = new wxPanel(this, -1);

	// Création du wxBoxSizer intermédiaire
	sizer_intermed = new wxBoxSizer(wxVERTICAL);

	// Création du wxStaticBoxSizer pour le cadre
	cadre = new wxStaticBoxSizer(wxVERTICAL, panelAffichage, _T("Configurations réseaux sauvegardées : "));

	// AFFICHAGE ECRAN DE SELECTION
	TiXmlDocument doc(CHEMIN_PHYSIQUE);
	if (!doc.LoadFile()) {

		std::cerr << "erreur lors du chargement" << std::endl;
		std::cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << std::endl;
	}

	TiXmlHandle hdl(&doc);
	TiXmlElement* elem2 = hdl.FirstChildElement().FirstChildElement().Element();
	while (elem2) {
	
		str.Add(std::string(elem2->Attribute("date")));
		elem2 = elem2->NextSiblingElement();
	}

	choixConfig = new wxListBox(panelAffichage, -1, wxPoint(0,0), wxDefaultSize, str, wxLB_SINGLE);
	cadre->Add(choixConfig, 1, wxALL | wxEXPAND, 1);
	
	// Ajout du wxStaticBoxSizer au sizer intermédiaire
	sizer_intermed->Add(cadre, 1, wxALL | wxEXPAND, 5);

	// Création de la ligne de séparation horizontale et ajout au sizer intermédiaire
	ligneHoriz = new wxStaticLine(panelAffichage, -1);
	sizer_intermed->Add(ligneHoriz, 0, wxALL | wxEXPAND, 5);

	// Création du wxBoxSizer pour les boutons
	sizer_boutons = new wxBoxSizer(wxHORIZONTAL);

	// Création du bouton "Effacer"
	btnConsulter = new wxButton(panelAffichage, ID_BTN_CONSULTER, _T("Consulter"));
	sizer_boutons->Add(btnConsulter, 0);

	// Ajout d'un espace entre les deux boutons
	sizer_boutons->AddSpacer(5);

	// Création du bouton "Effacer"
	btnEffacer = new wxButton(panelAffichage, ID_BTN_EFFACER, _T("Effacer"));
	sizer_boutons->Add(btnEffacer, 0);

	// Ajout d'un espace entre les deux boutons
	sizer_boutons->AddSpacer(5);

	// Création du bouton "EffacerTout"
	btnEffacerTout = new wxButton(panelAffichage, ID_BTN_EFFACER_TOUT, _T("Effacer Tout"));
	sizer_boutons->Add(btnEffacerTout, 0);
	
	// Ajout d'un espace entre les deux boutons
	sizer_boutons->AddSpacer(5);
	
	// Création du bouton "Rechercher"
	m_btnRechercher = new wxButton(panelAffichage, ID_BTN_RECHERCHER, _T("Rechercher"));
	sizer_boutons->Add(m_btnRechercher, 0);

	// Ajout d'un espace entre les deux boutons
	sizer_boutons->AddSpacer(5);

	// Création du bouton "OK"
	btnOK = new wxButton(panelAffichage, ID_BTN_OK, _T("OK"));
	sizer_boutons->Add(btnOK, 0);

	// Ajout du sizer des boutons au sizer intermédiaire
	sizer_intermed->Add(sizer_boutons, 0, wxALIGN_RIGHT | wxALL, 5);

	// Affectation du sizer intermédiaire au wxPanel
	panelAffichage->SetSizer(sizer_intermed);

	// Connexion des évènements à leurs méthodes respectives
	Connect(ID_BTN_RECHERCHER, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FenetreConfigReseau::OnBtnRechercherClicked));
	Connect(ID_BTN_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FenetreConfigReseau::OnBtnOKClicked));
	Connect(ID_BTN_EFFACER_TOUT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FenetreConfigReseau::OnBtnEffacerToutClicked));
	Connect(ID_BTN_EFFACER, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FenetreConfigReseau::OnBtnEffacerClicked));
	Connect(ID_BTN_CONSULTER, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FenetreConfigReseau::OnBtnConsulterClicked));

}

FenetreConfigReseau::~FenetreConfigReseau()
{
}

void FenetreConfigReseau::OnBtnOKClicked(wxCommandEvent& event)
{
	if (choixConfig->GetSelection() == wxNOT_FOUND)
	{
		//Message si pas de configuration sélectionnée
		wxMessageBox(_T("aucune configuration sélectionnée"), _T("Information"), wxICON_INFORMATION);
	}
	else {
		std::string config = admin.getUneConfigEnregistrée(choixConfig->GetString(choixConfig->GetSelection()).ToStdString());
		int reponse = wxMessageBox(_T("Si vous quitter la configuration choisie sera :\n") + config + _T("\nVoulez-vous quitter ?"), _T("Confirmation"), wxYES_NO | wxICON_QUESTION);
		//Enregistrement de la configuration choisie
		if (reponse == wxYES) {
			config_choisie = choixConfig->GetString(choixConfig->GetSelection()).ToStdString();
			Show(false);
		}
	}
}

void FenetreConfigReseau::OnBtnRechercherClicked(wxCommandEvent& event)
{
	
	admin.effacerConfigPresente();
	//Envoie requête Broadcast 2 fois
	admin.envoyer("OK");
	admin.ecouter();
	admin.envoyer("OK");
	admin.ecouter();

	std::string configAff;
	if (admin.getConfigPresente() == "\n") {

		configAff = "Aucun utilisateur trouvé";
		wxMessageBox(configAff, _T("Information"), wxICON_INFORMATION);
	}
	else {

		configAff = "Voici la configuration trouvée : \n" + admin.getConfigPresente();


		//Affichage de la configuration trouvée
		wxMessageBox(configAff, _T("Information"), wxICON_INFORMATION);
		
		//Demande enregistrement
		int reponse = wxMessageBox(_T("Voulez vous enregistrer la configuration réseau pour un usage ultérieur?"), _T("Confirmation"), wxYES_NO | wxICON_QUESTION);

		if (reponse == wxYES) {

			//Enregistrement
			int resultat =admin.enregistrerConfigPresente();



			//UPDATE AFFICHAGE TO DO
			if (resultat == 1){
				wxMessageBox("Un problème est survenue lors de l'enregistrement", _T("Erreur"), wxICON_INFORMATION);
			}
			else if (resultat == 2) {
				wxMessageBox("Un problème est survenue lors de l'enregistrement", _T("Erreur"), wxICON_INFORMATION);
			}
			else {
				str.Add(admin.getDerniereDate());
				delete choixConfig;
				choixConfig = new wxListBox(panelAffichage, ID_BTN_CHOIX, wxPoint(0, 0), wxDefaultSize, str);
				cadre->Add(choixConfig, 1, wxALL | wxEXPAND, 1);
				panelAffichage->Layout();
			}
		}
		

		//Demande  pour l'utilisation de la configuration trouvée
		int reponse2 = wxMessageBox(_T("Voulez vous utiliser la configuration réseau ?"), _T("Confirmation"), wxYES_NO | wxICON_QUESTION);

		if (reponse2 == wxYES) {

			config_choisie = "list";
			Show(false);
		}
		else if (reponse2 == wxNO) {

				admin.effacerConfigPresente();
		}
		
	}
}

void FenetreConfigReseau::OnBtnEffacerToutClicked(wxCommandEvent& event) {
	
	//Demande suppression de toutes les configurations
	int reponse = wxMessageBox(_T("Voulez vous effacer toutes les configurations réseaux ?"), _T("Confirmation"), wxYES_NO | wxICON_QUESTION); 

	if (reponse == wxYES) {

		//Suppression
		admin.effacerToutesLesConfig();
		str.clear();

		//UPDATE affichage
		delete choixConfig;
		choixConfig = new wxListBox(panelAffichage, -1, wxPoint(0, 0), wxDefaultSize, str);
		cadre->Add(choixConfig, 1, wxALL | wxEXPAND, 1);
		panelAffichage->Layout();

		//Information par rapport à la suppression
		wxMessageBox(_T("Toutes les configurations réseaux ont été effacées"), _T("Information"), wxICON_INFORMATION); 
	}
}

void FenetreConfigReseau::OnBtnEffacerClicked(wxCommandEvent& event) {

	//Si aucune configuration est sélectionée
	if (choixConfig->GetSelection()== wxNOT_FOUND){

		wxMessageBox(_T("aucune configuration sélectionnée"), _T("Information"), wxICON_INFORMATION);
	}
	else {
		
		//Confirmation de la suppression
		int reponse = wxMessageBox(_T("Voulez vous effacer la configuration sélectionnée ?"), _T("Confirmation"), wxYES_NO | wxICON_QUESTION);

		if (reponse == wxYES) {

			//Suppression de la configuration
			wxString list = choixConfig->GetString(choixConfig->GetSelection());
			admin.effacerUneConfigEnregistrée(list.ToStdString());
			str.Remove(list);

			//UPDATE affichage
			delete choixConfig;
			choixConfig = new wxListBox(panelAffichage, -1, wxPoint(0, 0), wxDefaultSize, str);
			cadre->Add(choixConfig, 1, wxALL | wxEXPAND, 1);
			panelAffichage->Layout();

			//Information par rapport à la suppression
			wxMessageBox(_T("La configuration a été supprimée"), _T("Information"), wxICON_INFORMATION); 
		}
	}
}

void FenetreConfigReseau::OnBtnConsulterClicked(wxCommandEvent& event) {

	
	std::string config;

	//Si aucune configuration est sélectionnée
	if (choixConfig->GetSelection() == wxNOT_FOUND){

		wxMessageBox(_T("aucune configuration sélectionnée"), _T("Information"), wxICON_INFORMATION); 
	}
	else {
		
		if (admin.getUneConfigEnregistrée(choixConfig->GetString(choixConfig->GetSelection()).ToStdString()) == "\n") {

			config = choixConfig->GetString(choixConfig->GetSelection()).ToStdString() + "\n" + "\n Pas d'utilisateurs enregistrés";
		}
		else {

			config = choixConfig->GetString(choixConfig->GetSelection()).ToStdString() + "\n" + admin.getUneConfigEnregistrée(choixConfig->GetString(choixConfig->GetSelection()).ToStdString());
		}

		//Affichage configuration sélectionnée
		wxMessageBox(config, _T("Information"), wxICON_INFORMATION); 
	}
}

void FenetreConfigReseau::OnClose(wxCloseEvent& event)
{
	
	Show(false);
	
	
	/*if (choixConfig->GetSelection() == wxNOT_FOUND)
	{
		//Message si pas de configuration sélectionnée
		int reponse = wxMessageBox(_T("Si vous quitter la configuration choisie sera :\n") + config + _T("\nVoulez-vous quitter ?"), _T("Confirmation"), wxYES_NO | wxICON_QUESTION);
		//Enregistrement de la configuration choisie
		if (reponse == wxYES) {
			config_choisie = choixConfig->GetString(choixConfig->GetSelection()).ToStdString();
			Show(false);
	}
	else {
		std::string config = admin.getUneConfigEnregistrée(choixConfig->GetString(choixConfig->GetSelection()).ToStdString());
		int reponse = wxMessageBox(_T("Si vous quitter la configuration choisie sera :\n")+config+ _T("\nVoulez-vous quitter ?"), _T("Confirmation"), wxYES_NO | wxICON_QUESTION);
		//Enregistrement de la configuration choisie
		if (reponse == wxYES) {
			config_choisie = choixConfig->GetString(choixConfig->GetSelection()).ToStdString();
			Show(false);
		}
	}*/
}

std::string FenetreConfigReseau::get_Config_Choisie() {
	return config_choisie;
}

void FenetreConfigReseau::recuperer_AdrIP(std::list<std::string>* ip) {
	admin.recuperer_AdrIP(ip);
}

void FenetreConfigReseau::recuperer_AdrIPXML(std::list<std::string>* ip, std::string nomConfig) {
	

	TiXmlDocument doc(CHEMIN_PHYSIQUE);
	if (!doc.LoadFile()) {
		std::cerr << "erreur lors du chargement" << std::endl;
		std::cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << std::endl;

	}

	bool trouve = false;
	TiXmlHandle hdl(&doc);
	TiXmlElement* f = doc.FirstChildElement();
	TiXmlElement* elem = hdl.FirstChildElement().FirstChildElement().Element();
	TiXmlElement* elem2 = NULL;

	while (elem && !trouve) {

		if (std::string(elem->Attribute("date")) == nomConfig) {

			trouve = true;
			break;
		}

		elem = elem->NextSiblingElement();
	}

	if (!trouve) {

		std::cerr << "user inexistant" << std::endl;
	}
	else {

		elem2 = elem->FirstChildElement();
	}

	std::string espace = " ; ";
	if (elem2 != NULL) {

		while (elem2) {

			ip->push_back(elem2->Attribute("ip"));
			elem2 = elem2->NextSiblingElement();
		}
	}

	doc.SaveFile(CHEMIN_PHYSIQUE);
}
