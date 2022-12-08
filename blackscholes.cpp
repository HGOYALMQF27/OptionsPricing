#include "project.h"
#include "blackscholes.h"

blackscholes::blackscholes() {
}


// black scholes call option value calculation
double blackscholes::callprice(double strike, double index, double volatility, double timetomaturity)
{
    
    timetomaturity=(30-timetomaturity)/365;
    // cout<<index<<endl<<strike<<endl<<timetomaturity;
    double d1 = ((log(index / strike) + (0.06 + 0.5 * pow(volatility, 2)) * timetomaturity)) / (volatility * sqrt(timetomaturity));
    double d2 = d1 - volatility * sqrt(timetomaturity);
    // cout<<d1<<endl<<d2<<endl;
    
    double ndf1=0.5 * erfc(-d1 * M_SQRT1_2);
    double ndf2=0.5 * erfc(-d2 * M_SQRT1_2);
    double call = (ndf1*index) - (strike * exp(-0.06 * timetomaturity) * ndf2);
    
    return call;
}

// black scholes call option value calculation
double blackscholes::putprice(double strike, double index, double volatility, double time) 
{
    time=(30-time)/365;
    double put = strike * exp(-0.06 * time) - index + callprice(strike, index, volatility, time);
    return put;
}

// black scholes destructor
blackscholes :: ~blackscholes() {

}