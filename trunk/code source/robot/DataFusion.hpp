/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier DataFusion.hpp
 *
 */

#ifndef DEF_DATAFUSION
#define DEF_DATAFUSION

	#include "../Debug.hpp"

	#include "Data.hpp"

	class DataFusion
	{
		public:
			DataFusion(Data &, Data &);
			~DataFusion();

			void run();

		private:
			Data & a_sensorsData;
			Data & a_environmentData;
	};

#endif

