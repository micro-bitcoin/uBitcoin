#ifndef __UBTC_NETWORKS_H__
#define __UBTC_NETWORKS_H__

#include <stdint.h>

/** \brief Prefixes for particular network (Mainnet / Testnet ).<br>
 *  HD key prefixes are described here:<br>
 *  https://github.com/satoshilabs/slips/blob/master/slip-0132.md<br>
 *  useful tool: in https://iancoleman.io/bip39/
 */
typedef struct {
    /** \brief Pay-To-Pubkey-Hash addresses */
    uint8_t p2pkh;   
    /** \brief Pay-To-Script-Hash addresses */
    uint8_t p2sh;    
    /** \brief Prefix for segwit addreses ...for regtest it is larger */
    char bech32[5];  
    /** \brief Wallet Import Format, used in PrivateKey */
    uint8_t wif;     
    /** \brief HD private key for legacy addresses (P2PKH) */
    uint8_t xprv[4]; 
    /** \brief HD private key for nested Segwit (P2SH-P2WPKH) */
    uint8_t yprv[4]; 
    /** \brief HD private key for native Segwit (P2WPKH) */
    uint8_t zprv[4]; 
    /** \brief HD private key for nested Segwit Multisig (P2SH-P2WSH) */
    uint8_t Yprv[4]; 
    /** \brief HD private key for native Segwit Multisig (P2WSH) */
    uint8_t Zprv[4]; 
    /** \brief HD public key for legacy addresses (P2PKH) */
    uint8_t xpub[4]; 
    /** \brief HD public key for nested Segwit (P2SH-P2WPKH) */
    uint8_t ypub[4]; 
    /** \brief HD public key for native Segwit (P2WPKH) */
    uint8_t zpub[4]; 
    /** \brief HD public key for nested Segwit Multisig (P2SH-P2WSH) */
    uint8_t Ypub[4]; 
    /** \brief HD public key for native Segwit Multisig (P2WSH) */
    uint8_t Zpub[4]; 
    /** \brief bip32 coin index */
    uint32_t bip32;
} Network;

extern const Network Mainnet;
extern const Network Testnet;
extern const Network Regtest;
extern const Network Signet;

extern const Network * networks[];
extern const uint8_t networks_len;

#endif // __UBTC_NETWORKS_H__