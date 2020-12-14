#!/bin/bash

# unlock wallet
cleos wallet unlock --password $(cat ~/eosio-wallet/.pass)

# create accounts
cleos create account eosio dust.sx EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio eosio.token EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio flash.sx EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio gateway.sx EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

# deploy
cleos set contract dust.sx . dust.sx.wasm dust.sx.abi
cleos set contract eosio.token include/eosio.token eosio.token.wasm eosio.token.abi

# permission
cleos set account permission dust.sx active --add-code
cleos set account permission flash.sx active dust.sx --add-code

# create EOS token
cleos push action eosio.token create '["eosio", "100000000.0000 EOS"]' -p eosio.token
cleos push action eosio.token issue '["eosio", "100000000.0000 EOS", "init"]' -p eosio
cleos transfer eosio flash.sx "500.0000 EOS" "init"
