#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>
#include <string.h>

/*création de la fonction nbrpremier qui va nous permettre de générer un très 
grand nombre premier
En plus de la fonction nbrpremiere qui est la même chose mais juste pour 
la variable e*/

int nbrpremier(int n, int n1,mpz_t p,mpz_t q) {

	
	unsigned long seed;
	mpz_t a, b, c;
	gmp_randstate_t r;

	mpz_inits(a, b, c, p, q, NULL);

	seed = time(NULL);
	gmp_randinit_default(r);
	gmp_randseed_ui(r, seed);


	// generation d’un nombre premier de n bits sur p
	do {
		mpz_urandomb(p, r, n);
		mpz_setbit(p, n-1 );
		mpz_nextprime(p, p);

	} while (mpz_sizeinbase(p, 2) != n);
	// generation d'un nombre premier de n bits sur q
	do {
		mpz_urandomb(q, r, n1);
		mpz_setbit(q, n1 - 1);
		mpz_nextprime(q, q);

	} while (mpz_sizeinbase(q, 2) != n1);
}

int nbrpremiere(int n, mpz_t p) {


	unsigned long seed;
	mpz_t a, b, c;
	gmp_randstate_t r;

	mpz_inits(a, b, c, p, NULL);

	seed = time(NULL);
	gmp_randinit_default(r);
	gmp_randseed_ui(r, seed);


	// generation d’un nombre premier de n bits sur p
	do {
		mpz_urandomb(p, r, n);
		mpz_setbit(p, n - 1);
		mpz_nextprime(p, p);

	} while (mpz_sizeinbase(p, 2) != n);
}

/*Création de la fonction qui permet d'élever un nombre à une puissance
modulo un entier qui finalement ne nous a pas été utile*/

/*int exponentiation_modulaire(mpz_t exp, mpz_t mod, mpz_t base)
{
	mpz_t e1, res;
	mpz_inits(e1,)
	while (e1 < exp)
		 Cette boucle nous permet de calculer le nombre à la puissance
		e modulo m 
	{
		res = (base * res) % mod;
		e1 += 1;
	};
	return res;
}*/

/* On a fait une fonction concaténation pour nous permettre de concatener toute les informations données pour le certificat electronique */

void concatenation(char nom[100], char prenom[100], char datenaissance[100], char email[100], mpz_t identite[10000], mpz_t e, int a, int b, int c, int d)
{
	int i,j,k,l;
	for (i = 0; i < a; i++)
	{
		mpz_set_ui(identite[i], (int)nom[i]);
	}
	for (j = 0; j < b; j++)
	{
		mpz_set_ui(identite[j+a], (int)prenom[i]);
	}
	for (k = 0; k < c; k++)
	{
		mpz_set_ui(identite[k+a+b], (int)datenaissance[i]);
	}
	for (l = 0; l < d; l++)
	{
		mpz_set_ui(identite[l+a+b+c], (int)email[i]);
	}
	mpz_set(identite[a + b + c + d], e);
}

/* la fonction chiffrement_certif nous permet de chiffrer les données du certificat electronique */

void chiffrement_certif(mpz_t identite[10000], mpz_t identitecettifie[10000], int nbcar, mpz_t e, mpz_t n)
{
	int i;
	for (i = 0; i < nbcar; i++)
	{
		mpz_powm(identitecettifie[i], identite[i], e, n);
	}
}

/* La fonction verif sert à verifier que identite et identitecettifie sont bien les mêmes pour le certificat electronique */

void verif(mpz_t verifcertif[10000], mpz_t identite[10000], mpz_t identitecettifie[10000], mpz_t d, mpz_t n, int nbcar)
{
	int j;
	for (j = 0; j < nbcar; j++) {
		mpz_powm(verifcertif[j], identitecettifie[j], d, n);
		if (mpz_cmp(verifcertif[j], identite[j]) != 0) {
			printf("La certificat electronique est pas correcte.\n");
			break;
		}
	}
	printf("Le certificat electronique est correct\n");
}

/* la fonction verification nous sert a verifier si c1 et c2 (ici tabM et tabM3) sont les même pour la signature numérique */

void verificaton(mpz_t tabM[10][64], mpz_t tabM2[10][64], mpz_t e, mpz_t n, int compt, mpz_t tabM3[10][64])
{
	int j, i;
	for (i = 0; i < compt; i++)
	{
		for (j = 0; j < 64; j++) {
			mpz_powm(tabM3[i][j], tabM2[i][j], e, n);
			if (mpz_cmp(tabM[i][j], tabM3[i][j])!=0) {
				printf("La signature numerique est pas correcte.\n");
				break;
			}
		}

	};
	printf("La signature est bonne.\n");

}

/* la fonction dechiffrement_matrice nous sert a calculer c (ici tabM2) pour la signature numerique */

void dechiffrement_matrice(mpz_t tabM[10][64], mpz_t tabM2[10][64], mpz_t d, mpz_t n, int compt)
{
	int j, i;
	for (i = 0; i < compt; i++)
	{
		for (j = 0; j < 64; j++) {
			mpz_powm(tabM2[i][j], tabM[i][j], d, n);
		}
	};

}

/* la fonction MD8 est notre fonction de hashage */

void MD8(char M[], mpz_t tabM[10][64], int compt)
{
	int j, t;
	for (j = 0; j < compt; j++)
	{
		for (t = 0; t < 64; t++)
		{
			mpz_set_ui(tabM[j][t], (int)M[j * 64 + t]);
		};
	};
}


int main()
{
/* déclaration et initialisation de toute les variables et tableaux necessaires et on demande a l'utilisaturs de rentrer toutes les informations necessaires */
	int  k, lp, lq, tphi, i, i1, i2, i3, compt = 1, i4, t, u, v, w, i5, nbcar;
	mpz_t p, q, n, phi, p1, q1, e, d, c, m, messdechi, tabM2[10][64], tabM[10][64], tabM3[10][64], identite[10000], identitecettifie[10000], verifcertif[10000];
	char M[10000], nom[100], prenom[100], datenaissance[100], email[100];
	printf("Entrez votre nom: ");
	scanf("%s", &nom);
	printf("\nEntrez votre prenom: ");
	scanf("%s", &prenom);
	printf("\nEntrez votre date de naissance (jj/mm/aaaa): ");
	scanf("%s", &datenaissance);
	printf("\nEntrez votre email: ");
	scanf("%s", &email);
	printf("\nEntrer la taille du module de chiffrement que vous souhaitez: ");
	scanf("%d", &k);
	printf("\nEntrez le message que vous voulez chiffrer: ");
	scanf("%s", &M);
	
	i1 = strlen(M);
	t = strlen(nom);
	u = strlen(prenom);
	v = strlen(datenaissance);
	w = strlen(email);
	nbcar = t + u + v + w;

	while (i1 > 64)
	{
		compt++;
		i1 -= 64;
	};

/* on initialise les tableaux de type mpz_t pour pouvoir les utiliser dans nos fonctions */
	mpz_inits(n, phi, p1, q1, e, c, d, m, messdechi, tabM2, tabM, tabM3, identite, identitecettifie, verifcertif, NULL);
	for (i2 = 0; i2 < compt; i2++)
	{
		for (i3 = 0; i3 < 64; i3++)
		{
			mpz_init(tabM[i2][i3]);
			mpz_init(tabM2[i2][i3]);
			mpz_init(tabM3[i2][i3]);
		}

	}
	for (i5 = 0; i5 < (t + u + v + w); i5++) {
		mpz_init(identite[i5]);
		mpz_init(identitecettifie[i5]);
		mpz_init(verifcertif[i5]);

	}

/* on initialise les valeurs lp et lq pour nous permettre de generer des nombres premiers aléatoires de type mpz_t */
	if (k % 2 == 0) {
		lp = k / 2;
		lq = k / 2;
	}
	else {
		lp = (k - 1) / 2;
		lq = lp + 1;
	};

/* on récupère 2 valeurs aléatoires, des nombres premiers de taille lp et lq, puis on initialise n */
	nbrpremier(lp, lq,p,q);
	mpz_mul(n, p, q); /* n = p * q */

/* on récupère la valeur de phi ((p-1) x (q-1)) qui nous permet de prendre son nombre de bit et initialiser tphi a cette valeur pour avoir e, notre cle publique */
	mpz_sub_ui(p1, p, 1);
	mpz_sub_ui(q1, q, 1);
	mpz_mul(phi, p1, q1);

	tphi = mpz_sizeinbase(phi, 2);
	
	mpz_setbit(e, tphi-2);
	nbrpremiere(tphi-2, e);

/* grace a e, nous pouvons donc obtenir la valeur de d, la cle privée */
	mpz_invert(d, e, phi);

	gmp_printf("La cle publique est: %Zd\n", e);

/* l'appel de toute les fonctions dont nous avons besoin pour le chiffrement, la signature numérique, et le certificat electronique */
	MD8(M, tabM, compt);
	dechiffrement_matrice(tabM, tabM2, d, n, compt);
	verificaton(tabM, tabM2, e, n, compt, tabM3);
	concatenation(nom, prenom, datenaissance, email, identite, e, t, u, v, w);
	chiffrement_certif(identite, identitecettifie, (t + u + v + w), e, n);
	verif(verifcertif, identite, identitecettifie, d, n, nbcar);
	printf("Voici le certificat de %s %s: ", nom, prenom);

/* on affiche le certificat electronique de l'utilisateur */
	for (i4 = 0; i4 < nbcar; i4++)
	{
		gmp_printf("%Zd\n", identitecettifie[i4]);
	}
		
}