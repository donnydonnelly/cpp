#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "parse.h"
#include "Discount.h"
#include "Forward.h"
#include "Option.h"
#include "black_scholes.h"

template <class Forward>
std::tuple<std::vector<OptionPrice>,
           std::unordered_map<std::string, Discount>,
           std::unordered_map<std::string, Forward>>
parse_data(std::string options_filename,
           std::string discounts_filename,
          std::string forwards_filename)
{
    std::ifstream options_file(options_filename);
    if (!options_file)
    {
        std::ostringstream ss;
        ss << "Could not open option prices file from '" << options_filename;
        throw std::runtime_error(ss.str());
    }
    auto option_prices = parse_options_and_prices(options_file);
    options_file.close();
	
	std::sort(option_prices.begin(), option_prices.end(),
			  [](const OptionPrice & rhs, const OptionPrice & lhs){	  
		if (lhs.option.underlying==rhs.option.underlying) {
			return rhs.option.expiry<lhs.option.expiry;
		}
		return rhs.option.underlying<lhs.option.underlying;
	});
	
    std::ifstream discounts_file(discounts_filename);
    if (!discounts_file)
    {
        std::ostringstream ss;
        ss << "Could not open discounts file from '" << discounts_filename;
        throw std::runtime_error(ss.str());
    }
    auto discounts = parse_discounts(discounts_file);
    discounts_file.close();

    std::ifstream forwards_file(forwards_filename);
    if (!forwards_file)
    {
        std::ostringstream ss;
        ss << "Could not open forwards file from '" << forwards_filename;
        throw std::runtime_error(ss.str());
    }
    std::unordered_map<std::string, Forward> forwards;
    parse_forwards(forwards_file, discounts, forwards);
    forwards_file.close();

    return {option_prices, discounts, forwards};
}


std::vector<std::pair<Date,double>>
boostrap_volatility(std::vector<OptionPrice>::const_iterator begin,
					std::vector<OptionPrice>::const_iterator end,
					const Discount &discount,
					const ForwardYield &forward) {
	
	std::vector<std::pair<Date,double>> vol_curve; 
	while(begin != end){
		bool is_call = (*begin).option.is_call;
		double K = (*begin).option.strike;
		double T = ((*begin).option.expiry - discount.valuation_date())/365.0;
		double F = forward.forward((*begin).option.expiry);
		double fwd_price = (*begin).price/(discount.discount((*begin).option.expiry));
		auto vol = bs_implied_vol(is_call, K, T, F, fwd_price, 1e-10);
		std::pair <Date,double> vol_pair ((*begin).option.expiry,vol);
		vol_curve.emplace(vol_curve.end(), vol_pair);
		
		begin++;
	}

	return vol_curve;
}
using namespace std;
int main(int argc, const char *argv[]) {
    ofstream outfile("volatilities.csv");
	
	if (argc!=4) {
        cerr<<"usage: "<<argv[0]<<" <option prices>  <discounts> <forwards>"<<endl;
        return -1;
    }   
    string options_filename=argv[1];
    string discounts_filename=argv[2];
    string forwards_filename=argv[3];
    
    auto [option_prices,discounts,forwards]=
             parse_data<ForwardYield>(
              options_filename,
              discounts_filename,
              forwards_filename);

	std::vector<OptionPrice>::const_iterator iter = option_prices.begin();
	std::vector<OptionPrice>::const_iterator begin = option_prices.begin();

	while(iter != option_prices.end()){
		if(((iter+1) == option_prices.end())||((*iter).option.underlying != (*(iter+1)).option.underlying)){
			Discount discount = discounts.at((*iter).option.currency);
			ForwardYield forward = forwards.at((*iter).option.underlying);
			vector<pair<Date,double>> vol_curve = boostrap_volatility(begin, iter+1, discount, forward);
			outfile << (*iter).option.underlying<<endl;
			for(pair<Date,double> vols:vol_curve){
				outfile << vols.first <<","<<vols.second<<endl;
			}
			outfile << endl;
			begin = iter+1;	
		}
		iter++;
	}
}