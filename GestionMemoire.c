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

	if (mem->tp->entreeValide[page]) {
		req->estDansTablePages = 1;
		req->adressePhysique = calculerAdresseComplete(mem->tp->numeroCadre[page], calculerDeplacementDansLaPage(req->adresseVirtuelle));
	}
}

void ajouterDansMemoire(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	for(int i = 0; i < TAILLE_MEMOIRE; i++) {
		if (!mem->memoire->utilisee[i]) {
			mem->memoire->utilisee[i] = 1;
			mem->memoire->numeroPage[i] = calculerNumeroDePage(req->adresseVirtuelle);
			mem->memoire->dernierAcces[i] = req->date;
			mem->memoire->dateCreation[i] = req->date;
			req->adressePhysique = calculerAdresseComplete(i, calculerDeplacementDansLaPage(req->adresseVirtuelle));
			return;
		}
	}
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
