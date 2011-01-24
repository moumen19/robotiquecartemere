/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Data.hpp
 *
 */

#ifndef DEF_DATA
#define DEF_DATA

	#include <vector>
	#include <map>
	#include <boost/any.hpp>

	#include "../Debug.hpp"

	namespace DataOption
	{
		/**
		 * Enumeration permettant de definir la place de l'information a obtenir
		 */
		enum Place
		{
			LAST,
			FIRST
		};
	}
	
	/**
 	 * Classe de stockage de donnees
	 */
	class Data
	{
		public:
			Data();				// Constructeur
			~Data();			// Destructeur

			boost::any get(int, DataOption::Place, int in = 0);	// Methode retournant la donnee
			boost::any get(int, int);	// Methode retournant la donnee
			void set(int, boost::any);	// Methode ajoutant une donnee

		private:
			std::map <int, std::vector<boost::any> > a_data;	// Les donnees sous forme de liste mappe
	};

#endif

