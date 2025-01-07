#pragma once

# include "Request.hpp"
# include "PackageResponse/Response.hpp"

class RessourceHandler
{
        private:

                Location&               __location;
                String&                 __URI;
                String                  __fullPath;
                Type                    __type;

                void                    loadPathExploring(void);
                Type                    loadType(const char* path);


        public:

                String  getPath() const;
                Type    getType() const;

                RessourceHandler(Location& location, String& uri);
		RessourceHandler(const RessourceHandler &copy);
		RessourceHandler &operator=(const RessourceHandler &assign);
		~RessourceHandler();
};
