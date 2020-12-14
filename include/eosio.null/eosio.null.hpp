#pragma once

#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract("eosio.null")]] eosio_null : public contract {
public:
    using contract::contract;

    [[eosio::action]]
    void nonce( const uint64_t nonce );

    using nonce_action = eosio::action_wrapper<"nonce"_n, &eosio_null::nonce>;
};