// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Veda Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp,
		const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce,
		uint32_t nBits, int32_t nVersion, const CAmount& genesisReward) {
	CMutableTransaction txNew;
	txNew.nVersion = 1;
	txNew.vin.resize(1);
	txNew.vout.resize(1);
	txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4)
			<< std::vector<unsigned char>((const unsigned char*) pszTimestamp,
					(const unsigned char*) pszTimestamp + strlen(pszTimestamp));
	txNew.vout[0].nValue = genesisReward;
	txNew.vout[0].scriptPubKey = genesisOutputScript;

	CBlock genesis;
	genesis.nTime = nTime;
	genesis.nBits = nBits;
	genesis.nNonce = nNonce;
	genesis.nVersion = nVersion;
	genesis.vtx.push_back(txNew);
	genesis.hashPrevBlock.SetNull();
	genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
	return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce,
		uint32_t nBits, int32_t nVersion, const CAmount& genesisReward) {
	const char* pszTimestamp = "I create veda coin 04/24/2018 20:30:00.";
	const CScript genesisOutputScript =
			CScript()
					<< ParseHex(
							"0426d1c5aac0e7b98f37f5f8ca10a18bb915820516723a727093cca65108ac24cdf3467ff06a39ad388ccc3d83802c85df73dba14e3db3835cec6892b9647e92fa")
					<< OP_CHECKSIG;
	return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce,
			nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams: public CChainParams {
public:
	CMainParams() {
		strNetworkID = "main";
		consensus.nSubsidyHalvingInterval = 210240; // Note: actual number of blocks per calendar year with DGW v3 is ~200700 (for example 449750 - 249050)
		consensus.nMasternodePaymentsStartBlock = 100; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
		consensus.nMasternodePaymentsIncreaseBlock = 158000; // actual historical value
		consensus.nMasternodePaymentsIncreasePeriod = 576 * 30; // 17280 - actual historical value
		consensus.nInstantSendKeepLock = 24;
		consensus.nBudgetPaymentsStartBlock = 328008; // actual historical value
		consensus.nBudgetPaymentsCycleBlocks = 16616; // ~(60*24*30)/2.6, actual number of blocks per month is 200700 / 12 = 16725
		consensus.nBudgetPaymentsWindowBlocks = 100;
		consensus.nBudgetProposalEstablishingTime = 60 * 60 * 24;
		consensus.nSuperblockStartBlock = 614820; // The block at which 12.1 goes live (end of final 12.0 budget cycle)
		consensus.nSuperblockCycle = 16616; // ~(60*24*30)/2.6, actual number of blocks per month is 200700 / 12 = 16725
		consensus.nGovernanceMinQuorum = 10;
		consensus.nGovernanceFilterElements = 20000;
		consensus.nMasternodeMinimumConfirmations = 15;
		consensus.nMajorityEnforceBlockUpgrade = 750;
		consensus.nMajorityRejectBlockOutdated = 950;
		consensus.nMajorityWindow = 1000;
		consensus.BIP34Height = 1;
		consensus.BIP34Hash =
				uint256S(
						"0x000007d91d1254d60e2dd1ae580383070a4ddffa4c64c2eeb4a2f9ecc0414343");
		consensus.powLimit =
				uint256S(
						"00000fffff000000000000000000000000000000000000000000000000000000");
		consensus.nPowTargetTimespan = 30 * 60; // Veda: 0.5 hour
		consensus.nPowTargetSpacing = 1.5 * 60; // Veda: 1.5 minutes
		consensus.fPowAllowMinDifficultyBlocks = false;
		consensus.fPowNoRetargeting = false;
		consensus.nPowKGWHeight = 15200;
		consensus.nPowDGWHeight = 34140;
		consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
		consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime =
				1199145601; // January 1, 2008
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout =
				1230767999; // December 31, 2008

		// Deployment of BIP68, BIP112, and BIP113.
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime =
				9999999999; // Feb 5th, 2017
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 9999999999; // Feb 5th, 2018

		// Deployment of DIP0001
		consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
		consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime =
				9999999999; // Oct 15th, 2017
		consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout =
				9999999999; // Oct 15th, 2018
		consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize =
				4032;
		consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 3226; // 80% of 4032

		// The best chain should have at least this much work.
		consensus.nMinimumChainWork = uint256S("0x00"); // 782700

		// By default assume that the signatures in ancestors of this block are valid.
		consensus.defaultAssumeValid = uint256S("0x00"); // 782700

		/**
		 * The message start string is designed to be unlikely to occur in normal data.
		 * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
		 * a large 32-bit integer with any alignment.
		 */
		pchMessageStart[0] = 0x1c;
		pchMessageStart[1] = 0xbd;
		pchMessageStart[2] = 0xcb;
		pchMessageStart[3] = 0x4f;
		vAlertPubKey =
				ParseHex(
						"04b36556d4e6822708431cce73eaf447a0ec89a8ae6eb48aa412cb5b56bb6410acaa7cda7000e270b9900eb77667bb421728cab77e720c7ca2118150430c4f418a");
		nDefaultPort = 21967;
		nMaxTipAge = 6 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
		nDelayGetHeadersTime = 24 * 60 * 60;
		nPruneAfterHeight = 100000;

		genesis = CreateGenesisBlock(1524601800, 561576, 0x1e0ffff0, 1,
				0 * COIN);
		consensus.hashGenesisBlock = genesis.GetHash();
		assert(
				consensus.hashGenesisBlock
						== uint256S(
								"0x0000038cab4d0145835a5fe6e0838b4c7860f9d0265ff22501e572186aad1938"));
		assert(
				genesis.hashMerkleRoot
						== uint256S(
								"0x695d90f6fb3fd759c83f6a62bb096c8827db365757a9c8b6784b7d3019a0ece3"));

//        vSeeds.clear();
		vSeeds.push_back(CDNSSeedData("206.189.30.217", "206.189.30.217"));
		vSeeds.push_back(CDNSSeedData("50.63.160.147", "50.63.160.147"));
		vSeeds.push_back(CDNSSeedData("167.99.234.99", "167.99.234.99"));
		vSeeds.push_back(CDNSSeedData("206.81.6.102", "206.81.6.102"));
		vSeeds.push_back(CDNSSeedData("206.81.6.137", "206.81.6.137"));
		vSeeds.push_back(CDNSSeedData("206.81.6.156", "206.81.6.156"));
		vSeeds.push_back(CDNSSeedData("206.81.14.0", "206.81.14.0"));
		vSeeds.push_back(CDNSSeedData("206.81.14.25", "206.81.14.25"));
		vSeeds.push_back(CDNSSeedData("206.81.14.70", "206.81.14.70"));
		vSeeds.push_back(CDNSSeedData("206.81.1.43", "206.81.1.43"));
		vSeeds.push_back(CDNSSeedData("206.81.1.45", "206.81.1.45"));

		// Veda addresses start with 'X'
		base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 76);
		// Veda script addresses start with '7'
		base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 16);
		// Veda private keys start with '7' or 'X'
		base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 204);
		// Veda BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
		base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(
				0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
		// Veda BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
		base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(
				0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

		// Veda BIP44 coin type is '5'
		nExtCoinType = 5;

		vFixedSeeds = std::vector < SeedSpec6
				> (pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

		fMiningRequiresPeers = true;
		fDefaultConsistencyChecks = false;
		fRequireStandard = true;
		fMineBlocksOnDemand = false;
		fTestnetToBeDeprecatedFieldRPC = false;

		nPoolMaxTransactions = 3;
		nFulfilledRequestExpireTime = 60 * 60; // fulfilled requests expire in 1 hour
		strSporkPubKey =
				"04549ac134f694c0243f503e8c8a9a986f5de6610049c40b07816809b0d1d06a21b07be27b9bb555931773f62ba6cf35a25fd52f694d4e1106ccd237a7bb899fdd";

		checkpointData =
				(CCheckpointData ) {
								boost::assign::map_list_of(0,
										uint256S(
												"0x0000038cab4d0145835a5fe6e0838b4c7860f9d0265ff22501e572186aad1938"))(
										100,
										uint256S(
												"0x000001ff94b709840c79ac568a4657c5a968555a9d3e38872c37c8ae4c59dda8"))/*(
										100,
										uint256S(
												"0x0000022b32afc5db0a7f9389fa41fb745c13c851455a40db5a5b7cc90ec6d2d1"))*/,
								1524601800, // * UNIX timestamp of last checkpoint block
								0, // * total number of transactions between genesis and last checkpoint
								   //   (the tx=... number in the SetBestChain debug.log lines)
								5000 // * estimated number of transactions per day after checkpoint
						};
	}
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams: public CChainParams {
public:
	CTestNetParams() {
		strNetworkID = "test";
		consensus.nSubsidyHalvingInterval = 210240;
		consensus.nMasternodePaymentsStartBlock = 4010; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
		consensus.nMasternodePaymentsIncreaseBlock = 4030;
		consensus.nMasternodePaymentsIncreasePeriod = 10;
		consensus.nInstantSendKeepLock = 6;
		consensus.nBudgetPaymentsStartBlock = 4100;
		consensus.nBudgetPaymentsCycleBlocks = 50;
		consensus.nBudgetPaymentsWindowBlocks = 10;
		consensus.nBudgetProposalEstablishingTime = 60 * 20;
		consensus.nSuperblockStartBlock = 4200; // NOTE: Should satisfy nSuperblockStartBlock > nBudgetPeymentsStartBlock
		consensus.nSuperblockCycle = 24; // Superblocks can be issued hourly on testnet
		consensus.nGovernanceMinQuorum = 1;
		consensus.nGovernanceFilterElements = 500;
		consensus.nMasternodeMinimumConfirmations = 1;
		consensus.nMajorityEnforceBlockUpgrade = 51;
		consensus.nMajorityRejectBlockOutdated = 75;
		consensus.nMajorityWindow = 100;
		consensus.BIP34Height = 1;
		consensus.BIP34Hash =
				uint256S(
						"0x0000047d24635e347be3aaaeb66c26be94901a2f962feccd4f95090191f208c1");
		consensus.powLimit =
				uint256S(
						"00000fffff000000000000000000000000000000000000000000000000000000");
		consensus.nPowTargetTimespan = 24 * 60 * 60; // Veda: 1 day
		consensus.nPowTargetSpacing = 1.5 * 60; // Veda: 1.5 minutes
		consensus.fPowAllowMinDifficultyBlocks = true;
		consensus.fPowNoRetargeting = false;
		consensus.nPowKGWHeight = 4001; // nPowKGWHeight >= nPowDGWHeight means "no KGW"
		consensus.nPowDGWHeight = 4001;
		consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
		consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime =
				1199145601; // January 1, 2008
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout =
				1230767999; // December 31, 2008

		// Deployment of BIP68, BIP112, and BIP113.
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime =
				1506556800; // September 28th, 2017
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1538092800; // September 28th, 2018

		// Deployment of DIP0001
		consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
		consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime =
				1505692800; // Sep 18th, 2017
		consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout =
				1537228800; // Sep 18th, 2018
		consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 100;
		consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 50; // 50% of 100

		// The best chain should have at least this much work.
		consensus.nMinimumChainWork = uint256S("0x00"); // 37900

		// By default assume that the signatures in ancestors of this block are valid.
		consensus.defaultAssumeValid = uint256S("0x00"); // 37900

		pchMessageStart[0] = 0xce;
		pchMessageStart[1] = 0xe2;
		pchMessageStart[2] = 0xca;
		pchMessageStart[3] = 0xff;
		vAlertPubKey =
				ParseHex(
						"04517d8a699cb43d3938d7b24faaff7cda448ca4ea267723ba614784de661949bf632d6304316b244646dea079735b9a6fc4af804efb4752075b9fe2245e14e412");
		nDefaultPort = 31967;
		nMaxTipAge = 0x7fffffff; // allow mining on top of old blocks for testnet
		nDelayGetHeadersTime = 24 * 60 * 60;
		nPruneAfterHeight = 1000;

		genesis = CreateGenesisBlock(1390666206UL, 3861367235UL, 0x1e0ffff0, 1,
				200000 * COIN);
		consensus.hashGenesisBlock = genesis.GetHash();
//        assert(consensus.hashGenesisBlock == uint256S("0x00000bafbc94add76cb75e2ec92894837288a481e5c005f6563d91623bf8bc2c"));
//        assert(genesis.hashMerkleRoot == uint256S("0xe0028eb9648db56b1ac77cf090b99048a8007e2bb64b68f092c03c7f56a662c7"));

		vFixedSeeds.clear();
		vSeeds.clear();
		vSeeds.push_back(CDNSSeedData("vedadot.io", "testnet-seed.vedadot.io"));
		vSeeds.push_back(
				CDNSSeedData("masternode.io", "test.dnsseed.masternode.io"));

		// Testnet Veda addresses start with 'y'
		base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 140);
		// Testnet Veda script addresses start with '8' or '9'
		base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);
		// Testnet private keys start with '9' or 'c' (Bitcoin defaults)
		base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);
		// Testnet Veda BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
		base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(
				0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
		// Testnet Veda BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
		base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(
				0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

		// Testnet Veda BIP44 coin type is '1' (All coin's testnet default)
		nExtCoinType = 1;

		vFixedSeeds = std::vector < SeedSpec6
				> (pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

		fMiningRequiresPeers = true;
		fDefaultConsistencyChecks = false;
		fRequireStandard = false;
		fMineBlocksOnDemand = false;
		fTestnetToBeDeprecatedFieldRPC = true;

		nPoolMaxTransactions = 3;
		nFulfilledRequestExpireTime = 5 * 60; // fulfilled requests expire in 5 minutes
		strSporkPubKey =
				"046f78dcf911fbd61910136f7f0f8d90578f68d0b3ac973b5040fb7afb501b5939f39b108b0569dca71488f5bbf498d92e4d1194f6f941307ffd95f75e76869f0e";

		checkpointData =
				(CCheckpointData ) {
								boost::assign::map_list_of(0,
										uint256S(
												"0x00000741b645d3fc16a05f555adcca84674c162704b1ece207ffcb394e90dcc4")),

								1521799201, // * UNIX timestamp of last checkpoint block
								0, // * total number of transactions between genesis and last checkpoint
								   //   (the tx=... number in the SetBestChain debug.log lines)
								500 // * estimated number of transactions per day after checkpoint
						};

	}
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams: public CChainParams {
public:
	CRegTestParams() {
		strNetworkID = "regtest";
		consensus.nSubsidyHalvingInterval = 150;
		consensus.nMasternodePaymentsStartBlock = 240;
		consensus.nMasternodePaymentsIncreaseBlock = 350;
		consensus.nMasternodePaymentsIncreasePeriod = 10;
		consensus.nInstantSendKeepLock = 6;
		consensus.nBudgetPaymentsStartBlock = 1000;
		consensus.nBudgetPaymentsCycleBlocks = 50;
		consensus.nBudgetPaymentsWindowBlocks = 10;
		consensus.nBudgetProposalEstablishingTime = 60 * 20;
		consensus.nSuperblockStartBlock = 1500;
		consensus.nSuperblockCycle = 10;
		consensus.nGovernanceMinQuorum = 1;
		consensus.nGovernanceFilterElements = 100;
		consensus.nMasternodeMinimumConfirmations = 1;
		consensus.nMajorityEnforceBlockUpgrade = 750;
		consensus.nMajorityRejectBlockOutdated = 950;
		consensus.nMajorityWindow = 1000;
		consensus.BIP34Height = -1; // BIP34 has not necessarily activated on regtest
		consensus.BIP34Hash = uint256();
		consensus.powLimit =
				uint256S(
						"7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
		consensus.nPowTargetTimespan = 24 * 60 * 60; // Veda: 1 day
		consensus.nPowTargetSpacing = 1.5 * 60; // Veda: 1.5 minutes
		consensus.fPowAllowMinDifficultyBlocks = true;
		consensus.fPowNoRetargeting = true;
		consensus.nPowKGWHeight = 15200; // same as mainnet
		consensus.nPowDGWHeight = 34140; // same as mainnet
		consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
		consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout =
				999999999999ULL;
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout =
				999999999999ULL;
		consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
		consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0;
		consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout =
				999999999999ULL;

		// The best chain should have at least this much work.
		consensus.nMinimumChainWork = uint256S("0x00");

		// By default assume that the signatures in ancestors of this block are valid.
		consensus.defaultAssumeValid = uint256S("0x00");

		pchMessageStart[0] = 0xfc;
		pchMessageStart[1] = 0xc1;
		pchMessageStart[2] = 0xb7;
		pchMessageStart[3] = 0xdc;
		nMaxTipAge = 6 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
		nDelayGetHeadersTime = 0; // never delay GETHEADERS in regtests
		nDefaultPort = 41993;
		nPruneAfterHeight = 1000;

		genesis = CreateGenesisBlock(1417713337, 1096447, 0x207fffff, 1,
				200000 * COIN);
		consensus.hashGenesisBlock = genesis.GetHash();
//        assert(consensus.hashGenesisBlock == uint256S("0x000008ca1832a4baf228eb1553c03d3a2c8e02399550dd6ea8d65cec3ef23d2e"));
//        assert(genesis.hashMerkleRoot == uint256S("0xe0028eb9648db56b1ac77cf090b99048a8007e2bb64b68f092c03c7f56a662c7"));

		vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
		vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

		fMiningRequiresPeers = false;
		fDefaultConsistencyChecks = true;
		fRequireStandard = false;
		fMineBlocksOnDemand = true;
		fTestnetToBeDeprecatedFieldRPC = false;

		nFulfilledRequestExpireTime = 5 * 60; // fulfilled requests expire in 5 minutes

		checkpointData =
				(CCheckpointData ) {
								boost::assign::map_list_of(0,
										uint256S(
												"0x13c7f6f7798b0fc512bfde9d92bfa9257dcecb349fd0045fd4dfbf4d572fcbdb")),
								0, 0, 0 };
		// Regtest Veda addresses start with 'y'
		base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 140);
		// Regtest Veda script addresses start with '8' or '9'
		base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);
		// Regtest private keys start with '9' or 'c' (Bitcoin defaults)
		base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);
		// Regtest Veda BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
		base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(
				0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
		// Regtest Veda BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
		base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(
				0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

		// Regtest Veda BIP44 coin type is '1' (All coin's testnet default)
		nExtCoinType = 1;
	}
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
	assert(pCurrentParams);
	return *pCurrentParams;
}

CChainParams& Params(const std::string& chain) {
	if (chain == CBaseChainParams::MAIN)
		return mainParams;
	else if (chain == CBaseChainParams::TESTNET)
		return testNetParams;
	else if (chain == CBaseChainParams::REGTEST)
		return regTestParams;
	else
		throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
	}

void SelectParams(const std::string& network) {
	SelectBaseParams(network);
	pCurrentParams = &Params(network);
}
