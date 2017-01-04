
// --- constantes des broches ---

const int RVar=0; //declaration constante de broche analogique

// --- Déclaration des variables globales ---
int mesureBrute=0;// Variable pour acquisition résultat brut de conversion analogique numérique

//**************** FONCTION SETUP = Code d'initialisation *****

void setup()   { // debut de la fonction setup()

Serial.begin(115200); // initialise connexion série à 115200 bauds
// IMPORTANT : régler le terminal côté PC avec la même valeur de transmission 


} // fin de la fonction setup()

//***** FONCTION LOOP = Boucle sans fin *****


void loop(){ // debut de la fonction loop()

// acquisition conversion analogique-numerique (CAN) sur la voie analogique
mesureBrute=analogRead(RVar);

// affiche valeur numerique entière ou à virgule au format décimal
Serial.println(mesureBrute);

delay(100);

} // fin de la fonction loop() - le programme recommence au début de la fonction loop sans fin
