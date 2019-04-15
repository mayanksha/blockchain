pragma solidity ^0.5.1;
import "./CandidateContract.sol";
import "./VoterContract.sol";
import "./VoteTallyContract.sol";

contract Manager {
    address public owner;

    enum AUTH_LEVELS {
        NO_PRIVILEGE,
        CAN_VIEW,
        CAN_CREATE,
        CAN_CAST_VOTE,
        ALL_PRIVILEGES
    };

    // Mapping between the RollNo and Voter
    mapping (int256 => Voter) addressVoterMap;
    // Mapping between wallet of user and Voter
    mapping (address => Voter) rollNoVoterMap;

    // Mapping from person's address to enum AUTH_LEVELS
    mapping (address => int16) public ECAuthorisedPeople;


    address voteTallyContractAddress;
    // TODO: 1. set head of EC 
    constructor() public {
        owner = msg.sender;
        VoteTally newTallyContract = new VoteTally();
    }

    function createVoter(
        address personAddr,
        string Name,
        string FathersName,
        string MothersName,
        string HallOfResidence;
        string RollNumber
    ) public accessModifier {
        //Anyone on the network can become a Voter if not one already

		require (rollNoVoterMap[RollNumber] == Voter(0));

        Voter newVoter = new Voter(
            msg.sender,
            Name,
            FathersName,
            MothersName,
            HallOfResidence;
            RollNumber
        );
    }

    function getVoterDetails(int256 RollNo) public returns Voter {

        // Can be the address of the Voter or of someone within EC with adequate privileges
        address personAddr = msg.sender;
        
        // In the case the person provides some RollNo, you gotta check his Roll No
        if (!RollNo) {
            //  Check that the person has actually registered as a voter or not
            require (addressVoterMap[personAddr] != Voter(0));
            return addressVoterMap[personAddr].getAllDetails();
        }

        // This case will happen when this fn is called by someone from EC
        else {
            // Check Auth level
            require (
                ECAuthorisedPeople[personAddr] != 0 && 
                ECAuthorisedPeople[personAddr] >= AUTH_LEVELS.CAN_VIEW
            );
            // Check RollNo exists as Voter or not
            require (rollNoVoterMap[RollNo]  Voter(0));
            return rollNoVoterMap[RollNo].getAllDetails();
        }
    }

    // Roll No is immutable?
    function updateVotersDetails() public ECOnly {
        // TODO: Finish this

        // Can be the address of the Voter or of someone within EC with adequate privileges
        address personAddr = msg.sender;
        
        // In the case the person provides some RollNo, you gotta check his Roll No
        if (!RollNo) {
            //  Check that the person has actually registered as a voter or not
            require (addressVoterMap[personAddr] != Voter(0));
            return addressVoterMap[personAddr].updateAllDetails();
        }

        // This case will happen when this fn is called by someone from EC
        else {
            // Check Auth level
            require (
                ECAuthorisedPeople[personAddr] != 0 && 
                ECAuthorisedPeople[personAddr] >= AUTH_LEVELS.CAN_VIEW
            );
            // Check RollNo exists as Voter or not
            require (rollNoVoterMap[RollNo]  Voter(0));
            return rollNoVoterMap[RollNo].getAllDetails();
        }
        
    }

    function registerFingerprintData() public {

    }

    function updateFingerprintData() public {

    }

    function createCandidate() public {

    }

    function updateCandidateDetails() public {

    }

    function castVote() {

    }

    function countVotes() {

    }

    function verifyVotes() {

    }

    function getCandidatePenalties() {

    }

    // Should this function be moved to CandidateContract?
    function getCandidateManifesto() {

    }

    function penalizeCandiate() {

    }

    function dismissCandidateNomination() {

    }

    function addProposerForCandidate() {

    }

    function addSeconderForCandidate() {

    }

    function verifyProposerIsValidPerson() {

    }

    function verifySeconderIsValidPerson() {

    }

    // Change the owner to a new head of EC
    function transferOwnership() {

    }

    function registerVoteCastingComputer() public {

    }

    // All the access modifiers here
    modifier accessToAdminOnly {
        require(msg.sender == admin);
        _;
    }

    modifier accessToVoterOnly {

    }

    modifier accessToCandidateOnly {

    }

    modifier accessToHeadOnly {

    }

    modifier accessToRegisteredComputersOnly {

    }

    // Should have access only to register people, and update their details
    // upon their fingerprint verification
    modifier accessToECWorkersOnly {
        
    }
    modifier PersonAndECOnly {
  
    }
	function kill() public accessToAdminOnly {
		//The admin has the right to kill the contract at any time.
		//Take care that no one else is able to kill the contract
		selfdestruct((address)((uint160)(admin)));
	}
}
