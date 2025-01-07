#pragma once

# include "Request.hpp"

class RessourceHandler
{
        private:

                Location&               __location;
                String&                 __URI;
                String                  __fullPath;

        public:

                String  getPath() const;
                Type    getType() const;

                RessourceHandler(Location& location, String& uri);
		RessourceHandler(const RessourceHandler &copy);
		RessourceHandler &operator=(const RessourceHandler &assign);
		~RessourceHandler();
};
