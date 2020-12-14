#pragma once

#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract("tagtokenfarm")]] tagtokenfarm : public contract {
public:
    using contract::contract;

    [[eosio::action]]
    void harvest( const name farmer );

    using harvest_action = eosio::action_wrapper<"harvest"_n, &tagtokenfarm::harvest>;
};
