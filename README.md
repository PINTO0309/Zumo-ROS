# ZUMO Robot Package for ROS
Ce package permet d'interfacer le robot Zumo Pololu Arduino sous ROS.
<https://www.pololu.com/product/2506>

![alt text](https://a.pololu-files.com/picture/0J4110.1200.jpg?a2562fe9f34e986d0d194118dcff0d58)

## ZUMO node
	rosrun zumo zumo.py
Ce noeud permet la communication série avec la carte Arduino et son shield Zumo.
Le robot envoie une trame d'état de ses capteurs et actionneurs :
`!AN:[TEMPS],[ACCELERATION_X],[ACCELERATION_Y],[ACCELERATION_Z],[MAGNETOMETRE_X],[MAGNETOMETRE_Y],[MAGNETOMETRE_Z],[VITESSE_MOTEUR_LEFT],[VITESSE_MOTEUR_RIGHT]\r\n`
et attend la réception de la trame de consigne :
`~X;[CONSIGNE_VITESSE];[CONSIGNE_ANGLE];#`

A partir de la trame capteurs, le noeud publie 2 topics :

- `/zumo/imu` de type `sensor_msgs.Imu`
- `/zumo/mag` de type `sensor_msgs.MagneticField`

La construction de la trame de consigne est faite à partir des données souscrites sur le topic `/nunchuk/cmd_vel` de type `geometry_msgs.Twist`. Seules les valeurs `linear.x` et `angular.z` de `/nunchuk/cmd_vel` sont utilisées pour piloter le robot. Ces valeurs sont des `float` compris entre -1 à 1. Elles sont ensuite multipliées par 100 et transformées en `int` avant d'être envoyées dans la trame de consigne.

### Paramètres
2 paramètres ROS sont définis pour ce noeud :

- `ZUMO_PORT`, port série sur lequel est connecté le robot, `/dev/ttyACM0` par défaut
- `ZUMO_BAUDRATE`, baudrate de la communication série, 9600 par défaut

Ces 2 paramètres sont définis et modifiables dans le fichier `/config/param.YAML` du package. Le chargement se fait automatiquemeent dans le cas d'un `roslaunch`, et dans le cas d'un `rosrun` du noeud, ils sont initiés dans le script Python sans prendre en compte le fichier de paramètres.

## Nunchuk2cmdvel node 
	rosrun nunchuk2cmdvel nunchuk2cmvel.py
Ce noeud permet de récupérer les états des capteurs du pad Wiimote+Nunchuk, en Bluetooth, et publier un topic `/nunchuk/cmd_vel` compatible avec le zumo.
Pour ma part, la clé Bluetooth doit être réinitialisée à chaque redémarrage PC ou reconnexion physique de celle ci, commande `sudo hciconfig hci0 reset`.
Seul le joystick 2 axes du Nunchuk est utilisé :

![alt text](https://agita/cea-robotics/ros-zumo/raw/master/Galerie/pilotage.png)

## Launch file
`roslaunch zumo zumo.launch` permet :
	
- de créer et initialiser les paramètres,
- lancer le noeud `zumo`
- lancer le noeud `nunchuk2cmdvel`
- lancer Rviz avec la config définie dans `/rviz/zumo.rviz` du package, permettant la visualisation du topic `/zumo/imu`

![alt text](https://agita/cea-robotics/ros-zumo/raw/master/Galerie/rviz.png)






 