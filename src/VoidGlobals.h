//
//  VoidGlobals.h
//  VoidNoise
//
//  Created by Peter Holzkorn on 03/08/2014.
//
//

#ifndef VoidNoise_VoidGlobals_h
#define VoidNoise_VoidGlobals_h

#define REGIONSIZE 600.0

namespace voidnoise {
	
	class Settings {
		
	public:
		static Settings &get()
		{
			static Settings instance;
			return instance;
		}
		
		
		float attractStrength;
		float repelStrength;
		float orientStrength;
		float maxSpeed;
		float minSpeed;
		float gravity;
		float gravityDistance;
		float decay;
		float repulsionDist;
		float repulsionStrength;
		
		float stemThickness;
		
	private:
		Settings() {}
		Settings(const Settings&) {}
		void operator=(const Settings&) {}
	};
	
	
}

#endif
