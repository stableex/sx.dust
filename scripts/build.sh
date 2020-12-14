#!/bin/bash

eosio-cpp dust.sx.cpp -I include
cleos set contract dust.sx . dust.sx.wasm dust.sx.abi
