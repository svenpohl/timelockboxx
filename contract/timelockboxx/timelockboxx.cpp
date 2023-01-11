/*
timelockboxx.cpp
*/


#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
#include <eosio/asset.hpp>
#include <eosio/transaction.hpp> // tapos...
//#include <eosiolib/crypto.h>        // sha...

#include "timelockboxx.hpp"     

//#include "types.hpp"

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
      const uint32_t     hashwert    = 2334;      
      //const name team_address = name("olgaolgaolga");   
      
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

    
    /*
      // ---
      // struct global
      //
      struct global
      {
      uint64_t id; 
      

      
      auto primary_key() const { return id; }
      EOSLIB_SERIALIZE(global, (id)(round) )
    //  EOSLIB_SERIALIZE(global, (id)(gameid)(amountteam)(minbet)(maxbet) )
      }; // struct global      
      
//      typedef eosio::multi_index<N(global), global> globals;
      typedef eosio::multi_index< name("global"), global> globals;
      */
      
           
      

      /*
      // ---
      // struct gamestruct
      //
      struct gamestruct
      {
      uint64_t      id;
      //account_name player;
      name          player;
      std::string   hash;
      uint64_t      betamount;
      uint32_t      betnumber;
      uint32_t      timestamp;
      uint32_t      frogpos;
      uint32_t      win;
      std::string   symbol;

      auto primary_key() const { return id; }      
      EOSLIB_SERIALIZE(gamestruct, (id)(player)(hash)(betamount)(betnumber)(timestamp)(frogpos)(win)(symbol))

      }; //struct gamestruct
      
      typedef eosio::multi_index<name("gamestruct"), gamestruct> game;
      
*/
    
      
      // ---
      // Status - minimal state
      [[eosio::action]]  
      void status() 
      {
      print(" VERSION: ",version," ");
      
 

      } // status() 
    

 

//---------------------------
    
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
      // cmessage - xxx.
      //
      [[eosio::action]]
      inline void cmessage(name to, std::string memo) 
           {    
           //require_auth(get_self());
           require_recipient(to);
           // require_recipient( get_self() );           
           } // cmessage




      // ---
      // send_token - sending token.
      //  
      inline void send_token(name receiver, uint64_t amount, std::string memo) 
      {          
      //require_auth(get_self());
      //print( " FUNCTION withdraw_team, to:" , name{receiver}  );      
      asset quant = asset(amount, symbol("EOS", 4)  );            
      action(
            permission_level{_self, name("active")},
            name("eosio.token"), name("transfer"),
            std::make_tuple(_self, receiver, quant, memo )
            ).send();
      } // void send_token()


/*
      void dostep()
      {
      print("Do step...");
      
     
      // Globals
      //
      int id = 0;
      globals myglobals(_self, _self.value);
      auto iterator_globals = myglobals.find(id);

      uint64_t round = iterator_globals->round;
      
      print( "ROUND1: ", round," " );
      round = round + 1;

      std::string the_round        = std::to_string( round );
      print( "ROUND: ", the_round, "-", round);
      print( "ROUND2: ", round," " );
      
      
      // ---
      // Gamestruct anlegen
      //
      game mygame(_self, _self.value);
      auto iterator_game = mygame.find(0);
    
      if ( iterator_game != mygame.end() )
         {
         print(" game Existiert schon");
         } else
              {              
              print(" game will be created... ");
              mygame.emplace(_self, [&](auto& gametupel) 
                 {
                 gametupel.id      = round;
                // gametupel.player  = player;               
                           
                 gametupel.hash       = "STEP";  
                          
                 gametupel.timestamp  = now();                                
                        
                 });
              } // else
              
              
      print("Update globals");
      // ---
      // Finaly update globals
      //

      if ( iterator_globals != myglobals.end() )
            {                        
            myglobals.modify(iterator_globals, _self, [&](auto& global) 
                 {                                  
                 global.round            = round;                          
                 });            
            } // if...
                         
                         
          
                        
      } //      void dostep()
      
*/

/*
      // ---
      // send_token - sending token.
      //  
      inline void send_token_frog(name receiver, uint64_t amount, std::string memo) 
      {          
      //require_auth(get_self());
      //print( " FUNCTION withdraw_team, to:" , name{receiver}  );      
      asset quant = asset(amount, symbol("FROG", 4)  );            
      action(
            permission_level{_self, name("active")},
            name("frogfrogcoin"), name("transfer"),
            std::make_tuple(_self, receiver, quant, memo )
            ).send();
      } // void send_token_frog()
*/
      


      // ---
      // handle_transfer - xxx.
      //
      [[eosio::action]]  
      void hdltransfer()      
      {
      
      /*
         auto data = unpack_action_data<transfer_args>();
      print(" hdltoken ");
      
       name code = get_first_receiver();
      print(" code: ",code," ");
      
      
      
      
      
      
      
       if ( data.from == get_self()) 
         {
         print(" DO NOTHING ");
         return;
         }
      


     if(data.to != _self) 
      {
      print(" DO NOTHING_ ");
         return;
      }
      
      
        std::vector<std::string> results;
      splitMemo(results, data.memo);
      
      
      
     //   check( 2 == 1 , " Break! " ); // break


       if (code == name("sovmintofeos") ||
           code == name("eosio.token") 
      */
      
      /*
      auto data = unpack_action_data<transfer_args>();

      print(" from: ", data.from );
      print(" quantity: ", data.quantity.amount );
      print(" symbol: ", data.quantity.symbol ); 
      print(" memo: ", data.memo );
      
      
      std::vector<std::string> results;
      splitMemo(results, data.memo);
       
      

       
      if (results[0] == "catch")
         { 
         print("Symbol: ", data.quantity.symbol );
         
         string thesymbol = "";
         int found_symbol = 0;
         if ( data.quantity.symbol == symbol("EOS" , 4) ) 
            {
            thesymbol = "EOS";
            found_symbol = 1;
            }
         if ( data.quantity.symbol == symbol("FROG", 4) ) 
            {
             thesymbol = "FROG";
            found_symbol = 1;
            }

        
         
         print("Symbol2: ", data.quantity.symbol );         
//         eosio_assert(data.quantity.symbol == symbol("EOS", 4) , "Must pay with EOS token");
         eosio_assert( found_symbol == 1  , "Must pay with EOS token!");
 // FROG...
 
         // --- 
         // get minbet,maxbet  
         globals myglobals( _self , _self.value );         
         auto iterator = myglobals.find(0);

 
    //     int inrange = 0;
  //       if (data.quantity.amount >= minbet && data.quantity.amount <= maxbet) inrange = 1;
      
      //   eosio_assert( inrange==1 , "out of range!");


//         int players_catch = atoi(results[2].c_str());
         
         
    
    

  //  int players_catch = std::stoi( results[2] );
//    print(" ++++ players_catch: ", results[2] , " - ",players_catch );
         
         
//         eosio_assert( (players_catch >= 3 && players_catch <=96) , " Wrong position! ");
 
         
 

         } // catch 
       
       
       */
      } // hdltransfer
    
    
    
    /*
      // ---
      // dogame - xxx
      //
      [[eosio::action]]  
      void dogame()
      {
      print(" DOGAME-1 ");
      require_auth(get_self());
      
   

    
      print(" DOGAME-2 ");

      }
 
*/

      
      // ---
      // admin - superuser functions.
      //
      [[eosio::action]]  
      void admin() 
      {
      /*
      print(" ADMIN_1 ");     
      require_auth(get_self());
         
      print(" weiter1 ");     
      auto data = unpack_action_data<goodthing::param_struct>();
      print(" weiter2 "); 
      print(" ADMIN...[",data.param,"] ");
      
      std::vector<std::string> results;
      splitMemo(results, data.param);
      
      print(" first:...[", results[0] ,"] ");
      
      if (results[0] == "test")
         {
         print(" test... ");
         int64_t value = random();
         print("rValue: ", value);
         }
         
      if (data.param == "reset")
         {
         print("Reset ");
         
//         globals myglobals(_self, _self);
         globals myglobals(_self, _self.value);
         
         for (auto itr = myglobals.begin(); itr != myglobals.end();) 
             {
             itr = myglobals.erase(itr);
             }        
         
         print(" FIN-Reset "); 
         } // if (data.param == "reset")

             
      if (results[0] == "init")
         {
         print(" Init... ");
         
         
         
        
         // ---
         // Globals
         //
         int id = 0;
         globals myglobals( _self , _self.value );         
         auto iterator = myglobals.find(id);
    
         if ( iterator != myglobals.end() )
            {
            print(" globals Existiert schon");
            } else
              {
              print(" globals wird angelegt2b... ");
              myglobals.emplace(_self, [&](auto& global) 
                 {
                 global.id = id;
                 global.round        = 3;
                 });
             } // else
      
      
         } // if (data.param == "init")
         
         
         if (data.param == "resetgames")
         {
         print(" Reset ALL Game ");    
         
         
         game mygame(_self, _self.value);
      
         //uint64_t greatest = 0;      
         for (auto itr = mygame.begin(); itr != mygame.end();) 
             {
             itr = mygame.erase(itr);
             }
         
         print(" FIN-Reset Games "); 
         } // if (data.param == "resetgames")


         if (data.param == "step")
         {
         dostep();
                  
         } // if (data.param == "step")

         
         
       
     
         */
      } // admin
      
      
      
      
      
      
  
      
    
 
       
      
      
 
      
      
      
      
      
      
}; // CONTRACT timelockboxx      




extern "C"
{

void apply(uint64_t receiver, uint64_t code, uint64_t action) 
{
  //  print("Apply2..." ,  name{action} );

    if (action == name("transfer").value)
       {       
       execute_action(name(receiver), name(code), &timelockboxx::hdltransfer);  
       }

    if (action == name("status").value)
       {
       execute_action(name(receiver), name(code), &timelockboxx::status);  
       }

/*
    if (action == name("admin").value)
       {
       execute_action(name(receiver), name(code), &goodthing::admin);  
       }
*/
    


   

} // apply
    
    
    
       
} // extern "C"      

