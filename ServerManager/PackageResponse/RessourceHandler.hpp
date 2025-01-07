#pragma once

# include "../Request.hpp"

class RessourceHandler
{
        private:

                Location&               __location;
                String&                 __URI;
                String                  __fullPath;
                t_type                  __type;

                void                    loadPathExploring(void);
                void                    loadType(const char* path);


        public:

                String  getPath() const;
                t_type    getType() const;

                RessourceHandler(Location& location, String& uri);
		RessourceHandler(const RessourceHandler &copy) : __location(copy.__location), __URI(copy.__URI)
                {
                        *this = copy;
                }
		RessourceHandler &operator=(const RessourceHandler &assign)
                {
                        if (this != &assign) {
                                __location = assign.__location;
                                __fullPath = assign.__fullPath;
                                __URI = assign.__URI;
                                __type = assign.__type;
                        }
                        return *this;
                }
		~RessourceHandler() {}
};
