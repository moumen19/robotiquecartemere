
# definition du corps du robot

define troll position
(

	size [0.705 0.5]		# taile du robot
	origin [0 0 0]			# centre de rotation/ origine des capteurs
	
	polygons 1
	polygon[0].points 8
	polygon[0].point[0] [0.3525 0.15]	# position des points [x y]
	polygon[0].point[1] [0.2525 0.25]
	polygon[0].point[2] [-0.2525 0.25]
	polygon[0].point[3] [-0.3525 0.15]
	polygon[0].point[4] [-0.3525 -0.15]
	polygon[0].point[5] [-0.2525 -0.25]
	polygon[0].point[6] [0.2525 -0.25]
	polygon[0].point[7] [0.3525 -0.15]

	mass 20				# masse éstimée du robot
	drive "diff"			# modèle de direction différentielle

	troll_ultrason()
	troll_infrarouge()
	troll_ultrason_arriere()

)

# defintion de la position des capteurs

define troll_ultrason ranger
(

  scount 7				# nombre de capteur

  spose[0] [ 0.35 0 0 ]			# avant
  spose[1] [ 0.35 0.15 30 ]		# avant gauche
  spose[2] [ 0.25 0.24 60 ]		# avant gauche gauche
  spose[3] [ 0.35 -0.15 -30 ]		# avant droite
  spose[4] [ 0.25 -0.24 -60 ]		# avant droite droite
  spose[5] [ 0 0.245 90 ]		# gauche
  spose[6] [ 0 -0.245 -90 ]		# droite

#  spose[6] [ -0.35 0.15 150 ]		# arrière gauche
#  spose[4] [ -0.35 -0.15 210 ]		# arrière doitre

  ssize [0.01 0.05]			# taille du capteur [x y]

  sview [0.14 4.5 30]			# distance et angle de detection [min max view_angle]

)

define troll_infrarouge ranger
(

  scount 2

  spose[0] [ 0.35 0.075 0 ]		# devant
  spose[1] [ 0.35 -0.075 0 ]

  #spose[2] [ 0.17 0.245 90 ]		# gauche
  #spose[3] [ -0.17 0.245 90 ]
  #spose[4] [ 0.17 -0.245 270 ]		# droite
  #spose[5] [ -0.17 -0.245 270 ]

  ssize [0.01 0.05]

  sview [0.01 1.5 5]
)

define troll_ultrason_arriere ranger
(

  scount 1				# nombre de capteur

  spose[0] [ -0.35 0 180 ]		# arrière

  ssize [0.01 0.05]			# taille du capteur [x y]

  sview [0.14 4.5 60]			# distance et angle de detection [min max view_angle]

)



