//
//  ChainParams.h
//
//  Created by Aaron Voisine on 1/10/18.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#ifndef ChainParams_h
#define ChainParams_h

#include "BRMerkleBlock.h"
#include "BRSet.h"
#include <assert.h>

static const int64_t COIN = 100000000;

// Burn Amounts
static const uint64_t IssueAssetBurnAmount = 500 * COIN;
static const uint64_t ReissueAssetBurnAmount = 100 * COIN;
static const uint64_t IssueSubAssetBurnAmount = 100 * COIN;
static const uint64_t IssueUniqueAssetBurnAmount = 5 * COIN;


typedef struct {
    uint32_t height;
    UInt256 hash;
    uint32_t timestamp;
    uint32_t target;
} CheckPoint;

typedef struct {
    const char *const *dnsSeeds; // NULL terminated array of dns seeds
    uint16_t standardPort;
    uint32_t magicNumber;
    uint64_t services;

    int (*verifyDifficulty)(const BRMerkleBlock *block, const BRSet *blockSet); // blockSet must have last 2016 blocks or 180 for DGW
    const CheckPoint *checkpoints;
    size_t checkpointsCount;
} ChainParams;

static const char *MainNetDNSSeeds[] = {
        "australiacash.org.", "seed1.genix.cx.", "seed2.genix.cx.",
        "seed3.genix.cx.", NULL
};

static const char *TestNetDNSSeeds[] = {
        "tn.seed1.genix.cx.", "tn.seed2.genix.cx.", "tn.seed3.genix.cx.",
        "australiacash.org.", NULL
};

static const char *RegTestDNSSeeds[] = {
        "127.0.0.1", NULL
};

// blockchain checkpoints - these are also used as starting points for partial chain downloads, so they must be at
// difficulty transition boundaries in order to verify the block difficulty at the immediately following transition
static const CheckPoint MainNetCheckpoints[] = {
//        {      0, "0000038977617c01646209e33e354174ef916df8284346b29aecfbc98fa43dd0", 1551279600, 0x1e0ffff0 },  //Genix Main Genesis
//        { 13205, "00000000017e55ee79052eee6704097d7743fd4756122abc08ede04059fe56f8", 1556021284, 0x1c01bbc2 },
//        { 26410, "00000000009fad15414efa4a1be2c067a14dd3a120159f34ba3b07b4682422d9", 1557686721, 0x1c018f37 },
        { 58382, "000000000029aec185b7462e1661e726bef920b41f1b728a1879c5d7527aae6d", 1561717114, 0x1c065b71 }
};

static const CheckPoint TestNetCheckpoints[] = {
        {       0, "000006874678aa53f78b7676ced0f443cd22ae8917199b5ec14d0b7b7df7b93d", 1549043100, 0x1e0ffff0 }  //Genix Testnet
};

static const CheckPoint RegTestCheckpoints[] = {
        {} // todo: retrieve using RPC call on local wallet!!
};

static int MainNetVerifyDifficulty(const BRMerkleBlock *block, const BRSet *blockSet) {
    const BRMerkleBlock *previous, *b = NULL;
    uint32_t i;

    assert(block != NULL);
    assert(blockSet != NULL);

    // check if we hit a difficulty transition, and find previous transition block
    if ((block->height % BLOCK_DIFFICULTY_INTERVAL) == 0) {
        for (i = 0, b = block; b && i < BLOCK_DIFFICULTY_INTERVAL; i++) {
            b = BRSetGet(blockSet, &b->prevBlock);
        }
    }

    previous = BRSetGet(blockSet, &block->prevBlock);
    return BRMerkleBlockVerifyDifficulty(block, previous, (b) ? b->timestamp : 0);
}

static int TestNetVerifyDifficulty(const BRMerkleBlock *block, const BRSet *blockSet) {
    return 1; // XXX skip testnet difficulty check for now
}

static int RegTestVerifyDifficulty(const BRMerkleBlock *block, const BRSet *blockSet) {
    return 1; // regtest diff check
}

// MainNet Burn Addresses
static const char strIssueAssetBurnAddressMainNet[] = "RXissueAssetXXXXXXXXXXXXXXXXXhhZGt";
static const char strReissueAssetBurnAddressMainNet[] = "RXReissueAssetXXXXXXXXXXXXXXVEFAWu";
static const char strIssueSubAssetBurnAddressMainNet[] = "RXissueSubAssetXXXXXXXXXXXXXWcwhwL";
static const char strIssueUniqueAssetBurnAddressMainNet[] = "RXissueUniqueAssetXXXXXXXXXXWEAe58";
static const char strGlobalBurnAddressMainNet[] = "RXBurnXXXXXXXXXXXXXXXXXXXXXXWUo9FV"; // Global Burn Address

// TestNet Burn Addresses
static const char strIssueAssetBurnAddressTestNet[] = "n1issueAssetXXXXXXXXXXXXXXXXWdnemQ";
static const char strReissueAssetBurnAddressTestNet[] = "n1ReissueAssetXXXXXXXXXXXXXXWG9NLd";
static const char strIssueSubAssetBurnAddressTestNet[] = "n1issueSubAssetXXXXXXXXXXXXXbNiH6v";
static const char strIssueUniqueAssetBurnAddressTestNet[] = "n1issueUniqueAssetXXXXXXXXXXS4695i";
static const char strGlobalBurnAddressTestNet[] = "n1BurnXXXXXXXXXXXXXXXXXXXXXXU1qejP"; // Global Burn Address

// RegTest Burn Addresses
static const char strIssueAssetBurnAddressRegTest[] = "n1issueAssetXXXXXXXXXXXXXXXXWdnemQ";
static const char strReissueAssetBurnAddressRegTest[] = "n1ReissueAssetXXXXXXXXXXXXXXWG9NLd";
static const char strIssueSubAssetBurnAddressRegTest[] = "n1issueSubAssetXXXXXXXXXXXXXbNiH6v";
static const char strIssueUniqueAssetBurnAddressRegTest[] = "n1issueUniqueAssetXXXXXXXXXXS4695i";
static const char strGlobalBurnAddressRegTest[] = "n1BurnXXXXXXXXXXXXXXXXXXXXXXU1qejP"; // Global Burn Address

static const ChainParams MainNetParams = {
        MainNetDNSSeeds,
        43649,       // standardPort
        0x493b4ecf, // magicNumber
        0,          // services
        MainNetVerifyDifficulty,
        MainNetCheckpoints,
        sizeof(MainNetCheckpoints) / sizeof(*MainNetCheckpoints)
};

static const ChainParams TestNetParams = {
        TestNetDNSSeeds,
        32538,      // standardPort
        0x493b4ecf, // magicNumber
        0,          // services
        TestNetVerifyDifficulty,
        TestNetCheckpoints,
        sizeof(TestNetCheckpoints) / sizeof(*TestNetCheckpoints)
};

static const ChainParams RegTestParams = {
        RegTestDNSSeeds,
        32538,      // standardPort
        0x493b4ecf, // magicNumber
        0,          // services
        RegTestVerifyDifficulty,
        RegTestCheckpoints,
        sizeof(RegTestCheckpoints) / sizeof(*RegTestCheckpoints)
};
#endif // ChainParams_h
