# XRPL Networking
## Requirements
- server mush have unique identities
- server must establish a secure connection between each other
- servier must connect to the network
## Server identity
- Facilitate secure communication, each server
    - Generate a public/private key (secp256k1) pair
    - tokens.cpp
## Two key system
- Each node has a unique node ID
- Validators have an additional validator ID
- No cryptographic link btw two
## Connection between servers
- connectAttemp.cpp, handshake.cpp
- Peering protocol
    - Establish a tcp connection
    - Upgrade to SSL/TLS
    - Handshake
## Connect to the network
- Peering discovery
    - Bootstrapping
        - Hardcoded notes: 4 nodes
        - Config files: ips or ips_fixed / private
    - Additional peers
        - At least 15 peers by default
    - Peer discovery
        - Advertisement
        - Propagation
        - Peer lists
    - Connection strategy
        - Stage 1: Fixed slots
        - Stage 2: Live cache
        - Stage 3: Boot cache
## P2P communication
- Protocol messages
    - Total 20, but 5 primary messages
    - TMGetLedger / TMLedgerData
    - TMTransaction
    - TMProposeSet
    - TMValidation
- Message relaying
    - TMGetLedger / TMLedgerData aren't sent beyond one hop
    - TMTransaction, TMProposeSet, TMValidation must reach majority of servers in the network using flooding or broadcasting
        - Flooding
            - If a msg is new: cache it, process, fwd to peers
            - if the msg is a duplicate: drop it
## XRPL network topology
- Network robustness
- Msg propagation delay
- Redundant msg

