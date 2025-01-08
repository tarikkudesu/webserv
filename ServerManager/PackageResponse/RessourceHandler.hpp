#pragma once

# include "../Request.hpp"

class RessourceHandler
{
	private:


		void			loadType(const char* path);


	public:
		String& 		__URI;
		String			__fullPath;
		t_type			__type;

		//public temporarely
		Location&		__location; // should stay public

		void			loadPathExploring(void);

		String	getPath() const;
		String	getURI() const;
		t_type	  getType() const;

		RessourceHandler(Location& location, String& uri);
		RessourceHandler(const RessourceHandler &copy);
		RessourceHandler &operator=(const RessourceHandler &assign);
		~RessourceHandler();
};

std::ostream &operator<<(std::ostream &o, RessourceHandler const &r);
