#include "project.h"

class blackscholes {
  
  	// access specifier are set public so that the class members are available across the project 
    public:
        // constructor
        blackscholes();
        
        double callprice(double , double, double, double);
        double putprice(double, double, double, double);

        ~blackscholes();
};