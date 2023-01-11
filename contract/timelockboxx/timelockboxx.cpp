/*
timelockboxx.cpp
*/

#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
#include <eosio/asset.hpp>
#include <eosio/transaction.hpp> 

#include "timelockboxx.hpp"     


/*
timelockboxx. (17.dez.2022 - Beginn)

17.dez.2022 - Beginn

todo:

*/

using namespace eosio;
using namespace std;

 
    


const std::string   version   = "V1.0";
 

CONTRACT timelockboxx : public eosio::contract {

  public:
      using contract::contract;
      
      // ---
      // Constants
      //
      const uint32_t     hashwert    = 12345;      
 
      
      struct param_struct
      {
      std::string param;    
      };



      struct transfer_args
      {
      name 			from;
      name 			to;
      asset 		quantity;
      std::string 	memo;
      };    

    
    
      // ---
      // struct global
      //
      TABLE global
      {
      uint64_t id; 
      uint64_t deposits; 
      
      auto primary_key() const { return id; }
      EOSLIB_SERIALIZE(global, (id)(deposits) )
      }; // struct global      
      
      typedef eosio::multi_index< name("global"), global> globals;
  
      
           
      
  
      //
      // Table nodes
      // 
      TABLE deposits
      {
      uint64_t  id; 
      name      beneficiary;
      asset     payout;
      name 	    contract;
      uint32_t  withdrawtime;
      name      from;
      string    description;
      
                
      uint64_t primary_key() const { return id; } 
      uint64_t by_owner() const { return beneficiary.value; } 
      
      EOSLIB_SERIALIZE(deposits, (id)(beneficiary)(payout)(contract)(withdrawtime)(from)(description) )  
      }; // TABLE deposits 
      

      typedef eosio::multi_index <name("deposits"), deposits ,    
              indexed_by <name("byowner"), const_mem_fun <deposits, uint64_t, &deposits::by_owner>>   
             > deposits_s;

  

  
      inline void splitMemo(std::vector<std::string> &results, std::string memo)
            {
            auto end = memo.cend();
            auto start = memo.cbegin();

            for (auto it = memo.cbegin(); it != end; ++it)
                {
                if (*it == ';')
                   {
                   results.emplace_back(start, it);
                   start = it + 1;
                   }
                }
            if (start != end)
            results.emplace_back(start, end);
            } // splitMemo



 

 
 
     
   
      
   
      // ---
      // handle_transfer token - xxx.
      //
      [[eosio::action]]  
      void hdltoken()
      {
       
      auto data = unpack_action_data<transfer_args>();
      print(" hdltoken ");
      
      name code = get_first_receiver();
      print(" code: ",code," ");
      
      
 
      
      
      if ( data.from == get_self()) 
         {
         print(" DO NOTHING ");
         return;
         }
      


      if ( data.to != _self ) 
         {
         print(" DO NOTHING_ ");
         return;
         }
      
      
      std::vector<std::string> results;
      splitMemo(results, data.memo);
      
      
      
    
 

       //
       // DEPOSIT
       //   
       if ( results[0] == "deposit"  )
          {
          print(" DEPOSIT...");
          
          uint64_t now = current_time_point().sec_since_epoch();             
          print(" ",now," ");
          
          
          
          int id = 0;
          globals myglobals( _self , _self.value );           
          auto globaliterator = myglobals.find(id);
          uint64_t deposits = globaliterator->deposits;
          deposits++;

             


                     
                     
          
          // Check beneficiary account  
          name beneficiary = name( results[1] );
          print(" B:",beneficiary," " );
          
          // Check if account does exist
          check( is_account( beneficiary ), "to account does not exist");


          // Check Timestamp
          uint32_t withdrawtime    = atoll(results[2].c_str());                      
          print("[",results[2],"] withdrawtime: ",withdrawtime," " );   
          

          check( withdrawtime > now , "withdrawtime must be in the future");
  
          
          
          // Create table entry
          deposits_s mydeposits( _self , _self.value );    
          
          mydeposits.emplace(_self, [&](auto&  tupel) 
                {          
                tupel.id                     = deposits;
                tupel.beneficiary            = beneficiary;                
                tupel.payout                 = data.quantity;                
                tupel.contract               = code;                
                tupel.withdrawtime           = withdrawtime;        
                tupel.withdrawtime           = withdrawtime;        
                tupel.from                   = data.from;        
                tupel.description            = results[3];        
                
                      
                });            
                
          myglobals.modify( globaliterator, _self, [&](auto& tupel) 
                 {                  
                 tupel.deposits            = deposits;                                                
                 });                                
          
          } // deposit
          
          
          
        
 
      print(" FIN ");
      } // hdltoken
 
  


      
    
    
    
      // ---
      // withdraw
      //
      [[eosio::action]]  
      void withdraw( name beneficiary, uint64_t id )
      {      
      print(" withdraw ");
      
      require_auth( beneficiary );
                            
      uint64_t now = current_time_point().sec_since_epoch();             
      print(" ",now," ");
      
      // Create table entry
      deposits_s mydeposits( _self , _self.value );    
      
      auto itr = mydeposits.find(id);
      
      if ( itr != mydeposits.end() )
         {
         print(" FOUND ");
         
         check ( beneficiary == itr->beneficiary, " No permission ");

         check ( now >= itr->withdrawtime, " The time is not up yet ");
         
                  
       
         //
         // payout token
         //  
         string memo = "Timelock withdraw";
         
         if ( itr->contract == name("sovmintofeos") ) 
            { 
            
            action(
               permission_level{_self, name("active")},
               itr->contract, name("xtransfer"),
               std::make_tuple(_self, itr->beneficiary, itr->payout, memo )
               ).send();

            } else
            	{ 
            
           		action(
               		permission_level{_self, name("active")},
               		itr->contract, name("transfer"),
               		std::make_tuple(_self, itr->beneficiary, itr->payout, memo )
               		).send();

                } 
                   
         //
         // remove entry        
         //
         mydeposits.erase( itr );
         
         
      
         
         } else
           {
           print(" NOT FOUND ");
           }

              
 

      
      print(" FIN ");             
      } // withdraw
      
     

      /*
      // ---
      // admin - superuser functions.
      //
      [[eosio::action]]  
      void admin() 
      {
      require_auth(get_self());
      
      return;
     
      } // admin
      */
      
      
                     
      
      
}; // CONTRACT timelockboxx      




extern "C"
{

void apply(uint64_t receiver, uint64_t code, uint64_t action) 
{

    if ( action == name("transfer").value )
       {       
       execute_action(name(receiver), name(code), &timelockboxx::hdltoken);  
       }  

    if (action == name("withdraw").value)
       {
       execute_action(name(receiver), name(code), &timelockboxx::withdraw);  
       }
       
} // apply
    
    
    
       
} // extern "C"      

// EOF