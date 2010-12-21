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
		enum Place
		{
			LAST,
			FIRST
		};
	}

	class Data
	{
		public:
			Data();
			~Data();

			boost::any get(int, DataOption::Place);
			boost::any get(int, int);
			void set(int, boost::any);

		private:
			std::map <int, std::vector<boost::any> > a_data;
	};

#endif

