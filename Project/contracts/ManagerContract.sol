pragma solidity ^0.5.1;
import "./CandidateContract.sol"
import "./VoterContract.sol"
import "./VoteTallyContract.sol"

contract Manager {
    address public owner;
    //mapping (address => )

    // TODO: 1. set head of EC 
    constructor() public {
        owner = msg.sender;
    }

    function createVoter(
        address personAddr,
        string Name,
        string FathersName,
        string MothersName,
        string HallOfResidence;
        string RollNumber,
    ) public accessModifier {
        //Anyone on the network can become a student if not one already
        //Remember to assign the new student a unique roll number
        Voter newVoter = new Voter(
            msg.sender,
            kccName,
            FathersName,
            MothersName,
            HallOfResidence;
            RollNumber
        )
    }

    function getVoterDetails() public PersonAndECOnly {
        
    }

    function updateVotersDetails() public ECOnly {

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
}
