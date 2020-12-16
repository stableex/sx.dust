#include <eosio.token/eosio.token.hpp>
#include <tagtokenfarm/tagtokenfarm.hpp>

#include "dust.sx.hpp"

[[eosio::action]]
void sx::dust::sell( const permission_level account, const extended_symbol token )
{
    if ( !has_auth( get_self() )) require_auth( account.actor );

    const asset balance = get_balance( account.actor, token );
    transfer( account, "gateway.sx"_n, { balance.amount, token }, "EOS");
}

[[eosio::action]]
void sx::dust::harvest( const permission_level account )
{
    if ( !has_auth( get_self() )) require_auth( account.actor );

    tagtokenfarm::harvest_action harvest( "tagtokenfarm"_n, account );
    harvest.send( account.actor );
}

[[eosio::action]]
void sx::dust::dustall( )
{
    require_auth( get_self() );

    // tables
    sx::dust::settings_table _settings( get_self(), get_self().value );
    auto settings = _settings.get();
    bool is_dust = false;

    for ( permission_level account : settings.accounts ) {
        for ( extended_symbol token : settings.tokens ) {
            const asset balance = get_balance(account.actor, token );
            if ( balance.amount > 0 ) {
                transfer( account, "gateway.sx"_n, { balance.amount, token }, "EOS");
                is_dust = true;
            }
        }
    }
    check( is_dust, "nothing to dust");
}

[[eosio::action]]
void sx::dust::claim()
{
    require_auth( get_self() );

    sx::dust::settings_table _settings( get_self(), get_self().value );
    auto settings = _settings.get();

    //check last harvest date
    auto now = eosio::current_time_point().sec_since_epoch();
    auto last_claim = settings.last_harvest.sec_since_epoch();

    check(now - last_claim > 1 * 60 * 60, "Dust.sx: can harvest once an hour");

    settings.last_harvest = eosio::time_point_sec( eosio::current_time_point() );
    _settings.set(settings, get_self());

    //harvest all
    for ( permission_level account : settings.accounts ) {
        harvest( account );
    }

    //sell all dust
    dustall_action _dustall( get_self(), { get_self(), "active"_n } );
    _dustall.send();

}

[[eosio::action]]
void sx::dust::setsettings( const set<permission_level> accounts, const vector<extended_symbol> tokens )
{
    require_auth( get_self() );

    sx::dust::settings_table _settings( get_self(), get_self().value );
    auto settings = _settings.get_or_default();
    settings.accounts = accounts;
    settings.tokens = tokens;
    _settings.set( settings, get_self() );
}

void sx::dust::transfer( const permission_level from, const name to, const extended_asset value, const string memo )
{
    eosio::token::transfer_action transfer( value.contract, from );
    transfer.send( from.actor, to, value.quantity, memo );
}

asset sx::dust::get_balance(name owner, extended_symbol ext_sym) {

    eosio::token::accounts accountstable( ext_sym.get_contract(), owner.value );

    auto it = accountstable.find( ext_sym.get_symbol().code().raw() );
    if(it == accountstable.end()) return { 0, ext_sym.get_symbol() };

    return it->balance;
}