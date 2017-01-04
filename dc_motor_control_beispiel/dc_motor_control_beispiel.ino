
// --- Programme Arduino --- 
// Trame de code g�n�r�e par le g�n�rateur de code Arduino
// du site www.mon-club-elec.fr 

// Auteur du Programme : X. HINAULT - Tous droits r�serv�s 
// Programme �crit le : 2/5/2012.

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
 /* Teste la rotation des moteurs.  */ 

// --- Fonctionnalit�s utilis�es --- 


// -------- Circuit � r�aliser --------- 


// /////////////////////////////// 1. Ent�te d�clarative /////////////////////// 
// A ce niveau sont d�clar�es les librairies incluses, les constantes, les variables, les objets utiles...

// --- D�claration des constantes ---

// --- Inclusion des librairies ---

// --- D�claration des constantes utiles ---

// --- D�claration des constantes des broches E/S num�riques ---

const int vitesseMotA=3; // Constante pour la broche 3
const int sensMotA=12; // Constante pour la broche 12
const int freinMotA=9; // Constante pour la broche 9
const int intensiteMotA=A0; // intensit� du moteur 0

const int vitesseMotB=11; // Constante pour la broche 11
const int sensMotB=13; // Constante pour la broche 13
const int freinMotB=8; // Constante pour la broche 8
const int intensiteMotB=A1; // intensit� du moteur 1


// --- D�claration des constantes des broches analogiques ---


//const int Voie[6]={0,1,2,3,4,5}; //declaration constante de broche analogique


// --- D�claration des variables globales ---


// --- D�claration des objets utiles pour les fonctionnalit�s utilis�es ---


// ////////////////////////// 2. FONCTION SETUP = Code d'initialisation ////////////////////////// 
// La fonction setup() est ex�cut�e en premier et 1 seule fois, au d�marrage du programme

void setup()   { // debut de la fonction setup()

// --- ici instructions � ex�cuter 1 seule fois au d�marrage du programme --- 

// ------- Initialisation fonctionnalit�s utilis�es -------  

Serial.begin(115200); 

// ------- Broches en sorties num�riques -------  
 pinMode (vitesseMotA,OUTPUT); // Broche vitesseMotA configur�e en sortie
 pinMode (freinMotA,OUTPUT); // Broche freinMotA configur�e en sortie
 pinMode (vitesseMotB,OUTPUT); // Broche vitesseMotB configur�e en sortie
 pinMode (sensMotA,OUTPUT); // Broche sensMotA configur�e en sortie
 pinMode (sensMotB,OUTPUT); // Broche senMotB configur�e en sortie

// ------- Broches en entr�es num�riques -------  

// ------- Activation si besoin du rappel au + (pullup) des broches en entr�es num�riques -------  

// ------- Initialisation des variables utilis�es -------  

// ------- Codes d'initialisation utile -------  
digitalWrite(vitesseMotA,LOW); // a l'arret
digitalWrite(sensMotA,LOW); 
digitalWrite(freinMotA,LOW); // frein off 

digitalWrite(vitesseMotB,LOW); // � l'arret 
digitalWrite(sensMotB,LOW);  
digitalWrite(freinMotB,LOW); // frein off

} // fin de la fonction setup()
// ********************************************************************************

////////////////////////////////// 3. FONCTION LOOP = Boucle sans fin = coeur du programme //////////////////
// la fonction loop() s'ex�cute sans fin en boucle aussi longtemps que l'Arduino est sous tension

void loop(){ // debut de la fonction loop()


//------ test initial du moteur A ---- 
//- sens 1
digitalWrite(sensMotA,LOW); // sens 1
digitalWrite(vitesseMotA, HIGH); // vitesse maximale
delay(2000); // 2 secondes
digitalWrite(vitesseMotA, LOW); // vitesse maximale

//- sens 2
digitalWrite(sensMotA,HIGH); // sens 2
digitalWrite(vitesseMotA, HIGH); // vitesse maximale
delay(1000); // 2 secondes
Serial.println(analogRead(intensiteMotA)); 
delay(1000); // 2 secondes
digitalWrite(vitesseMotA, LOW); // vitesse maximale


//------ test initial du moteur B ---- 
//- sens 1
digitalWrite(sensMotB,LOW); // sens 1
digitalWrite(vitesseMotB, HIGH); // vitesse maximale
delay(2000); // 2 secondes
digitalWrite(vitesseMotB, LOW); // vitesse maximale

//- sens 2
digitalWrite(sensMotB,HIGH); // sens 2
digitalWrite(vitesseMotB, HIGH); // vitesse maximale
delay(1000); // 2 secondes
Serial.println(analogRead(intensiteMotB)); 
delay(1000); // 2 secondes
digitalWrite(vitesseMotB, LOW); // vitesse maximale

//---- test vitesse variable moteur A --- 

for (int i=0; i<=255; i++) {
  analogWrite(vitesseMotA,i); // PWM croissant
  delay(50); // pause
  Serial.println(analogRead(intensiteMotA)); 

}

for (int i=0; i<=255; i++) {
  analogWrite(vitesseMotA,255-i); // PWM d�croissant
  delay(50); // pause
  Serial.println(analogRead(intensiteMotA)); 
}

//---- test vitesse variable moteur B --- 

for (int i=0; i<=255; i++) {
  analogWrite(vitesseMotB,i); // PWM croissant
  delay(50); // pause
  Serial.println(analogRead(intensiteMotB)); 

}

for (int i=0; i<=255; i++) {
  analogWrite(vitesseMotB,255-i); // PWM d�croissant
  delay(50); // pause
  Serial.println(analogRead(intensiteMotB)); 

}


while(1); // stop loop

} // fin de la fonction loop() - le programme recommence au d�but de la fonction loop sans fin
// ********************************************************************************

