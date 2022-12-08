#include "project.h"
#include "blackscholes.cpp"

// niftydata structure identifier with members price and volatility (vol)
struct niftydata{
  double price;
  double vol;
};


/* filehandling snippet fetching price and volatility from the csv or txt (comma separated files) */
niftydata indexprice(string file_name, string &search_term1, string &search_term2, string &search_term3) 
{
  // record is a string vector to store the fetched values
  vector<string> record;
  ifstream file;
  file.open(file_name);

  // boolean variable found_record is set as a toggle
  bool found_record = false;
 
  string field_one, field_two, field_three, price,vol;

  /* getline() allows accepting and reading single or multiple line strings from the input stream 
     Different from cin as cin just allows input from the user which is not multiline or multiword */
     
  while(getline(file, field_one,',') && !found_record)
  {
    getline(file,field_two,',');
    getline(file,field_three,',');
    getline(file,price, ',');
    getline(file,vol, '\n');
    
    /* Comparison of search term with those that of data in the file 
       Returns 1 in case of a match else 0 
       found_record flag is set to true and the values are stores in string vector record */
       
    if(field_one == search_term1 && field_two == search_term2 && field_three == search_term3)
    {
      found_record = true;
      record.push_back(field_one);
      record.push_back(field_two);
      record.push_back(field_three);
      record.push_back(price);
      record.push_back(vol);
    }
  }

  // cout << record[0] << " " << record[1] << " " << record[2] << " " << record[3] <<"  "<<record[4]<< endl;
  niftydata x;
  
  /* record[3] stores price
     record[4] stores volatility 
     stod is an inbuilt function converting string type to double for further mathematical usage */
     
  x.price=stod(record[3]);
  x.vol=stod(record[4]);
  
  return x;
}

/*class blackscholes {
  
  	// access specifier are set public so that the class members are available across the project 
    public:
        // constructor
        blackscholes();
        
        double callprice(double , double, double, double);
        double putprice(double, double, double, double);

        ~blackscholes();
};*/

//non parameterized black scholes constructor
/*blackscholes::blackscholes() {
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

}*/

int main() 
{
    string yr,mon,e1,e2;
    niftydata ent,exit;
    
    // creating blackscholes class object "o"
    blackscholes o;
    
    /* Here our limitation is that the entry and exit position is within the month.
       We are not rolling over the position to a further expiry by providing extra premium. 
    */
    
    /* Provide a valid enter and exit date in yyyy/mm/dd format.
       We are working with last 10 years data. 
    */
    
    cout<<"enter Year of the trade from last 10 years in yyyy format: ";
    cin>>yr;
    cout<<"Enter month of the trade in mm format: ";
    cin>>mon;
    cout<<"enter entry date of the trade: ";
    cin>>e1;
    cout<<"enter exit date of the trade: ";
    cin>>e2;
    
    // prints entry price and volatility in the console considering filename of the dataset, year, month and entry date.
    ent=indexprice("niftydata.csv",yr,mon,e1);
    cout<<ent.price<<"  "<<ent.vol<<endl;
    
    // prints exit price and volatility in the console considering filename of the dataset, year, month and exit date.
    exit=indexprice("niftydata.csv",yr,mon,e2);
    cout<<exit.price<<"  "<<exit.vol<<endl;
    
    // stod is an inbuilt function converting string type to double for further mathematical usage.
    double entry=stod(e1);
    double exitdt=stod(e2);
    
    // Number of legs of the strategy i.e how many long or short positions the user wants to take (restricted to 1 to 4 for simplification).
    int n;
    cout<<"enter number of legs of the strategy from 1 to 4: ";
    cin>>n;
    
    // opt is a 2D array of type double.
    double opt[n][3];
    
    for(int x=0;x<n;x++)
  	{
      int strk=0; // strike price of the option
      string pos,type; 
      
      cout<<"enter details of "<<x+1<<" Leg"<<endl;
      cout<<"enter a valid Strike price: ";
      cin>>strk;
      
      cout<<"enter put/call: "; // type of option (Call/Put)
      cin>>type;
      
      cout<<"enter long/short: "; // Position in an option (Long/Short)
      cin>>pos; 
      
      cout<<"enter number of lots: ";
      cin>>opt[x][0];
      
      // Call position computation.
      if(type=="call")
      {
          if(pos=="long")
            {
            	opt[x][1]=o.callprice(strk,ent.price,ent.vol,entry); // computes the call price of long call at entry.
            	opt[x][2]=o.callprice(strk,exit.price,exit.vol,exitdt); // computes the call price of long call at exit.
            }
            
          else if(pos=="short")
            {
            	opt[x][1]=-1* o.callprice(strk,ent.price,ent.vol,entry); // computes the call price of short call at entry.
            	opt[x][2]=-1* o.callprice(strk,exit.price,exit.vol,exitdt); // computes the call price of short call at exit.
            }
            
          else
            {
            	cout<<"incorrect entry for long/short";
            	break;
            }
      }
      
      else if(type=="put")
      {
          if(pos=="long")
            {
            	opt[x][1]= o.putprice(strk,ent.price,ent.vol,entry); // computes the put price of long put at entry.
            	opt[x][2]= o.putprice(strk,exit.price,exit.vol,exitdt); // computes the put price of long put at exit.
            }
            
          else if(pos=="short")
            {
            	opt[x][1]=-1* o.putprice(strk,ent.price,ent.vol,entry); // computes the put price of short put at entry.
            	opt[x][2]=-1* o.putprice(strk,exit.price,exit.vol,exitdt); // computes the put price of short put at exit.
            }
            
          else
           {
           		cout<<"incorrect entry for long/short";
          	    break;
            }
      }
      
      else
      {
            cout<<"incorrect entry for call/put";
            break;
            
       }
      cout<<"Entry and Exit prices of the options are: "<<opt[x][1]<<" and "<<opt[x][2]<<endl;
     
  }
  
  // Calculating the PnL of the net position based on fetched and input data provided. 
  int pnl=0;
  for(int x=0;x<n;x++)
  {
    pnl=pnl+opt[x][0]*(opt[x][2]-opt[x][1]);
  }
  cout<<"Net profit/loss for the trade is: "<<pnl;
  return 0; 
}