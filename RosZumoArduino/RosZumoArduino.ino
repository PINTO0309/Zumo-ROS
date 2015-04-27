#include <Wire.h>
#include <LSM303.h>
#include <ZumoMotors.h>

long timer=0;
int vmax = 250;
int vright = 0;
int vleft = 0;

LSM303 compass;
ZumoMotors motors;

// Buffer qui va contenir la trame série
#define TAILLE_MAX 32
 
// Buffer qui va contenir le texte (taille du buffer / 2, oui j'ai mis ça au pif)
char texte[TAILLE_MAX];
// Données utiles extraites
int cons_vitesse,cons_angle,angle;
float vitesse;

void setup() {
  Serial.begin(9600);
  Serial.println("Serial");
  Wire.begin();
  compass.init();
  compass.enableDefault();
}


void loop()
{
// Récupération d'une trame + parsing
  if(recupInfo(texte,&cons_vitesse,&cons_angle)==1) {Serial.println("Erreur de trame 1!");}
  if(recupInfo(texte,&cons_vitesse,&cons_angle)==2) {Serial.println("Erreur de trame 2!");}

if (cons_vitesse>-30 && cons_vitesse<30) // tourne sur place
  {vleft=int(vmax*float(cons_angle)/100);vright=int(-vmax*float(cons_angle)/100);}
else
  {
    if (cons_angle>-30 && cons_angle<30) //tout droit
      {vleft=int(vmax*float(cons_vitesse)/100);vright=int(vmax*float(cons_vitesse)/100);}
    else if (cons_angle<=-30) //tourne a gauche
      {vright=int(vmax*float(cons_vitesse)/100*(0.4-(float(cons_angle)/100)));vleft=int(float(vright)*(1+float(cons_angle)/100));}
    else if (cons_angle>=30) //tourne a droite
      {vleft=int(vmax*float(cons_vitesse)/100*(0.4+(float(cons_angle)/100)));vright=int(float(vleft)*(1-float(cons_angle)/100));} 
  }

  
  motors.setRightSpeed(vright);
  motors.setLeftSpeed(vleft);
  delay(1);


// Envoie les infos de la centrale
  compass.read();
  timer = millis();
  Serial.print("!");Serial.print("AN:");
  Serial.print(timer); Serial.print (",");   
  Serial.print (compass.a.x);Serial.print (",");
  Serial.print (compass.a.y); Serial.print (","); 
  Serial.print (compass.a.z);Serial.print (","); 
  Serial.print (compass.m.x);Serial.print (",");
  Serial.print (compass.m.y);Serial.print (",");  
  Serial.print (compass.m.z);Serial.print(",");
  Serial.print (vleft);
  Serial.print (",");
  Serial.print (vright);
  Serial.println(); 
  
}


int recupInfo(char *texte, int *cons_vitesse,int *cons_angle) {
  char c, buf[TAILLE_MAX + 1];
  unsigned char i = 0;
    if(Serial.available() > 1){/* Attente de 2 char sur le port série */
  while(Serial.read() != '~' && Serial.read() != 'X'); /* Tant que chaine != ~X -> boucle */
  /* Remplissage du buffer */
  do{
    if(i == (TAILLE_MAX + 1)) /* Si la chaine a dépassé la taille max du buffer*/
      return 1;/* retourne 1 -> erreur */
    while(Serial.available() < 1); /* Attente d'un char sur le port série */ 
  } 
  while((buf[i++] = Serial.read()) != '#'); /* Tant que char != # (fléche) -> boucle */
 
  /* Copie le texte au début de buf[] dans texte[] */
  i = 0;
  while((texte[i] = buf[i]) != '#') i++;
  texte[i] = '\0';
  /* Parse la chaine de caractères et extrait les champs */
  Serial.println(texte);
  if(sscanf(texte, "X;%d;%d;",cons_vitesse,cons_angle) != 2)
    {//Serial.print("test");Serial.print(*cons_vitesse);Serial.println(*cons_angle);
  return 2;} /* Si sscanf n'as pas pu extraire les 2 champs -> erreur*/
  
  return 0;/* retourne 0 -> pas d'erreur */
}
}

