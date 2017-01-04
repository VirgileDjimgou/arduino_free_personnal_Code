
// --- Programme Arduino --- 
// Trame de code g�n�r�e par le g�n�rateur de code Arduino
// du site www.mon-club-elec.fr 

// Auteur du Programme : X. HINAULT - Tous droits r�serv�s 
// Programme �crit le : 6/11/2010.

// ------- Licence du code de ce programme ----- 
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License,
//  or any later version.
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

// ////////////////////  PRESENTATION DU PROGRAMME //////////////////// 

// -------- Que fait ce programme ? ---------
 /*  Ecrit la valeur courante de milliS dans un fichier 
  sur la carte SD et affiche le fichier dans le Terminal S�rie 
 */ 

// --- Fonctionnalit�s utilis�es --- 

// Utilise la connexion s�rie vers le PC 
// Utilise une carte m�moire micro-SD avec le module Ethernet Arduino 

// -------- Circuit � r�aliser --------- 

// La connexion s�rie vers le PC utilise les broches 0 et 1 (via le c�ble USB) 

// Le module Ethernet est � enficher broche � broche sur la carte Arduino 
// Connecter broche SCLK du module Ethernet sur la broche 13
// Connecter broche MISO du module Ethernet sur la broche 12
// Connecter broche MOSI du module Ethernet sur la broche 11
// Connecter broche Select Ethernet du module Ethernet sur la broche 10
// Connecter broche Select SD Card du module Ethernet sur la broche 4
// Le module Ethernet est compatible avec la carte Mega via le connecteur ICSP


// /////////////////////////////// 1. Ent�te d�clarative /////////////////////// 
// A ce niveau sont d�clar�es les librairies incluses, les constantes, les variables, les objets utiles...

// --- D�claration des constantes ---

// --- Inclusion des librairies ---

#include <SdFat.h> // inclusion de la librairie pour carte m�moire SD en connexion SPI

// --- D�claration des constantes utiles ---

// --- D�claration des constantes des broches E/S num�riques ---

const int brocheSDCardSelect=4; // broche utilis�e pour s�lectionner la SD card 

// --- D�claration des constantes des broches analogiques ---


// --- D�claration des variables globales ---


int testSD; // Variable utilis�e pour tester valeur renvoy�e par fonctions SD Card


// --- D�claration des objets utiles pour les fonctionnalit�s utilis�es ---

Sd2Card card; // cr�ation d'un objet Sd2Card
SdVolume volume; // cr�ation d'un objet SdVolume
SdFile root; // cr�ation d'un objet SdFile pour le r�pertoire racine
SdFile file; // cr�ation d'un objet SdFile pour le fichier utilis�


// ////////////////////////// 2. FONCTION SETUP = Code d'initialisation ////////////////////////// 
// La fonction setup() est ex�cut�e en premier et 1 seule fois, au d�marrage du programme

void setup()   { // debut de la fonction setup()

// --- ici instructions � ex�cuter 1 seule fois au d�marrage du programme --- 

// ------- Initialisation fonctionnalit�s utilis�es -------  

Serial.begin(115200); // initialise connexion s�rie � 115200 bauds
// IMPORTANT : r�gler le terminal c�t� PC avec la m�me valeur de transmission 

//---- initialise l'utilisation de la carte m�moire SD en mode SPI  
pinMode(10, OUTPUT); // met la broche 10 (SS) en sortie (n�cessaire avec module ethernet)
digitalWrite(10, HIGH); // mais d�sactive le  circuit int�gr� W5100 du module ethernet!

testSD= card.init(SPI_HALF_SPEED,brocheSDCardSelect); // initialise la carte SD � la vitesse et avec la broche CS voulues
if (testSD==1) Serial.println ("Initialisation SD card OK"); else ("Erreur initialisation SD Card");

testSD=volume.init(&card); // initialise le volume FAT de la carte SD 
if (testSD==1) Serial.println ("Initialisation Volume FAT OK"); else ("Erreur initialisation Volume FAT");

testSD=root.openRoot(&volume); // ouverture du r�pertoire root
if (testSD==1) Serial.println ("Ouverture repertoire root OK"); else ("Echec ouverture repertoire root");

// ------- Broches en sorties num�riques -------  

// ------- Broches en entr�es num�riques -------  

// ------- Activation si besoin du rappel au + (pullup) des broches en entr�es num�riques -------  

// ------- Initialisation des variables utilis�es -------  

} // fin de la fonction setup()
// ********************************************************************************

////////////////////////////////// 3. FONCTION LOOP = Boucle sans fin = coeur du programme //////////////////
// la fonction loop() s'ex�cute sans fin en boucle aussi longtemps que l'Arduino est sous tension

void loop(){ // debut de la fonction loop()


// --- ici instructions � ex�cuter par le programme principal --- 

//
//---- ouverture du r�pertoire root 
if (!root.isOpen()) { // si root pas ouvert 
  testSD=root.openRoot(&volume); // si root pas ouvert, on l'ouvre

  if (testSD==1) Serial.println ("Ouverture repertoire root OK"); else ("Echec ouverture repertoire root");

}

else Serial.println("Root deja ouvert"); // si root d�j� ouvert

//----- ouverture de fichier ---- 

// nom du fichier court (8 caract�res maxi . 3 caract�res maxi) (dit format DOS 8.3)
char nomFichier[] = "FICHIER.TXT"; 

file.writeError = false; // r�initialise la variable writeError

// ouverture / cr�ation fichier en �criture + position fin du fichier
testSD=file.open(&root, nomFichier, O_CREAT| O_APPEND | O_WRITE); 
    // O_CREAT - cr�e le fichier si il n'existe pas
    // O_APPEND - se postionne � la fin du fichier avant chaque �criture
    // O_WRITE - ouverture en mode �criture de donn�e

if (testSD==1) Serial.print ("Ouverture fichier "), Serial.print(nomFichier),Serial.println(" OK"); 
else Serial.println ("Echec ouverture fichier");

//----- effacement du fichier 
// R�duit la longueur du fichier � la valeur indiqu�e. 
testSD=file.truncate(0); // Longueur = 0 pour effacement complet du fichier
if (testSD==1) Serial.print ("Effacement fichier"), Serial.print(nomFichier),Serial.println(" OK"); 
else Serial.println ("Echec effacement fichier");

//----- Ecriture dans le fichier ----- 

file.print("Ecriture de la valeur millis() dans le fichier = ");
delay(10); 
file.println(millis());

//----- v�rification erreur ecriture 
// lit la variable writeError - attention writeError n'est pas une fonction
if (file.writeError==1) Serial.println ("Une erreur est survenue lors de l'ecriture"); else Serial.println("Ecriture reussie"); 

//----- fermeture de fichier -----
testSD=file.close(); // fermeture fichier
if (testSD==1) Serial.println("Fermeture fichier reussie !"); 


//*************** lecture fichier sur SD Card ***************

//---- ouverture du fichier en lecture --- 
//char nomFichier[] = "FICHIER.TXT"; // nom du fichier court (8 caract�res maxi . 3 caract�res maxi) (dit format DOS 8.3)

testSD=file.open(&root, nomFichier, O_READ); // ouverture du fichier en lecture
if (testSD==1) Serial.print ("Ouverture fichier "), Serial.print(nomFichier),Serial.println(" en lecture OK"); 
else Serial.println ("Echec ouverture fichier");

//------ lecture des donn�es du fichier ----- 

Serial.println(); 
Serial.println("****** Debut du fichier ******"); 

int c; // variable de reception donn�es lues

// lit tous les octets disponibles et les affiche sur le port s�rie
while ((c = file.read()) > 0) Serial.print(char(c));  

Serial.println("****** fin du fichier ******"); 
Serial.println(); 

//----- fermeture de fichier -----
testSD=file.close(); // fermeture fichier
if (testSD==1) Serial.println("Fermeture fichier reussie !"); 


//delay(3000); 

while(1); // stop loop

} // fin de la fonction loop() - le programme recommence au d�but de la fonction loop sans fin
// ********************************************************************************


// ////////////////////////// FONCTIONS DE GESTION DES INTERRUPTIONS //////////////////// 


// ////////////////////////// AUTRES FONCTIONS DU PROGRAMME //////////////////// 


// ////////////////////////// Fin du programme //////////////////// 


