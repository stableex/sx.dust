#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/singleton.hpp>

#include <optional>

using namespace eosio;
using namespace std;

namespace sx {
class [[eosio::contract("dust.sx")]] dust : public eosio::contract {
public:
    using contract::contract;
    /**
     * ## TABLE `settings`
     *
     * ### params
     *
     * - `{set<permission_level>} accounts` - accounts to dust
     * - `{vector<extended_symbol>} tokens` - token symbols to dust
     * - `{time_point_sec} last_harvest` - last harvest timestamp
     *
     * ### example
     *
     * ```json
     * {
     *     "accounts": [{"actor": "flash.sx", "permission": "dust"}],
     *     "tokens": [
     *         {"symbol": "8,TAG", "contract": "tagtokenmain"}
     *     ],
     *     "last_harvest": "2020-12-14T00:00:00"
     * }
     * ```
     */
    struct [[eosio::table("settings")]] settings_row {
        set<permission_level>       accounts;
        vector<extended_symbol>     tokens;
        time_point_sec              last_harvest;
    };
    typedef eosio::singleton< "settings"_n, settings_row> settings_table;

    /**
     * ## ACTION `harvest`
     *
     * Harvest DFS TAG claim
     *
     * - **authority**: `get_self()` or `account`
     *
     * ### params
     *
     * - `{name} account` - account name
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action dust.sx harvest '[["flash.sx", "dust"]]' -p dust.sx
     * ```
     */
    [[eosio::action]]
    void harvest( const permission_level account );

    /**
     * ## ACTION `sell`
     *
     * Sell single token for EOS
     *
     * - **authority**: `get_self()` or `account`
     *
     * ### params
     *
     * - `{name} account` - account name
     * - `{extended_symbol} token` - token to dust
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action dust.sx sell '[["flash.sx", "dust"], ["8,TAG", "tagtokenmain"]]' -p dust.sx
     * ```
     */
    [[eosio::action]]
    void sell( const permission_level account, const extended_symbol token );

    /**
     * ## ACTION `dustall`
     *
     * Dust all tokens from all accounts
     *
     * - **authority**: `get_self()`
     *
     * ### params
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action dust.sx dustall '[]' -p dust.sx
     * ```
     */
    [[eosio::action]]
    void dustall();

    /**
     * ## ACTION `setsettings`
     *
     * Set settings
     *
     * - **authority**: `get_self()`
     *
     * ### params
     *
     * - `{set<name>} accounts` - accounts to dust
     * - `{vector<extended_symbol>} tokens` - token symbols to dust
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action dust.sx setsettings '[[["flash.sx", "dust"]], [["8,TAG", "tagtokenmain"]]]' -p dust.sx
     * ```
     */
    [[eosio::action]]
    void setsettings( const set<permission_level> accounts, const vector<extended_symbol> tokens );

    // static actions
    using harvest_action = eosio::action_wrapper<"harvest"_n, &sx::dust::harvest>;
    using sell_action = eosio::action_wrapper<"sell"_n, &sx::dust::sell>;
    using dustall_action = eosio::action_wrapper<"dustall"_n, &sx::dust::dustall>;
    using setsettings_action = eosio::action_wrapper<"setsettings"_n, &sx::dust::setsettings>;

private:
    // eosio.token helper
    void transfer( const permission_level from, const name to, const extended_asset quantity, const string memo );
};
}
