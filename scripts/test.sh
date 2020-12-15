#!/bin/bash

# set settings
cleos push action dust.sx setsettings '[[["flash.sx","dust"]], [["8,TAG", "tagtokenmain"]]]' -p dust.sx

# harvest DFS TAG claim
cleos push action dust.sx harvest '["flash.sx"]' -p dust.sx

# dust single token
cleos push action dust.sx dust '["flash.sx", ["8,TAG", "tagtokenmain"]]' -p dust.sx

# dust all tokens from all accounts
cleos push action dust.sx dustall '[]' -p dust.sx