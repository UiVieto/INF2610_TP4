#include "./libs/lib.h"
unsigned int calculerNumeroDePage(unsigned long adresse) {
	return adresse >> 10;
}

unsigned long calculerDeplacementDansLaPage(unsigned long adresse) {
    return adresse % 1024;
}

unsigned long calculerAdresseComplete(unsigned int numeroDePage, unsigned long deplacementDansLaPage) {
    return (numeroDePage << 10) + deplacementDansLaPage;
}

void rechercherTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	u_int8_t estPresent;
	int page = calculerNumeroDePage(req->adresseVirtuelle);
	req->estDansTLB = 0;
	req->adressePhysique = 0;

	for (int i = 0; i < TAILLE_TLB; i++) {
		estPresent = mem->tlb->entreeValide[i];
		if (mem->tlb->numeroPage[i] == page && estPresent) {
			req->estDansTLB = 1;
			req->adressePhysique = calculerAdresseComplete(mem->tlb->numeroCadre[i], calculerDeplacementDansLaPage(req->adresseVirtuelle));
			req->date = mem->tlb->dernierAcces[i];
			return;
		}
	}
}

void rechercherTableDesPages(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	u_int8_t estPresent;
	int page = calculerNumeroDePage(req->adresseVirtuelle);
	req->estDansTablePages = 0;
	req->adressePhysique = 0;

	for (int i = 0; i < TAILLE_TP; i++) {
		estPresent = mem->tp->entreeValide[i];
		if (mem->tp->numeroCadre[i] == page && estPresent) {
			req->estDansTablePages = 1;
			req->adressePhysique = calculerAdresseComplete(mem->tp->numeroCadre[i], calculerDeplacementDansLaPage(req->adresseVirtuelle));
			return;
		}
	}
}

void ajouterDansMemoire(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	//TODO
}

void mettreAJourTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	// TODO
}

// NE PAS MODIFIER
int main() {
    evaluate(
		&calculerNumeroDePage, 
		&calculerDeplacementDansLaPage, 
		&calculerAdresseComplete, 
        &rechercherTLB, 
		&rechercherTableDesPages,
		&mettreAJourTLB,
		&ajouterDansMemoire
    );
    return 0;
}
