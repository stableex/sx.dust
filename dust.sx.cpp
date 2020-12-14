#include <eosio.token/eosio.token.hpp>
#include <tagtokenfarm/tagtokenfarm.hpp>

#include "dust.sx.hpp"

[[eosio::action]]
void sx::dust::sell( const name account, const extended_symbol token )
{
    if ( !has_auth( get_self() )) require_auth( account );

    // tables
    sx::dust::settings_table _settings( get_self(), get_self().value );
    auto settings = _settings.get();

    const asset balance = eosio::token::get_balance( token.get_contract(), account, token.get_symbol().code() );
    transfer( account, "gateway.sx"_n, extended_asset{ balance.amount, token }, "EOS");
}

[[eosio::action]]
void sx::dust::harvest( const name account )
{
    if ( !has_auth( get_self() )) require_auth( account );
    tagtokenfarm::harvest_action harvest( "tagtokenfarm"_n, { account, "active"_n });
    harvest.send( account );
}

[[eosio::action]]
void sx::dust::dustall()
{
    require_auth( get_self() );

    // tables
    sx::dust::settings_table _settings( get_self(), get_self().value );
    auto settings = _settings.get();
}

[[eosio::action]]
void sx::dust::setsettings( const set<name> accounts, const vector<extended_symbol> tokens )
{
    require_auth( get_self() );

    sx::dust::settings_table _settings( get_self(), get_self().value );
    auto settings = _settings.get_or_default();
    settings.accounts = accounts;
    settings.tokens = tokens;
    _settings.set( settings, get_self() );
}

void sx::dust::transfer( const name from, const name to, const extended_asset value, const string memo )
{
    eosio::token::transfer_action transfer( value.contract, { from, "active"_n });
    transfer.send( from, to, value.quantity, memo );
}
